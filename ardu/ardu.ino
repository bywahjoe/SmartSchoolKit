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
int suhu = 0;
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
  pinMode(pin_tombol, INPUT);

  //Buzzer Speaker
  pinMode(buzz, OUTPUT);

  //Ultra
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  //Led
  pinMode(led, OUTPUT);
  pinMode(led_suhu,OUTPUT);

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
  suhu = getSuhu();

  Serial.print("CAHAYA : "); Serial.println(cahaya);
  Serial.print("JARAK  : "); Serial.println(jarak);
  Serial.print("SUARA  : "); Serial.println(suara);
  Serial.print("TOMBOL : "); Serial.println(ditekan);
  Serial.print("PUTARAN: "); Serial.println(putaran);
  Serial.print("SUHU   : "); Serial.println(suhu);

  //Jika Suhu < 41
  if (suhu <= 41) {
    ledSuhuON();
    buzzOFF();
  } else {
    ledSuhuOFF();
    buzzON();
    delay(3000);
  }

  //Jika Ditekan
  if (ditekan) {
    unsigned long start = millis(), last = start;
    while (ditekan) Serial.println("TAHAN");
    last = millis();

    if (last - start >= 5000L) {
      resetAbsen();
    } else {
      absen[urutan] = 1;
    }
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
void ledSuhuON() {
  digitalWrite(led_suhu, HIGH);
}
void ledSuhuOFF() {
  digitalWrite(led_suhu, LOW);
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
int getSuhu() {
  int result,readSuhu;
  readSuhu=analogRead(pin_suhu);
//  Serial.println(readSuhu);
//  result =  readSuhu* 500 / 1023;
  result =  readSuhu/2.0479;

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
  display.setCursor(0, 0);
  display.print(nama[urutan]);
  display.setCursor(0, 25);
  display.print(result);
  display.setCursor(0, 45);
  display.print("ABSEN");
//  display.setCursor(60, 0);
//  display.print(suhu);
  display.display();
  delay(250);
}
void resetAbsen() {
  for (int i = 0; i < 6; i++)absen[i] = 0;

  display.clearDisplay();
  display.display();

  display.setCursor(30, 10);
  display.print("Absen");
  display.setCursor(25, 30);
  display.print("Reset!! ");

  display.display();
  delay(5000);
}
