#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

/* Put your SSID & Password */
const char* ssid = "NodeMCU";  // Enter SSID here
const char* password = "12345678";  //Enter Password here

/* Put IP Address details */
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

ESP8266WebServer server(80);

uint8_t LEDbuiltpin = LED_BUILTIN;
bool LEDbuiltstatus = LOW;

//uint8_t LED1pin = LED_BUILTIN;
bool LED1status = LOW;

//uint8_t LED2pin = D6;
bool LED2status = LOW;

//uint8_t LED3pin = D2;
//bool LED3status = LOW;

int motor1Pin1 = D8;
int motor1Pin2 = D7;
int motor2Pin1 = D6;
int motor2Pin2 = D5;
int enablePin = D3;
int enablePin2 = D4;
int motor1enable = 0;
int motor2enable =0;
bool previousmotor1stat = LOW;
bool previousmotor2stat = LOW;
int powertime1 = 0;
int powertime2= 0;
//bool motor1stat = LOW;
//bool motor2stat = LOW;
int enable = 0;
int enabletime = 0;

void setup() {
  Serial.begin(115200);
  pinMode(LEDbuiltpin, OUTPUT);
  digitalWrite(LEDbuiltpin,LOW);
//  pinMode(LED2pin, OUTPUT);
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  pinMode(enablePin, OUTPUT);
  pinMode(enablePin2, OUTPUT);

  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
  
  server.on("/", handle_OnConnect);
  server.on("/led1on", handle_led1on);
  server.on("/led1off", handle_led1off);
  server.on("/led2on", handle_led2on);
  server.on("/led2off", handle_led2off);
  server.onNotFound(handle_NotFound);
  
  server.begin();
  Serial.println("HTTP server started");
  delay(200);
  //fermeture des verin
      Serial.println("fermeture des verins");
      digitalWrite(motor1Pin1, LOW);
      digitalWrite(motor1Pin2, HIGH);
      digitalWrite(motor2Pin1, LOW);
      digitalWrite(motor2Pin2, HIGH);
      digitalWrite(enablePin, HIGH);
      digitalWrite(enablePin2, HIGH);
      delay(22000);
  digitalWrite(LEDbuiltpin,LOW);
  digitalWrite(enablePin,LOW);
  digitalWrite(enablePin2,LOW);
}
void loop() {
  server.handleClient();
  enablemotor1();
  enablemotor2();
  if((previousmotor1stat != LED1status)){
   Serial.println("status changed");
   motor1enable= millis();
   
    if(LED1status){
      
      powertime1 = 22000;
      digitalWrite(motor1Pin1, HIGH);
      digitalWrite(motor1Pin2, LOW);
      Serial.println("ouverture moteur 1");
    }
    else{
      
      powertime1 = 44000;
      digitalWrite(motor1Pin1, LOW);
      digitalWrite(motor1Pin2, HIGH);
      Serial.println("fermeture moteur 1");
      }
    
   previousmotor1stat = LED1status; 
  }
  if((previousmotor2stat != LED2status)){
    motor2enable= millis();
  if(LED2status){
    
    powertime2=22000;
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW);
    Serial.println("ouverture moteur 2");
    }
  else{
    
    powertime2 =44000;
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, HIGH);
    Serial.println("fermeture moteur 2");
    }
    
    previousmotor2stat = LED2status; 
  }
}

void handle_OnConnect() {
  //LED1status = LOW;
  //LED2status = LOW;
 // Serial.print("LED1:");
  //Serial.println((LED1status));
  //Serial.print("LED2:");
  //Serial.println((LED2status));
  server.send(200, "text/html", SendHTML(LED1status,LED2status)); 
}

void handle_led1on() {
  LED1status = HIGH;
  Serial.print("LED1:");
  Serial.println((LED1status));
  Serial.print("LED2:");
  Serial.println((LED2status));
  server.send(200, "text/html", SendHTML(true,LED2status)); 
}

void handle_led1off() {
  LED1status = LOW;
  Serial.print("LED1:");
  Serial.println((LED1status));
  Serial.print("LED2:");
  Serial.println((LED2status));
  server.send(200, "text/html", SendHTML(false,LED2status)); 
}

void handle_led2on() {
  LED2status = HIGH;
  Serial.print("LED1:");
  Serial.println((LED1status));
  Serial.print("LED2:");
  Serial.println((LED2status));
  server.send(200, "text/html", SendHTML(LED1status,true)); 
}

void handle_led2off() {
  LED2status = LOW;
  Serial.print("LED1:");
  Serial.println((LED1status));
  Serial.print("LED2:");
  Serial.println((LED2status));
  server.send(200, "text/html", SendHTML(LED1status,false)); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML(uint8_t led1stat,uint8_t led2stat){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>LED Control</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #1abc9c;}\n";
  ptr +=".button-on:active {background-color: #16a085;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>H4 serrewebserver 2.0</h1>\n";
  ptr +="<h3>Using Access Point(AP) Mode</h3>\n";
  
   if(led1stat)
  {ptr +="<p>Verin1 Status: Open</p><a class=\"button button-off\" href=\"/led1off\">OPEN</a>\n";}
  else
  {ptr +="<p>Verin1 Status: close</p><a class=\"button button-on\" href=\"/led1on\">CLOSE</a>\n";}

  if(led2stat)
  {ptr +="<p>Verin2 Status: open</p><a class=\"button button-off\" href=\"/led2off\">OPEN</a>\n";}
  else
  {ptr +="<p>Verin2 Status: close</p><a class=\"button button-on\" href=\"/led2on\">CLOSE</a>\n";}

  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}


void led(){
  
  
  if(enable ==1 ){
    Serial.println(millis()-enabletime);
    digitalWrite(LEDbuiltpin, LOW);
    delay(50);
    digitalWrite(LEDbuiltpin,HIGH);
    delay(50);
   }
  else{
   // Serial.println("fermeture en cours");
 digitalWrite(LEDbuiltpin, HIGH);

 enable = 0;
  }
}
void enablemotor1(){
  if(millis()-motor1enable< powertime1){
   digitalWrite(enablePin,HIGH);
   //Serial.print("temps1: ");
   //Serial.println(millis()-motor1enable);
   enable = 1;
  }
  else{
    digitalWrite(enablePin,LOW);
    
    enable = 0;
  }
}

void enablemotor2(){
  if(millis()-motor2enable< powertime2){
   digitalWrite(enablePin2,HIGH);
   //Serial.print("temps2: ");
   //Serial.println(millis()-motor2enable);
   enable = 1;
  }
  else{
    digitalWrite(enablePin2,LOW);
    enable = 0;
  }
}
