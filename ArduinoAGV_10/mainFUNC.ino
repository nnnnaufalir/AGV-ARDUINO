void mainFUNC() {
  while (1) {
    bool play = false;
    int nowDegree;
    delay(10);
    lcd.setCursor(0, 0); lcd.print("Lagi Jalan");
    digitalWrite(ledJalan, HIGH);
    delay(10);
    play = true;

    if (play) {
      parsingData(false);
      if (pos == 0) {
        Serial.println("A");
      }
      if (pos == 1) {
        Serial.println("B");
      }
      if (pos == 2) {
        Serial.println("C");
      }
      delay(10);
      nowDegree = Degree;
      //      if (prevDegree == nowDegree) {
      //        rotationTPID(prevDegree - nowDegree);
      //        prevDegree = nowDegree;
      //      }
      //      else {
      translationTPID(Speed);
      //        if (rencana) {
      //          Serial.println("Path Selesai");
      //          translationTPID(0);
      //          play = false;
      //        }
      //      }
    }
  }
}
