// Visual Micro is in vMicro>General>Tutorial Mode
// 
/*
    Name:       AirQualitySensor.ino
    Created:	26.10.2018 12:21:23
    Author:     Bogumil Piskorz
*/

#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <Wire.h>
#include <MQ135/MQ135.h>

#define DHTPIN 2									// DHT11 data
#define DHTTYPE DHT11								// DHT11
DHT dht(DHTPIN, DHTTYPE);

LiquidCrystal_I2C lcd_1(0x3f, 16, 2);
LiquidCrystal_I2C lcd_2(0x3b, 16, 2);

byte degree[] = {									// custom char degree
	B00111,
	B00101,
	B00111,
	B00000,
	B00000,
	B00000,
	B00000,
	B00000
};

int buzz = 1;										// buzzer PIN
int pin = A5;										// MQ135 PIN
int sensorValue;

MQ135 gasSensor = MQ135(pin);

void lcd1Init()
{
	lcd_1.init();									// LCD 1 initialization
	lcd_1.backlight();
	lcd_1.setCursor(0, 0);
}

void lcd2Init()
{
	lcd_2.init();									// LCD 2 initialization
	lcd_2.backlight();
	lcd_2.setCursor(0, 0);
}

void setup()
{
	dht.begin();
	pinMode(buzz, OUTPUT);
	digitalWrite(buzz, LOW);

	lcd1Init();
	lcd2Init();
}

void loop()
{
	float t = dht.readTemperature();				// read temp
	float h = dht.readHumidity();					// read  humidity
	sensorValue = analogRead(pin);					// data read form air sensor

	float rzero = gasSensor.getRZero();					// calibration air sensor
	float co2_ppm = gasSensor.getPPM();					// air quality
	int ppm = co2_ppm / 4;
		
	if (isnan(t) || isnan(h))						// DHT11 test
	{
		lcd_1.clear();
		lcd_1.setCursor(0, 0);
		lcd_1.print("Blad czujnika ");
		lcd_1.setCursor(0, 1);
		lcd_1.print("temperatury !!!");

		delay(3000);
	}
	else
	{
		if (sensorValue > 1020)						// MQ135 test
		{				
			lcd_2.clear();
			lcd_2.setCursor(0, 0);
			lcd_2.print("Blad czujnika ");
			lcd_2.setCursor(0, 1);
			lcd_2.print("powietrza !!!");
			delay(3000);
		}
		else
		{
			lcd_1.createChar(0, degree);
			lcd_1.clear();
			lcd_1.setCursor(0, 0);
			lcd_1.print("Temp :");
			lcd_1.setCursor(10, 0);
			lcd_1.print("Wilg :");
			lcd_1.setCursor(0, 1);
			lcd_1.print(t);
			lcd_1.write(0);
			lcd_1.print("C");
			lcd_1.setCursor(10, 1);
			lcd_1.print(h);
			lcd_1.print("%");

			//float rzero = gasSensor.getRZero();					// calibration air sensor
			lcd_2.setCursor(0, 0);
			lcd_2.print("RZero= " + (String)rzero + " ");

			//float co2_ppm = gasSensor.getPPM();					// air quality
			lcd_2.setCursor(0, 1);
			lcd_2.print("co2 ppm= " + (String)co2_ppm + " ");
			//int ppm = co2_ppm / 4;
			//lcd_2.print((String)ppm);
			delay(100);
		}

	}

}
