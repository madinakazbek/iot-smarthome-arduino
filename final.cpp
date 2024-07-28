// Template and device credentials for Blynk  
#define BLYNK_TEMPLATE_ID "insert yours"
#define BLYNK_TEMPLATE_NAME "IoT Smart Exhaust Fan"
#define BLYNK_AUTH_TOKEN "insert yours" 
#define BLYNK_PRINT Serial  // Directs Blynk messages to the serial port  
  
#include <ESP8266WiFi.h>  // Include WiFi library for ESP8266  
#include <BlynkSimpleEsp8266.h>  // Include Blynk ESP8266 library  
#include <DHT.h>  // Include DHT sensor library  
#include <Servo.h> 
// Blynk and WiFi credentials (token, name, password)   
char auth[] = "****";   
char ssid[] = "***";  
char pass[] = "***; 
 
// Pin configuration  
#define DHTPIN 13   // DHT sensor pin (D4 on NodeMCU)  
#define DHTTYPE DHT11  // Type of DHT sensor  
Servo servo; 
const int gasSensorPin = 12;  // Gas sensor analog pin  
const int relayPin = 2;  // Relay module pin (D1 on NodeMCU)  
  
const int gasThreshold = 20;  // Gas level threshold for triggering the relay  
  
DHT dht(DHTPIN, DHTTYPE);  // Initialize DHT sensor  
BLYNK_WRITE(V4) {
int s1 = param.asInt();
servo.write(s1);
Blynk.virtualWrite(V4, s1);

}
void setup()  
{  
  Serial.begin(115200);  // Start serial communication at 115200 baud rate  
  dht.begin();  // Initialize DHT sensor  
  Blynk.begin(auth, ssid, pass);  // Connect to Blynk using WiFi  
  servo.attach(14); 
  pinMode(relayPin, OUTPUT);  // Set relay pin as output  
  digitalWrite(relayPin, LOW);  // Ensure relay is off initially  
  
  delay(2000);  // Wait for 2 seconds before proceeding  
}  
  
void loop()  
{  
  Blynk.run();  // Run Blynk  
  
  int sensorValue = analogRead(gasSensorPin);  // Read gas sensor value  
  int gas_percentage = map(sensorValue, 0, 1023, 0, 100);  // Convert to percentage  
  
  float humidity = dht.readHumidity();  // Read humidity from DHT sensor  
  float temperature = dht.readTemperature();  // Read temperature from DHT sensor  
  int pos_servo = 90; 
  int pos_status = 90;
  // Check if sensor readings are valid  
  if (isnan(humidity) || isnan(temperature))  
  {  
    Serial.println(F("Failed to read from DHT sensor!"));  
    return;  
  }  
  
  // Print sensor values to the Serial Monitor  
  Serial.print("Humidity: ");  
  Serial.print(humidity);  
  Serial.println("%");  
  
  Serial.print("Temperature: ");  
  Serial.print(temperature);  
  Serial.println("°C ");  
  
  Serial.print("Gas sensor value: ");  
  Serial.println(sensorValue);  
  
  Serial.print("Gas Percentage: ");  
  Serial.print(gas_percentage);  
  Serial.println("%");  

  Serial.print("Servo_status: ");  
  Serial.print(pos_status);  
  Serial.println(""); 
  Serial.println();  
  
  // Automatic control logic based on gas levels  
  if (gas_percentage > gasThreshold)  
  {  
    digitalWrite(relayPin, HIGH);  // Activate relay if gas above threshold  
    Blynk.virtualWrite(V3, HIGH);  // Update Blynk app  
  }  
  else  
  {  
    digitalWrite(relayPin, LOW);  // Deactivate relay if gas below threshold  
    Blynk.virtualWrite(V3, LOW);  // Update Blynk app  
  }  
  if (temperature > 27.0)  
  {  
    pos_servo = 180; 
    pos_status = 180;
    servo.write(pos_servo);              // tell servo to go to position in variable 'pos' 
    delay(15); // Activate relay if gas above threshold  
    Blynk.virtualWrite(V4, pos_status);  // Update Blynk app  
  }  
  else if (temperature < 20.0)  
  {  
    pos_servo = 0; 
    pos_status = 0;
    servo.write(pos_servo);  // Deactivate relay if gas below threshold  
    Blynk.virtualWrite(V4, pos_status);  // Update Blynk app  
  }
  
  // Send sensor values to Blynk  
  Blynk.virtualWrite(V0, gas_percentage);  // Send gas percentage  
  Blynk.virtualWrite(V1, temperature);  // Send temperature  
  Blynk.virtualWrite(V2, humidity);  // Send humidity  
  
  delay(1000);  // Wait for a second before the next loop iteration  
}
