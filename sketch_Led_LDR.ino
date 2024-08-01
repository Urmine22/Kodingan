//TERANG 0 JIKA GELAP 1

int LDRPin = 13;     // Pin analog untuk membaca data dari LDR
int ledPin = 15;      // Pin digital untuk mengontrol LED
int LDRValue = 0;    // Variabel untuk menyimpan nilai pembacaan dari LDR
int threshold = 500; // Ambang batas untuk menyalakan/mematikan lampu

void setup() {
  pinMode(LDRPin, INPUT);
  pinMode(ledPin, OUTPUT);  // Menyetel pin LED sebagai output
  Serial.begin(9600);       // Memulai komunikasi serial dengan baud rate 9600
}

void loop() {
  LDRValue = digitalRead(LDRPin);  // Membaca nilai dari LDR
  Serial.println(LDRValue);       // Mencetak nilai LDR ke monitor serial

  if (LDRValue ==1 ) {     // Jika nilai LDR kurang dari ambang batas
    digitalWrite(ledPin, HIGH);   // Menyalakan LED (lampu)
  } else {
    digitalWrite(ledPin, LOW);    // Mematikan LED (lampu)
  }
  
  delay(100); // Jeda 100ms sebelum membaca kembali
}
