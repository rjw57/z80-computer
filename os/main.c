void main(void) {
  unsigned int i, j;
  static __sfr __at 0x7 port_a;
  static __sfr __at 0x0 io_port;

  /*
  __asm__("im 2"); // mode 2 interrupts
  __asm__("ld a, #0"); // set I register to high byte of ISR
  __asm__("ld I, a");
  */
  __asm__("ei");

  port_a = 0;

  while(1) {
    for(i=0; i<0x100; ++i) {
      //io_port = i;
      port_a = i;

      j=0; do { ++j; } while(j != 0);
    }
  }

  while(1) {
    __asm__("halt");
  }
}
