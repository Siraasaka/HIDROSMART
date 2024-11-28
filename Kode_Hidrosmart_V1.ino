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

void setup() {
  // Inisialisasi komunikasi serial
  Serial.begin(9600);

  // Set pin sebagai input/output
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(pumpPin, OUTPUT);

  // Pastikan pompa mati saat awal
  digitalWrite(pumpPin, LOW);

  Serial.println("Sistem HC-SR04 dengan kontrol pompa dimulai...");
}

void loop() {
  // Kirim sinyal trigger
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Baca sinyal echo
  duration = pulseIn(echoPin, HIGH);

  // Hitung jarak dalam cm
  distance = (duration * 0.034 / 2) + faktorKalibrasi;

  // Tampilkan hasil di Serial Monitor
  Serial.print("Jarak: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Logika kontrol pompa
  if (distance > 2) {
    digitalWrite(pumpPin, HIGH); // Hidupkan pompa
    Serial.println("Pompa: HIDUP");
  } else if (distance <= 2) {
    digitalWrite(pumpPin, LOW); // Matikan pompa
    Serial.println("Pompa: MATI");
  }

  // Tunggu sebelum pengukuran berikutnya
  delay(500);
}
