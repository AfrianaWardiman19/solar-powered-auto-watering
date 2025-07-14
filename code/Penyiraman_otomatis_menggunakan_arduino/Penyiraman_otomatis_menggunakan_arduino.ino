#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define sensor 33
#define relay 4
#define ledRed 26  // GPIO untuk LED Merah
#define ledGreen 27 // GPIO untuk LED Hijau

// Inisialisasi LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Variabel global
int moistureValue = 0;  // Nilai kelembaban tanah
int value = 0;          // Nilai analog dari sensor

void setup() {
  // Inisialisasi Serial Monitor
  Serial.begin(115200);

  // Inisialisasi LCD
  lcd.init();
  lcd.backlight();

  // Konfigurasi pin
  pinMode(relay, OUTPUT);
  pinMode(ledRed, OUTPUT);
  pinMode(ledGreen, OUTPUT);

  // Status awal
  digitalWrite(relay, HIGH); // Pompa mati awal
  digitalWrite(ledRed, LOW);

  // Tampilan awal di LCD
  lcd.setCursor(1, 0);  digitalWrite(ledGreen, HIGH);

  lcd.print("System Loading");
  for (int a = 0; a <= 15; a++) {
    lcd.setCursor(a, 1);
    lcd.print(".");
    delay(200);
  }
  lcd.clear();
}

void loop() {
  // Membaca nilai kelembaban tanah
  value = analogRead(sensor);
  value = map(value, 0, 4095, 0, 100);  // Konversi nilai ke 0-100
  moistureValue = (value - 100) * -1;  // Hitung kelembaban tanah

  // Tampilkan nilai kelembaban di LCD
  lcd.setCursor(0, 0);
  lcd.print("Moisture: ");
  lcd.print(moistureValue);
  lcd.print("%   ");  // Spasi tambahan untuk membersihkan teks lama

  // Logika kontrol otomatis
  if (moistureValue < 40) {  // Batas kelembaban rendah
    digitalWrite(relay, LOW);  // Hidupkan pompa
    lcd.setCursor(0, 1);
    lcd.print("Pompa ON      ");
  } else {
    digitalWrite(relay, HIGH);  // Matikan pompa
    lcd.setCursor(0, 1);
    lcd.print("Pompa OFF     ");
  }

  // Kontrol LED berdasarkan kelembaban tanah
  digitalWrite(ledRed, (moistureValue < 50) ? HIGH : LOW);
  digitalWrite(ledGreen, (moistureValue >= 50) ? HIGH : LOW);

  // Tampilkan di Serial Monitor
  Serial.print("Moisture: ");
  Serial.print(moistureValue);
  Serial.println("%");

  delay(200);  // Tunggu sebelum loop berikutnya
}
