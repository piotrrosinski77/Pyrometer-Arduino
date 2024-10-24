#include <Arduino.h>
#include <Adafruit_MLX90614.h>
#include <LiquidCrystal_I2C.h>

// Ustaw adres I2C dla wyświetlacza (zmień na odpowiedni, np. 0x27)
LiquidCrystal_I2C lcd(0x27, 16, 2);  // 16 znaków, 2 linie

// Inicjalizacja czujnika MLX90614
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

float ems = 1.0; // Domyslna emisyjnosc

void setup() {

  lcd.init();                   // Inicjalizacja wyświetlacza
  lcd.clear();
  lcd.backlight();     			// Włącz podświetlenie
  lcd.setCursor(0, 0); 			// Ustaw kursor na początku (wiersz 0, kolumna 0)

  // Inicjalizacja czujnika MLX90614
  if (!mlx.begin()) {
    lcd.setCursor(0, 1); // Przejdź do drugiej linii
    lcd.print("MLX error!");
    while (1);  // Zatrzymaj program, jeśli czujnik nie działa
  }

  // Wyświetl początkowe informacje
  lcd.setCursor(0, 0);  // Ustaw kursor w pierwszej linii
  lcd.print("Temp: ");
  
  lcd.setCursor(0, 1);  // Ustaw kursor w drugiej linii
  lcd.print("Emissivity: ");
  lcd.print(ems);  // Wyświetl początkową wartość emisyjności
}


void loop() {
  // Odczyt temperatury z czujnika
  float ObjTemp = mlx.readObjectTempC();  // Odczyt temperatury obiektu

  // Wyświetl temperaturę na wyświetlaczu
  lcd.setCursor(6, 0);  // Ustaw kursor po "Temp: " (6 kolumna w pierwszej linii)
  lcd.print(ObjTemp);  // Wyświetl temperaturę
  lcd.print(" C");  // Upewnij się, że zostawiasz miejsce na ewentualne zmiany
  lcd.print(223); // Degree symbol (optional: lcd.write(233)), ASCII or something...(?)
 
  // Wyświetl aktualną wartość emisyjności w drugiej linii (na razie bez możliwości zmiany)
  lcd.setCursor(12, 1);  // Ustaw kursor po "Emisyjnosc: " (12 kolumna w drugiej linii)
  lcd.print(ems, 2);  // Wyświetl wartość emisyjności z dwoma miejscami po przecinku

  delay(1000);  // Aktualizacja co 1 sekunda

}
