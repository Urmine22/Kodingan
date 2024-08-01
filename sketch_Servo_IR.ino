#include <Servo.h>  // Mengimpor library Servo

Servo myservo;     // Membuat objek servo untuk mengontrol motor servo
int irSensorPin = 12; // Pin digital untuk membaca data dari sensor inframerah
int servoPin = 3;   // Pin digital untuk mengontrol servo

void setup() {
  pinMode(irSensorPin, INPUT); // Menyetel pin sensor inframerah sebagai input
  myservo.attach(servoPin);    // Menghubungkan objek servo ke pin digital 3
   myservo.write(0);     // Menggerakkan servo ke 0 derajat
}

void loop() {
  int irValue = digitalRead(irSensorPin); // Membaca nilai dari sensor inframerah

  if (irValue == LOW) {  // Jika sensor mendeteksi objek (nilai HIGH)
    // Menggerakkan servo ke beberapa posisi berturut-turut
    myservo.write(0);     // Menggerakkan servo ke 0 derajat
    delay(5000);          // Menunggu selama 5 detik

    myservo.write(90);    // Menggerakkan servo ke 90 derajat
    delay(1000);          // Menunggu selama 1 detik

    myservo.write(180);   // Menggerakkan servo ke 180 derajat
    delay(5000);          // Menunggu selama 5 detik

    myservo.write(90);    // Menggerakkan servo kembali ke 90 derajat
    delay(1000);          // Menunggu selama 1 detik

    myservo.write(0);     // Menggerakkan servo kembali ke 0 derajat
    delay(1000);          // Menunggu selama 1 detik
  }else {
     myservo.write(0); 
  }
}
