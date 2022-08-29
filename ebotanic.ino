#include <LiquidCrystal.h>         
#include "DHT.h"

#define LDR A0  // Analog pin connected to LDR sensor
#define DHTPIN 7     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); //Digital pins to which you connect the LCD
const int inPin = 0;                   // A0 is where you connect the sensor
int loopIndex = 0;
const int carouselCount = 4;

typedef String (*SensorFunction) ();

typedef struct {
  String title;
  int delay;
  SensorFunction valueFunction;
  String error;
} CarouselItem;

String getPlantName() {
  return "{PLANT_NAME}";
}

String getTemperature() {
  String valueString = "";

  float temperature = dht.readTemperature();

  if (isnan(temperature)) {
    return valueString;
  }

  valueString = temperature;
  valueString += "C";
}

String getHumidity() {
  String valueString = "";
  
  float humidity = dht.readHumidity();

  if (isnan(humidity)) {
    return valueString;
  }

  valueString = humidity;
  valueString += "%";
}

String getPhotoresistance() {
  String valueString = "";

  int photoresistance = analogRead(LDR);

  if (isnan(photoresistance)) {
    return valueString;
  }

  valueString = photoresistance;
  valueString += " Ohms";
}

CarouselItem items[carouselCount] = {
  {
    .title = "Hello, I'm",
    .delay = 3000,
    .valueFunction = getPlantName,
    .error = "",
  },
  {
    .title = "Temperature",
    .delay = 2000,
    .valueFunction = getTemperature,
    .error = "Failed to read temperature",
  },
  {
    .title = "Humidity",
    .delay = 2000,
    .valueFunction = getHumidity,
    .error = "Failed to read humidity",
  },
  {
    .title = "Photoresistance",
    .delay = 2000,
    .valueFunction = getPhotoresistance,
    .error = "Failed to read photoresistance",
  },
};

void treatCarouselItem(CarouselItem item) {
  lcd.setCursor(0,0);
  lcd.print(item.title);
  lcd.setCursor(0,1);
  lcd.print(item.valueFunction());
}

void setup()
{
  Serial.begin(9600);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Starting...");

  lcd.begin(16,2);
  dht.begin();
  pinMode(LDR, INPUT);
}

void loop()
{
  lcd.clear();

  int itemsCount = carouselCount;
  int carouselIndex = loopIndex % itemsCount;
  treatCarouselItem(items[carouselIndex]);

  loopIndex++;
  delay(items[carouselIndex].delay);
}