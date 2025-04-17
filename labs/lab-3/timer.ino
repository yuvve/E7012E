int frequencyToTicks(int frequency, int clockDivisor = 128) {
  const int masterClock = 84000000;
  float timerClock = (float)(masterClock / clockDivisor);
  return (int)(timerClock / frequency);
}

void setupTimer() {
  TC_Configure(TC2, 0,
  TC_CMR_TCCLKS_TIMER_CLOCK4 |  // MCK/128
  TC_CMR_WAVE |                 // Waveform mode
  TC_CMR_WAVSEL_UP_RC);         // Counter resets on RC match

  if DEBUG {
    Serial.println("Initialized timer for interrupts");
  }
}

void setupTimerInterruptChannel0(int frequencyHz) {
  pmc_set_writeprotect(false);
  pmc_enable_periph_clk(ID_TC6); // TC6 = Timer 2 channel 0

  int ticks = frequencyToTicks(frequencyHz);
  
  TC_SetRC(TC2, 0, ticks);
  TC_Start(TC2, 0);

  TC2->TC_CHANNEL[0].TC_IER = TC_IER_CPCS;
  TC2->TC_CHANNEL[0].TC_IDR = ~TC_IER_CPCS;

  NVIC_EnableIRQ(TC6_IRQn);
  
  if DEBUG {
    Serial.print("Initialized timer channel 0 with interrupt frequency: ");
    Serial.print(frequencyHz);
    Serial.println("Hz");
  }
}

void setupTimerInterruptChannel1(int frequencyHz) {
  pmc_set_writeprotect(false);
  pmc_enable_periph_clk(ID_TC7); // TC7 = Timer 2 channel 1

  int ticks = frequencyToTicks(frequencyHz);
  
  TC_SetRC(TC2, 1, ticks);
  TC_Start(TC2, 1);

  TC2->TC_CHANNEL[1].TC_IER = TC_IER_CPCS;
  TC2->TC_CHANNEL[1].TC_IDR = ~TC_IER_CPCS;

  NVIC_EnableIRQ(TC7_IRQn);
  
  if DEBUG {
    Serial.print("Initialized timer channel 1 with interrupt frequency: ");
    Serial.print(frequencyHz);
    Serial.println("Hz");
  }
}

void setupTimerInterruptChannel2(int frequencyHz) {
  pmc_set_writeprotect(false);
  pmc_enable_periph_clk(ID_TC8); // TC8 = Timer 2 channel 2

  int ticks = frequencyToTicks(frequencyHz);
  
  TC_SetRC(TC2, 2, ticks);
  TC_Start(TC2, 2);

  TC2->TC_CHANNEL[2].TC_IER = TC_IER_CPCS;
  TC2->TC_CHANNEL[2].TC_IDR = ~TC_IER_CPCS;

  NVIC_EnableIRQ(TC8_IRQn);
  
  if DEBUG {
    Serial.print("Initialized timer channel 2 with interrupt frequency: ");
    Serial.print(frequencyHz);
    Serial.println("Hz");
  }
}