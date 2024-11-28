#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// --- Konfigurasi Hardware ---
LiquidCrystal_I2C lcd(0x27, 16, 2); // Konfigurasi LCD
const int trigPin = 9;              // Pin trig untuk sensor HC-SR04
const int echoPin = 10;             // Pin echo untuk sensor HC-SR04
const int pumpPin = 8;              // Pin pompa
const int solenoidPin = 7;          // Pin solenoid valve
const int ph_Pin = A0;              // Pin sensor pH

// --- Level Air ---
const int fullWaterLevel = 2;       // Level air penuh (cm)
const int lowWaterLevel = 5;       // Level air hampir habis (cm)
const int floorWaterLevel = 6;    // Level air habis (cm)

// --- Variabel ---
long duration;                      // Durasi sinyal echo
int distance;                       // Jarak yang diukur oleh sensor HC-SR04
float pHValue = 0;                  // Nilai pH
float voltagePH = 0;                // Tegangan sensor pH

// --- Kalibrasi pH ---
const float PH4 = 2.90;             // Tegangan pada larutan pH 4
const float PH7 = 2.60;             // Tegangan pada larutan pH 7
const int sampleCount = 13;         // Jumlah sampel tegangan untuk rata-rata

// --- Kalibrasi Sensor Ulrasonic ---
float faktorKalibrasiJarak = 0;  // Variabel kalibrasi (disesuaikan berdasarkan hasil pengujian) 

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

  // Inisialisasi Serial Monitor
  Serial.begin(9600);

  // Konfigurasi pin sebagai input/output
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(pumpPin, OUTPUT);
  pinMode(solenoidPin, OUTPUT);
  pinMode(ph_Pin, INPUT);

  // Matikan pompa dan valve saat awal
  digitalWrite(pumpPin, LOW);
  digitalWrite(solenoidPin, LOW);

  Serial.println("Sistem HIDROSMART dimulai");
}

void loop() {
  bacaPH();        // Fungsi membaca nilai pH
  bacaJarak();     // Fungsi membaca jarak air
  kontrolValve();  // Fungsi mengontrol solenoid valve
  kontrolPompa();  // Fungsi mengontrol pompa
  delay(1500);      // Tunda sebelum pembacaan ulang
}

// --- Fungsi Membaca Nilai pH ---
void bacaPH() {
  voltagePH = 0;
  for (int i = 0; i < sampleCount; i++) {
    int nilaiAnalogPH = analogRead(ph_Pin);
    voltagePH += 3.3 / 1024.0 * nilaiAnalogPH;
    delay(10);
  }
  voltagePH /= sampleCount; // Hitung rata-rata tegangan
  float phStep = (PH4 - PH7) / 3; // Hitung langkah pH
  pHValue = 7.00 + ((PH7 - voltagePH) / phStep);

  // Tampilkan nilai pH di LCD dan Serial Monitor
  lcd.setCursor(0, 0);
  lcd.print("pH Value: ");
  lcd.print(pHValue, 2);
  lcd.print("   ");
  
  Serial.print("Nilai pH: ");
  Serial.println(pHValue, 2);

  lcd.setCursor(0, 1);
  if (pHValue < 6.0) {
    lcd.print("Kondisi: ASAM  ");
  } else if (pHValue >= 6.0 && pHValue <= 7.5) {
    lcd.print("Kondisi: IDEAL ");
  } else {
    lcd.print("Kondisi: BASA  ");
  }
}

// --- Fungsi Membaca Jarak Air ---
void bacaJarak() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration * 0.034 / 2) + faktorKalibrasiJarak;

  // Tampilkan jarak di Serial Monitor
  Serial.print("Jarak: ");
  Serial.print(distance);
  Serial.println(" cm");
}

// --- Fungsi Mengontrol Solenoid Valve ---
void kontrolValve() {
  if (distance <= fullWaterLevel) {
    digitalWrite(solenoidPin, LOW); // Tutup valve
    Serial.println("Valve: TUTUP");
  } else {
    digitalWrite(solenoidPin, HIGH); // Buka valve
    Serial.println("Valve: BUKA");
  }
}

// --- Fungsi Mengontrol Pompa ---
void kontrolPompa() {
  if (distance >= floorWaterLevel) {
    digitalWrite(pumpPin, LOW); // Matikan pompa
    Serial.println("Pompa: MATI");
  } else {
    digitalWrite(pumpPin, HIGH); // Hidupkan pompa
    Serial.println("Pompa: HIDUP");
  }
}
