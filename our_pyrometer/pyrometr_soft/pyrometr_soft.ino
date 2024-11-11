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

// Własny znak epsilon
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

float ems = 1.0; // Domyślna emisyjność

void setup() {
  // Konfiguracja klawiatury jako wejścia z podciąganiem
  delay(200);
  pinMode(BTN1, INPUT_PULLUP);
  pinMode(BTN2, INPUT_PULLUP);
  pinMode(BTN3, INPUT_PULLUP);
  pinMode(BTN4, INPUT_PULLUP); 

  // Serial port for debugging
  Serial.begin(9600);
  Serial.print("Initializing LCD...\n");
  lcd.init();                   // Inicjalizacja wyświetlacza
  lcd.clear();
  lcd.backlight();     			// Włącz podświetlenie
  lcd.setCursor(0, 0); 			// Ustaw kursor na początku (wiersz 0, kolumna 0)
  lcd.createChar(0, epsilon);   // Stworzenie greckiego znaku epsilon
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
}

void loop() {
  // Sprawdzenie stanu przycisków i aktualizacja emisyjności
  int BTN1V = digitalRead(BTN1);
  int BTN2V = digitalRead(BTN2);
  int BTN3V = digitalRead(BTN3);
  int BTN4V = digitalRead(BTN4);	

  if (!BTN1V && ems <= 1) {
    Serial.println("Zwiększono emisyjność");
    ems += 0.01;
  }
  if (!BTN2V && ems >= 0) {
    Serial.println("Zmniejszono emisyjność");
    ems -= 0.01;
  }
  if (!BTN3V) {
    Serial.println("Zresetowano emisyjność");
    ems = 1;
  }
  if (!BTN4V) {
    Serial.println("Key 4 pressed");
  }

  // Aktualizacja emisyjności czujnika
  mlx.writeEmissivity(ems);

  // Odczyt temperatury z czujnika
  float ObjTemp = mlx.readObjectTempC();  // Odczyt temperatury obiektu
  ems = mlx.readEmissivity();

  // Sprawdzenie, czy emisyjność jest NaN, jeśli tak, ustaw na domyślną wartość
  if (isnan(ems)) {
    Serial.println("Emisyjność NaN, ustawiam na domyślną wartość");
    ems = 1.0;
    mlx.writeEmissivity(ems); // Ustaw ponownie w czujniku
  }

  // Wyjście na Serial dla debugowania
  Serial.println("Temperature: ");
  Serial.println(ObjTemp);
  Serial.println("\n");
  Serial.println("Emissivity: ");
  Serial.println(ems);
  Serial.println("\n");
  
  // Wyświetlanie na LCD
  lcd.setCursor(6, 0);  // Ustaw kursor po "Temp: " (6 kolumna w pierwszej linii)
  lcd.print(ObjTemp);   // Wyświetl temperaturę
  lcd.print(" C");      // Symbol stopnia
  //lcd.write(223);       // Stopień Celsiusza
 
  lcd.setCursor(0, 1);  // Wyświetl emisyjność
  lcd.write(byte(0));   // Symbol epsilon
  lcd.print(":");
  lcd.print(ems);       // Wyświetl wartość emisyjności

  delay(500);
}
