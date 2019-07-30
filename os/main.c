static __sfr __at 0x0 arduino_port;
static __sfr __at 0x1 port_a;

void main(void) {
  unsigned char i;
  unsigned int j;
  __asm__("im 1"); // mode 1 interrupts, jump to 0x0038
  __asm__("ei");

  for(;;++i) {
    //arduino_port = port_a;
    arduino_port = i;
    //port_a = arduino_port;
    //port_a = i;
    j=0; do { ++j; } while(j!=0x5000);
  }
}
