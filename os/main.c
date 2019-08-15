static __sfr __at 0x80 arduino_port;
static __sfr __at 0x81 port_a;

void main(void) {
  unsigned char i;
  unsigned int j, k;

  port_a = 0;

#if 0
  i = 0;
  while(1) {
    port_a = ++i;
    j=0; do { ++j; } while(j!=0x5000);
  }
#endif

  __asm__("im 1"); // mode 1 interrupts, jump to 0x0038
  __asm__("ei");

  unsigned char* base = (unsigned char*)0x2000;
  for(i=0; i<360/2; ++i, base+=128) {
    for(k=0; k<64; ++k) {
      base[k] = 0x55;
      base[k+64] = 0xaa;
    }
  }

  i = 0;
  while(1) {
    port_a = ++i;
    j=0; do { ++j; } while(j!=0x5000);
  }

  i = 0x01;
  //i = 0x55;
  while(1) {
    unsigned char n=1<<(i&0x7);
    i<<=1;
    if(i == 0) { i = 0x01; }
    //i = ~i;
    j=0; do { ++j; } while(j!=0x1000);
  }

  for(;;++i) {
    //arduino_port = port_a;
    arduino_port = i;
    //port_a = arduino_port;
    //port_a = i;
    j=0; do { ++j; } while(j!=0x5000);
  }
}
