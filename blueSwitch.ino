#include <SoftwareSerial.h>

SoftwareSerial Blue(4, 3); // RX, TX

#define RELAY_PIN 0
#define CAP_SEND 1
#define CAP_REC 2

int STATE = LOW;
int baseline = 0;
int touch = 0;

void setup() {
  Blue.begin(9600);
  pinMode(RELAY_PIN, OUTPUT);
  calibrate();
  
}

void loop() {
    if (Blue.available() > 0) {
      char r = Blue.read();
      if (r == 49 || r == 113 ) {
        on(true);
      } else if (r == 48 || r== 112) {
        off(true);
      } else if (r == 50 || r == 114) {
        toggle(true);
      } else if (r == 51 || r == 115) {
        stat();
      } else if (r == 'R') {
         calibrate(); 
      }
    }
    touch = readCapacitivePinAvg(30)-baseline;
    if (touch >= 4) {
      toggle(false);
      delay(1000);
    }
}

void on(boolean p) {
  STATE = HIGH;
  digitalWrite(0, HIGH);
  if (p) Blue.println("ON");
}

void off(boolean p) {
  STATE = LOW;
  digitalWrite(0,LOW);
  if (p) Blue.println("OFF");
}

void toggle(boolean p) {
  if (STATE == HIGH) {
    off(p);
  } else {
    on(p);
  }
}

void calibrate() {
  delay(1000);
  baseline = readCapacitivePinAvg(50);
}

void stat() {
  Blue.print("Status...");
  if (digitalRead(RELAY_PIN) == HIGH) {
    Blue.println("ON");
  } else {
    Blue.println("OFF");
  }
}

uint8_t readCapacitivePinAvg(int samples) {
  unsigned long total = 0;
  for(int i=0; i<samples; i++) {
    total += readCapacitivePin();
    delay(1);
  }
  return total/samples;
}

uint8_t readCapacitivePin(){
  uint8_t count = 0;
  
  pinMode(CAP_SEND,OUTPUT);
  digitalWrite(CAP_SEND,LOW);
  
  pinMode(CAP_REC,INPUT);
  digitalWrite(CAP_REC,LOW);
  pinMode(CAP_REC,OUTPUT);
  pinMode(CAP_REC,INPUT);
 
  digitalWrite(CAP_SEND,HIGH); 
  
  while(digitalRead(CAP_REC)==LOW) {
    count++;
  }
  
  digitalWrite(CAP_REC,HIGH);
  pinMode(CAP_REC,OUTPUT);
  pinMode(CAP_REC,INPUT);
  digitalWrite(CAP_REC,LOW);
  
  digitalWrite(CAP_SEND,LOW);

  while(digitalRead(CAP_REC)==HIGH) {
    count++;
  }
  
  return count;
  
}


