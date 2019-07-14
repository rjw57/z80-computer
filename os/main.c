void main(void) {
  int i, j;
  static __sfr __at 0x10 io_port;

  /*
  __asm__("im 2"); // mode 2 interrupts
  __asm__("ld a, #0"); // set I register to high byte of ISR
  __asm__("ld I, a");
  */
  __asm__("ei");

  for(i=0; i<0x100; ++i) {
    io_port = i;
  }

  while(1) {
    __asm__("halt");
  }
}
