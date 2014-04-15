#include <Wire.h>
/* Pinouts */
#define panDir 12
#define panPWM 3
#define panBrake 9
#define DLED 13


void setup(){
  Serial.begin(9600);//Interface to computer
  Wire.begin();
  pinMode(13,OUTPUT);
}

void loop(){
  while(1){
    int data = Serial.read();
    Wire.beginTransmission(data);
    Wire.write(data);
    Wire.endTransmission();
    delay(500);
  }
  
  
  
  
  /* Wait for packet */
  /*
  while(1){
    //int temp = Serial.available();
    //int data = 0;
    //while(data != -1) data = Serial.read();
    //digitalWrite(13,HIGH);
    int temp = Serial.read();
    delay(2000);
    Serial1.write(temp);
    delay(2000);
    //digitalWrite(13,LOW);
    //delay(1000);
  }
  if(Serial.available() >= 5){//What if header is not first byte in buffer?
    Serial1.write(0x55);
    Serial.println("bytes found");
    if(Serial.read() != 0x80) return;
    digitalWrite(DLED,HIGH);
    byte device = Serial.read();
    byte cmd0 = Serial.read();
    byte cmd1 = Serial.read();
    byte cmd2 = Serial.read();
    byte cmd3 = Serial.read();
    Serial.print("Packet:");
    Serial.print(device);
    Serial.print(",");
    Serial.print(cmd0);
    Serial.print(",");
    Serial.print(cmd1);
    Serial.print(",");
    Serial.print(cmd2);
    Serial.print(",");
    Serial.print(cmd3);
    Serial.print('\n');
    */
    /* Strip header, send 5-byte packet to device */
    /*
    if(device == 0x10){      //LED Controller
      Serial1.write(device);//address
      delay(10);
      Serial1.write(cmd0);  //Red LED
      delay(10);
      Serial1.write(cmd1);  //Green LED
      delay(10);
      Serial1.write(cmd2);  //Blue LED
      delay(10);
      Serial1.write(cmd3);  //Servo
      delay(10);
    } else if(device == 0x20){//Servo Controller
      Serial2.write(device);//address
      delay(10);
      Serial2.write(cmd0);  //Servo1
      delay(10);
      Serial2.write(cmd1);  //Servo2
      delay(10);
      Serial2.write(cmd2);  //Servo3
      delay(10);
      Serial3.write(cmd3);  //Servo4
      delay(10);
    } else if(device == 0x30){//Motor controller (same chip)
      digitalWrite(panDir, cmd0);
      analogWrite(panPWM, cmd1);
      digitalWrite(panBrake, cmd2);
      //Discard last byte
    } else {
      Serial.println("Invalid device");
    }
    digitalWrite(DLED,LOW);
  }
  if(Serial1.available() >= 1){
    Serial.print("Serial1:");
    Serial.print(Serial1.read(),HEX);
    Serial.print('\n');
  }
  if(Serial2.available() >= 1){
    Serial.print("Serial2:");
    Serial.print(Serial2.read(),HEX);
    Serial.print('\n');
  }
  */
}
      
      
      
  
