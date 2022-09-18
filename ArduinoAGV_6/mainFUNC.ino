void mainFUNC() {
  parsingData(false);
  if (Degree != prevSpeed) {
    rotationTPID(Degree);
  }
  else {
    translationTPID(Speed);
    prevDegree = Degree;
  }
}
