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
  server.send(200, "text/plane","");
}

//void HThandleRoot(char* apssid) {                         // When URI / is requested, send a web page with a button to perform the actions
//  
//  String download_string="<form method=\"get\" action=\"temp.csv\" download><button type=\"submit\">Download temp.csv</button></form>";
//  String clear_string="<form method=\"GET\" action=\"clear\"><input type=\"submit\" value=\"Clear temp.csv\"></form>";
//  String list_of_files=listAllFilesNameSize();
//  char* title = apssid ;
//  //sprintf(title,"<h1> %s </h1>",apssid);
//  String _title;
//  _title = "<h1>"+String(title)+"</h1>";
//
//  String string_subnet =  String(subnet);
//  String sub_title = "<h2>Also available on private subnet: "+  string_subnet +"</h2>";
//  String flashing_notice = "This device flashes for " + String(flash_duration) + " milliseconds";
//  String form="<br><form action=\"/setting\"> Update frequency (ms): <input type=\"text\" name=\"frequency\" value=\"" + String(update_frequency) + "\">  <input type=\"submit\" value=\"Submit\"><br>";
//  String form2="LED flash duration (ms): <input type=\"text\" name=\"flash\" value=\"" + String(flash_duration) + "\">  <input type=\"submit\" value=\"Submit\"></form><br>";
//
//  server.send(200, "text/html", _title+sub_title+flashing_notice+"<h3>Actions</h3>"+download_string+" "+clear_string+"<br>"+"<h3>Files</h3>"+list_of_files+form+form2);
//}



void handleSimpleRoot() {
  int read_pos = myservo.read();
  String _title = "<h1>Servo Server</h1>";
  String sub_title = "<h2>Currently turned at: "+ String(read_pos)  +"</h2>";
  String form="<br><form action=\"/setPOS\"> Update position (0-300?): <input type=\"text\" name=\"servoPOS\" value=\"" + String(read_pos) + "\">  <input type=\"submit\" value=\"Submit\"><br>";

String myHTML= _title+ sub_title+form ;
  
 server.send(200, "text/html", myHTML); //Send web page
}



void handleDevelRoot() {
//  const char INDEX[] = R"***(
//<!DOCTYPE html>
//<html>
//<head>
//<title>ESP8266 Servo | Circuits4you.com</title>
//<meta name="viewport" content="width=device-width, initial-scale=1">
//</head>
//<style>
// 
//<body>
//
//<p> I am a paragraph</p>
//</body>
//</html>
//)***";
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
  //server.on("/setting", HTTP_GET, handleSetting);
  server.on("/setPOS",handleServo); //Sets servo position from Web request
  server.begin();  
  Serial.println("*** HTTP server started.");

}

//================================================
//     LOOP
//================================================
void loop() {
   server.handleClient();
  
   long randNumber = random(10, 30);
   myservo.write(randNumber);
   delay(2000);

 
}
//================================================
