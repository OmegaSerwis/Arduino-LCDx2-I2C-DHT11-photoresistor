#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define DHTPIN 2									// wyjœcie danych sensora jest do³¹czone do linii 2 Arduino.
#define DHTTYPE DHT11								// DHT11
DHT dht(DHTPIN, DHTTYPE);

LiquidCrystal_I2C lcd_1(0x3F, 16, 2);				// deklaracja wyœwietlaczy I2C
LiquidCrystal_I2C lcd_2(0x3B, 16, 2);

byte degree[] = {									// custom char
	B00111,
	B00101,
	B00111,
	B00000,
	B00000,
	B00000,
	B00000,
	B00000
};

int menu = 0;


void setup()
{
	dht.begin();									// inicjalizacja czujnika
	pinMode(5, INPUT_PULLUP);						// pin switcha



	lcd_1.init();
	lcd_1.backlight();
	lcd_1.setCursor(0, 0);

	lcd_2.init();
	lcd_2.backlight();
	lcd_2.setCursor(0, 0);
}

void loop()
{
	float t = dht.readTemperature();				// odczyt temperatury
	float h = dht.readHumidity();					// odczyt wilgotnoœci powietrza

	int sensorValue = analogRead(A0);				// odczut z fotorezystora
	float l = sensorValue * (100.0 / 1023.0);

	// czy odczytano wartoœci?
	if (isnan(t) || isnan(h))
	{
		// NIE -&gt; informacja o b³êdzie
		lcd_1.clear();
		lcd_1.setCursor(0, 0);
		lcd_1.print("Blad komunikacji ");
		lcd_1.setCursor(0, 1);
		lcd_1.print("z czunikiem !!!");		
		lcd_2.clear();
		lcd_2.setCursor(0, 0);
		lcd_2.print("Blad komunikacji ");
		lcd_2.setCursor(0, 1);
		lcd_2.print("z czunikiem !!!");
		delay(3000);
	}
	else
	{
		// TAK -&gt; wysy³amy wyniki przez port szeregowy

		if (digitalRead(5) == LOW) {				// sprawdzenie pozycji menu na switchu
			menu = menu + 1;
			if (menu == 3) {
				menu = 0;
			}
		}

		lcd_2.createChar(0, degree);
		lcd_2.clear();
		lcd_2.setCursor(0, 0);
		lcd_2.print("Temp:");
		lcd_2.setCursor(10, 0);
		lcd_2.print("Wilg:");
		lcd_2.setCursor(0, 1);
		lcd_2.print(t);
		lcd_2.write(0);
		lcd_2.print("C");
		lcd_2.setCursor(10, 1);
		lcd_2.print(h);
		lcd_2.print("%");

		switch (menu)
		{
			case 0:
				{
					lcd_1.createChar(0, degree);
					lcd_1.clear();
					lcd_1.setCursor(0, 0);
					lcd_1.print("Temperatura : ");
					lcd_1.setCursor(0, 1);
					lcd_1.print(t);
					lcd_1.print(" ");
					lcd_1.write(0);
					lcd_1.print("C");
					delay(100);
					break;
				}
			case 1:
				{
					lcd_1.clear();
					lcd_1.setCursor(0, 0);
					lcd_1.print("Wilgotnosc : ");
					lcd_1.setCursor(0, 1);
					lcd_1.print(h);
					lcd_1.print(" %");
					delay(100);
					break;
				}
			case 2:
				{
					lcd_1.clear();
					lcd_1.setCursor(0, 0);
					lcd_1.print("Swiatlo : ");
					lcd_1.setCursor(0, 1);
					lcd_1.print(l);
					lcd_1.print(" % ");
					delay(500);
					break;
				}
		}
		
	}
}
