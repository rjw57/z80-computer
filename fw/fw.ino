#define RST_BAR         A5

#define BUS_SER         5
#define BUS_OE_BAR      4
#define BUS_RCLK        3
#define BUS_SRCLK       2

#define CPU_RD_BAR      A4
#define CPU_WR_BAR      A3
#define CPU_BUSRQ_BAR   A2
#define CPU_BUSAK_BAR   A1

#define BUS_CLAIMED     13

// Claims the CPU bus. A0-15 and D0-7 will be HiZ. RD and WR bar are outputs set
// high.
void claim() {
  digitalWrite(CPU_RD_BAR, HIGH);
  digitalWrite(CPU_WR_BAR, HIGH);
  digitalWrite(CPU_BUSRQ_BAR, LOW);

  while(digitalRead(CPU_BUSAK_BAR) == HIGH) {}

  pinMode(CPU_RD_BAR, OUTPUT);
  pinMode(CPU_WR_BAR, OUTPUT);
  digitalWrite(BUS_CLAIMED, HIGH);

  // Start asserting bus
  digitalWrite(BUS_SER, LOW);
  digitalWrite(BUS_OE_BAR, LOW);
  digitalWrite(BUS_RCLK, LOW);
  digitalWrite(BUS_SRCLK, LOW);
}

// Releases the CPU bus. RD and WR bar are inputs.
void release() {
  pinMode(CPU_RD_BAR, INPUT_PULLUP);
  pinMode(CPU_WR_BAR, INPUT_PULLUP);
  digitalWrite(CPU_RD_BAR, HIGH);
  digitalWrite(CPU_WR_BAR, HIGH);
  digitalWrite(CPU_BUSRQ_BAR, HIGH);

  // Ensure we're not asserting bus
  digitalWrite(BUS_SER, LOW);
  digitalWrite(BUS_OE_BAR, HIGH);
  digitalWrite(BUS_RCLK, LOW);
  digitalWrite(BUS_SRCLK, LOW);

  while(digitalRead(CPU_BUSAK_BAR) == LOW) {}

  digitalWrite(BUS_CLAIMED, LOW);
}

// Asserts data and address bus values. Only has an effect between calls to
// claim()/release().
void assert_bus(uint8_t data, uint16_t address) {
  shiftOut(BUS_SER, BUS_SRCLK, MSBFIRST, data);
  shiftOut(BUS_SER, BUS_SRCLK, MSBFIRST, address >> 8);
  shiftOut(BUS_SER, BUS_SRCLK, MSBFIRST, address & 0xff);
  digitalWrite(BUS_RCLK, HIGH);
  digitalWrite(BUS_RCLK, LOW);
}

void setup() {
  // Bus claim LED
  digitalWrite(BUS_CLAIMED, LOW);
  pinMode(BUS_CLAIMED, OUTPUT);

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

  // Claim bus and wait for claim
  digitalWrite(CPU_BUSRQ_BAR, LOW);
  pinMode(CPU_BUSRQ_BAR, OUTPUT);
  pinMode(CPU_BUSAK_BAR, INPUT_PULLUP);

  // Start processor
  digitalWrite(RST_BAR, HIGH);

  // Claim bus
  claim();

  // Clear memory
  for(uint16_t addr=0; addr<0x8000; ++addr) {
    switch(addr) {
      case 0x7FFF:
        assert_bus(0x76, addr); // halt
        break;
      default:
        assert_bus(0x00, addr); // nop
        break;
    }
    digitalWrite(CPU_WR_BAR, LOW);
    digitalWrite(CPU_WR_BAR, HIGH);
  }

  // Release bus resetting CPU
  digitalWrite(RST_BAR, LOW);
  release();
  delay(10);
  digitalWrite(RST_BAR, HIGH);
}

void loop() {}

// vim:sw=2:sts=2:et
