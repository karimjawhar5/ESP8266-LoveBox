+#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h> 
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <Servo.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

unsigned long current;
unsigned long prev=0;

boolean done=false;

boolean doneit=true;

int turns=1;
int runs=0;

const unsigned char image1 [] PROGMEM = {
  0xff, 0xff, 0xff, 0xff, 0xf0, 0x01, 0xff, 0xf1, 0xe0, 0x00, 0x00, 0x60, 0x18, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x3f, 0xf3, 0xc0, 0x00, 0x30, 0x60, 0x1c, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xf0, 0x01, 0xff, 0x1f, 0xf3, 0x80, 0x0c, 0x7c, 0x00, 0x3c, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xe1, 0x07, 0xff, 0xc7, 0xf3, 0x00, 0x1e, 0x30, 0x00, 0x3c, 0x7f, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0x82, 0x0f, 0xff, 0xf1, 0xf2, 0x00, 0x7f, 0x00, 0x00, 0x3c, 0x7f, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0x08, 0x3f, 0xff, 0xfc, 0xf0, 0x1f, 0xff, 0x80, 0x00, 0x7c, 0x7f, 0xff, 0xff, 
  0xff, 0xff, 0xfc, 0x20, 0x7f, 0xff, 0xe0, 0x00, 0x00, 0xff, 0xe0, 0x00, 0xf8, 0x7f, 0xff, 0xff, 
  0xff, 0xff, 0xf8, 0x40, 0x7f, 0xff, 0xe0, 0x04, 0x00, 0x7c, 0x02, 0x00, 0xf0, 0x7f, 0xff, 0xff, 
  0xff, 0xff, 0xf0, 0x80, 0xff, 0x80, 0x07, 0xe0, 0x7f, 0xfc, 0x00, 0x60, 0x60, 0x7f, 0xff, 0xff, 
  0xff, 0xff, 0xf0, 0x80, 0xf8, 0x00, 0x00, 0x00, 0x00, 0xff, 0xfe, 0x30, 0x00, 0x7f, 0xff, 0xff, 
  0xff, 0xff, 0xe0, 0x00, 0xe0, 0x1f, 0xb0, 0x06, 0x00, 0x7e, 0x0f, 0x98, 0x02, 0x7f, 0xff, 0xff, 
  0xff, 0xff, 0xc0, 0x04, 0xc3, 0xff, 0xbc, 0x3c, 0x0f, 0xfe, 0x03, 0xf8, 0x04, 0x7f, 0xff, 0xff, 
  0xff, 0xff, 0xc4, 0x00, 0x06, 0x8f, 0xbf, 0x01, 0x83, 0xfe, 0x18, 0xfc, 0x0c, 0x7f, 0xff, 0xff, 
  0xff, 0xff, 0xc4, 0x00, 0x02, 0x1f, 0xbf, 0xfe, 0xff, 0xff, 0x8f, 0xfc, 0x08, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0x8c, 0x00, 0x19, 0x1c, 0x3f, 0xfe, 0xff, 0xff, 0xff, 0xf8, 0x08, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0x9c, 0x00, 0x18, 0x00, 0xf8, 0x7e, 0x7f, 0xff, 0xff, 0xf8, 0x38, 0x7f, 0xff, 0xff, 
  0xff, 0xff, 0x18, 0x01, 0x1f, 0x1f, 0xc0, 0xfe, 0x40, 0x7f, 0xff, 0xf0, 0x78, 0x7f, 0xff, 0xff, 
  0xff, 0xff, 0x30, 0x01, 0x1f, 0xff, 0xe7, 0xfe, 0x18, 0x01, 0xfe, 0xe1, 0xf8, 0xfe, 0x7f, 0xff, 
  0xff, 0xff, 0x20, 0x01, 0x9f, 0xff, 0xff, 0x80, 0x1f, 0xfe, 0xc4, 0xe3, 0xf8, 0xe0, 0x3f, 0xff, 
  0xff, 0xff, 0x20, 0x01, 0x8f, 0xff, 0xfe, 0x3e, 0x07, 0xfe, 0x99, 0xa7, 0xf0, 0xe0, 0x3f, 0xff, 
  0xff, 0xfd, 0x20, 0x00, 0xcf, 0xff, 0x80, 0x0e, 0x00, 0x01, 0x3a, 0x07, 0xe0, 0xe0, 0xff, 0xff, 
  0xff, 0xfc, 0xa4, 0x00, 0x07, 0xff, 0xe0, 0x03, 0x5c, 0x02, 0x7c, 0x4f, 0xe0, 0xe0, 0xff, 0xff, 
  0xff, 0xfc, 0xe4, 0x00, 0x03, 0xff, 0xff, 0xff, 0x2f, 0xfc, 0xfc, 0x9f, 0x81, 0xe0, 0xff, 0xff, 
  0xff, 0xfc, 0xc6, 0x00, 0x01, 0xff, 0xff, 0xff, 0x20, 0x7d, 0xfc, 0x3c, 0x01, 0xc1, 0xff, 0xff, 
  0xff, 0xfc, 0xe6, 0x00, 0x00, 0xff, 0xff, 0xff, 0xa6, 0x73, 0xd8, 0x78, 0x0f, 0xc3, 0xff, 0xff, 
  0xff, 0xfc, 0xe3, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xb7, 0x63, 0xd8, 0xf8, 0x3f, 0xc3, 0xff, 0xff, 
  0xff, 0xec, 0xf1, 0x80, 0x00, 0x1f, 0xff, 0xff, 0x37, 0xe7, 0xd8, 0xf8, 0x3f, 0x3f, 0xff, 0xff, 
  0xff, 0xcc, 0xf8, 0xc0, 0x00, 0x0f, 0xff, 0xfe, 0x37, 0xe7, 0x99, 0xfb, 0x00, 0x1f, 0xff, 0xff, 
  0xff, 0xcd, 0xfc, 0x00, 0x00, 0x01, 0xff, 0xfc, 0x73, 0xe7, 0xb9, 0xf9, 0xfc, 0x0f, 0xff, 0xff, 
  0xff, 0xc9, 0xff, 0x01, 0x00, 0x00, 0x7f, 0xf8, 0x71, 0xcf, 0xa1, 0xfc, 0xff, 0x07, 0xff, 0xff, 
  0xff, 0xd9, 0xff, 0x80, 0x00, 0x00, 0x00, 0x30, 0xf8, 0x0f, 0xfd, 0xfc, 0x7f, 0xc3, 0xff, 0xff, 
  0xff, 0xf9, 0x98, 0xe0, 0x00, 0x00, 0x00, 0x60, 0xfc, 0x1f, 0xfd, 0xfe, 0x0f, 0xe1, 0xff, 0xff
};

const int red = 14;
const int green = 12;
const int blue = 13;

const int buzzer= 9;

const int shaker = 10;
boolean messagestate=false;
int buttonPushCounter = 0;   
int buttonState = 0;        
int lastButtonState = 0;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

#define OLED_RESET LED_BUILTIN
#define SCREEN_ADDRESS 0x3C

uint8_t ServoMotorpin = D4;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

String message="";
String emotion="";

ESP8266WiFiMulti wifiMulti;     // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'

ESP8266WebServer server(80);    // Create a webserver object that listens for HTTP request on port 80

void handleRoot();
void handleReceive();
void handleLogin();
void handleNotFound();

void setup(void){
  Serial.begin(115200);// Start the Serial communication to send messages to the computer
  delay(10);
  Serial.println('\n');

  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);

  pinMode(buzzer, OUTPUT);
  
  pinMode(shaker, INPUT);

  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();
  //wifiMulti.addAP("BELL449", "E1557EAE");   // add Wi-Fi networks you want to connect to
  wifiMulti.addAP("dlink-4379", "cfoxu00938");

  Serial.println("Connecting ...");
  int i = 0;
  while (wifiMulti.run() != WL_CONNECTED) { // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
    delay(250);
    Serial.print('.');
  }
  Serial.println('\n');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());               // Tell us what network we're connected to
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());            // Send the IP address of the ESP8266 to the computer

  if (MDNS.begin("esp8266")) {              // Start the mDNS responder for esp8266.local
    Serial.println("mDNS responder started");
  } else {
    Serial.println("Error setting up MDNS responder!");
  }

  server.on("/", HTTP_GET, handleRoot);        // Call the 'handleRoot' function when a client requests URI "/"
  server.on("/sent", HTTP_POST, handleLogin);
  server.on("/rec", HTTP_GET, handleReceive);// Call the 'handleLogin' function when a POST request is made to URI "/login"
  server.onNotFound(handleNotFound);           // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"

  server.begin();                            // Actually start the server
  Serial.println("HTTP server started");
  noTone(buzzer);
}

void loop(void){
  server.handleClient();
  current=millis();
  servomove(messagestate);
  diplayemotion(emotion);
  buzzit(messagestate);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(1,1);
  if (message==""){
    display.drawBitmap(0, 0, image1, 128, 64, WHITE);
  }else{
    display.println(message);
  }
  display.display();

  buttonState = digitalRead(shaker);
  if (buttonState != lastButtonState && messagestate==true) {
    if (buttonState == LOW) {
      buttonPushCounter++;
      Serial.print("number of shakes: ");
      Serial.println(buttonPushCounter);
    }
  }
  lastButtonState = buttonState;

  if(buttonPushCounter>=2){
    message="";
    emotion= "";
    messagestate=false;
    buttonPushCounter=0;
    runs=0;
    turns=1;
    doneit=true;
  }
}


void handleRoot() {                          // When URI / is requested, send a web page with a button to toggle the LED
  if(messagestate==false){
    server.send(200, "text/html","<head><style>@import url('https://fonts.googleapis.com/css2?family=VT323&display=swap');body{font-family: VT323;}.logo{position: relative;font-size:80px;height:20%;text-align: center;padding-right:6%;padding-left:6%; }iframe{width:150px;height:150px;float: left;margin-top:100px;}.tit{float: left;margin-left: 5%;margin-right:5%;padding-bottom:30px; }.information{position: relative;height:60%;font-size:70px;text-align: center;padding-top:5%;}textarea{width: 60%;height:12%;font-family:VT323; font-size:40px;padding:20px;border: solid;border-width: 3px;border-color: black;}textarea:focus{outline: none;border: solid;border-width: 3px;border-color: black;}.messagespace{padding-bottom: 100px;}[type=radio] { position: absolute; opacity: 0; width: 0; height: 0; } input[type=radio] + label>img{ width:140px; height:auto; margin: 0px; margin-right: 1%; margin-left: 1%; }input[id=happy] + label>img{content:url(\"https://i.ibb.co/zfZC9hM/emo1-01.png\");}input[id=happy]:checked + label>img{content:url(\"https://i.ibb.co/wswWrdx/emo-11.png\");}input[id=sad] + label>img{content:url(https://i.ibb.co/sbpWD5f/emo-02.png);}input[id=sad]:checked + label>img{content:url(\"https://i.ibb.co/sVvcDFj/emo-03.png\");}input[id=lovey] + label>img{content:url(\"https://i.ibb.co/RBdbKxT/emo-04.png\");}input[id=lovey]:checked + label>img{content:url(\"https://i.ibb.co/27kWQtk/emo-05.png\");}.radio_control{height:25%;padding-left: 20%;padding-right: 20%;}#submit{border:none;width:230px;height:90px;background-image: url(\"https://i.ibb.co/w0v5Cvt/submit.png\"); background-position: center; background-repeat: no-repeat; background-size: 230px 90px;}#submit:active{background-image: url(\"https://i.ibb.co/zhq0dfn/submit1.png\");}</style></head><body><div class=\"logo\"><h1 class=\"tit\">LOVE</h1><iframe src=\"https://giphy.com/embed/fa93oHXZjY8dF7kAeE\"></iframe> <h1 class=\"tit\">BOX</h1></div><div class=\"information\"><form class=\"info\" action=\"/sent\" method=\"POST\"><div class=\"messagespace\"><p>MESSAGE:</p><textArea type=\"text\" name=\"LoveText\" placeholder=\"Type your love...\"></textArea></div><div class=\"emotionspace\"><p>WHAT EMOTION IS THE MESSAGE?</p><div class=\"radio_control\"><input type=\"radio\" id=\"happy\" name=\"emotion\" value=\"happy\"><label for=\"happy\"><img></label><input type=\"radio\" id=\"sad\" name=\"emotion\" value=\"sad\"><label for=\"sad\"><img></label><input type=\"radio\" id=\"lovey\" name=\"emotion\" value=\"lovey\"><label for=\"lovey\"><img></label></div></div><input id=\"submit\" type=\"submit\" value=\"\"> </form></div></body>");
  }else{
    prev=millis();    
    server.send(200, "text/html", "<style>@import url('https://fonts.googleapis.com/css2?family=VT323&display=swap');body{font-family: VT323;text-align: center;}h2{margin-top:35%;font-size:90px;margin-bottom:0px;}h3{margin-top:30px;color:rgb(100,100,100);font-size:55px;margin-bottom:80px;}iframe{border:none;width:55%;height:30%;margin-bottom:40px;}p{font-size: 45px;} span::before { content: \".\"; animation: animate infinite 2s; padding-left: 10px; } @keyframes animate { 0% { content: \".\"; } 25% { content: \"..\"; } 50% { content: \"...\"; } 75% { content: \"....\"; } }</style><html><head><meta http-equiv=\"refresh\" content=\"10\"></head><body><h2>You Sent a Message</h2><h3>"+message+"</h2><iframe src=\"https://giphy.com/embed/fa93oHXZjY8dF7kAeE\"></iframe><p>Waiting for Message to be Seen<span></span></p></body></html>");
  }
}

void handleLogin() {                         // If a POST request is made to URI /login
  if( ! server.hasArg("LoveText") || server.arg("LoveText") == NULL) { // If the POST request doesn't have username and password data
    server.send(400, "text/plain", "400: Invalid Request");         // The request is invalid, so send HTTP status 400
    return;
  }else{
    message= server.arg("LoveText");
    emotion= server.arg("emotion");
    messagestate=true;
    server.sendHeader("Location","/");
    server.send(303);
  }
}

void handleNotFound(){
  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}

void handleReceive(){
  server.send(200, "text/html", "<h2> You Sent " + server.arg("LoveText") + " to Box</h2><p>message has been seen...</p>");
}
void servomove(boolean state){ 
  //if(!done){
   // prev=current;
   // done=true;
  //}
  if(state && runs<=2){
    if(turns==1){
      analogWrite(ServoMotorpin,28);
      analogWriteFreq(50);
      if(current-prev>=500){
        turns++;
        prev=current;
      }
    }

    if(turns==2){
      analogWrite(ServoMotorpin,10);
      analogWriteFreq(50);
      if(current-prev>=500){
        turns--;
        prev=current;
        runs++;
      }
    }
    }
    else{
      analogWrite(ServoMotorpin,19);
      analogWriteFreq(50);
    }
}
void diplayemotion(String emo){
  if(emo=="happy"){
    digitalWrite(red, LOW);
    digitalWrite(green, LOW);
    digitalWrite(blue, HIGH);
  }else if(emo=="sad"){
    digitalWrite(red, HIGH);
    digitalWrite(green, HIGH);
    digitalWrite(blue, LOW);
    
  }else if(emo=="lovey"){
    digitalWrite(red, HIGH);
    digitalWrite(green, LOW);
    digitalWrite(blue, HIGH);
    
  }else{
    digitalWrite(red, HIGH);
    digitalWrite(green, HIGH);
    digitalWrite(blue, HIGH);
  }
}

void buzzit(boolean state){
  if(state && doneit){
    analogWrite(buzzer,2000);
    delay(300);
    noTone(buzzer);
    delay(100);
    analogWrite(buzzer,1000);
    delay(100);
    doneit=false;
  }else{
    noTone(buzzer);
  }
}

