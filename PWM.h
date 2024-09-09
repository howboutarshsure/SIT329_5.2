#include "PWM.h"

PWM pwmController;
const int pwmPin = 6;  // Use pin 6 for PWM
const int port = 0;    // PORTA is typically port 0

void setup() {
    pwmController.output_PWM(port, pwmPin);  // Configure pin for PWM output
    pwmController.set_PWM(1000, 50);         // Set frequency to 1kHz, duty cycle to 50%
}

void loop() {
    // Here, you can adjust the PWM settings or perform other operations as needed
}
