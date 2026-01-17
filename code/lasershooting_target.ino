#include <LiquidCrystal.h> //mengontrol lcd
#include <Wire.h> //komunikasi I2C antara arduino dan LCD
#include <Servo.h> //mengontrol servo motor
#include <LiquidCrystal_I2C.h> //mengontrol lcd

LiquidCrystal_I2C lcd(0x27, 16, 2); // Inisialisasi LCD, alamat I2C 0x27, ukuran 16 karakter, dan 2 baris.
Servo myServo; // Membuat objek servo untuk mengontrol pergerakan motor servo

int servoPin = 4; // Pin untuk servo
int sensorPin = A2; // Pin untuk sensor LDR
int readingSensor = 0; //Untuk menyimpan nilai yang dibaca dari sensor LDR
int score = 0; //Untuk mencatat skor pemain
int timer = 20; //Untuk menghitung waktu permainan (10 detik)
unsigned long previousMillis = 0; //Untuk pembacaan sensor
unsigned long interval = 100; //Interval waktu (100 ms/0,1s) untuk pembaruan sensor
unsigned long lastTimerUpdate = 0; // Variabel untuk menghitung waktu timer
bool laserDetected = false; //Menandai apakah cahaya telah terdeteksi
int lightThreshold = 900; // cahaya dianggap terang di nilai 900)

void setup() {
  Serial.begin(9600); //Mengaktifkan komunikasi serial arduino dan perangkat dengan kecepatan 9600 baud rate standar

  // Inisialisasi LCD
  lcd.init(); //memulai lcd
  lcd.backlight(); //menyalakan lampu latar LCD
  lcd.clear(); //menghapus semua tampilan di LCD

  displayStartMessage(); // Menampilkan "START" sebelum permainan dimulai

  lcd.setCursor(0, 0); //menentukan posisi karakter di layar (kolom, baris))
  lcd.print("SCORE: 0");
  lcd.setCursor(0, 1); //menampilkan teks atau angka di layar
  lcd.print("TIMER: 20");

  // Inisialisasi servo
  myServo.attach(servoPin); //menentukan pin servo
  myServo.write(0); // mengatur ke posisi awal servo di 0 derajat
}

void loop() {
  unsigned long currentMillis = millis(); //melacak waktu saat ini dalam milidetik 

  // membaca nilai sensor 
  if (currentMillis - previousMillis >= interval) {
    readingSensor = analogRead(sensorPin); //Baca nilai sensor:
    previousMillis = currentMillis; //Update waktu terakhir:
    Serial.println(readingSensor); //cetak hasil

    // Periksa jika cahaya melebihi ambang batas dan hanya tambah skor sekali
    if (readingSensor > lightThreshold && !laserDetected) {
      laserDetected = true; //menandai cahaya sebagai sdh terdeteksi
      score++; //skor bertambah
      lcd.setCursor(7, 0); 
      lcd.print(score); //cetak

      // Gerakkan servo ke 90 derajat dan kembali ke 0 derajat
      myServo.write(90); //servo ke 90 derajat
      delay(500); //tunggu selama 500ms
      myServo.write(0); //servo kembali ke 0 derajat atau posisi awal
    } else if (readingSensor <= lightThreshold) {
      laserDetected = false; //mengatur ulang status, karena setiap deteksi cahaya hanya menghasilkan 1 gerakan servo dan peningkatan skor
    }
  }

  // Perbarui timer
  Countdowntimer(currentMillis);

  // Periksa kondisi menang atau kalah
  if (timer == 0) {
    checkWinOrLose();
  }
}

void Countdowntimer(unsigned long currentMillis) {
  if (currentMillis - lastTimerUpdate >= 1000) {
    lastTimerUpdate = currentMillis; //Menentukan kapan timer harus diperbarui (setiap detik)
    timer--;
    lcd.setCursor(7, 1);
    lcd.print("   "); // Hapus angka sebelumnya
    lcd.setCursor(7, 1);
    lcd.print(timer);
  }
}

void checkWinOrLose() {
  lcd.clear();
  if (score >= 10) { // Kondisi menang jika skor di atas atau sama dengan 10
    lcd.setCursor(0, 0);
    lcd.print("YOU WIN!");
    lcd.setCursor(0, 1);
    lcd.print("SCORE: ");
    lcd.print(score);
  } else { // Kondisi kalah jika waktu habis
    lcd.setCursor(0, 0);
    lcd.print("GAME OVER");
    lcd.setCursor(0, 1);
    lcd.print("SCORE: ");
    lcd.print(score);
  }
  delay(5000); // Tampilkan pesan selama 5 detik

  resetGame();
}

void resetGame() {
  score = 0;
  timer = 20;
  lcd.clear(); 
  displayStartMessage(); // Menampilkan "START" sebelum permainan dimulai kembali
  lcd.setCursor(0, 0);
  lcd.print("SCORE: 0");
  lcd.setCursor(0, 1);
  lcd.print("TIMER: 20");
}

void displayStartMessage() {
  for (int i = 0; i < 3; i++) { //perulangan 3 kali
    lcd.setCursor(5, 0); // Menampilkan teks di tengah layar
    lcd.print("START");
    delay(500);
    lcd.clear();
    delay(500);
  }
}
