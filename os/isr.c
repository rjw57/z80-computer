// Called once per frame
void isr(void) __critical __interrupt(0) {
  static unsigned int i=0;
  static __sfr __at 0x1 port_a;
  static __sfr __at 0x0 io_port;
  port_a = (++i) >> 2;
}
