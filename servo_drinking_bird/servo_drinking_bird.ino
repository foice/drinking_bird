/*
 * ESP8266 Servo Motor Control With Web Server 
 * https://circuits4you.com
 */

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Servo.h>
#include <time.h>

#include "index.h";
#include "credentials.h";

#define LED 2
#define ServoPin 14   //D5 is GPIO14


int max_angle = 1; // the value that I control from the web interface
int min_angle = 6; // the value that I control from the web interface




Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

ESP8266WebServer server(80);

//================================================
void handleServo(){
  String POS = server.arg("servoPOS");
  int pos = POS.toInt();
  myservo.write(pos);   // tell servo to go to position
  delay(15);
  Serial.print("Servo Angle:");
  Serial.print(myservo.read() );
  Serial.print("=?=");
  Serial.println(pos);
  digitalWrite(LED,!(digitalRead(LED))); //Toggle LED
  server.send(200, "text/html", "new position:"+String(pos) );
}


void handleSetting()
{ // If a POST request is made to URI /clear
  String value;
  value = server.arg("minValue");
  Serial.println("handleSetting(" + value + ")");
  Serial.println("min_angle: " + String(min_angle));
  min_angle = value.toInt(); // string to integer
  Serial.println("min_angle: " + String(min_angle));

  value = server.arg("maxValue");
  Serial.println("handleSetting(" + value + ")");
  Serial.println("max_value: " + String(max_angle));
  max_angle = value.toInt(); // atof?
  Serial.println("max_angle: " + String(max_angle));

  server.send(200, "text/html", "New min_angle: " + String(min_angle) + " .<br> " + "New max_angle: " + String(max_angle) + " .");
  // will use `value` to update the parameter I want to change
  //return true;
}



void handleSimpleRoot() {
  int read_pos = myservo.read();
  String _title = "<h1>Servo Server</h1>";
  String sub_title = "<h2>Currently turned at: "+ String(read_pos)  +"</h2>";
  String range = "<h3>Current range: "+ String(min_angle) +","+ String(max_angle) +"</h3>";
  //String form="<br><form action=\"/setPOS\"> Update position (0-300?):<input type=\"text\" name=\"servoPOS\" value=\"" + String(read_pos) + "\">  <input type=\"submit\" value=\"Submit\"><br></form>";
  String open_form  = "<br><form action=\"/setting\">";
  String max_value  = "Min position: <input type=\"text\" name=\"minValue\" value=\"" + String(min_angle) + "\">  <input type=\"submit\" value=\"Submit\"><br>";
  String min_value  = "Max position: <input type=\"text\" name=\"maxValue\" value=\"" + String(max_angle) + "\">  <input type=\"submit\" value=\"Submit\"><br>";
  String close_form = "</form><br>";

  String myHTML= _title+sub_title+range+open_form+max_value+min_value+close_form ;
  
 server.send(200, "text/html", myHTML); //Send web page
}



void handleDevelRoot() {
  String myHTML= par ;
  server.send(200, "text/html", myHTML); //Send web page
}


//void handleSetting() {                          // If a POST request is made to URI /clear  
//  String value;
//  value=server.arg("servoposition");
//  Serial.println("handleSetting(" + value+")");
//  Serial.println("update_frequency: " + String(update_frequency) );
//  update_frequency=value.toInt(); // atof?
//  Serial.println("update_frequency: " + String(update_frequency) );
//  
//  server.send(200, "text/plain", "New measurement frequency: "+String(update_frequency)+" milliseconds. \n " + "New LED flash duration: "+String(flash_duration)+" milliseconds.");
//  // will use `value` to update the parameter I want to change
//  //return true;
//}


//================================================
//            Setup
//================================================
void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println();

  pinMode(LED,OUTPUT);
  myservo.attach(ServoPin); // attaches the servo on GIO2 to the servo object
  
  //Connect to wifi Network
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");  
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP

  //Initialize Webserver
  server.on("/",handleSimpleRoot);
  server.on("/devel",handleDevelRoot);
  server.on("/setting", HTTP_GET, handleSetting);
  server.on("/setPOS",handleServo); //Sets servo position from Web request
  server.begin();  
  Serial.println("*** HTTP server started.***");

}

//================================================
//     LOOP
//================================================
void loop() {

   delay(2000);
   //long randNumber = random(10, 30);
   myservo.write(min_angle);
   delay(2000);
   myservo.write(max_angle);
   server.handleClient();
 
}
//================================================
