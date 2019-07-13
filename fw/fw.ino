#include <avr/pgmspace.h>

#define IO_ACK_BAR      9
#define SR_CLK          8
#define SR_MODE         7
#define SR_OUT          6
#define SR_SER          5
#define RD_DEV          4

#define CPU_IOREQ_BAR   2

#define RST_BAR         A5
#define CPU_RD_BAR      A4
#define CLK_SEL         A0
#define OV_CLK          A1

// RAM contents
#define sram_bin ram_contents
#include "sram_contents.h"
#undef sram_bin

const uint16_t ram_contents_len = sizeof(ram_contents) / sizeof(uint8_t);

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
  digitalWrite(RD_DEV, LOW);
}

void release_read_dev() {
  digitalWrite(RD_DEV, HIGH);
}

// Set data bus value. Output depends on state of BUS_OE_BAR pin.
void set_data(uint8_t data) {
  digitalWrite(SR_MODE, LOW); // shifting data
  shiftOut(SR_SER, SR_CLK, MSBFIRST, data);
}

// Read data bus value.
uint8_t read_data() {
  digitalWrite(SR_MODE, HIGH); // load
  digitalWrite(SR_CLK, HIGH);
  digitalWrite(SR_CLK, LOW);
  digitalWrite(SR_MODE, LOW); // read
  return shiftIn(SR_OUT, SR_CLK, MSBFIRST);
}

// Override crystal-based clock. CPU clock is now OV_CLK output.
void claim_clock() {
  digitalWrite(OV_CLK, LOW);
  digitalWrite(CLK_SEL, LOW);
}

// Release CPU clock. Clock is now crystal.
void release_clock() {
  digitalWrite(CLK_SEL, HIGH);
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

void io_request_handler() {
  // TODO: BROKEN

  // Resume CPU
  claim_clock();
  digitalWrite(IO_ACK_BAR, LOW);
  while(digitalRead(CPU_IOREQ_BAR) == LOW) {
    digitalWrite(OV_CLK, HIGH);
    digitalWrite(OV_CLK, LOW);
  }
  digitalWrite(IO_ACK_BAR, HIGH);
  release_clock();
}

void setup() {
  Serial.begin(9600);
  while (!Serial);  // wait for serial port to connect. Needed for native USB

  // Reset memory image copy code loop
  copy_loop_reset();

  // Setup CPU control lines
  pinMode(CPU_RD_BAR, INPUT);

  // Setup clock override pins
  pinMode(OV_CLK, OUTPUT);
  pinMode(CLK_SEL, OUTPUT);

  // Ensure we are the read device and set up data bus
  pinMode(SR_OUT, INPUT);
  digitalWrite(SR_SER, LOW);
  pinMode(SR_SER, OUTPUT);
  digitalWrite(SR_CLK, LOW);
  pinMode(SR_CLK, OUTPUT);
  claim_read_dev();
  pinMode(RD_DEV, OUTPUT);
  digitalWrite(SR_MODE, LOW); // shifting data
  pinMode(SR_MODE, OUTPUT);

  // Ensure CPU is in RESET state.
  digitalWrite(RST_BAR, LOW);
  pinMode(RST_BAR, OUTPUT);

  // Ensure CPU is not in WAIT state
  digitalWrite(IO_ACK_BAR, LOW);
  pinMode(IO_ACK_BAR, OUTPUT);
  digitalWrite(IO_ACK_BAR, HIGH);

  // Connect port IO handler
  pinMode(CPU_IOREQ_BAR, INPUT);
  //attachInterrupt(0, io_request_handler, FALLING);

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
  reset_off();
}

void loop() {
  /*
  static bool prev_ioreq = false;
  bool ioreq = digitalRead(CPU_IOREQ_BAR) == LOW;

  if(ioreq && !prev_ioreq) {
    io_request_handler();
    prev_ioreq = false;
    return;
  }

  prev_ioreq = ioreq;
  */
}

// vim:sw=2:sts=2:et
