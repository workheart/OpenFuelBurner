// will be a kinda adv_Config file
#include <Arduino.h>

     #define Vmesure_pin      A0         // to correct voltage VCC
     #define wasser0_pin      A1         //Backup WasserTemp sensor
     #define wasser1_pin      A2         //normal wasserTemp sensor
     #define flame0_pin       A3         //Flammwächter PT1000 vmtl
     #define wasser2_pin      A4         //extern zz ungenutz
     #define car_mesure_pin   A5         //Bord Spannung wenn kleiner car_Voltage_min alles aus auser lüfter                                                                                     //                
     #define wasser3_pin      A6         //extern zz ungenutz
     #define current_mesure_pin A7      //Spannung am eingang messen wenn der abgeschaltet wird
     
     #define V_ref_flame_pin 2          //schaltet pin an nur während des messens
     #define pump_pin 3                 //output pin wasserpumpe                PWM TIM2                             
     #define fuel_pin 5                 //output pin fuel pump                  PWM TIM0!!       
     #define relay    7                 //maybe rlay or so                      PWM TIM0!!
     #define extern1  6                 //s.o.     
     #define glow_pin 9                //output pin GlowPlug how many amps? 
     #define fan0_pin 11                //output pin brennerfan PWM  notwendig  PWM TIM2          
          // Pin # 3, 5, 6, 9, 11 ar PW
          //https://i.redd.it/bggdd9srui351.png good pinout
// Variablen:

int   modus = 0; // 0 aus;1 bootup; 2 5kw Heizen; 3 kaltblasen; 4 zirkulieren;  5 schlafen;
float TempWasser0, TempWasser1, TempFlame0 , current_total, Board_Voltage, Extern_Voltage;     // actual read values
float fuel_pump_speed;                                                         // vitual mesurement
int   water_pump_speed, Fan_speed, ignit_fail, TempWasserMax;
bool  major_fail, Flame, glowing, dry ,sleep_mode = false;


 // Settings
 int AirExtra = 100, FuelExtra = 140;        // more is less air, / fuel
 unsigned long extratime= 1;                                          // extra time due to timer 1 is responible for millis()
 int pulse_lenght = 11*extratime;                           // time the fuelpump stays on during pulse
 const unsigned long user_delay = 1000UL*extratime;           // made a variable for seconds oO
 const unsigned long usr_securety_delay = 4000UL*extratime;   // securety cyclus startet erst später

 //werte für Wasserfühler
 float T0Wasser=0;           // Nenntemperatur des NTC-Widerstands in °C     =default in function
 float R0Wasser=37000;      // Nennwiderstand des NTC-Sensors in Ohm                                TODO: research values ckeck
 float T1Wasser=80;         // erhöhte Temperatur des NTC-Widerstands in °C  =default in function
 float R1Wasser=1000;       // Widerstand des NTC-Sensors bei erhöhter Temperatur in Ohm            TODO: research values check
 float RVWasser=1000;       // Vorwiderstand in Ohm  =default in function
 //werte für PT1000 lesen
 float RVFlame=1000;        //2k R als Spannungsteiler Flammsensor                                  TODO: research values
                            //                                                             laut werkstatthandbuch: -50grad 800ohm; 0grad 1000ohm; 500grad 2800ohm
