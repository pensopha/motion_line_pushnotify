void Line_Notify(String message) ;
#include <ESP8266WiFi.h>

// Update these with values suitable for your network.

const char* ssid = "";
const char* password = "";

int counter = 0;
int previousReading = LOW;


#define LINE_TOKEN "" // แก้ LINE TOKEN
String message = "มีคนเข้าบ้าน"; // ใส่ข้อความที่ต้องการส่ง
WiFiClient espClient; 

long lastMsg = 0;
char msg[50];
int value = 0;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
void setup() {
  // put your setup code here, to run once:

  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
 
  pinMode(15, INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:

   int reading = digitalRead(15);
  Serial.println(reading);
  if (previousReading == LOW && reading == HIGH) {
    counter++;
    Line_Notify(message);
   
    Serial.print("Triggered " );
    Serial.print(counter);
    Serial.print("xs! ");
    delay(1000);
  }
  previousReading = reading;
 }


  void Line_Notify(String message) {
      WiFiClientSecure client;
      
      if (!client.connect("notify-api.line.me", 443)) {
      Serial.println("connection failed");
      return;
      }
      
      String req = "";
      req += "POST /api/notify HTTP/1.1\r\n";
      req += "Host: notify-api.line.me\r\n";
      req += "Authorization: Bearer " + String(LINE_TOKEN) + "\r\n";
      req += "Cache-Control: no-cache\r\n";
      req += "User-Agent: ESP8266\r\n";
      req += "Content-Type: application/x-www-form-urlencoded\r\n";
      req += "Content-Length: " + String(String("message=" + message).length()) + "\r\n";
      req += "\r\n";
      req += "message=" + message;
      Serial.println(req);
      client.print(req);
      delay(20);
      
      Serial.println("-------------");
      while (client.connected()) {
      String line = client.readStringUntil('\n');
      if (line == "\r") {
      break;
      }
      Serial.println(line);
      }
      Serial.println("-------------");
      }

