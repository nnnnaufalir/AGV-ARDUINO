int dWhell, dRobot, pulse;

int rotate(int degree) {
  int result = ((degree / 180) * (dWhell / dRobot) * pulse);
  return result;
}
