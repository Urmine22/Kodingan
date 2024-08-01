#include <Servo.h>  // Mengimpor library Servo

Servo myservo;  // Membuat objek servo untuk mengontrol motor servo

void setup() {
  myservo.attach(3);  // Menghubungkan objek servo ke pin digital 9
}

void loop() {
  myservo.write(0);   // Menggerakkan servo ke 0 derajat
  delay(1000);        // Menunggu selama 1 detik

  myservo.write(90);  // Menggerakkan servo ke 90 derajat
  delay(1000);        // Menunggu selama 1 detik

  myservo.write(180); // Menggerakkan servo ke 180 derajat
  delay(1000);        // Menunggu selama 1 detik

  myservo.write(90);  // Menggerakkan servo kembali ke 90 derajat
  delay(1000);        // Menunggu selama 1 detik

  myservo.write(0);   // Menggerakkan servo kembali ke 0 derajat
  delay(1000);        // Menunggu selama 1 detik
}
