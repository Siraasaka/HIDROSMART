#include <Wire.h>
#include <LiquidCrystal_I2C.h>  

// Konfigurasi LCD (ubah alamat jika diperlukan, biasanya 0x27 atau 0x3F)
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int ph_Pin = A0;
float Po = 0;
float PH_step;
float TeganganPh;

// Kalibrasi
float PH4 = 2.90;  // Tegangan pada larutan pH 4
float PH7 = 2.60;  // Tegangan pada larutan pH 7

// Jumlah sampel untuk rata-rata
const int sampleCount = 13;

void setup()
{
  pinMode(ph_Pin, INPUT);

  lcd.init();           // Inisialisasi LCD
  lcd.backlight();      // Aktifkan backlight LCD
  lcd.clear();          // Bersihkan layar LCD
  
  lcd.setCursor(0, 0);  
  lcd.print("   pH Meter");  
  lcd.setCursor(0, 1);  
  lcd.print("  Initializing");  
  delay(2000);          // Tampilkan pesan awal selama 2 detik
  lcd.clear();
}

void loop()
{
  // Ambil beberapa sampel dan hitung rata-rata tegangan
  TeganganPh = 0;
  for (int i = 0; i < sampleCount; i++) {
    int nilai_analog_PH = analogRead(ph_Pin);
    TeganganPh += 3.3 / 1024.0 * nilai_analog_PH;
    delay(10); // Tunda kecil antara pembacaan
  }
  TeganganPh /= sampleCount; // Rata-rata tegangan

  // Hitung nilai pH
  PH_step = (PH4 - PH7) / 3;  // Selisih tegangan dibagi selisih nilai pH
  Po = 7.00 + ((PH7 - TeganganPh) / PH_step);

  // Tampilkan nilai pH ke LCD
  lcd.setCursor(0, 0);
  lcd.print("pH Value: ");
  lcd.print(Po, 2);  // Tampilkan nilai pH dengan 2 angka desimal
  lcd.print("   ");  // Tambahkan spasi untuk menghapus sisa karakter
  
  // Tampilkan status aman atau tidak aman
  lcd.setCursor(0, 1);
  if (Po >= 6 && Po <= 7.5) {
    lcd.print("Aman     ");
  } else {
    lcd.print("TidakAman         ");
  }
  
  delay(500); // Tunda sebelum pembacaan ulang
}