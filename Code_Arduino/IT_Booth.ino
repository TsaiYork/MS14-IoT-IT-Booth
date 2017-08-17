#include "DHT.h"
#include "FastLED.h"
#include <ArduinoJson.h>

//溫度sensor設定
//#define DHTPIN 2
//#define DHTTYPE DHT11
//DHT dht(DHTPIN, DHTTYPE);

#define DATA_PIN 3
#define MAX_BRIGHTNESS 250
#define BRIGHT_INTERVAL MAX_BRIGHTNESS/NUM_METEORS
#define NUM_LEDS 16
#define NUM_METEORS 3
#define MAX_VOL 160

CRGB leds[NUM_LEDS];
int meteors[NUM_METEORS][3];

void MeteorLight();
void BlinkLight();
String readLine();


StaticJsonBuffer<200> jsonBuffer;
DynamicJsonBuffer readBuffer(JSON_OBJECT_SIZE(8));
JsonObject& Jsonroot = jsonBuffer.createObject();


int leds_start,leds_end,start,num;
int bright = 50;

void setup() {
    Serial1.begin(9600);
    Serial1.setTimeout(50);
    Serial.begin(9600);
    FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
    //dht.begin();

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
void loop() {   
    //BlinkLight();
    //MeteorLight();   
    
//    while(Serial1.available()){
//        msgJSON = Serial1.readString();
//
//        Serial.println(msgJSON);
//        JsonObject& root = readBuffer.parseObject(msgJSON);
//        if (!root.success()) {
//            Serial.println("parseObject() failed");
//            return;
//        }
//
//        String deviceID = root["deviceId"];
//        int val_R = root["temperature"];
//        int val_G = root["humidity"];
//
//        Serial.print("deviceID:");Serial.println(deviceID);
//        Serial.print("val_R:");Serial.println(val_R);
//        Serial.print("val_G:");Serial.println(val_G);
//    }
}
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
