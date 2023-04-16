 bool SerialDebug = 0; //self explanairy?

//#include <Arduino.h>
#include "buffer.h"
#include "pinout.h"    // also variables and most declarations
#include "PWM_stuff.h"
#include "SensorUpdates.h"
#include "securety.h"
#include "control_all.h"
#include "Serial_interaction.h"


void setup(){
    analogReference(EXTERNAL);
                         //for debugging put out error via serial
    Serial.begin(19200);                         
    //Serial.setTimeout(100);
    pinMode(V_ref_flame_pin,OUTPUT);
    pinMode(avcc_on,OUTPUT);
    pinMode(fan0_pin,OUTPUT);
    pinMode(pump_pin,OUTPUT);
    pinMode(fuel_pin,OUTPUT);
    pinMode(glow_pin,OUTPUT);

    digitalWrite(V_ref_flame_pin, LOW);
    digitalWrite(fan0_pin, LOW);                // beim start  
    digitalWrite(pump_pin, LOW);                //        ist 
    digitalWrite(fuel_pin, LOW);                //          besser alles 
    digitalWrite(glow_pin, LOW);   
    digitalWrite(avcc_on, HIGH);            //                       aus

    

//---------------------------------------------- Set PWM frequency for  D5 & D6 --------------------------------
//                                                                      fuel
TCCR0B = TCCR0B & B11111000 | B00000011;    // set timer 0 divisor to    64 for PWM frequency of   976.56 Hz (The DEFAULT)
//TCCR0B = TCCR0B & B11111000 | B00000100;    // set timer 0 divisor to   256 for PWM frequency of   244.14 Hz
//TCCR0B = TCCR0B & B11111000 | B00000101;    // set timer 0 divisor to  1024 for PWM frequency of    61.04 Hz

//---------------------------------------------- Set PWM frequency for  D9 & D10 -----------------------------
//                                                                      Fan und Pump?
//TCCR1B = TCCR1B & B11111000 | B00000001;    // set timer 1 divisor to     1 for PWM frequency of 31372.55 Hz
TCCR1B = TCCR1B & B11111000 | B00000010;    // set timer 1 divisor to     8 for PWM frequency of  3921.16 Hz
//TCCR1B = TCCR1B & B11111000 | B00000011;    // set timer 1 divisor to    64 for PWM frequency of   490.20 Hz (The DEFAULT)

//---------------------------------------------- Set PWM frequency for D3 & D11 ------------------------------
//                                                                    Glow 
//TCCR2B = TCCR2B & B11111000 | B00000100;    // set timer 2 divisor to    64 for PWM frequency of   490.20 Hz (The DEFAULT)
TCCR2B = TCCR2B & B11111000 | B00000101;    // set timer 2 divisor to   128 for PWM frequency of   245.10 Hz
//TCCR2B = TCCR2B & B11111000 | B00000110;    // set timer 2 divisor to   256 for PWM frequency of   122.55 Hz
//TCCR2B = TCCR2B & B11111000 | B00000111;    // set timer 2 divisor to  1024 for PWM frequency of    30.64 Hz



// SecuretyPartCheck();                    // alle teile einmal kurz einschalten messen und auschalten(could detect broken wires)
if(SerialDebug){Serial.print("fehlercode:"); Serial.println(fehlercode);}
  analogWrite(pump_pin, 200);
  delay(2000);
  analogWrite(pump_pin, 0);


    }

void loop(){
   //unsigned long now = millis();
    //pwm_test();                         //hardware test
    //fake_Temp();                        // simulation und test
    update_Temp();                   // temperaturen aktualisieren
    SecuretyTempCheck();            // temeraturen okay?
    glow_plug();                    // schalten den GP ab wenn zeit rum
    PWM_Map();                      // percentage for convinience
    fuel_pump();                    // sets up the pulsing in Hz
    water_pump();                   // wasser zirkulation sicherstellen
    control();                      // controll all combustion based on mode

    Serial_displ(); 
    Serial_recive_fromdispl();
      firstrun = 0;
      
    //unsigned long than = millis();
    //Serial.println(than-now);

}