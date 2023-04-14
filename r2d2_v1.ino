/// Librairies
#include "SevenSegmentTM1637.h" // https://github.com/bremme/arduino-tm1637
#include "SevenSegmentExtended.h" // idem au dessus
#include "SevenSegmentFun.h" // idem au dessus
#include "RTClib.h" /https://github.com/adafruit/RTClib
#include "SoftwareSerial.h" // https://github.com/PaulStoffregen/SoftwareSerial
#include "DFRobotDFPlayerMini.h" //https://wiki.dfrobot.com/DFPlayer_Mini_SKU_DFR0299#target_6 NOTE: The order you copy the mp3 into micro SD card will affect the order mp3 played , which means play(1) function will play the first mp3 copied into micro SD card. 


//// Module MP3
static const uint8_t PIN_MP3_TX = 6; // Connects to module's RX 
static const uint8_t PIN_MP3_RX = 7; // Connects to module's TX 
SoftwareSerial softwareSerial(PIN_MP3_RX, PIN_MP3_TX);
DFRobotDFPlayerMini player;

     
//// Afficheur TM1637
const byte PIN_CLK_Screen = A1;   // CLK pin 
const byte PIN_DIO_Screen = 8; // DIO pin
//SevenSegmentExtended TM1637(PIN_CLK_Screen, PIN_DIO_Screen);
SevenSegmentFun TM1637(PIN_CLK_Screen, PIN_DIO_Screen);
RTC_DS3231 rtc;
int backlight = 30; // Luminosité TM1637

///// bouttons 
int buttonPinHour=2; // Heure +
int buttonPinMin=3; // Min +

//// variables année mois date heure 
int yearupg;
int monthupg;
int dayupg;
int hourupg;
int minupg;
int secupg;
long lastShowTime = 0;  
int showTime=1000 ;// Delay affichage Heure -> millis

void setup() {
  pinMode(PIN_CLK_Screen, OUTPUT);
  pinMode(PIN_DIO_Screen, OUTPUT); 
  
// buttons 
  pinMode(buttonPinHour,INPUT);
  digitalWrite(buttonPinHour,HIGH);
  pinMode(buttonPinMin,INPUT);
  digitalWrite(buttonPinMin,HIGH);

// initialisation du port série @9600 bauds      
  Serial.begin(9600);
  softwareSerial.begin(9600);          

  // initialisation du TM1637 Boot
  TM1637.begin();               
  TM1637.setBacklight(backlight);
  TM1637.clear();  
  byte repeats = 2;
  TM1637.scrollingText("r2d2 r2d2", repeats);
  player.begin(softwareSerial);
  player.volume(14);
  TM1637.print("r2d2");
  player.play(0006);  
  delay(5000); 
  
// initialisation du module RTC      
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");   
  } 

   //Memorisation en dur de la date et l'heure : 18:03:30 02/04/2023   
   //rtc.adjust(DateTime(2023, 4, 14, 10, 34, 30)); 
   
}


void loop() {
  
//// variables upgrade     
    DateTime now = rtc.now();
    yearupg=now.year();
    monthupg=now.month();
    dayupg=now.day();
    hourupg=now.hour();
    minupg=now.minute();
    secupg=now.second();

//// boucle millis 1 seconde affichage de l'écran  
    if(abs(millis() - lastShowTime) > showTime){
        TM1637.printTime(hourupg, minupg, true);
        lastShowTime = millis();   
    }

////  fonctions 
    hourupdate();
    minupdate();
    annoncevocale();
    //debug();  
    //mp3_debug();
    
   
}

//// fonctions 
void hourupdate() { // bouton heure    
    if(digitalRead(buttonPinHour)==0) {
          hourupg++;
          if(hourupg>23)hourupg=0;
          rtc.adjust(DateTime(yearupg, monthupg, dayupg, hourupg, minupg, secupg));
          TM1637.printTime(hourupg, minupg, true);
    }               
}

void minupdate() {   // bouton minute    
     if(digitalRead(buttonPinMin)==0) {
         minupg++;
         if(minupg>59)minupg=0;
         rtc.adjust(DateTime(yearupg, monthupg, dayupg, hourupg, minupg, secupg));
         TM1637.printTime(hourupg, minupg, true);
    }
}

void annoncevocale() { // fonction annonce heure conditions 
   if(hourupg==07 && minupg==00 && secupg==00){
      player.play(0007);  // joue 07h00
      blink();     
   }
   if(hourupg==07 && minupg==30 && secupg==00){
      player.play(0010);  // joue 07h30
      blink();       
   } 
   if(hourupg==8 && minupg==00 && secupg==00){
      player.play(0011);  // joue 08h00
      blink();
   }
   if(hourupg==8 && minupg==30 && secupg==00){
      player.play(0012);  // joue 08h00
      blink(); 
   }
   if(hourupg==9 && minupg==00 && secupg==00){
      player.play(0013);  // joue 08h00
      blink(); 
   }  
   if(hourupg==9 && minupg==30 && secupg==00){
      player.play(0014);  // joue 08h00
      blink(); 
   }
   if(hourupg==10 && minupg==00 && secupg==00){ 
      player.play(0004);  // joue 10h00
      blink(); 
   } 
   if(hourupg==10 && minupg==30 && secupg==00){ 
      player.play(005);  // joue 10h30
      blink(); 
   } 
   if(hourupg==11 && minupg==00 && secupg==00){ 
      player.play(0015);  // joue 11h00
      blink(); 
   }
   if(hourupg==11 && minupg==30 && secupg==00){ 
      player.play(0016);  // joue 11h30
      blink(); 
   }
   if(hourupg==12 && minupg==00 && secupg==00){ 
      player.play(0017);  // joue 12h00
      blink(); 
   }
   if(hourupg==12 && minupg==30 && secupg==00){
      player.play(0020);  // joue 12h30
      blink(); 
   }
   if(hourupg==13 && minupg==0 && secupg==00){
      player.play(0021);  // joue 13h00
      blink(); 
   }
   if(hourupg==13 && minupg==30 && secupg==00){
      player.play(0022);  // joue 13h30
      blink(); 
   }
   if(hourupg==14 && minupg==00 && secupg==00){
      player.play(0023);  // joue 14h00
      blink(); 
   }
   if(hourupg==14 && minupg==30 && secupg==00){
      player.play(0024);  // joue 11h00
      blink(); 
   }
   if(hourupg==15 && minupg==00 && secupg==00){
      player.play(0025);  // joue 11h00
   }
   if(hourupg==15 && minupg==30 && secupg==00){
      player.play(0026);  // joue 11h00
      blink(); 
   }
   if(hourupg==16 && minupg==00 && secupg==00){
      player.play(0027);  // joue 11h00
      blink(); 
   }
   if(hourupg==16 && minupg==30 && secupg==00){
      player.play(0001);  // joue 11h00
   }
   if(hourupg==17 && minupg==00 && secupg==00){
      player.play(000017);  // joue 11h00
      blink(); 
   }
   if(hourupg==17 && minupg==30 && secupg==00){  
      player.play(0025);  // joue 11h00
      blink(); 
   }

}

void debug() { // fonctiuon debug RTC / Upgrade
    DateTime now = rtc.now();
    hourupg=now.hour();
    minupg=now.minute();
    Serial.print("Heure rtc: "); Serial.println(now.hour(), DEC);
    Serial.print("Heure upgrade: "); Serial.println(hourupg);
    Serial.print("Minute rtc: "); Serial.println(now.minute(), DEC);
    Serial.print("Minute upgrade: "); Serial.println(minupg);
    delay(1000); 
}

void mp3_debug() { // fonction debug MP3 l'ordre de copie des fichiers affecte l'ordre des fichiers joués,
                   // cela signifie que le fonction play(1) jouera le premier fichier  mp3 copié sur la micro SD card. 
    player.play(0007);  
    Serial.println(" mp3 7 07h00");
    delay(10000);
    player.play(0010);  
     Serial.println(" mp3 10 07h30");
    delay(10000);
    player.play(00011); 
     Serial.println(" mp3 11 08h00");
    delay(10000);
    player.play(0012);  
     Serial.println(" mp3 12 08h30");
    delay(10000);
    player.play(0013); 
     Serial.println(" mp3 13 09h00");
    delay(10000);
     player.play(0014);  
     Serial.println("mp3 14 09h30");
    delay(10000);
    player.play(0004);  
     Serial.println("mp3 4 10h00");
    delay(10000);
     player.play(0005);  
     Serial.println("mp3 5 10h30");
    delay(10000);
    player.play(0015);  
     Serial.println("mp3 15 11h00");
   delay(10000);   
    player.play(0016);  
     Serial.println("mp3 16 11h30");
    delay(10000);   
    player.play(0017); 
     Serial.println("mp3 17 12h00");
    delay(10000);  
    player.play(0020);  
     Serial.println("mp3 20 12h30");
    delay(10000);
    player.play(0021);  
     Serial.println("mp3 21 13h00");
   delay(10000);  
    player.play(0022);  
     Serial.println("mp3 22 13h30");
   delay(10000);
    player.play(0023);  
     Serial.println("mp3 23 14h00");
   delay(10000);
    player.play(0024);  
     Serial.println("mp3 24 14h30");
   delay(10000);
     player.play(0025);  
     Serial.println("mp3 25 15h00");
   delay(10000);
    player.play(0026);  
     Serial.println("mp3 26 15h30");
   delay(10000);
    player.play(0027);  
     Serial.println("mp3 27 16h00");
  delay(10000);
    player.play(0200);  
    Serial.println("mp3 200 16h30");
   delay(10000);  
    player.play(0002);  
    Serial.println("mp3 2 17h00");
    delay(10000);  
     player.play(00003);  
    Serial.println("mp3 3 17h30");
    delay(10000);  
 
}  

void blink() { // fonction clignotement TM1637
  TM1637.blink();
  TM1637.setBacklight(backlight);    
}
