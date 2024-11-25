float onThreshold = 1550; // < this value == OFF

void StateSync(void *pvParameters) {
  int count = 0;
  float total = 0;
  for(;;) {
    count++;
    total += analogRead(34);
    if (count != 10) {
      delay(25);
      continue;
    }

    
    float avg = total / 10;
    bool observedState = avg > onThreshold;

    lightState = observedState;
    count = 0;
    total = 0;
  }
}