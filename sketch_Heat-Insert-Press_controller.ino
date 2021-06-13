// Heat-Insert Press controller firmware
// Written by Matteo (matti04.net)
// More information: https://matti04.net/projects/heat-insert-press/
// Code is for reference only - do not leave the press running unattented! 

#define SENSOR_PIN             A0         // Thermistor pin
#define mosfet                 5          // Mosfet pin
#define btn_UP                 14         // Button UP pin
#define btn_OK                 16         // Button OK pin
#define btn_DOWN               15         // Button DOWN pin
#define REFERENCE_RESISTANCE   3300       // R2 in the voltage divider for thermistor
#define NOMINAL_RESISTANCE     100000     // Resistance of the thermistor at the nomial temperature
#define NOMINAL_TEMPERATURE    25         // Nominal Temperature for the nominal resistance of the thermistor
#define B_VALUE                4725       // B-Value of thermistor (see datasheet)
#define READINGS_NUMBER        15         // How many thermistor readings for average?
#define DELAY_TIME             25         // Delay between thermistor readings

char *profiles[] = {"custom", "PLA", "PETG", "ABS", "PA", "PA-CF"}; // Names of Filament profiles
int temperatures[] = {30, 205, 230, 220, 250};                      // Temperatures of Filament profiles

// WARNING: Safety-critical features - do not change
#define maxtemp                280        // Maximal temperature the thermistor can handle before Max-Temp triggers
#define mintemp                0          // Minimal temperature before Min-Temp triggers
#define runtimeLimit           10         // Maximal runtime in minutes


#include <Wire.h>                 // Library for I2C
#include <Adafruit_GFX.h>         // Library for graphics
#include <Adafruit_SSD1306.h>     // Library for OLED
#include <Thermistor.h>           // Library for thermistor
#include <NTC_Thermistor.h>       // Additional library for thermistor
#include <AverageThermistor.h>    // Additional library for thermistor

Adafruit_SSD1306 display(128, 64, &Wire, -1); // Create instance for display
Thermistor* thermistor = NULL;                // Create instance for thermistor

int currentProfile = 1;  // For storing the currently selected profile
float celsius = 1;       // For storing the current temperature

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // Display communication
  delay(2000);
  display.clearDisplay();

  Thermistor* originThermistor = new NTC_Thermistor(  // Thermistor configuration
    SENSOR_PIN,
    REFERENCE_RESISTANCE,
    NOMINAL_RESISTANCE,
    NOMINAL_TEMPERATURE,
    B_VALUE
  );
  thermistor = new AverageThermistor(   // For thermistor averaging
    originThermistor,
    READINGS_NUMBER,
    DELAY_TIME
  );
  pinMode(btn_UP, INPUT);     // Pin definition for button UP
  pinMode(btn_OK, INPUT);     // Pin definition for button OK
  pinMode(btn_DOWN, INPUT);   // Pin definition for button DOWN

  pinMode(mosfet, OUTPUT);    // Pin definition for mosfet

  display.setTextSize(1); //Bootscreen
  display.setTextColor(WHITE);
  display.drawRect(0, 18, 100, 46, WHITE);
  display.setCursor(2, 20);
  display.println(F("Press OK to"));
  display.setCursor(2, 30);
  display.println(F("start the heater"));
  display.drawLine(0, 39, 99, 39, WHITE);
  display.setCursor(2, 48);
  display.println(F("DEVELOPMENT ONLY"));
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println(F("HIP-HEATER"));
  display.setCursor(115, 19);
  display.setTextSize(1);
  display.println(F("UP"));
  display.drawRect(113, 17, 15, 11, WHITE);
  display.setCursor(115, 37);
  display.println(F("OK"));
  display.drawRect(113, 35, 15, 11, WHITE);
  display.setCursor(103, 55);
  display.println(F("DOWN"));
  display.drawRect(101, 53, 27, 11, WHITE);
  display.display();
  while (digitalRead(btn_OK) == 1) {  // Wait for user to confirm heating
  }
  delay(750); // Delay so OK is not pressed too early

}

void loop() {
  // Intro cleanup
  display.fillRect(0, 18, 100, 46, BLACK);
  display.drawRect(0, 18, 100, 46, WHITE);
  display.drawLine(0, 39, 99, 39, WHITE);
  display.display();
  delay(50);

  // Profile Selection
  display.setTextSize(2);
  display.setCursor(10, 44);
  display.println(profiles[currentProfile]);
  display.setTextSize(1);
  display.setCursor(2, 48);
  display.println(currentProfile);
  display.setCursor(2, 20);
  display.println(F("Select the"));
  display.setCursor(2, 30);
  display.println(F("material here:"));
  display.display();
  while (digitalRead(btn_OK) == 1) {  // Cycle through profile selector until OK is pressed
    if (digitalRead(btn_UP) == 0 && currentProfile < 5) {   // One profile higher
      currentProfile++;
      display.fillRect(0, 39, 100, 25, BLACK);
      display.drawRect(0, 39, 100, 25, WHITE);
      display.setTextSize(1);
      display.setCursor(2, 48);
      display.println(currentProfile);
      display.setTextSize(2);
      display.setCursor(10, 44);
      display.println(profiles[currentProfile]);
      display.display();
      delay(100);
    }
    if (digitalRead(btn_DOWN) == 0 && currentProfile > 0) {   // One profile lower
      currentProfile--;
      display.fillRect(0, 39, 100, 25, BLACK);
      display.drawRect(0, 39, 100, 25, WHITE);
      display.setTextSize(1);
      display.setCursor(2, 48);
      display.println(currentProfile);
      display.setTextSize(2);
      display.setCursor(10, 44);
      display.println(profiles[currentProfile]);
      display.display();
      delay(100);
    }
    delay(100);
  }

  // Heater Interface
  display.fillRect(0, 18, 100, 46, BLACK);
  display.drawRect(0, 18, 100, 46, WHITE);
  display.drawLine(0, 39, 99, 39, WHITE);
  display.setTextSize(1);
  display.setCursor(2, 20);
  display.println(F("Heating..."));
  display.setCursor(2, 30);
  display.println(profiles[currentProfile]);
  display.setCursor(2, 48);
  display.println(F("C"));
  display.display();

  while (1) {   // Cycle through heating
    celsius = thermistor->readCelsius();
    display.fillRect(0, 39, 100, 25, BLACK);
    display.drawRect(0, 39, 100, 25, WHITE);
    display.setCursor(10, 44);
    display.setTextSize(2);
    display.println(celsius);
    display.setTextSize(1);
    display.setCursor(2, 48);
    display.println(F("C"));
    display.display();

    // Heater Protection
    if (celsius < mintemp || celsius > maxtemp) {   // Check if temperature is in allowed range
      digitalWrite(mosfet, LOW);
      display.clearDisplay();
      display.setTextSize(4);
      display.setCursor(0, 16);
      display.println(F("ERROR"));
      display.setTextSize(1);
      display.setCursor(0, 50);
      display.println(F("Min-/Max-Temp"));
      display.display();
      delay (100);
      digitalWrite(mosfet, LOW);
      while (1) {
        delay(100);
      }
    }
    if (millis() > 60000*runtimeLimit) {    // Check if the press was turned on for too long
      digitalWrite(mosfet, LOW);
      display.clearDisplay();
      display.setTextSize(4);
      display.setCursor(0, 16);
      display.println(F("ERROR"));
      display.setTextSize(1);
      display.setCursor(0, 50);
      display.println(F("Runtime"));
      display.display();
      delay (100);
      digitalWrite(mosfet, LOW);
      while (1) {
        delay(100);
      }
    }

    // Simple Heater
    if (celsius < temperatures[currentProfile]) {
      digitalWrite(mosfet, HIGH);
    }
    else {
      digitalWrite(mosfet, LOW);
    }
  }
  digitalWrite(mosfet, LOW);
}
