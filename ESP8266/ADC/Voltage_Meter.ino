// For a connection via I2C using the Arduino Wire include:
#include <Wire.h>               // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h"        // legacy: #include "SSD1306.h"

SSD1306Wire display(0x3c, D3, D5);  // ADDRESS, SDA, SCL  -  If not, they can be specified manually.

#include <EEPROM.h>
int address = 0;
byte value;

int init_value = 0;
float init_vcc = 3.2;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  Serial.flush();
  delay(1000);
  Serial.flush();
  Serial.println();
  EEPROM.begin(100);
  delay(500);

  for(address =0; address < 10; address++){
    value = EEPROM.read(address);
    Serial.printf("%d\t%02X\n", address, value);
    delay(50);
  }

  // Initialising the UI will init the display too.
  display.init();

  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_24);

}

void loop() {

  
  // read the input on analog pin 0:
  int sensorValue = analogRead(A0);
  if(sensorValue < 30){
    init_value = sensorValue;
  }
  float voltage = (sensorValue - init_value) * init_vcc / 1024.0;

  Serial.print(init_value);
  Serial.print(",");
  Serial.print(init_vcc);
  Serial.print(",");
  Serial.print(voltage);
  Serial.print(",");
  Serial.println(sensorValue);
  
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(0, 16, String(voltage) + " V");
  display.display();
  delay(1000);
  
  if(Serial.available()){
    String get_str = Serial.readString();

    //  Set the init value
    if(get_str.startsWith("init_value=")){
      init_value = get_str.substring(11).toInt();
      Serial.printf("Set init_value = %d", init_value);
    }

    //  Set the init value
    if(get_str.startsWith("init_vcc=")){
      init_vcc = get_str.substring(9).toFloat();
      Serial.printf("Set init_vcc = %f", init_vcc);
    }
  }
}
