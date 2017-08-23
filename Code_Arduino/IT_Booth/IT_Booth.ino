///////////////////
//Servo Pin set
//黃 <--->(signal)
//紅<--->(voltage)
//棕 <--->(ground)
/////////////////////
#include <Wire.h>
#include "DHT.h"
#include "FastLED.h"
#include <Servo.h>
#include <ArduinoJson.h>
#include <LiquidCrystal_I2C.h>


//溫度sensor設定
//#define DHTPIN 2
//#define DHTTYPE DHT11
//DHT dht(DHTPIN, DHTTYPE);

//LCD
#define ROW_NUM 2                             
#define COL_NUM 16                            
LiquidCrystal_I2C lcd(0x27,COL_NUM,ROW_NUM);  


#define DATA_PIN 2
#define MAX_BRIGHTNESS 250
#define BRIGHT_INTERVAL MAX_BRIGHTNESS/NUM_METEORS
#define NUM_LEDS 8
#define NUM_METEORS 3
#define MAX_VOL 160

#define RELAY_PIN 4 

Servo myservo;

CRGB leds[NUM_LEDS];
int meteors[NUM_METEORS][3];

void MeteorLight();
void BlinkLight();
String readLine();
void CloseDoor();
void OpenDoor();

const size_t bufferSize = JSON_OBJECT_SIZE(5) + 40;
DynamicJsonBuffer jsonBuffer(JSON_OBJECT_SIZE(8));
//StaticJsonBuffer<200> jsonBuffer;
//JsonObject& Jsonroot = readBuffer.createObject();

int leds_start,leds_end,start,num;
int bright = 50;

String Name = "";
bool Lock = false;
int val_R = 0;
int val_G = 0;
int val_B = 0;

void setup() {
    Serial1.begin(9600);
    Serial1.setTimeout(50);
    Serial.begin(9600);
    FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
    //dht.begin();
    lcd.begin();                      // initialize the lcd 
    lcd.backlight();
    lcd.clear(); 
    lcd.setCursor(0, 0);
    lcd.print("Who is coming?");
    myservo.attach(9); 
    myservo.write(10);
    digitalWrite(RELAY_PIN,LOW);
    //設定Metrolight的顏色
    bright = 250;
    for(int i = 0; i < NUM_METEORS  ; i++ ){
        meteors[i][0] = 0;
        meteors[i][1] = 200;
        meteors[i][2] = bright;
        bright = bright + BRIGHT_INTERVAL;
    }

    //設定初始顏色
    for(int i = 0; i <= NUM_LEDS  ; i++ ){
        leds[i].setHSV(0,0,0);
        //leds[i].setRGB(0,255,255);
    }
    FastLED.show();

    Serial.println("start!");
    delay(1000);
}

String msgJSON;
char C_msgJSON[] = "";
bool DoorStatus = false; //false = close , true = open
void loop() { 
    while(Serial1.available()){
    
        msgJSON = Serial1.readString();
//        msgJSON.toCharArray(C_msgJSON,msgJSON.length()+1);
//        delay(1000);
//        Serial.print("C:");Serial.println(C_msgJSON);
        Serial.print("S:");Serial.println(msgJSON);
        JsonObject& root = jsonBuffer.parseObject(msgJSON);
        if (!root.success()) {
            Serial.println("parseObject() failed");
            return;
        }

        String man = root["Name"];
        Name = man;
        Lock = root["Lock"];
        val_R = root["R"];
        val_G = root["G"];
        val_B = root["B"];

        Serial.print("Name:");Serial.println(Name);
        Serial.print("Lock:");Serial.println(Lock);
        Serial.print("val_R:");Serial.println(val_R);
        Serial.print("val_G:");Serial.println(val_G);
        Serial.print("val_B:");Serial.println(val_B);
        
        lcd.setCursor(0, 1);
        lcd.print("        ");
        lcd.setCursor(0, 1);
        lcd.print(Name);
        
        if(DoorStatus != Lock){
            if(Lock == 1){
                OpenDoor();
                lcd.setCursor(11, 1);
                lcd.print("      ");
                lcd.setCursor(11, 1);
                lcd.print("OPEN");  
                DoorStatus = Lock;   
            }
            else{
                CloseDoor(); 
                lcd.setCursor(11, 1);
                lcd.print("      ");
                lcd.setCursor(11, 1);
                lcd.print("CLOSE"); 
                DoorStatus = Lock;      
            }    
        }

// = 0; i < NUM_METEORS  ; i++ ){
//            meteors[i][0] = val_R;
//            met
//        for(int ieors[i][1] = val_G;
//            meteors[i][2] = val_B;
//        }
//        for(int i = 0; i <= NUM_LEDS  ; i++ ){
//        //leds[i].setHSV(0,0,0);
//        leds[i].setRGB(val_R,val_G,val_B);
//        }
//        man = "";  
    }
    //Serial.println("...");
    //delay(10);
    //BlinkLight();
    jsonBuffer.clear();
    msgJSON = ""; 
    Name = "";
    
    //MeteorLight();

}
//    lock = true;
//    ControlDoor(lock);
//    delay(5000); 
//    lock = false;
//    ControlDoor(lock);
//    delay(5000); 

    //BlinkLight();
    //MeteorLight();  

//    if(Serial1.available()){
//        //Serial.print("read:");
//        cmdJson = Serial.readString();  
//        Serial.println(cmdJson);
//        cmdJson = "";
//        }
//    String str = '';
//    while (Serial.available()) {
//        char cmd = Serial.read();
//        if(c!='\n'){
//            str += cmd;
//        }
//        delay(5);    // 沒有延遲的話 UART 串口速度會跟不上Arduino的速度，會導致資料不完整
//    }

    
    
   
//    float Humidity = dht.readHumidity();
//    float Temperature = dht.readTemperature();
//
//    if (isnan(Humidity) || isnan(Temperature)) {
//        Serial.println("Failed to read from DHT sensor!");
//        return;
//    }
//
//    float HeatIndex = dht.computeHeatIndex(Temperature, Humidity, false);
//
//    Jsonroot["Humidity"] = Humidity;
//    Jsonroot["Temperature"] = Temperature;
//    Jsonroot["HeatIndex"] = HeatIndex;
//
//    Jsonroot.printTo(Serial);
//    Serial.println();
//    Serial1.flush();
//    delay(2000);


//String readLine(){
//    String s = "";
//    char c;
//    while(Serial1.available()){
//        c = Serial1.read();
//        s += c;
//        delay(50);
//        }
//    return s;
//}
//

void MeteorLight(){
    for(int i=0 ; i < NUM_LEDS+NUM_METEORS ; i++){
    for(int j = 0; j <= NUM_LEDS  ; j++ ){
    leds[j].setHSV(0,0,0);
    }
    
    if(i < NUM_METEORS){
        leds_start = 0;
        start = NUM_METEORS - i;
        num = i;
    }
    else if(i > NUM_LEDS){
        leds_start = i-NUM_METEORS;
        start = 0;
        num = (NUM_LEDS+NUM_METEORS) - i;
    }
    else{
        leds_start = i-NUM_METEORS;
        start = 0;
        num = NUM_METEORS;
      }

    for(int j = 0 ; j < num ; j++){
        leds[leds_start].setHSV(meteors[start][0],meteors[start][1],meteors[start][2]); 
        leds_start ++;
        start ++ ;
      }
      
    FastLED.show();
    delay(50);
    }
}


void BlinkLight(){
  for(int i=0; i<=MAX_BRIGHTNESS;i=i+5){
    for(int j=0; j<NUM_LEDS ; j++){
      leds[j].setHSV(0,100,i);   
      }
    FastLED.show();
    delay(10);
    }
  for(int i=MAX_BRIGHTNESS ; i>=0 ; i=i-5){
    for(int j=0; j<NUM_LEDS ; j++){
      leds[j].setHSV(0,100,i);   
      }
    FastLED.show();
    delay(10);
    }
}
void OpenDoor(){
    for(int i = 00; i <= 90; i+=2){
        myservo.write(i); // 使用write，傳入角度，從0度轉到180度
        delay(5);    
    }
}
void CloseDoor(){
    for(int i = 90; i >= 0; i-=2){
        myservo.write(i);// 使用write，傳入角度，從180度轉到0度
        delay(5);   
    }
}
