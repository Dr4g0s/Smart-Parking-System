
//Arduino Code

#include<SoftwareSerial.h>

SoftwareSerial toESP(2, 3);
const int Slots[] = {4,5,6,7,8,9,10,11};
int Distance;

void setup() {
  Serial.begin(9600);
  toESP.begin(9600);
}

void loop() {
  for(int x=0;x<8;x++){
     pinMode(Slots[x],OUTPUT);					//set pin mode to OUTPUT for TRIG
     digitalWrite(Slots[x], LOW);
     delayMicroseconds(2);
     digitalWrite(Slots[x], HIGH);
     delayMicroseconds(10);
     digitalWrite(Slots[x], LOW);
     
     pinMode(Slots[x],INPUT);					//set pin mode to OUTPUT for ECHO
     Distance = pulseIn(Slots[x], HIGH)/29.1/2; //Calculate Distance
     Serial.print("Distance ");
     Serial.print(x+1);
     Serial.print(" : ");
     Serial.print(Distance);					//Print Result in Arduino Serial Monitor
     Serial.println();
	 
     //Send Results to ESP Node MCU "toESP" Serial Monitor
     toESP.print(x+1);
     toESP.print(" : ");
     toESP.print(Distance);
     toESP.println("");
     delay(3000);
  }
}
