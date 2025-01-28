#include <Arduino.h>
#include <Adafruit_MLX90614.h>
#include <LiquidCrystal_I2C.h>
#include <math.h>
 // Define buttons 
 #define BTN1 2 
 #define BTN2 3 
 #define BTN3 4 
 #define BTN4 5 // Set I2C address for the LCD (change if needed, e.g., 0x27) 
 LiquidCrystal_I2C lcd(0x27, 16, 2); // 16 chars, 2 lines 
 // Custom epsilon character 
 byte epsilon[8] = { B00000, B00000, B01110, B10000, B11110, B10000, B01110, B00000}; 
 // Initialize MLX90614 sensor 
 Adafruit_MLX90614 mlx = Adafruit_MLX90614(); 
 float ems = 1.0; 
 // Default emissivity 
 int tempScale = 0; 
 // 0 - Celsius, 1 - Fahrenheit, 2 - Kelvin 
 float correctTemperature(float measuredTemp, float ambientTemp, float emissivity) { 
  // Zamiana temperatur z °C na K 
  float measuredTempK = measuredTemp + 273.15; 
  float ambientTempK = ambientTemp + 273.15; // Obliczenie rzeczywistej temperatury w kelwinach 
  float trueTempK = pow( (pow(measuredTempK, 4) - (1 - emissivity) * pow(ambientTempK, 4)) / emissivity ,0.25);
  // Zamiana temperatury z K na °C 
  return trueTempK - 273.15; } 
  void setup() { 
    // Configure keyboard as input with pull-up resistors 
    delay(200); 
    pinMode(BTN1, INPUT_PULLUP); 
    pinMode(BTN2, INPUT_PULLUP); 
    pinMode(BTN3, INPUT_PULLUP); 
    pinMode(BTN4, INPUT_PULLUP); 
    // Serial port for debugging 
    Serial.begin(9600); 
    // LCD initialization 
    lcd.init(); 
    lcd.clear(); 
    lcd.backlight(); 
    lcd.setCursor(0, 0); 
    lcd.createChar(0, epsilon); 
    if (isnan(ems)) { ems = 1.0; } // Initialize MLX90614 sensor 
    if (!mlx.begin()) { lcd.setCursor(0, 1); lcd.print("MLX error!"); 
    Serial.print("MLX error!\n"); 
    while (1); // Halt if the sensor doesn't work 
    } 
    } 
    
    void loop() { 
      // Check button states and update emissivity 
    int BTN1V = digitalRead(BTN1); 
    int BTN2V = digitalRead(BTN2); 
    int BTN3V = digitalRead(BTN3); 
    int BTN4V = digitalRead(BTN4); 
    if (!BTN1V && ems < 1.0) { 
      Serial.println("Increased emissivity"); 
      ems += 0.01;
      if (ems > 1.0) ems = 1.0; // Limit to max 1.0 
      } 
      if (!BTN2V && ems > 0.0) { 
        Serial.println("Decreased emissivity"); 
        ems -= 0.01; if (ems < 0.0) ems = 0.0; 
        // Limit to min 0.0 
        } 
        if (!BTN3V) { Serial.println("Emissivity reset");
         ems = 1.0; } 
         if (!BTN4V) { tempScale = (tempScale + 1) % 3;
          // Cycle through 0, 1, 2 (Celsius, Fahrenheit, Kelvin) 
          Serial.println(tempScale == 0 ? "Switched to Celsius" : (tempScale == 1 ? "Switched to Fahrenheit" : "Switched to Kelvin"));
           delay(300);
            // Debounce delay 
            } 
            // Odczyt temperatury 
            float ObjTemp = mlx.readObjectTempC(); 
            // Temperatura obiektu 
            float AmbientTemp = mlx.readAmbientTempC(); 
            // Temperatura otoczenia 
            // Korekta temperatury 
            float correctedTemp = correctTemperature(ObjTemp, AmbientTemp, ems); 
            // Check if temperature is NaN, which may indicate a read error 
            if (isnan(correctedTemp)) { 
              Serial.println("Read error: Temperature NaN");
               correctedTemp = 0.0; 
               // Default to 0 if read fails 
               } 
               // Convert temperature based on selected scale 
               float displayTemp = correctedTemp; 
               char scaleLabel = 'C'; 
               bool showDegreeSymbol = true; 
               // Flag to show/hide degree symbol 
               if (tempScale == 1) 
               { displayTemp = correctedTemp * 9.0 / 5.0 + 32.0; 
               // Celsius to Fahrenheit 
               scaleLabel = 'F';
                } 
                else if (tempScale == 2) { 
                  displayTemp = correctedTemp + 273.15; 
                  // Celsius to Kelvin 
                  scaleLabel = 'K';
                   showDegreeSymbol = false; 
                   // No degree symbol for Kelvin 
                   } 
                   // Serial output for debugging 
                   Serial.print("Temperature: "); 
                   Serial.print(displayTemp); 
                   Serial.println("Environment Temperature: ");
                   Serial.print(AmbientTemp);
                   Serial.print(" "); 
                   Serial.println(scaleLabel); 
                   Serial.print("Emissivity: "); 
                   Serial.println(ems); 
                   // Display on LCD 
                   lcd.setCursor(0, 0); 
                   lcd.print("Temp:"); 
                   lcd.print(displayTemp); 
                   // Show temperature 
                   if (showDegreeSymbol) { 
                    lcd.print((char)223); 
                    // Degree symbol only for Celsius and Fahrenheit 
                    } lcd.print(scaleLabel);
                     lcd.setCursor(0, 1); 
                     // Display emissivity 
                     lcd.write(byte(0)); 
                     // Epsilon symbol 
                     lcd.print(":"); 
                     lcd.print(ems); 
                     delay(500); 
                     }
                