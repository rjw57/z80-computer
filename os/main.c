void main(void) {
  unsigned int i, j;
  static __sfr __at 0x1 port_a;
  static __sfr __at 0x0 io_port;

  /*
  __asm__("im 2"); // mode 2 interrupts
  __asm__("ld a, #0"); // set I register to high byte of ISR
  __asm__("ld I, a");
  */
  __asm__("ei");

  port_a = 0;

  for(;;++i) {
    port_a = i & 0xff;
    // io_port = i & 0xff;

    j=0; do { } while(++j != 0x1000);
  }

  while(1) {
    __asm__("halt");
  }
}
