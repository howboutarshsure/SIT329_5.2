#include "sam.h"
#include "PWM.h"

PWM::PWM(int pin) : pwmPin(pin) {}  // Constructor implementation

bool PWM::output_PWM(int port, int pin_number) {
    // Enable the multiplexer for the given pin (for PWM)
    if (port == 0) {  // Port 0 corresponds to PORTA
        PORT->Group[0].PINCFG[pin_number].bit.PMUXEN = 1;
        PORT->Group[0].PMUX[pin_number >> 1].reg = PORT_PMUX_PMUXE_F;  // Configure pin for TCC0
    } else if (port == 1) {  // Port 1 corresponds to PORTB
        PORT->Group[1].PINCFG[pin_number].bit.PMUXEN = 1;
        PORT->Group[1].PMUX[pin_number >> 1].reg = PORT_PMUX_PMUXE_F;  // Configure pin for TCC0
    }
    return true;
}

bool PWM::set_PWM(int frequency, int duty_cycle) {
    configure_timer(frequency, duty_cycle);  // Configure the timer
    return true;
}

void PWM::configure_timer(int frequency, int duty_cycle) {
    // Enable the GCLK for TCC0 (Generic Clock Control)
    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN |              // Enable the clock
                        GCLK_CLKCTRL_GEN_GCLK0 |          // Select GCLK0 (48 MHz)
                        GCLK_CLKCTRL_ID_TCC0_TCC1;        // Route GCLK0 to TCC0

    while (GCLK->STATUS.bit.SYNCBUSY);  // Wait for synchronization

    // Disable TCC0 during configuration
    TCC0->CTRLA.bit.ENABLE = 0;
    while (TCC0->SYNCBUSY.bit.ENABLE);

    // Set waveform generation to Normal PWM mode (NPWM)
    TCC0->WAVE.reg = TCC_WAVE_WAVEGEN_NPWM;
    while (TCC0->SYNCBUSY.bit.WAVE);

    // Calculate the timer counter value based on frequency
    uint32_t top_value = SystemCoreClock / (frequency * 256) - 1;
    TCC0->PER.reg = top_value;
    while (TCC0->SYNCBUSY.bit.PER);

    // Set duty cycle (compare value)
    uint32_t compare_value = (top_value * duty_cycle) / 100;
    TCC0->CC[0].reg = compare_value;  // Set the compare register for channel 0
    while (TCC0->SYNCBUSY.bit.CC0);

    // Enable TCC0
    TCC0->CTRLA.bit.ENABLE = 1;
    while (TCC0->SYNCBUSY.bit.ENABLE);
}
