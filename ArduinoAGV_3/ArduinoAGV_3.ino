#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

//interruptPin
const int interruptPin1 = 21;
const int channelPin1 = 23;
int varInt1 = 0;
const int interruptPin2 = 20;
const int channelPin2 = 25;
int varInt2 = 0;

//PID Controller
int errorPID, toleranceErr, target, scale, dir;
#define maxPIDPWM 80
#define minPIDPWM 20
#define tolerance 0.02
#define euler 2.71282
int finalPWM = 0;

//PID Calculation 1
int pos1 = 0;
long prevT1 = 0;
float eprev1 = 0;
float eintegral1 = 0;
float u1, pwr1, dedt1, e1, deltaT1;
long currT1;

//PID Calculation 2
int pos2 = 0;
long prevT2 = 0;
float eprev2 = 0;
float eintegral2 = 0;
float u2, pwr2, dedt2, e2, deltaT2;
long currT2;

float kp = 5;
float kd = 1;
float ki = 0.4;

//Mobile Robotics
int plan = 0;
int D = 9;
float ppr = 250;
float keliling = D * 3.14;
float hasiljarak = keliling / ppr;
float jarakAkhir;

int pwm;
float M1, M2;

#define MP1 6
#define MP2 7

#define M2A 34
#define M2B 36
#define M1A 32
#define M1B 30

int m1;
int m2;

//interfaces
#define OKE 0
#define TANDA 1
#define BLOK 2
#define JEMPOL 3
#define HOME 4
#define RESET 5
#define PANAH1 6
#define PANAH2 7
#define T1 22
#define T2 26
#define T3 30
#define T4 34
#define T5 38
#define T6 42
#define T7 46
#define led 13

//Custom Char
uint8_t cek[8] = {0x0, 0x1 , 0x3, 0x16, 0x1c, 0x8, 0x0};
uint8_t tanda[8] = {0x00, 0x02, 0x06, 0x0C, 0x06, 0x0C, 0x08, 0x00};
uint8_t fullblok[8] = {0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f};
uint8_t good[8] = {0x02, 0x06, 0x07, 0x0F, 0x1F, 0x1F, 0x1F, 0x01};
uint8_t omah[8] = {0x04, 0x0E, 0x1B, 0x1F, 0x1F, 0x1B, 0x00, 0x00};
uint8_t cross[8] = {0x0, 0x1b, 0xe, 0x4, 0xe, 0x1b, 0x0};
uint8_t arrow2[8] = {0x1, 0x1, 0x5, 0x9, 0x1f, 0x8, 0x4};
uint8_t arrow1[8] = {0x00, 0x04, 0x02, 0x1F, 0x02, 0x04, 0x00, 0x00};

byte pin [7] = {T1, T2, T3, T4, T5, T6, T7};

String posisi [3] = {"KRAI", "UEESRG", "BIG DATA"};//1,2,3
String tujuan [3] = {"KRAI    ", "UEESRG  ", "BIG DATA"};//1,2,3
int pos = 0;

bool pilih = false;
bool parsing = false;
bool detek = false;

String command;
char c;
int Index1, Index2, Index3, Index4, Index5, Index6;
int Speed, Degree;
String cam, renc, spd, dgr, path, kosong;

//S1=start,S2=atas menu,S3=bawah menu,S4=++,S5=--,S6=back,S7=oke
bool S1() {
  if (digitalRead(T1) == 0) {
    digitalWrite(led, HIGH);
    return true;
  } else {
    digitalWrite(led, LOW);
    return false;
  }
}
bool S2() {
  if (digitalRead(T2) == 0) {
    digitalWrite(led, HIGH);
    return true;
  } else {
    digitalWrite(led, LOW);
    return false;
  }
}
bool S3() {
  if (digitalRead(T3) == 0) {
    digitalWrite(led, HIGH);
    return true;
  } else {
    digitalWrite(led, LOW);
    return false;
  }
}
bool S4() {
  if (digitalRead(T4) == 0) {
    digitalWrite(led, HIGH);
    return true;
  } else {
    digitalWrite(led, LOW);
    return false;
  }
}
bool S5() {
  if (digitalRead(T5) == 0) {
    digitalWrite(led, HIGH);
    return true;
  } else {
    digitalWrite(led, LOW);
    return false;
  }
}
bool S6() {
  if (digitalRead(T6) == 0) {
    digitalWrite(led, HIGH);
    return true;
  } else {
    digitalWrite(led, LOW);
    return false;
  }
}
bool S7() {
  if (digitalRead(T7) == 0) {
    digitalWrite(led, HIGH);
    return true;
  } else {
    digitalWrite(led, LOW);
    return false;
  }
}

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600);
  delay(100);
  Serial2.println("Dimulai");
  pinMode(interruptPin1, INPUT_PULLUP);
  pinMode(channelPin1, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin1), interrupt1, RISING);

  pinMode(interruptPin2, INPUT_PULLUP);
  pinMode(channelPin2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin2), interrupt2, RISING);

  pinMode(MP1, OUTPUT);
  pinMode(MP2, OUTPUT);

  pinMode(M1A, OUTPUT);
  pinMode(M1B, OUTPUT);
  pinMode(M2A, OUTPUT);
  pinMode(M2B, OUTPUT);

  for (int i = 0; i < 7; i++) {
    pinMode(pin[i], INPUT_PULLUP);
  }

  pinMode(led, OUTPUT);

  lcd.begin();
  lcd.backlight();
  lcd.clear();

  //setup karakter
  lcd.createChar(OKE, cek);
  lcd.createChar(TANDA, tanda);
  lcd.createChar(BLOK, fullblok);
  lcd.createChar(JEMPOL, good);
  lcd.createChar(HOME, omah);
  lcd.createChar(RESET, cross);
  lcd.createChar(PANAH1, arrow1);
  lcd.createChar(PANAH2, arrow2);
  delay(1000);
  //acak = random(0, 2);
  lcd.setCursor(3, 1); lcd.print("TEKAN START!!!");
}

void loop() {
  if (S1()) {
    while (1) {
      lcd.clear();
      lcd.setCursor(5, 1); lcd.print("LOADING....");
      delay(500);
      for (int i = 1; i < 19; i++) {
        lcd.setCursor(i, 2); lcd.write(BLOK);
        delay(500);
      }
      delay(500);
      lcd.clear();
      lcd.setCursor(0, 1); lcd.print("checking communication");
      Serial.println("START KOMUNIKASI");
      while (Serial.available()) {
        if (command == "READY") {
          lcd.setCursor(0, 0); lcd.print("Communication oke");
          delay(500);
          lcd.clear();
          lcd.setCursor(2, 1); lcd.print("LOADING COMPLETE");
          lcd.setCursor(9, 2); lcd.write(JEMPOL);
          delay(2000);
          mainFUNC();
        }
      }
      delay(2000);
      lcd.setCursor(0, 0); lcd.print("Communication Error!");
      delay(1000);
      lcd.clear();
      lcd.setCursor(3, 1); lcd.print("TEKAN START!!!");
    }
  }
}
