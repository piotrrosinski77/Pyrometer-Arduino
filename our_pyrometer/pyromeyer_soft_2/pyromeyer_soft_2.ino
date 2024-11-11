#include <Arduino.h>
#include <Adafruit_MLX90614.h>
#include <LiquidCrystal_I2C.h>

// Define buttons
#define BTN1 2
#define BTN2 3
#define BTN3 4
#define BTN4 5

// Set I2C address for the LCD (change if needed, e.g., 0x27)
LiquidCrystal_I2C lcd(0x27, 16, 2);  // 16 chars, 2 lines

// Custom epsilon character
byte epsilon[8] = {
  B00000,
  B00000,
  B01110,
  B10000,
  B11110,
  B10000,
  B01110,
  B00000,
};

// Initialize MLX90614 sensor
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
float ems = 1.0;  // Default emissivity

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

  // Initialize MLX90614 sensor
  if (!mlx.begin()) {
    lcd.setCursor(0, 1);
    lcd.print("MLX error!");
    Serial.print("MLX error!\n");
    while (1);  // Halt if the sensor doesn't work
  }
}

void loop() {
  // Sprawdzenie stanu przycisków i aktualizacja emisyjności
  int BTN1V = digitalRead(BTN1);
  int BTN2V = digitalRead(BTN2);
  int BTN3V = digitalRead(BTN3);
  int BTN4V = digitalRead(BTN4);	

  if (!BTN1V && ems < 1.0) {
    Serial.println("Zwiększono emisyjność");
    ems += 0.01;
    if (ems > 1.0) ems = 1.0;  // Limit to max 1.0
  }
  if (!BTN2V && ems > 0.0) {
    Serial.println("Zmniejszono emisyjność");
    ems -= 0.01;
    if (ems < 0.0) ems = 0.0;  // Limit to min 0.0
  }
  if (!BTN3V) {
    Serial.println("Zresetowano emisyjność");
    ems = 1.0;
  }
  if (!BTN4V) {
    Serial.println("Key 4 pressed");
  }

  // Read temperature from the sensor
  float ObjTemp = mlx.readObjectTempC();  // Object temperature
  delay(200);

  // Check if temperature is NaN, which may indicate a read error
  if (isnan(ObjTemp)) {
    Serial.println("Read error: Temperature NaN");
    ObjTemp = 0.0;  // Default to 0 if read fails
  }

  // Serial output for debugging
  Serial.print("Temperature: ");
  Serial.println(ObjTemp);
  Serial.print("Emissivity: ");
  Serial.println(ems);
  
  // Display on LCD
  lcd.setCursor(0, 0);
  lcd.print("Temp:");
  lcd.print(ObjTemp);      // Show temperature
  lcd.print((char)223);    // Degree symbol
  lcd.print("C");

  lcd.setCursor(0, 1);     // Display emissivity
  lcd.write(byte(0));      // Epsilon symbol
  lcd.print(":");
  lcd.print(ems);

  delay(500);
}
