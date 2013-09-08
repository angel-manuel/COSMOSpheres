int seconds;

void init() {
  seconds = 0;
  phase1_init();
  phase2_init();
}

void loop() {
  ++seconds;
  if(seconds < 90) {
    phase1_loop();
  } else {
    phase2_loop();
  }
}
