#ifndef PWM_H
#define PWM_H

class PWM {
public:
    PWM(int pin);  // Constructor to initialize the pin
    bool set_PWM(int frequency, int duty_cycle);  // Configures PWM with a frequency and duty cycle
    bool output_PWM(int port, int pin_number);    // Sets the pin as the output for PWM
    void configure_timer(int frequency, int duty_cycle); // Helper function for timer configuration

private:
    int pwmPin; // Store the pin number
};

#endif // PWM_H
