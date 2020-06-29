//TODO: ADD POWER SAVING

//LCD includes
#include <Wire.h>
#include <LCD.h>  
#include <LiquidCrystal_I2C.h>
 
// The LCD constructor - address shown is 0x27 - may or may not be correct for yours
// Also based on YWRobot LCM1602 IIC V1
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

//DHT Sensor includes
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN  2 // Pin which is connected to the DHT sensor.

// Uncomment the type of sensor in use:
#define DHTTYPE           DHT11     // DHT 11 
//#define DHTTYPE           DHT22     // DHT 22 (AM2302)
//#define DHTTYPE           DHT21     // DHT 21 (AM2301)

DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;

void SerialDebug();

void setup() 
{
  Serial.begin(9600); 

  //Initialize LCD
  lcd.begin(16,2); // sixteen characters across - 2 lines
  lcd.backlight();
  // first character - 1st line
  lcd.setCursor(0, 0);
  lcd.print("Starting Temp + ");
  // first character - 2nd line 
  lcd.setCursor(0, 1);
  lcd.print("Hum Program");
  
  // Initialize device.
  dht.begin();
  
  // Set delay between sensor readings.
  delayMS = 30000; //30sec

  Serial.print("Delay: "); Serial.println(delayMS);
  //SerialDebug()

  lcd.clear();
}

void loop() 
{    
  // Get temperature event and print its value.
  sensors_event_t event;  
  dht.temperature().getEvent(&event);
  lcd.setCursor(0,0);
  lcd.print("Temp: ");
  
  if (isnan(event.temperature)) 
  {
    lcd.print("ERROR");    
  }
  else 
  {
    lcd.print(event.temperature);    
  }
  lcd.print(" ");
  lcd.print((char)223);
  lcd.print("C");
  
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  lcd.setCursor(0,1);
  lcd.print("Hum:  ");
  
  if (isnan(event.relative_humidity)) 
  {
    lcd.print("ERROR");
  }
  else 
  {
    lcd.print(event.relative_humidity);
    lcd.print(" %");
  }

  // Delay between measurements.
  delay(delayMS);
}

void SerialDebug()
{
    // Print temperature sensor details.
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.println("Temperature");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" *C");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" *C");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" *C");  
  Serial.println("------------------------------------");
  
  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.println("Humidity");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println("%");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println("%");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println("%");  
  Serial.println("------------------------------------");
}
