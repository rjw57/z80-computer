static __sfr __at 0x0 arduino_port;
static __sfr __at 0x1 port_a;

void main(void) {
  unsigned char i;
  unsigned int j, k;

  port_a = 0;

  __asm__("im 1"); // mode 1 interrupts, jump to 0x0038
  __asm__("ei");

  i = 0x55;
  while(1) {
    unsigned char n=1<<(i&0x7);
    for(k=0; k<255; ++k) {
      ((unsigned char*)0x4000)[k] = i;
      //j=0; do { ++j; } while(j!=0x5000);
    }
    i = ~i;
    j=0; do { ++j; } while(j!=0x5000);
  }

  for(;;++i) {
    //arduino_port = port_a;
    arduino_port = i;
    //port_a = arduino_port;
    //port_a = i;
    j=0; do { ++j; } while(j!=0x5000);
  }
}
