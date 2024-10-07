#include <SPI.h>
#include <WiFiNINA.h>
#include "PWM.h"  // Include the custom PWM class

char ssid[] = "NETGEAR21";   // your network SSID (name)
char pass[] = "69699696";    // your network password
int status = WL_IDLE_STATUS; // the Wifi radio's status

WiFiServer server(80);   // Start server on port 80
PWM pwmController(6);    // Initialize PWM class with pin 6

void setup() {
    Serial.begin(9600);

    // Try to connect to WiFi
    while (status != WL_CONNECTED) {
        Serial.print("Connecting to SSID: ");
        Serial.println(ssid);
        status = WiFi.begin(ssid, pass);
        delay(5000);  // wait 5 seconds for connection
    }

    server.begin();  // start the server
    Serial.println("Server is up and running");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    // Configure the PWM (set frequency to 1000 Hz and duty cycle to 50%)
    pwmController.configure_timer(1000, 50);
}

void loop() {
    WiFiClient client = server.available();  // Check if a client is connected
    if (client) {
        String request = client.readStringUntil('\r');
        client.flush();

        int pwmValue = extractPWMValue(request);

        if (pwmValue >= 0 && pwmValue <= 255) {
            pwmController.set_PWM(1000, pwmValue);  // Set frequency to 1000 Hz and duty cycle to pwmValue
        }

        // Send HTML with Water.css styling
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/html");
        client.println("Connection: close");
        client.println();
        client.println("<!DOCTYPE html>");
        client.println("<html>");
        client.println("<head>");
        client.println("<link rel='stylesheet' href='https://cdn.jsdelivr.net/npm/water.css@2/out/water.css'>");
        client.println("<title>LED PWM Controller</title>");
        client.println("</head>");
        client.println("<body>");
        client.println("<h1>LED PWM Control</h1>");
        client.println("<p>Adjust the brightness of the LED using the slider below:</p>");
        client.println("<form method='GET'>");
        client.println("<input type='range' min='0' max='255' value='" + String(pwmValue) + "' name='pwm'>");
        client.println("<br><input type='submit' value='Set Brightness'>");
        client.println("</form>");
        client.println("</body>");
        client.println("</html>");

        delay(1);
        client.stop();
    }
}

// Extract PWM value from the request
int extractPWMValue(String request) {
    int index = request.indexOf("pwm=");
    if (index > 0) {
        return request.substring(index + 4).toInt();
    }
    return -1;
}
