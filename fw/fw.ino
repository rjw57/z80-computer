#include <avr/pgmspace.h>

#define HSYNC           5 // =OC0B, fixed by hardware
#define VSYNC           10 // =OC1B, fixed by hardware
#define CHAR_CLK        4 // =T0, fixed by hardware

#define SR_SER          11
#define IO_ACK_BAR      9
#define SR_CLK          8
#define SR_MODE         7
#define SR_OUT          6

#define ARD_OVR_BAR     2
#define ARD_OVR         3

#define RST_BAR         A5
#define CPU_RD_BAR      A4
#define CPU_INT_BAR     A3
#define CLK_SEL         A0
#define OV_CLK          A1

// RAM contents
#define sram_bin ram_contents
#include "sram_contents.h"
#undef sram_bin

const uint16_t ram_contents_len = sizeof(ram_contents) / sizeof(uint8_t);

// Video timing: 640x480 VGA @ 60Hz
const double dot_clock_freq       = 25.175;           // MHz
const int h_visible_area          = 640;              // pixels
const int h_front_porch           = 16;               // pixels
const int h_sync_width            = 96;               // pixels
const int h_back_porch            = 48;               // pixels
const int h_polarity              = -1;               // sign

const int v_visible_area          = 480;              // lines
const int v_front_porch           = 10;               // lines
const int v_sync_width            = 2;                // lines
const int v_back_porch            = 33;               // lines
const int v_polarity              = -1;               // sign

// Derived timings
const double h_visible_area_t     = h_visible_area / dot_clock_freq;  // µs
const double h_front_porch_t      = h_front_porch / dot_clock_freq;   // µs
const double h_sync_width_t       = h_sync_width / dot_clock_freq;    // µs
const double h_back_porch_t       = h_back_porch / dot_clock_freq;    // µs

const double hvis_inactive_width  =
    h_sync_width_t + h_front_porch_t + h_back_porch_t;
const double hvis_inactive_offset =
    0.5 * (h_back_porch_t - h_front_porch_t);
const double whole_line           =
    h_visible_area_t + h_front_porch_t + h_sync_width_t + h_back_porch_t;
const uint16_t whole_frame             =
    v_visible_area + v_front_porch + v_sync_width + v_back_porch;

// Counter timer
const double timer_freq           = dot_clock_freq / 8;   // MHz

// Prepare for LDIR instruction for writing to memory. Resets BC and DE to zero
// so that LDIR writes to RAM starting from address $0000. We feed the LDIR
// instruction data directly in order so we don't care about the value of HL.
const PROGMEM uint8_t init_code[] = {
  0xf3,               // DI
  0x01, 0x00, 0x00,   // LD bc, $0000
  0x11, 0x00, 0x00,   // LD de, $0000
};
const uint8_t init_code_len = sizeof(init_code) / sizeof(uint8_t);

static uint8_t init_code_bytes_sent;
static uint8_t ldir_state = 0;
static uint8_t next_byte = 0x00;
static uint16_t ram_bytes_sent;

void copy_loop_reset() {
  next_byte = 0x00;
  init_code_bytes_sent = 0;
  ram_bytes_sent = 0;
  ldir_state = 0;
  set_data(next_byte);
}

void copy_loop_advance() {
  if(init_code_bytes_sent < init_code_len) {
    next_byte = pgm_read_byte_near(init_code + init_code_bytes_sent);
    ++init_code_bytes_sent;
    return;
  }

  switch(ldir_state) {
    case 0:
      next_byte = 0xed; // First LDIR instruction
      ++ldir_state;
      return;
    case 1:
      next_byte = 0xb0;
      ldir_state = 4;
      return;
    case 2:
      next_byte = 0xed; // Subsequent LDIR instructions
      ++ldir_state;
      return;
    case 3:
      next_byte = 0xb0;
      ++ram_bytes_sent;
      ++ldir_state;
      return;
    case 4:
      next_byte = pgm_read_byte_near(ram_contents + ram_bytes_sent);
      ldir_state = 2;
      return;
  }

  // Fall-through: send NOPs
  next_byte = 0x00;
}

void claim_read_dev() {
  digitalWrite(ARD_OVR_BAR, LOW);
  digitalWrite(ARD_OVR, HIGH);
}

void release_read_dev() {
  digitalWrite(ARD_OVR_BAR, HIGH);
  digitalWrite(ARD_OVR, LOW);
}

// Set data bus value. Output depends on state of BUS_OE_BAR pin.
void set_data(uint8_t data) {
  // the code here is slightly awkward because we want to leave SR_CLK high
  // after clocking in data so that writing to the arduino port works.
  digitalWrite(SR_MODE, LOW); // shifting data
  digitalWrite(SR_CLK, LOW); // prepare to clock data
  shiftOut(SR_SER, SR_CLK, MSBFIRST, data >> 1);
  digitalWrite(SR_SER, (data & 0x1) ? HIGH : LOW);
  digitalWrite(SR_CLK, HIGH);
  digitalWrite(SR_MODE, HIGH); // loading data unless register is being read
}

// Read value in shift reg.
uint8_t read_data() {
  return 0x00;

  digitalWrite(SR_MODE, LOW); // shifting data

  // At this point the SR_OUT pin *already* has the MSB of the data bus on it so
  // don't reset the clock pin. That way shiftIn reads data after the *falling*
  // edge so we don't miss the MSB
  digitalWrite(SR_MODE, LOW); // read
  uint8_t data = shiftIn(SR_OUT, SR_CLK, MSBFIRST);

  // Now make sure the clock pin is high to ~WR takes effect
  digitalWrite(SR_CLK, HIGH);
  digitalWrite(SR_MODE, HIGH); // loading data

  return data;
}

// Override crystal-based clock. CPU clock is now OV_CLK output.
void claim_clock() {
  digitalWrite(OV_CLK, LOW);
  digitalWrite(CLK_SEL, LOW);
}

// Release CPU clock. Clock is now crystal.
void release_clock() {
  digitalWrite(CLK_SEL, HIGH);
  digitalWrite(OV_CLK, HIGH);
}

void tick_update() {
  static bool prev_rd = false;
  bool rd = digitalRead(CPU_RD_BAR) == LOW;
  if(!rd && prev_rd) {
    copy_loop_advance();
    set_data(next_byte);
  }
  prev_rd = rd;
}

// Set reset. Note that reset must be active for a minimum of 3 cycles.
// Should be called with clock claimed to have effect.
void reset_on() {
  digitalWrite(RST_BAR, LOW);
  set_data(0x00);
  for(int i=0; i<5; ++i) {
    digitalWrite(OV_CLK, HIGH);
    digitalWrite(OV_CLK, LOW);
  }
}

// Release reset.
void reset_off() {
  digitalWrite(RST_BAR, HIGH);
}

// Tick the CPU clock
void tick() {
  digitalWrite(OV_CLK, HIGH);
  digitalWrite(OV_CLK, LOW);
  tick_update();
}

static bool should_handle_io_request = false;

void io_request_handler() {
  should_handle_io_request = true;
}

void io_request_handler_bottom() {
}

// Called around the middle of the HSYNC pulse which will be after TCNT1 update.
// Use the value of TCNT1 (the row counter) to determine when to trigger a
// vertical interrupt.
ISR(TIMER0_OVF_vect) {
  // Our video driver takes two lines to respond to the interrupt and enable the
  // display.
  if(TCNT1 == v_sync_width + v_back_porch - 2 + 60) {
    // The AVR is more than slow enough to make sure that the Z80 notices this
    // interrupt.
    digitalWrite(CPU_INT_BAR, LOW);
    digitalWrite(CPU_INT_BAR, HIGH);
  }
}

void setup_timers() {
  // Reset all timers and halt them
  GTCCR = _BV(TSM) | _BV(PSRASY) | _BV(PSRSYNC);

  // Arduino sets up some timers so we should make sure to change the timer
  // control registers before any of OCRxx.

  // Timer 0: HSYNC

  // Phase correct PWM, TOP = OCR0A, external rising edge clock, no pre-scaling,
  // OC0B pin is RESET when match on counting UP and SET when match on counting
  // DOWN for +ve HSYNC polarity. (Opposite for -ve.)
  TCCR0A = _BV(COM0B1) | _BV(WGM00) | ((h_polarity < 0) ? _BV(COM2B0) : 0);
  TCCR0B = _BV(WGM02) | _BV(CS02) | _BV(CS01) | _BV(CS00);

  // We use phase correct operation so the counter counts up from BOTTOM to TOP
  // and back down. Both BOTTOM and TOP are present for one counter cycle so the
  // number of cycles for each period is 2*(TOP-BOTTOM).
  //
  // BOTTOM is 0 and TOP and so the period, T, is 2*TOP/f where f is the counter
  // frequency. Therefore:
  const int top = whole_line * timer_freq / 2.;
  OCR0A = top;

  // We use the phase-correct PWM mode so the pulse width will be 2*COMPARE/f
  // and so, COMPARE = width * f / 2.

  // Set HSYNC pulse width,
  OCR0B = (int)(h_sync_width_t * timer_freq / 2.);

  // Timer 1: VSYNC

  // T1 == OC0B which is HSYNC so we can use it as a clock which is a happy
  // co-incidence.

  // Fast PWM, TOP = OCR1A
  TCCR1A = _BV(WGM11) | _BV(WGM10);
  TCCR1B = _BV(WGM13) | _BV(WGM12);

  if(v_polarity < 0) {
    // Set OC1B on match, clear at bottom.
    TCCR1A |= _BV(COM1B1) | _BV(COM1B0);
  } else {
    // Clear OC1B on match, set at bottom.
    TCCR1A |= _BV(COM1B1);
  }

  // Depending on polarity of HSYNC, clock on rising or falling edge of T1 (aka
  // HSYNC).
  if(h_polarity > 0) {
    // Clock on falling edge of T1.
    TCCR1B |= _BV(CS12) | _BV(CS11);
  } else {
    // Clock on rising edge of T1.
    TCCR1B |= _BV(CS12) | _BV(CS11) | _BV(CS10);
  }

  // TOP/OCR1A is set to the total size of the frame in lines minus 1 since it
  // is zero based.
  OCR1A = whole_frame - 1;

  // OCR1B is set to the pulse width minus 1 since it is zero based.
  OCR1B = v_sync_width - 1;

  // Enable OVF interrupt for timer 0
  TIMSK0 |= _BV(TOIE0);

  // Set initial timer values
  TCNT0 = 0;
  TCNT1 = 0;

  // start timer
  GTCCR = 0;
}

void setup() {
  //Serial.begin(9600);
  //while (!Serial);  // wait for serial port to connect. Needed for native USB

  // Set HSYNC as output
  pinMode(HSYNC, OUTPUT);

  // Set VSYNC as output
  pinMode(VSYNC, OUTPUT);

  setup_timers();

  // Reset memory image copy code loop
  copy_loop_reset();

  // Setup CPU control lines
  pinMode(CPU_RD_BAR, INPUT);
  digitalWrite(CPU_INT_BAR, HIGH);
  pinMode(CPU_INT_BAR, OUTPUT);

  // Setup clock override pins
  pinMode(OV_CLK, OUTPUT);
  pinMode(CLK_SEL, OUTPUT);

  // Ensure we are the read device and set up data bus
  pinMode(SR_OUT, INPUT);
  digitalWrite(SR_SER, LOW);
  pinMode(SR_SER, OUTPUT);
  digitalWrite(SR_CLK, HIGH);
  pinMode(SR_CLK, OUTPUT);
  pinMode(ARD_OVR_BAR, OUTPUT);
  pinMode(ARD_OVR, OUTPUT);
  claim_read_dev();
  digitalWrite(SR_MODE, HIGH); // loading data
  pinMode(SR_MODE, OUTPUT);

  // Ensure CPU is in RESET state.
  digitalWrite(RST_BAR, LOW);
  pinMode(RST_BAR, OUTPUT);

  // Ensure CPU is not in WAIT state by pulsing IO_ACK_BAR low
  digitalWrite(IO_ACK_BAR, LOW);
  pinMode(IO_ACK_BAR, OUTPUT);
  digitalWrite(IO_ACK_BAR, HIGH);
  digitalWrite(IO_ACK_BAR, LOW);
  digitalWrite(IO_ACK_BAR, HIGH);

  claim_clock();
  reset_on();
  reset_off();
  digitalWrite(RST_BAR, HIGH);

  do {
    tick();
  } while(ram_bytes_sent < ram_contents_len);

  reset_on();
  release_read_dev();

  release_clock();

  set_data(0xaa);

  // Connect IOREQ handler
  attachInterrupt(0, io_request_handler, FALLING);

  // ensure we're not in a wait state
  digitalWrite(IO_ACK_BAR, LOW);
  digitalWrite(IO_ACK_BAR, HIGH);

  reset_off();
}

void loop() {
  //if(should_handle_io_request) { io_request_handler_bottom(); }
}

// HACK: by rolling our own main() function we avoid linking in the wiring
// support library which, in turn, frees up timer 0's interrupt for our use.
int main(void) {
  // disable interrupts while performing setup
  noInterrupts();

  // setup pins, timers, etc.
  setup();

  // re-enable interrupts
  interrupts();

  while(1) { loop(); }
  return 0;
}

// vim:sw=2:sts=2:et
