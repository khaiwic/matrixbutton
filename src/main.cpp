#include <Arduino.h>
#include <string.h>
#include "matrixbutton.h"

#define PHAI 4
#define TIEN 5
#define TRAI 6
#define XUONG 7

#define ARRAYY 37

int state = 0;

String save[ARRAYY];
int walk = 0;

enum State{IDLE, RECORD, PLAYING, ERROR_STATE};
State currentState = IDLE;

String lastscan = {"0"};
String lastkhai = {"0"};

unsigned long pressTimeState = 0;

void offled(){
  digitalWrite(PHAI, LOW);
  digitalWrite(TIEN, LOW);
  digitalWrite(TRAI, LOW);
  digitalWrite(XUONG, LOW);
}

void run(){
  String khai = scan();
  if(khai != "0"){
    if(khai == lastscan){
      if(millis() - pressTimeState > 5000){
        currentState = ERROR_STATE;
      }
      else{
        pressTimeState = millis();
        lastscan = khai;
      }
      lastscan = "0";
    }
    if(currentState == ERROR_STATE){
      digitalWrite(PHAI, HIGH);
      digitalWrite(TIEN, HIGH);
      digitalWrite(TRAI, HIGH);
      digitalWrite(XUONG,HIGH);
      delay(1000);
      offled();
      delay(400);
    }
    if(khai == "OK"){
      currentState = IDLE;
      walk = 0;
      Serial.println("EXIT ERROR");
      delay(300);
    }
    return;
  }
  if(khai != lastkhai){
    delay(50);
    khai = scan();
    if(khai != "0"){
      switch(currentState){
        case IDLE:
          Serial.println("Chế độ chờ");
          walk = 0;
          delay(400);
        case RECORD:
          if(khai == "OK"){
            currentState = PLAYING;
            Serial.println("Chuyến sang chế độ phát");
            delay(300);
          }
          else{
            if (walk > 0){
              String BuocTruoc = save[walk - 1];
              if((BuocTruoc == "XUONG" && save[walk] == "TIEN") ||
                (BuocTruoc == "PHAI" && save[walk] == "XUONG") ||
                (BuocTruoc == "TRAI" && save[walk] == "PHAI") ||
                (BuocTruoc == "XUONG" && save[walk] == "TIEN")){
                  currentState = ERROR_STATE;
                  break;
                }
            }
            if(walk < ARRAYY){
              save[walk] = khai;
              Serial.println("Bấm nút: ");
              Serial.println(save[walk]);
              walk++;
              delay(500);
            }
          }
          break;
      }
    }
    lastkhai = khai;
  }
  if(currentState == PLAYING){
    for(int i = 0; i < walk; i++){
      if(save[i] == "TIEN") digitalWrite(TIEN, HIGH);
      if(save[i] == "XUONG") digitalWrite(XUONG, HIGH);
      if(save[i] == "TRAI") digitalWrite(TRAI, HIGH);
      if(save[i] == "PHAI") digitalWrite(PHAI, HIGH);
      delay(1000);
      offled();
      delay(300);
    }
    currentState = IDLE;
    offled();
    delay(300);
  }
}
void setup(){
  Serial.begin(115200);

  pinMode(PHAI, OUTPUT);
  pinMode(TIEN, OUTPUT);
  pinMode(TRAI, OUTPUT);
  pinMode(XUONG, OUTPUT);

  setupMatrix();
  offled();
}

void loop(){
  run();
}