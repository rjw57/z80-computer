void main(void) {
  __asm__("im 1"); // mode 1 interrupts, jump to 0x0038
  __asm__("ei");

  while(1) { }
}
