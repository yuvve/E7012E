int frequencyToTicks(int frequency, int clockDivisor = 128) {
  const int masterClock = 84000000;
  float timerClock = (float)(masterClock / clockDivisor);
  return (int)(timerClock / frequency);
}

void setupFeedbackTimer(int frequency_hz) {
  int ticks = frequencyToTicks(frequency_hz);

  pmc_set_writeprotect(false);
  pmc_enable_periph_clk(ID_TC6); // TC6 = Timer 2 channel 0

  TC_Configure(TC2, 0,
    TC_CMR_TCCLKS_TIMER_CLOCK4 |  // MCK/128
    TC_CMR_WAVE |                 // Waveform mode
    TC_CMR_WAVSEL_UP_RC);         // Counter resets on RC match

  TC_SetRC(TC2, 0, ticks);
  TC_Start(TC2, 0);

  TC2->TC_CHANNEL[0].TC_IER = TC_IER_CPCS;
  TC2->TC_CHANNEL[0].TC_IDR = ~TC_IER_CPCS;

  NVIC_EnableIRQ(TC6_IRQn);

  if DEBUG {
    Serial.print("Feedback timer initialized with frequency: ");
    Serial.println(frequency_hz);
  }
}

void TC6_Handler() {
  TC_GetStatus(TC2, 0);           // Clears interrupt flag
  sendFeedbackFlag = true;
}