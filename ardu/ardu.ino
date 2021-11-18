#include <Servo.h>
#include "pinku.h"

//--------OLED
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#define OLED_ADDR   0x3C
Adafruit_SSD1306 display(-1);
#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

#define ditekan digitalRead(pin_tombol)

Servo myservo;
int cahaya = 0, suara = 0, putaran = 0, urutan = 0;
long jarak = 0;

String nama[6] = {"Abi", "Brian", "Dida", "Ryan", "Sastia", "Wikan"};
int absen[6] = {0, 0, 0, 0, 0, 0};

void setup() {
  Serial.begin(9600);

  // initialize and clear display
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.display();

  //Tombol
  pinMode(pin_tombol, INPUT_PULLUP);

  //Buzzer Speaker
  pinMode(buzz, OUTPUT);

  //Ultra
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  //Led
  pinMode(led, OUTPUT);

  //Servo
  myservo.attach(pin_servo);
  myservo.write(90);

  // display a line of text
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(30, 10);
  display.print("Smart");
  display.setCursor(25, 30);
  display.print("School");

  display.display();
  delay(5000);
}

void loop() {
  viewDisplay();
  delay(50);

  cahaya = getCahaya();
  jarak = getJarak();
  suara = getSuara();
  putaran = getPutaran();
  urutan = map(putaran, 0, 1023, 0, 5);

  Serial.print("CAHAYA : "); Serial.println(cahaya);
  Serial.print("JARAK  : "); Serial.println(jarak);
  Serial.print("SUARA  : "); Serial.println(suara);
  Serial.print("TOMBOL : "); Serial.println(!ditekan);
  Serial.print("PUTARAN: "); Serial.println(getPutaran());

  if (ditekan) {
    absen[urutan] = 1;
  }

  //Suara Ramai
  if (suara > 400) {
    buzzON();
    delay(250);
  } else {
    buzzOFF();
    delay(250);
  }
  
  //Cahaya <600 -Siang
  if (cahaya > 600) {
    lampuOFF();
  } else {
    lampuON();
  }

  //Jika jarak ultra <12
  if (jarak < 12 && jarak != 0) {
    buka();
    delay(4000);
  } else {
    tutup();
  }
  // put your main code here, to run repeatedly:
  //delay(5000);
  //buka();
  //delay(5000);
  //tutup();
}
void buka() {
  myservo.write(0);
}
void tutup() {
  myservo.write(90);
}

int getCahaya() {
  int result = analogRead(pin_cahaya);
  return result;
}
long getJarak() {
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  long cm = pulseIn(echo, HIGH) / 58;
  return cm;
}
int getSuara() {
  int result = analogRead(pin_suara);
  return result;
}
int getPutaran() {
  int result = analogRead(pin_putaran);
  return result;

}
void lampuON() {
  digitalWrite(led, HIGH);
}
void lampuOFF() {
  digitalWrite(led, LOW);
}
void buzzON() {
  digitalWrite(buzz, HIGH);
}
void buzzOFF() {
  digitalWrite(buzz, LOW);
}
void viewDisplay() {
  String result = "SUDAH";
  if (absen[urutan] == 0) {
    result = "BELUM";
  }

  display.clearDisplay();
  display.display();

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(30, 0);
  display.print(nama[urutan]);
  display.setCursor(30, 25);
  display.print(result);
  display.setCursor(30, 45);
  display.print("ABSEN");
  display.display();
  delay(250);
}
