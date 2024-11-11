#include <Adafruit_MLX90614.h>
#include <LiquidCrystal.h>

// Pin definitions for the keypad
const int buttonPin1 = 7;
const int buttonPin2 = 8;
const int buttonPin3 = 10;
float ems;

// LCD and MLX90614 setup
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

void setup() {
  // Start serial communication
  Serial.begin(9600);
  
  // Setup LCD
  lcd.begin(16, 2);
  while (!Serial);

  // Print a message to the LCD
  Serial.println("MLX90614 and LCD1602 Test");

  // Initialize the MLX90614 sensor
  if (!mlx.begin()) {
    Serial.println("Error connecting to MLX sensor or LCD1602. Check wiring.");
    while (1);
  }

  // Print emissivity to serial
  Serial.print("Emissivity = "); Serial.println(mlx.readEmissivity());
  ems = mlx.readEmissivity();
  Serial.println("================================================");

  // Set button pins as input with internal pull-up resistors
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  pinMode(buttonPin3, INPUT_PULLUP);
}

void loop() {
  // Read the button states
  int buttonState1 = digitalRead(buttonPin1);
  int buttonState2 = digitalRead(buttonPin2);
  int buttonState3 = digitalRead(buttonPin3);

  // Check if any button is pressed
  /*
  if (buttonState1 == LOW) {
    if (buttonState1 == HIGH) {
      ems = ems + 0.01;
      if (ems > 1){
        ems=1;}
      delay(200);}
  }
  if (buttonState2 == LOW) {
    if (buttonState2 == HIGH) {
      mlx.writeEmissivity(ems);
      Serial.println(ems);
      delay(200);}
  }
  if (buttonState3 == LOW) {
      if (buttonState3 == HIGH) {
      ems = ems - 0.01;
      if (ems < 0.1){
        ems=0.1;}
      delay(200);}
  }

  // Read temperatures
  double ambientC = mlx.readAmbientTempC();
  double objectC = mlx.readObjectTempC();
  double ambientF = mlx.readAmbientTempF();
  double objectF = mlx.readObjectTempF();

  // Print temperatures to serial
 /* Serial.print("Ambient = "); Serial.print(ambientC);
  Serial.print("*C\tObject = "); Serial.print(objectC); Serial.println("*C");
  Serial.print("Ambient = "); Serial.print(ambientF);
  Serial.print("*F\tObject = "); Serial.print(objectF); Serial.println("*F");*/

  // Update the LCD display

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Amb:"); lcd.print(ambientC, 1); lcd.print("C");
  lcd.setCursor(0, 1);
  lcd.print("Obj:"); lcd.print(objectC, 1); lcd.print("C");
  lcd.setCursor(10, 0);
  lcd.print("E:"); lcd.print(ems,2); 
  // Wait before repeating the loop
  delay(500);
}
