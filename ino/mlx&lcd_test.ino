#include <Adafruit_MLX90614.h>
#include <LiquidCrystal.h>

int contrast=100;
LiquidCrystal lcd(12,11,5,4,3,2);
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

void setup() {
  Serial.begin(9600);
  analogWrite(6,contrast);
  lcd.begin(16,2);
  while (!Serial);

  Serial.println("MLX90614 and LCD1602 test");

  if (!mlx.begin()) {
    Serial.println("Error connecting to MLX sensor or LCD1602. Check wiring.");
    while (1);
  };

  Serial.print("Emissivity = "); Serial.println(mlx.readEmissivity());
  Serial.println("================================================");
}

void loop() {
  double ambientC = mlx.readAmbientTempC();
  double objectC = mlx.readObjectTempC();
  double ambientF = mlx.readAmbientTempF();
  double objectF = mlx.readObjectTempF();

  Serial.print("Ambient = "); Serial.print(ambientC);
  Serial.print("*C\tObject = "); Serial.print(objectC); Serial.println("*C");
  Serial.print("Ambient = "); Serial.print(ambientF);
  Serial.print("*F\tObject = "); Serial.print(objectF); Serial.println("*F");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Amb: "); lcd.print(ambientC, 1); lcd.print("*C");
  lcd.setCursor(0, 1);  
  lcd.print("Obj: "); lcd.print(objectC, 1); lcd.print("*C");

lcd.display();
  Serial.println();
  delay(500);
}
