#include "AtomWire.h"

#define NUM_LINES 4

OneWire **awms;

void setup(void) {
  Serial.begin(9600); 

  awms = new OneWire*[NUM_LINES];

  uint8_t offset = 10;
  uint8_t i;
  for (i = 0; i < NUM_LINES; i++) {  
    awms[i] = new OneWire(i + offset);
  }
}

void loop(void) {
  
  byte i;
  byte present = 0;
  byte data[12];
  byte addr[8];
  OneWire *awm;

  for (int j = 0; j < NUM_LINES; j++) {
    Serial.print("\nLine ");
    Serial.print(j + 1);
    Serial.print(" Nodes:\n");
    delay(1000);
    //awm = awms[j];
    while(awms[j]->search(addr)){
      Serial.print("\nAddress = ");
      for(i = 0; i < 8; i++) {
        Serial.print(addr[i], HEX);
        Serial.print(" ");
      }
  
      if ( addr[0] == 0x3A) 
          Serial.print("\nDevice belongs to AtomWire family.\n");
    
      else {
          Serial.print("\nDevice family is not recognized.\n");
          return;
      }
      delay(1000);
    }
  
    Serial.print("\nL");
    Serial.print(j + 1);
    Serial.print(": No further Slaves Detected.\n");
    awms[j]->reset_search();
  }

}




