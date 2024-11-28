#KALIBRASI SENSOR ULTRASONIK
# Deklarasi pin HC-SR04
const int trigPin = 9;
const int echoPin = 10;

# Variabel jarak
long duration;
int distance;

# Variabel kalibrasi (disesuaikan berdasarkan hasil pengujian)
float faktorKalibrasi = 0; # Misal: tambahkan nilai koreksi jika ada error

void setup() {
  # Inisialisasi komunikasi serial
  Serial.begin(9600);

  # Set pin sebagai input/output
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.println("Kalibrasi HC-SR04 dimulai...");
}

void loop() {
  # Kirim sinyal trigger
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  # Baca sinyal echo
  duration = pulseIn(echoPin, HIGH);

  # Hitung jarak dalam cm
  distance = (duration * 0.034 / 2) + faktorKalibrasi;

  # Tampilkan hasil di Serial Monitor
  Serial.print("Jarak: ");
  Serial.print(distance);
  Serial.println(" cm");

  # Tunggu sebelum pengukuran berikutnya
  delay(500);
}