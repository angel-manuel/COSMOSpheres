int seconds;

void init() {
  seconds = 0;
  phase_1_init();
  phase_2_init();
}

void loop() {
  ++seconds;
  if(seconds < 90) {
    phase_1_loop();
  } else {
    phase_2_loop();
  }
}
