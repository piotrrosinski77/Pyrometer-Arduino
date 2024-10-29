#include <Arduino.h>
#include <Adafruit_MLX90614.h>
#include <LiquidCrystal_I2C.h>

// Define buttons
#define BTN1 2
#define BTN2 3 
#define BTN3 4 
#define BTN4 5 

// Ustaw adres I2C dla wyświetlacza (zmień na odpowiedni, np. 0x27)
LiquidCrystal_I2C lcd(0x27, 16, 2);  // 16 znaków, 2 linie

// own character
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

// Inicjalizacja czujnika MLX90614
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

float ems = 1.0; // Domyslna emisyjnosc

void setup() {

  // Keyboard set as pull-up input
  pinMode(key1, INPUT_PULLUP);
  pinMode(key2, INPUT_PULLUP);
  pinMode(key3, INPUT_PULLUP);
  pinMode(key4, INPUT_PULLUP); 

  // Serial port for debugging
  Serial.begin(9600);
  Serial.print("Initializing LCD...\n");
  lcd.init();                   // Inicjalizacja wyświetlacza
  lcd.clear();
  lcd.backlight();     			// Włącz podświetlenie
  lcd.setCursor(0, 0); 			// Ustaw kursor na początku (wiersz 0, kolumna 0)
  lcd.createChar(0, epsilon);   // created E for emissivity (greek alphabet)
  Serial.print("Created char epsilon\n");

  // Inicjalizacja czujnika MLX90614
  if (!mlx.begin()) {
    lcd.setCursor(0, 1); // Przejdź do drugiej linii
    lcd.print("MLX error!");
    Serial.print("MLX error!\n");
    while (1);  // Zatrzymaj program, jeśli czujnik nie działa
  }

  // Wyświetl początkowe informacje
  lcd.setCursor(0, 0);  // Ustaw kursor w pierwszej linii
  lcd.print("Temp: ");
  lcd.setCursor(0, 1);  // Ustaw kursor w drugiej linii
  lcd.write(byte(0));   //output the custom character
  lcd.print(":");
  lcd.print(ems);  // Wyświetl początkową wartość emisyjności
}


void loop() {
	
  // Check whether the buttons are pressed or not
  int BTN1V = digitalRead(BTN1);
  int BTN2V = digitalRead(BTN2);
  int BTN3V = digitalRead(BTN3);
  int BTN4V = digitalRead(BTN4);	
  
  if(!BTN1V){
	  Serial.println("key 1 is pressed");
	  }
  if(!BTN2V){
      Serial.println("key 2 is pressed");
      }
  if(!BTN3V){
      Serial.println("key 3 is pressed");
      }
  if(!BTN4V){
      Serial.println("key 4 is pressed");
      }          
  delay(100);
		
  // Odczyt temperatury z czujnika
  float ObjTemp = mlx.readObjectTempC();  // Odczyt temperatury obiektu

  Serial.println("Temperature: "); //Output data to serial
  Serial.println(ObjTemp);
  Serial.println("\n");
  Serial.println("Emissivity: ");
  Serial.println(ems);
  Serial.println("\n");
  
  // Wyświetl temperaturę na wyświetlaczu
  lcd.setCursor(6, 0);  // Ustaw kursor po "Temp: " (6 kolumna w pierwszej linii)
  lcd.print(ObjTemp);  // Wyświetl temperaturę
  lcd.print(" C");  // Upewnij się, że zostawiasz miejsce na ewentualne zmiany
  lcd.write(223); // Degree symbol (optional: lcd.write(233)), ASCII or something...(?)
 
  // Wyświetl aktualną wartość emisyjności w drugiej linii (na razie bez możliwości zmiany)
  lcd.setCursor(12, 1);  // Ustaw kursor po "Emisyjnosc: " (12 kolumna w drugiej linii)
  lcd.print(ems, 2);  // Wyświetl wartość emisyjności z dwoma miejscami po przecinku

  delay(1000);  // Aktualizacja co 1 sekunda

}
