void main(void) {
  __asm__("im 2"); // mode 2 interrupts
  __asm__("ld a, #0"); // set I register to high byte of ISR
  __asm__("ld I, a");
  __asm__("ei");

  while(1) {
    __asm__("halt");
  }
}
