// main control cycle
#include <Arduino.h>
#include "burn_boot.h"
unsigned long ctimer;
unsigned long ctimerold;
int seconds= 0;
unsigned long cooltimer;
unsigned long cooltimerold;
int coolseconds =0, starttemp;

void control(){
          // jetzt kommt der switschige schwitsch

    switch (modus){
    
    case 0: {
        ignit_fail = 0;
        seconds = 0;
        fuel_pump_speed = 0;
        glow_time =0;
        Flame = false;
        if(cleanTempFlame0>90){Fan_speed = 60;} // läuft immer wenn zu warm
            else {Fan_speed = 0;}
            } break;
    
    
    case 1: { ///////// grub2
        if (major_fail){modus = 3;}
        if (millis() > ctimer){ctimer = millis();}
        if (ctimer - ctimerold >= 10UL*slowdown){ctimerold = ctimer; seconds ++;} // increment the seconds counter for convinience
        if (seconds < 0 ){fuel_pump_speed = 0;
                            dry = true;
                            glow_time = 30;
                            Fan_speed = 80;}               
        
        if (seconds == 0 ){                    // fahre kurve fuel und air nach burn_boot tabelle
            fuel_pump_speed=200;              //prinming
            Fan_speed=20*(AirExtra/100);        // 20% is about minimum fan speed
            dry = false;}
        if (seconds == 1){glow_time=91;}        //Kerze an
        if (seconds >= 3 && seconds <= 100){    fuel_pump_speed = (seconds*FuelExtra)/143;}  //sprit steigt jetzt von 0,2Hz auf ~7Hz über 100 sek   
        if (seconds == 15) {                    
                if (cleanTempFlame0 > 50)       {starttemp = cleanTempFlame0 - 30;}                 // ab jetzt wird temp steigung gemessen, bei warm start vorsprung...
                else { starttemp = cleanTempFlame0;} }
        if (seconds >= 25 && seconds <= 100){   Fan_speed = (seconds*AirExtra)/(125);}            // (seconds/1.25)*(AirExtra/100)  umgestellt u,m float zu vermeiden 125 = 1,25*100
        if (seconds == 90 && (cleanTempFlame0 - starttemp) > 100)
                {glow_time = 0;}  // wenns gut angegangen ist, kerze aus
        if (seconds > 100){                     fuel_pump_speed = (100*FuelExtra)/143;
                                                Fan_speed = (100*AirExtra)/125;}          // ab 100 sec nicht weiter stiegen
        if (seconds > 130 && cleanTempFlame0 - starttemp > 120){Flame = true;}  
        if (seconds > 130 && cleanTempFlame0 - starttemp <= 120){
                ignit_fail ++;                              // wenn nich fehlzündung und 
                fuel_pump_speed = 0;
                glow_time = 0;
                seconds = -60;}                                             // wieder von neuem mit 60sec pause
            
        if (Flame && !sleep_mode){seconds = 0; ignit_fail = 0; modus = 2;}                  //wenn kein schlaf dann brenn / regeln
        if (Flame && sleep_mode) {seconds = 0; ignit_fail = 0; modus = 5;}                  // wenn schlaf dann brenn leise
        if (seconds >132){major_fail = 1; modus = 3;}                       // wenn 122sec dann irgendwas schief gegangen
            
            } break;
            

    case 2: { ///////// Heizen
        if (major_fail){modus = 3;}
        ignit_fail = 0;                
        if (cleanTempWasserMax < 50){                      //5kw unter 50
            fuel_pump_speed = (100*FuelExtra) /143;
            Fan_speed       = (100*AirExtra)  /125;
            }
        if (cleanTempWasserMax >= 60 && cleanTempWasserMax < 85){
            fuel_pump_speed = (60*FuelExtra) /143;        //2.5hz 
            Fan_speed       = (70*AirExtra)  /125;        //60%
            
            }
        if (cleanTempWasserMax >= 70){
            fuel_pump_speed = 0;
            Fan_speed = 60;            
             modus = 4;}  
            }break;
    case 3: { ///////// 100sec aus Blasen bei flammabriss etc FAN77 heiß Fan55 kalt
        if (millis() > cooltimer) {cooltimer = millis();}
        if (cooltimer - cooltimerold >= 1000UL) {cooltimerold = cooltimer; coolseconds ++;}
        fuel_pump_speed = 0;
            if (TempFlame0 > 100){
            dry =1;
            glow_time = 10;
            Fan_speed = 77;}
                else {
                dry =0;
                glow_time = 0;
                Fan_speed = 40;            
                }
        if (coolseconds > 100){ dry = 0; coolseconds = 0; major_fail = 0; modus = 0;}
        } break;

    case 4: { ///////// zirkulieren bis unter 60 (am ende des Brands)

            fuel_pump_speed = 0;
            glow_time = 0;
        if (TempFlame0 > 80){Fan_speed = 30;}
            else {Fan_speed=0;}
        if (sleep_mode  && cleanTempWasserMax < 30){modus = 1;} //im schlafmodus warten bis wasser sehr kalt dann neustart
        if (!sleep_mode && cleanTempWasserMax < 65){modus = 0;} // wenn temp unter 60 aus

        } break;
    
    case 5: { ///////// Leise Lange Heizen 
        if (major_fail){modus = 3;}
        ignit_fail = 0;                                
        if (cleanTempWasserMax < 50){                      //5kw unter 60
            fuel_pump_speed = (70*FuelExtra) /143;
            Fan_speed       = (80*AirExtra)  /125;
            }
        if (cleanTempWasserMax >= 60 ){                     //XXX: zwischen 50 und 60 kann nicht getuned werden
            fuel_pump_speed = (50*FuelExtra) /143;
            Fan_speed       = (60*AirExtra)  /125;
            }
        if (cleanTempWasserMax > 65) {fuel_pump_speed = 0;  modus = 4;}    //regelpause
            
            }break;
    case 6: { ///////// seriel tuning (nix ändern nur eben nicht aus)                            
            
        if (major_fail){modus = 3;}

            fuel_pump_speed = (100*FuelExtra) /143;
            Fan_speed       = (100*AirExtra)  /125;
            }break;
    }

  }//ende contol funktion()



//wasser pumpe laufen lassen und so
void water_pump(){  


            if (cleanBoard_Voltage < 135 && modus > 0){          // muss nur laufen wenn auto aus ( volt kleiner 13.3V)
                if (cleanTempWasserMax > 60 )                            {water_pump_speed = 100;}
                if (cleanTempWasserMax >=43 &&
                    cleanTempWasserMax <= 60)                            {water_pump_speed = 75;}
                if (cleanTempWasserMax < 40 && !sleep_mode)              {water_pump_speed = 0;  }
                if (cleanTempWasserMax < 35 )                            {water_pump_speed = 0;  }
            } else{water_pump_speed = 0;}            
}          // TODO: elsif ?
//ende waserp
