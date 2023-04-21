/*  Simple utilisation d'un encodeur pour varier la valeur PWM 
 *   appliquée à une Led.
 *  Dans ce Sketch, je vais utiliser "PinChangeInterrupt" avec Pin2
 *  au lieu de "External_Interrupt"
 */
#include "PinChangeInterrupt.h"
const int S1 = 2;   // signal S1 sur pin2 (ou une autre pin) 
const int S2 = 3;   // signal S2 sur pin3 (ou une autre pin) 
const int key = 9;  // bouton poussoir de l'encodeur 

int lastPos = 0;               // ancienne valeur de l'encodeur 
volatile int actualPos = 0;    // On commence la Led par une petite valeur
volatile bool raz_flag = 0;    // Drapeau pour l'action sur le bouton
int menu = 0;
int hourupg = 12;
int minupg = 30;

void setup() {
  Serial.begin(115200);
  pinMode(S1, INPUT);         // signal S1 sur pin2
  pinMode(S2, INPUT);         // signal S2 sur pin3 
  pinMode(key, INPUT);        // bouton de RAZ sur pin8

//************ Setting 2 Pin_Change_Interrupts ***************** 
  attachPCINT(digitalPinToPCINT(S1), isr, CHANGE);    // PCInt(pour S1 sur pin2)
  attachPCINT(digitalPinToPCINT(key), isr2, CHANGE);  // PCInt(pour le bouton sur pin8)   
  Serial.println("Starting...");      // On commence avec...
  Serial.print("with: ");             // ...avec...
  Serial.println(actualPos);          // ...la valeur 5 pour la sortie_PWM  
}

void loop() {       //  mettre à jour seulement si l'encodeur a bougé.
    
   /* if (actualPos != lastPos) {       // l'encodeur a bougé? 
      if (actualPos > lastPos)        // Sens HORAIRE
        Serial.print(actualPos);        // "++" pour signaler l'augmentation
        else                            // Sens ANTI-HORAIRE
        Serial.print(actualPos);        // "--" pour signaler la diminution
       
        }
     //Serial.println(actualPos);   
  lastPos = actualPos;              // mémorise la valeur pour la prochaine fois
*/

 }         


// ******************** Pin Change Interrupt for S1 **************************
void isr(void) {
  static unsigned long lastInterruptTime = 0;     // variable1 
  unsigned long interruptTime = millis();         // variable2
   if (menu==1)
   { 
    if (interruptTime - lastInterruptTime > 5) {    //plus de 5ms, n'est pas un bruit  
    bool etatS1 = digitalRead(S1);    // lecture des 2 signaux A et B.
    bool etatS2 = digitalRead(S2);
    if (etatS1) {                   // Etat HAUT (from LOWtoHIGH) 
      if((etatS1) = (etatS2))       // 
        hourupg ++ ;            // Sens HORAIRE        
        else                          // 
        hourupg -- ;            // Sens ANTI-HORAIRE      
    }
   //Serial.println(actualPos); 
    Serial.print("Heure = "); Serial.println(hourupg);
  }
  
  lastInterruptTime = interruptTime;
 } 
   if (menu==2)
   { 
    if (interruptTime - lastInterruptTime > 5) {    //plus de 5ms, n'est pas un bruit  
    bool etatS1 = digitalRead(S1);    // lecture des 2 signaux A et B.
    bool etatS2 = digitalRead(S2);
    if (etatS1) {                   // Etat HAUT (from LOWtoHIGH) 
      if((etatS1) = (etatS2))       // 
         minupg ++ ;            // Sens HORAIRE        
        else                          // 
        minupg -- ;            // Sens ANTI-HORAIRE      
    }
   //Serial.println(actualPos); 
    Serial.print("Minutes = "); Serial.println(minupg);
  }
  
  lastInterruptTime = interruptTime;
 }  
} 

// **************** Pin Change Interrupt pour Bouton de RAZ *****************
void isr2(void) {
  if ((!digitalRead(key))) {    // Button pressed ?   
    actualPos = 0;             // reset valeur    
    while (!digitalRead(key))     // attend relachement du bouton(debounce)
      delay(10);
      menu = menu +1;
      Serial.print("menu = ");  
      Serial.println(menu);
     
    if (menu==1)
      {
        hourupg = hourupg + actualPos;
      Serial.print("réglage Heure = + 1 / - 1 "); Serial.println(hourupg);
      }
      if (menu==2)
      {
     Serial.print("réglage Minute = + 1 / - 1 "); Serial.println(minupg);
      } 
      if (menu==3)
      {
      Serial.print("Affiche l'heure = "); Serial.print(hourupg);Serial.print(":");Serial.println(minupg);  
      menu = 0;
      } 
    }  
  }
