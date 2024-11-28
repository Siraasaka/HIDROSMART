#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Inisialisasi LCD dengan alamat I2C 0x27
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Deklarasi pin HC-SR04
const int trigPin = 9;
const int echoPin = 10;

// Deklarasi pin pompa
const int pumpPin = 8;

// Variabel jarak
long duration;
int distance;

// Variabel kalibrasi (disesuaikan berdasarkan hasil pengujian)
float faktorKalibrasi = 1; // Misal: tambahkan nilai koreksi jika ada error

// Deklarasi pH meter
const int ph_Pin = A0;
float Po = 0;
float PH_step;
float TeganganPh;

// Kalibrasi pH
float PH4 = 2.90;  // Tegangan pada larutan pH 4
float PH7 = 2.60;  // Tegangan pada larutan pH 7

// Jumlah sampel untuk rata-rata
const int sampleCount = 13;

void setup() {
  // Inisialisasi LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("   HIDROSMART");  
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  delay(2000);
  lcd.clear();

  // Inisialisasi komunikasi serial
  Serial.begin(9600);

  // Set pin sebagai input/output
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(pumpPin, OUTPUT);
  pinMode(ph_Pin, INPUT);

  // Pastikan pompa mati saat awal
  digitalWrite(pumpPin, LOW);

  Serial.println("Sistem HIDROSMART dimulai");
}

void loop() {
  // --- Bagian pH Meter ---
  TeganganPh = 0;
  for (int i = 0; i < sampleCount; i++) {
    int nilai_analog_PH = analogRead(ph_Pin);
    TeganganPh += 3.3 / 1024.0 * nilai_analog_PH;
    delay(10);
  }
  TeganganPh /= sampleCount;
  PH_step = (PH4 - PH7) / 3;
  Po = 7.00 + ((PH7 - TeganganPh) / PH_step);

  lcd.setCursor(0, 0);
  lcd.print("pH Value: ");
  lcd.print(Po, 2);
  lcd.print("   ");

  lcd.setCursor(0, 1);
  if (Po >= 6 && Po <= 7.5) {
    lcd.print("Aman     ");
  } else {
    lcd.print("Tidak Aman ");
  }

  // Tampilkan nilai pH di Serial Monitor
  Serial.print("Nilai pH: ");
  Serial.println(Po, 2);

  // --- Bagian Sensor Jarak ---
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration * 0.034 / 2) + faktorKalibrasi;

  Serial.print("Jarak: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Logika kontrol pompa
  if (distance > 3) {
    digitalWrite(pumpPin, HIGH);
    Serial.println("Pompa: HIDUP");
  } else {
    digitalWrite(pumpPin, LOW);
    Serial.println("Pompa: MATI");
  }

  delay(2000); // Tunda sebelum pembacaan ulang
}
