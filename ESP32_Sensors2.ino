/* DEVELOPER PABLO SILVA  / *DEVCODES*
CONNECTION TO WIFI AND POST DATA TO ENDPOINT API (NODEJS,EXPRESS,SEQUELIZE AND POSTGRES DATABASE)
*/

#include <WiFi.h>        // Install from library control---> program/include library/admin bibliot/search Wifi
#include <HTTPClient.h>  // Install from library control---> program/include library/admin bibliot/search httpClient
#include <ArduinoJson.h> // Install from library control---> program/include library/admin bibliot/search ArduinoJson
#include <DHT.h>         // Install from library control---> program/include library/admin bibliot/search dht
#include <DHT_U.h>

DHT dht(15,DHT11);   // Sensor data input: pin 15 of ESP32

const char* ssid = "WIFI NETWORK NAME";// your wifi network name
const char* password = "***********"; //your WIFI password
int pinLedSuccess=16;

void setup() {
  Serial.begin(115200);
  dht.begin();
  delay(400);
  
  WiFi.begin(ssid,password);
  while(WiFi.status()!= WL_CONNECTED){
    delay(1000);
    Serial.println("Connecting...");
  }
  Serial.println("Connected WIFI");
  pinMode(pinLedSuccess,OUTPUT);
}

void loop() {
  
 float tempValue=readTemp();
 float humValue=readHum();
 String strTempValue=String(tempValue);
 String strHumValue=String(humValue);
 
if ((WiFi.status()==WL_CONNECTED)) {
     
    HTTPClient http;   
    
    http.begin("https://api-iot-esp32.devcodes.net/api_IOT/v1/data");  //endpoint API_IOT
    http.addHeader("Content-Type", "application/json");         
     
    StaticJsonDocument<512> doc;
    doc["tempValue"] = strTempValue;
    doc["humValue"] = strHumValue;
   
    String requestBody;
    serializeJson(doc, requestBody);
     
    int httpResponseCode = http.POST(requestBody);
 
    if(httpResponseCode>0){
       
      String response = http.getString();                       
      Serial.println(httpResponseCode);   
      Serial.println(response);
      
      digitalWrite(pinLedSuccess,HIGH);
      delay(1000);
      digitalWrite(pinLedSuccess,LOW);
      delay(1000);
      Serial.print("TEMPERATURE: ");
      Serial.println(strTempValue);
      Serial.print("HUMIDITY: ");
      Serial.println(strHumValue);
     }
    else { 
    Serial.println("Error occurred while sending HTTP POST" );
    }
  }

  
  else{
     Serial.println("Lost connection to Wi-Fi");
    }
      
  delay(5000);
}


float readTemp(){
  float floatTempValue=dht.readTemperature();
  return floatTempValue;
}

float readHum(){
  float floatHumValue=dht.readHumidity();
  return floatHumValue;  
}



  
