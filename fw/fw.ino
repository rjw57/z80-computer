#define BUS_SER         5
#define BUS_OE_BAR      4
#define BUS_RCLK        3
#define BUS_SRCLK       2

#define RST_BAR         A5
#define CPU_RD_BAR      A4
#define CLK_SEL         A0
#define OV_CLK          A1
#define CPU_A0          10
#define CPU_A1          11
#define CPU_A2          12

static bool asserting_data_bus = false;

static bool send_init_loop = true;
static uint16_t init_sp = 0x1020;
static uint8_t next_bytes[] = { 0x00, 0x00 };

// The effective code "seen" by the processor is as follows. For the first loop:
//
// 0000 LOOP:
// 0000 31 XX YY              LD   sp,$YYXX
// 0003 00                    NOP
// 0004 18 FA                 JR   LOOP
//
// For subsequent loops:
//
// 0000 LOOP:
// 0000 21 XX YY              LD   hl,$YYXX
// 0003 E5                    PUSH hl
// 0004 18 FA                 JR   LOOP
uint8_t lookup_code(uint8_t addr) {
  switch(addr) {
    case 0x00:
      return send_init_loop ? 0x31 : 0x21;
    case 0x01:
      return send_init_loop ? (init_sp & 0xff) : next_bytes[0];
    case 0x02:
      return send_init_loop ? ((init_sp >> 8) & 0xff) : next_bytes[1];
    case 0x03:
      return send_init_loop ? 0x00 : 0xe5;
    case 0x04:
      return 0x18;
    case 0x05:
      return 0xfa;
    default:
      return 0x00;
  }
}

uint8_t read_addr() {
  uint8_t addr = 0x00;
  if(digitalRead(CPU_A0) == HIGH) { addr |= 0x01; }
  if(digitalRead(CPU_A1) == HIGH) { addr |= 0x02; }
  if(digitalRead(CPU_A2) == HIGH) { addr |= 0x04; }
  return addr;
}

// Set data bus value. Output depends on state of BUS_OE_BAR pin.
void set_data(uint8_t data) {
  shiftOut(BUS_SER, BUS_SRCLK, MSBFIRST, data);
  digitalWrite(BUS_RCLK, HIGH);
  digitalWrite(BUS_RCLK, LOW);
}

void update_bus_assertion() {
  bool should_assert = (
      asserting_data_bus && (digitalRead(CPU_RD_BAR) == LOW) &&
      (digitalRead(CLK_SEL) == LOW)
  );
  digitalWrite(BUS_OE_BAR, should_assert ? LOW : HIGH);
}

void assert_data_bus() {
  asserting_data_bus = true;
  update_bus_assertion();
}

void release_data_bus() {
  asserting_data_bus = false;
  update_bus_assertion();
}

// Override crystal-based clock. CPU clock is now OV_CLK output.
void claimClock() {
  digitalWrite(OV_CLK, LOW);
  digitalWrite(CLK_SEL, LOW);
}

// Release CPU clock. Clock is now crystal.
void releaseClock() {
  digitalWrite(CLK_SEL, HIGH);
}

void tick_update() {
  update_bus_assertion();
  uint8_t addr = read_addr();
  set_data(lookup_code(addr));
  if((digitalRead(CPU_RD_BAR) == LOW) && (addr >= 0x04)) {
    send_init_loop = false;
  }
}

// Tick the CPU clock
void tick() {
  digitalWrite(OV_CLK, HIGH);
  tick_update();
  digitalWrite(OV_CLK, LOW);
  tick_update();
}

void setup() {
  // Setup address bus
  pinMode(CPU_A0, INPUT);
  pinMode(CPU_A1, INPUT);
  pinMode(CPU_A2, INPUT);

  // Setup clock override pins
  pinMode(OV_CLK, OUTPUT);
  pinMode(CLK_SEL, OUTPUT);

  // Ensure bus is not being asserted.
  digitalWrite(BUS_SER, LOW);
  pinMode(BUS_SER, OUTPUT);
  digitalWrite(BUS_OE_BAR, HIGH);
  pinMode(BUS_RCLK, OUTPUT);
  digitalWrite(BUS_RCLK, LOW);
  pinMode(BUS_OE_BAR, OUTPUT);
  digitalWrite(BUS_SRCLK, LOW);
  pinMode(BUS_SRCLK, OUTPUT);

  // Ensure CPU is in RESET state.
  digitalWrite(RST_BAR, LOW);
  pinMode(RST_BAR, OUTPUT);

  // Output NOPs
  set_data(0x00);
  assert_data_bus();

  claimClock();
  digitalWrite(RST_BAR, HIGH);

  while(1) {
    tick();
  //  delay(100);
  }

  releaseClock();
}

void loop() {}

// vim:sw=2:sts=2:et
