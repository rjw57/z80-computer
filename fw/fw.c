#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

// VGA timing generator.
//
// Output HSYNC, VSYNC, and VIS signals.

// Pin assignment - ATMEGA328P
#define HSYNC_PORT_BIT    5   // =OC0B, pin 11, fixed by hardware
#define HSYNC_PORT_NAME   D
#define VIS_PORT_BIT      3   // =OC2B, pin 5, fixed by hardware
#define VIS_PORT_NAME     D
#define VSYNC_PORT_BIT    2   // =OC1B, pin 16, fixed by hardware
#define VSYNC_PORT_NAME   B

#define ARD_WR_BAR_PORT_BIT       2 // = INT0, pin 4, fixed by hardware
#define ARD_WR_BAR_PORT_NAME      D

#define CPU_INT_BAR_PORT_BIT      0
#define CPU_INT_BAR_PORT_NAME     B
#define ARD_RD_BAR_PORT_BIT       7
#define ARD_RD_BAR_PORT_NAME      D

#define SR_CLK_PORT_BIT           0
#define SR_CLK_PORT_NAME          C
#define SR_SER_PORT_BIT           2
#define SR_SER_PORT_NAME          C
#define SR_MODE_PORT_BIT          1
#define SR_MODE_PORT_NAME         C
#define SR_OUT_PORT_BIT           3
#define SR_OUT_PORT_NAME          C

#define RST_BAR_PORT_BIT          6
#define RST_BAR_PORT_NAME         D
#define CLK_SEL_PORT_BIT          4
#define CLK_SEL_PORT_NAME         C
#define OV_CLK_PORT_BIT           5
#define OV_CLK_PORT_NAME          C

#define ARD_OVR_BAR_PORT_BIT      4
#define ARD_OVR_BAR_PORT_NAME     D

#define PS2_DATA_PORT_BIT         1
#define PS2_DATA_PORT_NAME        B
#define PS2_CLK_PORT_BIT          7 // = PCINT7
#define PS2_CLK_PORT_NAME         B

// RAM contents
#define sram_bin ram_contents
#include "sram_contents.h"
#undef sram_bin

const uint16_t ram_contents_len = sizeof(ram_contents) / sizeof(uint8_t);

const int h_crunch = 64;
const int v_crunch = 64;
const int h_shift = 0;
const int h_trim = 0;

// Video timing: 640x480 VGA @ 60Hz
#define DOT_CLOCK 25175000        // Hz
const double dot_clock_freq       = DOT_CLOCK * 1e-6; // MHz

const int h_visible_area          = 640-2*h_crunch-h_trim;              // pixels
const int h_front_porch           = 16+h_crunch-h_shift+h_trim;               // pixels
const int h_sync_width            = 96;               // pixels
const int h_back_porch            = 48+h_crunch+h_shift;               // pixels
const int h_polarity              = -1;               // sign

const int v_visible_area          = 480-2*v_crunch;              // lines
const int v_front_porch           = 10+v_crunch;               // lines
const int v_sync_width            = 2;                // lines
const int v_back_porch            = 33+v_crunch;               // lines
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
const double timer_freq           = dot_clock_freq / 2;   // MHz

// Set by interupt handler if Arduino port is written to
volatile bool data_in_port = false;

// Set by pin change handler
volatile uint8_t ps2_data = 0;
volatile bool ps2_changed = true;

// CPU frequency is half the dot clock. Configure this and include delay header.
#define F_CPU (DOT_CLOCK/2)

// Baud rate of serial
#define BAUD 9600

#include <util/delay.h>
#include <util/setbaud.h>

#define PASTER(x,y) x ## y
#define EVALUATOR(x, y) PASTER(x, y)

#define set_pin_output(port_name, port_bit) do { \
  EVALUATOR(DDR, port_name) |= _BV(EVALUATOR(PORT, EVALUATOR(port_name, port_bit))); \
} while(0)

#define set_pin_input(port_name, port_bit) do { \
  EVALUATOR(DDR, port_name) &= ~(_BV(EVALUATOR(PORT, EVALUATOR(port_name, port_bit)))); \
} while(0)

#define set_pin(port_name, port_bit) do { \
  EVALUATOR(PORT, port_name) |= _BV(EVALUATOR(PORT, EVALUATOR(port_name, port_bit))); \
} while(0)

#define reset_pin(port_name, port_bit) do { \
  EVALUATOR(PORT, port_name) &= ~_BV(EVALUATOR(PORT, EVALUATOR(port_name, port_bit))); \
} while(0)

#define read_pin(port_name, port_bit) (EVALUATOR(PIN, port_name) & _BV(EVALUATOR(PIN, EVALUATOR(port_name, port_bit))))

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

static FILE uart_output;
static FILE uart_input;

void set_data(uint8_t data);

int uart_putchar(char c, FILE*) {
    loop_until_bit_is_set(UCSR0A, UDRE0); /* Wait until data register empty. */
    UDR0 = c;
    return 0;
}

int uart_getchar(FILE*) {
    loop_until_bit_is_set(UCSR0A, RXC0); /* Wait until data exists. */
    return UDR0;
}

void uart_init(void) {
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;

#if USE_2X
    UCSR0A |= _BV(U2X0);
#else
    UCSR0A &= ~(_BV(U2X0));
#endif

    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */
    UCSR0B = _BV(RXEN0) | _BV(TXEN0);   /* Enable RX and TX */

    fdev_setup_stream(&uart_output, uart_putchar, NULL, _FDEV_SETUP_WRITE);
    fdev_setup_stream(&uart_input, NULL, uart_getchar, _FDEV_SETUP_READ);
    stdout = &uart_output;
    stdin = &uart_input;
}

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
  reset_pin(ARD_OVR_BAR_PORT_NAME, ARD_OVR_BAR_PORT_BIT);
}

void release_read_dev() {
  set_pin(ARD_OVR_BAR_PORT_NAME, ARD_OVR_BAR_PORT_BIT);
}

// Set data bus value. Output depends on state of BUS_OE_BAR pin.
void set_data(uint8_t data) {
  // the code here is slightly awkward because we want to leave SR_CLK high
  // after clocking in data so that writing to the arduino port works.
  reset_pin(SR_MODE_PORT_NAME, SR_MODE_PORT_BIT); // shifting data
  reset_pin(SR_CLK_PORT_NAME, SR_CLK_PORT_BIT); // prepare to clock data

  for(uint8_t i=0; i<8; ++i) {
    reset_pin(SR_CLK_PORT_NAME, SR_CLK_PORT_BIT);
    if(data & (1<<(7-i))) {
      set_pin(SR_SER_PORT_NAME, SR_SER_PORT_BIT);
    } else {
      reset_pin(SR_SER_PORT_NAME, SR_SER_PORT_BIT);
    }
    set_pin(SR_CLK_PORT_NAME, SR_CLK_PORT_BIT);
  }

  set_pin(SR_CLK_PORT_NAME, SR_CLK_PORT_BIT);
  set_pin(SR_MODE_PORT_NAME, SR_MODE_PORT_BIT); // ready for CPU to write to port
}

// Read value in shift reg. Destroy shift reg contents replacing it with all
// zeros.
uint8_t read_data() {
  uint8_t data = 0;

  // the code here is slightly awkward because we want to leave SR_CLK high
  // after clocking in data so that writing to the arduino port works.
  reset_pin(SR_MODE_PORT_NAME, SR_MODE_PORT_BIT); // shifting data
  reset_pin(SR_CLK_PORT_NAME, SR_CLK_PORT_BIT); // prepare to clock data
  reset_pin(SR_SER_PORT_NAME, SR_SER_PORT_BIT);

  for(uint8_t i=0; i<8; ++i) {
    reset_pin(SR_CLK_PORT_NAME, SR_CLK_PORT_BIT);
    data <<= 1;
    if(read_pin(SR_OUT_PORT_NAME, SR_OUT_PORT_BIT)) { data |= 1; }
    set_pin(SR_CLK_PORT_NAME, SR_CLK_PORT_BIT);
  }

  set_pin(SR_CLK_PORT_NAME, SR_CLK_PORT_BIT);
  set_pin(SR_MODE_PORT_NAME, SR_MODE_PORT_BIT); // ready for CPU to write to port

  return data;
}

// *OUTSIDE OF INITIAL MEMORY LOAD* call this to disable Arduino reads from port
// while setting/reading data
void disable_arduino_port() {
  set_pin(ARD_RD_BAR_PORT_NAME, ARD_RD_BAR_PORT_BIT);
  set_pin_output(ARD_RD_BAR_PORT_NAME, ARD_RD_BAR_PORT_BIT);
}

// *OUTSIDE OF INITIAL MEMORY LOAD* call this to enable Arduino reads from port
void enable_arduino_port() {
  set_pin_input(ARD_RD_BAR_PORT_NAME, ARD_RD_BAR_PORT_BIT);
}

// Override crystal-based clock. CPU clock is now OV_CLK output.
void claim_clock() {
  reset_pin(CLK_SEL_PORT_NAME, CLK_SEL_PORT_BIT);
  reset_pin(OV_CLK_PORT_NAME, OV_CLK_PORT_BIT);
}

// Release CPU clock. Clock is now crystal.
void release_clock() {
  set_pin(OV_CLK_PORT_NAME, OV_CLK_PORT_BIT);
  set_pin(CLK_SEL_PORT_NAME, CLK_SEL_PORT_BIT);
}

void tick_update() {
  static bool prev_rd = false;
  bool rd = !read_pin(ARD_RD_BAR_PORT_NAME, ARD_RD_BAR_PORT_BIT);
  if(!rd && prev_rd) {
    copy_loop_advance();
    set_data(next_byte);
  }
  prev_rd = rd;
}

// Set reset. Note that reset must be active for a minimum of 3 cycles.
// Should be called with clock claimed to have effect.
void reset_on() {
  reset_pin(RST_BAR_PORT_NAME, RST_BAR_PORT_BIT);
  set_data(0x00);
  for(int i=0; i<5; ++i) {
    set_pin(OV_CLK_PORT_NAME, OV_CLK_PORT_BIT);
    reset_pin(OV_CLK_PORT_NAME, OV_CLK_PORT_BIT);
  }
}

// Release reset.
void reset_off() {
  set_pin(RST_BAR_PORT_NAME, RST_BAR_PORT_BIT);
}

// Tick the CPU clock
void tick() {
  //_delay_ms(200);
  set_pin(OV_CLK_PORT_NAME, OV_CLK_PORT_BIT);
  //_delay_ms(200);
  reset_pin(OV_CLK_PORT_NAME, OV_CLK_PORT_BIT);
  tick_update();
}

// Called around the middle of the HSYNC pulse which will be after TCNT1 update.
// Use the value of TCNT1 (the row counter) to determine when to trigger a
// vertical interrupt.
ISR(TIMER0_OVF_vect) {
  // Note: TCNT1 == 0 implies start of VSYNC pulse which explains why all of
  // these values are offset by v_front_porch.
  switch(TCNT1) {
    case v_sync_width + v_back_porch:
      // Enable VIS  pulse output for this line
      TCCR2A |= (_BV(COM2B1) | _BV(COM2B0));
      break;
    case v_sync_width + v_back_porch + v_visible_area:
      // Disable VIS pulse output for this line
      TCCR2A &= ~(_BV(COM2B1) | _BV(COM2B0));
      break;
    case v_sync_width + v_back_porch - 2:
      // Our video driver takes two lines to respond to the interrupt. We delay
      // a few microseconds to make sure the Z80 has time to register the
      // interrupt. This is tyo avoid having HW to latch the INT_BAR line and
      // set it after INTACK_BAR.
      //
      // The longest time the Z80 will take to respond is the length of the
      // longest instruction, 23 t-states. At the 25.175MHz dot clock this is
      // appproximately 1 µs. Wait 2 to make sure.
      reset_pin(CPU_INT_BAR_PORT_NAME, CPU_INT_BAR_PORT_BIT);
      _delay_us(2);
      set_pin(CPU_INT_BAR_PORT_NAME, CPU_INT_BAR_PORT_BIT);
      break;
  }
}

ISR(INT0_vect) {
  data_in_port = true;
}

ISR(PCINT0_vect) {
  static bool prev_ps2_clock = true; // high
  static bool ps2_clk = true; // high
  static uint8_t n_bits = 0;
  static uint8_t parity = 0;
  static uint16_t data = 0;

  ps2_clk = read_pin(PS2_CLK_PORT_NAME, PS2_CLK_PORT_BIT);

  // Read data on *falling* edge
  if((ps2_clk != prev_ps2_clock) && !ps2_clk) {
    bool data_pin = read_pin(PS2_DATA_PORT_NAME, PS2_DATA_PORT_BIT);
    if((n_bits != 0) || !data_pin) {
      data >>= 1;
      if(data_pin) {
        data |= 1<<15;
        ++parity;
      }
      ++n_bits;

      // After 11 bits, data should look like:
      //
      // 1 <parity> <8 x data> 000000

      if(n_bits == 11) {
        // Odd parity plus stop bit means parity should be *even* and high bit
        // should be set.
        if(!(parity & 0x1) && data_pin) {
          ps2_data = data >> 6;
          ps2_changed = true;
        }
        n_bits = 0;
        parity = 0;
      }
    }
  }
  prev_ps2_clock = ps2_clk;
}

void setup_timers() {
  // Reset all timers and halt them
  GTCCR = _BV(TSM) | _BV(PSRASY) | _BV(PSRSYNC);

  // Timers 0 and 2:
  // We use phase correct operation so the counter counts up from BOTTOM to TOP
  // and back down. Both BOTTOM and TOP are present for one counter cycle so the
  // number of cycles for each period is 2*(TOP-BOTTOM).
  //
  // BOTTOM is 0 and TOP and so the period, T, is 2*TOP/f where f is the counter
  // frequency. Therefore:
  const int top = whole_line * timer_freq / 2.;
  OCR0A = top;
  OCR2A = top;

  // We use the phase-correct PWM mode so the pulse width will be 2*COMPARE/f
  // and so, COMPARE = width * f / 2.

  // Timer 0: HSYNC

  // Set HSYNC pulse width,
  OCR0B = (int)(h_sync_width_t * timer_freq / 2.);

  // Phase correct PWM, TOP = OCR0A, internal clock, no pre-scaling,
  // OC0B pin is RESET when match on counting UP and SET when match on counting
  // DOWN for +ve HSYNC polarity. (Opposite for -ve.)
  TCCR0A = _BV(COM0B1) | _BV(WGM00) | ((h_polarity < 0) ? _BV(COM2B0) : 0);
  TCCR0B = _BV(WGM02) | _BV(CS00);

  // Timer 1: VSYNC

  // T1 == OC0B which is HSYNC so we can use it as a clock which is a happy
  // co-incidence.

  // TOP/OCR1A is set to the total size of the frame in lines minus 1 since it
  // is zero based.
  OCR1A = whole_frame - 1;

  // OCR1B is set to the pulse width minus 1 since it is zero based.
  OCR1B = v_sync_width - 1;

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
  if(h_polarity < 0) {
    // Clock on falling edge of T1.
    TCCR1B |= _BV(CS12) | _BV(CS11);
  } else {
    // Clock on rising edge of T1.
    TCCR1B |= _BV(CS12) | _BV(CS11) | _BV(CS10);
  }

  // Timer 2: VISB

  // We set the display inactive pulse width because that pulse is nearly
  // aligned to HSYNC already.
  OCR2B = (int)(hvis_inactive_width * timer_freq / 2.);

  // Phase correct PWM, TOP = OCR2A, no clock, no pre-scaling,
  // OC2B pin is RESET when match on counting UP and SET when match on counting
  // DOWN (see timer 0 overflow handler).
  TCCR2A = _BV(WGM20);
  TCCR2B = _BV(WGM22) | _BV(CS20);

  // set default VIS value for when VIS is disabled
  reset_pin(VIS_PORT_NAME, VIS_PORT_BIT);

  // The centre points of the HSYNC and VIS pulses are *nearly* aligned
  // already. However, the HSYNC pulse is not quite in the middle of the
  // inactive VIS region so we offset the inactive pulse from the HSYNC pulse.
  TCNT0 = 0;
  TCNT1 = 0; // corresponds to start of vsync pulse
  TCNT2 = 256-(int)(hvis_inactive_offset * timer_freq);

  // Enable OVF interrupt for timer 0
  TIMSK0 |= _BV(TOIE0);

  // start timer
  GTCCR = 0;
}

void setup_pin_interrupts() {
  // INT0 interrupt triggered by rising edge on ARD_WR_BAR pin.
  EICRA = _BV(ISC01) | _BV(ISC00);

  // PIN0 interrupt triggered by changes on PCINT7
  PCMSK0 = _BV(PCINT7);

  // Enable interrupts
  EIMSK = _BV(INT0);
  PCICR = _BV(PCIE0);
}

void setup() {
  // disable interrupts while performing setup
  cli();

  // Set video timing outputs
  set_pin_output(HSYNC_PORT_NAME, HSYNC_PORT_BIT);
  set_pin_output(VSYNC_PORT_NAME, VSYNC_PORT_BIT);
  set_pin_output(VIS_PORT_NAME, VIS_PORT_BIT);

  // Arduino write port interrupt
  set_pin_input(ARD_WR_BAR_PORT_NAME, ARD_WR_BAR_PORT_BIT);

  setup_timers();

  setup_pin_interrupts();

  // Reset memory image copy code loop
  copy_loop_reset();

  // Configure UART
  uart_init();

  // re-enable interrupts
  sei();

  // Setup CPU control lines
  set_pin_input(ARD_RD_BAR_PORT_NAME, ARD_RD_BAR_PORT_BIT);
  set_pin(CPU_INT_BAR_PORT_NAME, CPU_INT_BAR_PORT_BIT);
  set_pin_output(CPU_INT_BAR_PORT_NAME, CPU_INT_BAR_PORT_BIT);

  // Setup clock override pins
  set_pin_output(OV_CLK_PORT_NAME, OV_CLK_PORT_BIT);
  set_pin_output(CLK_SEL_PORT_NAME, CLK_SEL_PORT_BIT);

  // Ensure we are the read device and set up data bus
  set_pin_input(SR_OUT_PORT_NAME, SR_OUT_PORT_BIT);
  reset_pin(SR_SER_PORT_NAME, SR_SER_PORT_BIT);
  set_pin_output(SR_SER_PORT_NAME, SR_SER_PORT_BIT);
  set_pin(SR_CLK_PORT_NAME, SR_CLK_PORT_BIT);
  set_pin_output(SR_CLK_PORT_NAME, SR_CLK_PORT_BIT);
  set_pin_output(ARD_OVR_BAR_PORT_NAME, ARD_OVR_BAR_PORT_BIT);
  claim_read_dev();
  set_pin(SR_MODE_PORT_NAME, SR_MODE_PORT_BIT); // loading data
  set_pin_output(SR_MODE_PORT_NAME, SR_MODE_PORT_BIT);

  // Ensure CPU is in RESET state.
  reset_pin(RST_BAR_PORT_NAME, RST_BAR_PORT_BIT);
  set_pin_output(RST_BAR_PORT_NAME, RST_BAR_PORT_BIT);

  claim_clock();
  reset_on();
  reset_off();
  set_pin(RST_BAR_PORT_NAME, RST_BAR_PORT_BIT);

  do {
    tick();
  } while(ram_bytes_sent < ram_contents_len);

  reset_on();
  release_read_dev();

  release_clock();

  set_data(0x00);
  data_in_port = false;

  reset_off();
}

void loop() {
  uint8_t kbd_buffer = 0x00;
  bool kbd_buffer_empty = true;
  uint8_t next_byte = 0x00;

  while(1) {
    if(ps2_changed) {
      if(kbd_buffer_empty) {
        kbd_buffer = ps2_data;
        kbd_buffer_empty = false;
        ps2_changed = false;
      }
    }

    if(data_in_port) {
      disable_arduino_port();
      uint8_t data = read_data();

      //printf("read data: 0x%02x\r\n", data);

      // reponse nibble from command
      uint8_t response = 0;

      switch(data) {
        case 0x40: // get next data
          response = next_byte & 0x0f;
          next_byte >>= 4;
          break;
        case 0x41: // read kbd
          next_byte = kbd_buffer;
          response = kbd_buffer_empty ? 0x00 : 0x01;
          kbd_buffer_empty = true;
          break;
        default:   // unknown command, ignore
          break;
      }

      set_data(0x80 | (response & 0x0f));

      data_in_port = false;
      enable_arduino_port();
    }
  }
}

int main(void) {
  // setup pins, timers, etc.
  setup();

  while(1) { loop(); }
  return 0;
}

// vim:sw=2:sts=2:et
