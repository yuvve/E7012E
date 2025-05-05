int frequencyToTicks(int frequency, int clockDivisor = 128) {
  const int masterClock = 84000000;
  float timerClock = (float)(masterClock / clockDivisor);
  return (int)(timerClock / frequency);
}

void setupTimerInterruptChannel0(int frequencyHz) {
  pmc_set_writeprotect(false);
  pmc_enable_periph_clk(ID_TC6); // TC6 = Timer 2 channel 0

  TC_Configure(TC2, 0,
    TC_CMR_TCCLKS_TIMER_CLOCK4 |  // MCK/128
    TC_CMR_WAVE |                 // Waveform mode
    TC_CMR_WAVSEL_UP_RC);         // Counter resets on RC match

  int ticks = frequencyToTicks(frequencyHz);
  
  TC_SetRC(TC2, 0, ticks);
  TC_Start(TC2, 0);

  TC2->TC_CHANNEL[0].TC_IER = TC_IER_CPCS; // Enable interrupt on RC compare
  TC2->TC_CHANNEL[0].TC_IDR = ~TC_IER_CPCS; // Disable all other interrupts

  NVIC_EnableIRQ(TC6_IRQn);
  
  DEBUG_PRINTF("Timer channel 0 initialized with frequency: %d Hz", frequencyHz);
}

void setupTimerInterruptChannel1(int frequencyHz) {
  pmc_set_writeprotect(false);
  pmc_enable_periph_clk(ID_TC7); // TC7 = Timer 2 channel 1

  TC_Configure(TC2, 1,
    TC_CMR_TCCLKS_TIMER_CLOCK4 |  // MCK/128
    TC_CMR_WAVE |                 // Waveform mode
    TC_CMR_WAVSEL_UP_RC);         // Counter resets on RC match

  int ticks = frequencyToTicks(frequencyHz);
  
  TC_SetRC(TC2, 1, ticks);
  TC_Start(TC2, 1);

  TC2->TC_CHANNEL[1].TC_IER = TC_IER_CPCS; // Enable interrupt on RC compare
  TC2->TC_CHANNEL[1].TC_IDR = ~TC_IER_CPCS; // Disable all other interrupts

  NVIC_EnableIRQ(TC7_IRQn);
  
  DEBUG_PRINTF("Timer channel 1 initialized with frequency: %d Hz", frequencyHz);
}

void setupTimerInterruptChannel2(int frequencyHz) {
  pmc_set_writeprotect(false);
  pmc_enable_periph_clk(ID_TC8); // TC8 = Timer 2 channel 2

  TC_Configure(TC2, 2,
    TC_CMR_TCCLKS_TIMER_CLOCK4 |  // MCK/128
    TC_CMR_WAVE |                 // Waveform mode
    TC_CMR_WAVSEL_UP_RC);         // Counter resets on RC match

  int ticks = frequencyToTicks(frequencyHz);
  
  TC_SetRC(TC2, 2, ticks);
  TC_Start(TC2, 2);

  TC2->TC_CHANNEL[2].TC_IER = TC_IER_CPCS; // Enable interrupt on RC compare
  TC2->TC_CHANNEL[2].TC_IDR = ~TC_IER_CPCS; // Disable all other interrupts

  NVIC_EnableIRQ(TC8_IRQn);
  
  DEBUG_PRINTF("Timer channel 2 initialized with frequency: %d Hz", frequencyHz);
}