// will be a kinda adv_Config file
#include <Arduino.h>

     #define wasser0_pin      A1         //Backup WasserTemp sensor
     #define wasser1_pin      A2         //achtung pinout falsch normal wasserTemp sensor
     #define flame0_pin       A3         //Flammwächter PT1000 vmtl
     #define car_mesure_pin   A5         //Bord Spannung wenn kleiner car_Voltage_min alles aus auser lüfter                                                                                     //                
    
     
     #define V_ref_flame_pin 18          //schaltet pin an nur während des messens
     #define avcc_on 14                 // schaltet analog ref an (testen)      
     #define pump_pin 9                 //output pin wasserpumpe                PWM TIM2                             
     #define fuel_pin 5                 //output pin fuel pump                  PWM TIM0!!       
     #define glow_pin 3                //output pin GlowPlug how many amps? 
     #define fan0_pin 10                //output pin brennerfan PWM  notwendig  PWM TIM2          11i s mosi?!
          // Pin # 3, 5, 6, 9, 11 ar PW
          //https://i.redd.it/bggdd9srui351.png good pinout



// Globale Variablen, die alle brauchen:

int   modus = 0; // 0 aus;1 bootup; 2 5kw Heizen; 3 kaltblasen; 4 zirkulieren;  5 schlafen;
float TempWasser0, TempWasser1, TempFlame0;     // actual read values
int cleanTempWasserMax= 0, cleanTempWasser0 = 0, cleanTempWasser1 = 0, cleanTempFlame0 = 0 , ignit_fail;    //bereinigte messwerte
unsigned int water_pump_speed, Fan_speed, fuel_pump_speed; // water 0-100, Fan 0-100, fuel pump in 100 = 1Hz
unsigned int Board_Voltage, cleanBoard_Voltage =0;
bool  major_fail, Flame, glowing, dry ,new_temp, sleep_mode = false, firstrun =1;


 // Settings
 //volatile unsigned int AirExtra = 100, FuelExtra = 140;                  // more is less air, but more fuel TODO: Alte variante?
 volatile unsigned int AirExtra = 100, FuelExtra = 80;                  // percentage of how nuch air / fuel 
 int pulse_lenght = 11;                               // time the fuelpump stays on during pulse
 const unsigned long usr_securety_delay = 6000UL;    // securety cyclus startet erst später
 int clean_securety = 20;                           // durch welchen schnitt sollen messwerte geglättet werden
 const unsigned long temp_update_intervall = 25UL;  // alle 25ms messen und jedes 4.mal flam ann (also alle 100ms)
 volatile unsigned long slowdown = 100UL;          // um die boot kurve zu strecken uder zu stauchen 100 = 1sek/sekunde norm
                                                  // Uptate zeit = clean_securety * temp_update_intervall



 //werte für Wasserfühler
 float T0Wasser=0;           // Nenntemperatur des NTC-Widerstands in °C     =default in function
 float R0Wasser=32540;      // Nennwiderstand des NTC-Sensors in Ohm                                0° 32540kohm 50° 3600kOhm 100° 670ohm 120° 380ohm bei D5WS neuer al meine
 float T1Wasser=100;         // erhöhte Temperatur des NTC-Widerstands in °C  =default in function 80 normal funktioniert also nicht
 float R1Wasser=670;       // Widerstand des NTC-Sensors bei erhöhter Temperatur in Ohm            
 float RVWasser=998;       // Vorwiderstand in Ohm  =default in function
 float RVFlame=997;        //2k R als Spannungsteiler Flammsensor                                  
 float RVoltagediv1 = 300;
 float RVoltagediv2 = 820;                            //laut werkstatthandbuch: -50grad 800ohm; 0grad 1000ohm; 500grad 2800ohm
