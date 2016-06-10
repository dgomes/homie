#include <UIPEthernet.h>
#include <homie.h>

IPAddress server(192,168,1,10);
int server_port = 1883;
uint8_t mac[6] = {0x72,0x00,0x05,0xbd,0x83,0xe1};

EthernetClient ethClient;
Homie homie(ethClient);

void setup() {
  // put your setup code here, to run once:
  homie.setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  homie.loop(); 
}
