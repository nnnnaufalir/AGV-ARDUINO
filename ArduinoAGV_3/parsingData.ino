void parsingData() {
  delay(10);
  c = Serial.read();
  command += c;
  if (c == '$') {
    parsing = true;
  }

  if (parsing) {
    if (command.length() > 0)
    {
      Index1 = command.indexOf('@');
      Index2 = command.indexOf(',', Index1 + 1);
      Index3 = command.indexOf(',', Index2 + 1);
      Index4 = command.indexOf(',', Index3 + 1);
      Index5 = command.indexOf('$', Index4 + 1);
      //Index6 = command.indexOf('$', Index5 + 1);
      cam = command.substring(0, Index1);
      renc = command.substring(Index1 + 1, Index2);
      spd = command.substring(Index2 + 1, Index3);
      dgr = command.substring(Index3 + 1, Index4);
      path = command.substring(Index4 + 1, Index5);
      //empty = command.substring(Index5 + 1, Index6);
      Speed = spd.toInt();
      Degree = dgr.toInt();

      //checkData();

      command = "";
      parsing = false;
    }
    delay(10);
  }
}

void checkData ()
{
  Serial.print("data masuk : "); Serial.println(command);
  Serial.print("data 1:"); Serial.println(cam);
  Serial.print("data 2:"); Serial.println(renc);
  Serial.print("data 3:"); Serial.print(spd); Serial.print(" "); Serial.print("tipe : "); Serial.println(sizeof(spd));
  Serial.print("data 4:"); Serial.print(dgr); Serial.print(" "); Serial.print("tipe : "); Serial.println(sizeof(dgr));
  Serial.print("data 5:"); Serial.println(path);
  Serial.println(Index1); Serial.println(Index2); Serial.println(Index3); Serial.println(Index4); Serial.println(Index5);
  //Serial.print("data 6:"); Serial.println(empty);
  Serial.print(Speed); Serial.print(" "); Serial.print("tipe : "); Serial.println(sizeof(Speed));
  Serial.print(Degree); Serial.print(" "); Serial.print("tipe : "); Serial.println(sizeof(Degree));
  if (cam == "krai" && Speed > 50) {
    Serial.println("path selesai");
  }
}
