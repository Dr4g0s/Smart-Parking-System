
//ESP Node MCU Code

#include <FirebaseESP8266.h>
#include <FirebaseESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <Servo.h>
                    
#define FIREBASE_HOST    "https://smart-parking-1a660.firebaseio.com/"  //firebase host
#define FIREBASE_AUTH    "OZ9Px3naCu8LiwJcjIdh7vb7nOsmYcMQw9OsRbpK"		//firebase Secret
#define ssid             "AYMAN"	//WIFI Name
#define password         "01111000" //WIFI Password
#define SERVO_PIN        D5			//set Pin "D5" for Servo Motor 

int Distance[] = {0,0,0,0,0,0,0,0}; //Initiate array for comming values
int y, z;
String x, data;
bool servo_STATE;

SoftwareSerial toESP(4,5);	
Servo servo;
FirebaseData firebaseData;

void setup(){
  Serial.begin(9600);
  toESP.begin(9600);	//Start SoftwareSerial for getting arduino values
  
  //Connecting to WIFI for getting Internet connection
  Serial.print("Connecting to ");
  Serial.print(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  servo.attach(SERVO_PIN); //Initiate Servo
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH); //Initiate communication to Firebase
  Firebase.reconnectWiFi(true);
}

void loop(){
  if(toESP.available()){				//if Arduino values available in Serial 
    data = toESP.readString();			//Read data as String
    Serial.println(data);				//test sentence
    x = getValue(data, ':', 0);			//get first  string of data before :
    y = getValue(data, ':', 1).toInt(); //get second string of data "Distance" after ":"
    if(y<7){	//if sensor distance smaller than 7
      z=1;  
    }
    else{
      z=0;  
    }
  }
  
  if(Firebase.setInt(firebaseData, x, z)){}	//send sensor value to firebase as "x : z"
  else{
      Serial.println("FAILED");
      Serial.println("REASON: " + firebaseData.errorReason());
      Serial.println("------------------------------------");
      Serial.println();
   }
   delay(1000);
   
   servo_STATE = Firebase.getBool(firebaseData, "servo");//get servo state("comming from android app") from firebase
   if(servo_STATE==1){
      for(int s=0;s<=90;s++){ //open gate
          servo.write(s);
          delay(50);
      }
      Serial.println("Servo Opend!");
      delay(500);
    }
   else{
      servo.write(0);	//close gate
    }
   delay(500);
}

//function to split string 
String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
