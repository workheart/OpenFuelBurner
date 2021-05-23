// main control cycle
#include <Arduino.h>
#include <burn_boot.h>
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
        if(TempFlame0>120){Fan_speed = 30;} // läuft immer wenn zu warm
            else {Fan_speed = 0;}
            } break;
    case 1: { ///////// boot 
        if (major_fail){modus = 3;}
        if (millis() > ctimer){ctimer = millis();}
        if (ctimer - ctimerold >= user_delay){ctimerold = ctimer; seconds ++;} // increment the seconds counter for convinience
        if (seconds < 0 ){fuel_pump_speed = 0;
                            dry = true;
                            glow_time = 30;
                            Fan_speed = 80;}               
        
        if (seconds >= 0 ){                    // fahre kurve fuel und air nach burn_boot tabelle
            fuel_pump_speed=fuel_boot[seconds]/FuelExtra;
            Fan_speed=fan_boot[seconds]/AirExtra;
            dry = false;}
        if (seconds == 1){dry = 0; glow_time=91;}
        if (seconds == 20) {starttemp = cleanTempFlame0;}
        if (seconds == 60 && (cleanTempFlame0 - starttemp) > 70)
                {glow_time = 10;}                                                     // glühkerze aus bei 20° anstieg
        if (seconds == 90 && (cleanTempFlame0 - starttemp) > 120)
                {glow_time = 0;}  
        // sollte jetz mal brennen und in nächsten modus starten
        if (seconds > 120 && cleanTempFlame0 - starttemp > 150){Flame = true; Serial.println("burnt!!!");}  
        if (seconds > 120 && cleanTempFlame0 - starttemp <= 150){
                ignit_fail ++;                              // wenn nich fehlzündung und 
                fuel_pump_speed = 0;
                glow_time = 0;
                seconds = -60;}                                             // wieder von neuem mit 60sec pause
            
        if (Flame && !sleep_mode){seconds = 0; modus = 6;}                  //wenn kein schlaf dann brenn / regeln
        if (Flame && sleep_mode) {seconds = 0; modus = 5;}                  // wenn schlaf dann brenn leise
        if (seconds >122){major_fail = 1; modus = 3;}                       // wenn 122sec dann irgendwas schief gegangen
            } break;


    case 2: { ///////// Heizen
            if (major_fail){modus = 3;}
                        
        if (TempWasserMax < 75){                      //5kw unter 70
            fuel_pump_speed=4;
            Fan_speed=80;
            
            }
        if (TempWasserMax >= 75 && TempWasserMax <85){
            fuel_pump_speed = 2.5;
            Fan_speed = 60;
            
            }
        if (TempWasserMax >= 85){
            fuel_pump_speed = 0;
            Fan_speed = 60;            
             modus = 3;}  
            }break;
    case 3: { ///////// 100sec aus Blasen bei flammabriss etc FAN77 heiß Fan55 kalt
        if (millis() > cooltimer) {cooltimer = millis();}
        if (cooltimer - cooltimerold >= user_delay) {cooltimerold = cooltimer; coolseconds ++;}
        fuel_pump_speed = 0;
            if (TempFlame0 > 150){
            dry =1;
            glow_time = 50;
            Fan_speed = 77;}
                else {
                dry =1;
                glow_time = 30;
                Fan_speed = 55;            
                }
        if (coolseconds > 100){ dry =0; coolseconds = 0;modus = 0;}
        } break;

    case 4: { ///////// zirkulieren bis unter 60 (am ende des Brands)

            fuel_pump_speed = 0;
            glow_time = 0;
        if (TempFlame0 > 70){Fan_speed = 30;}
            else {Fan_speed=0;}
        if (sleep_mode && TempWasserMax < 25){modus = 1;} //im schlafmodus warten bis wasser sehr kalt dann neustart

        } break;
    
    case 5: { ///////// Leise Lange Heizen 
        if (major_fail){modus = 3;}
                                
        if (TempWasserMax < 50){                      //5kw unter 60
            fuel_pump_speed = 3;
            water_pump_speed = 100;
            Fan_speed = 60;
            }
        if (TempWasserMax >= 60 ){
            fuel_pump_speed = 0;        //TODO kleinsmögliche leistungstufe finden
            water_pump_speed = 50;      // sollte hier leise heizen bis zu warm wird
            Fan_speed = 25;
            }
        if (TempWasserMax > 80) {fuel_pump_speed = 0;  modus = 4;}    //regelpause
            
            }break;
    case 6: { ///////// seriel tuning (nix ändern nur eben nicht aus)                            
            

            }break;
    }



  }//ende contol funktion()



//wasser pumpe laufen lassen und so
void water_pump(){          // wenn brennt  dann reglt  zwischen 60 u 70 hoch außer auto über 13.3V aber immer über 105°C

    if (modus >=1) { // nur regeln im Betrieb
        //FIXME geht gerad nicht weil if (cleanBoard_Voltage < 13.3)              // muss nur laufen wenn auto aus ( volt kleiner 13.3V) oder übertemperatur
        //{ 
        
        if (!sleep_mode && TempWasserMax < 59 )              {water_pump_speed=0;}       // unter 60 nicht regeln
        if (sleep_mode && TempWasserMax <59   )              {water_pump_speed = 50;}    // asußer schlafmodus
        if (TempWasserMax >= 60 && TempWasserMax <70)        {water_pump_speed=80;}      // 60 50%   
        if (TempWasserMax >= 70 )                            {water_pump_speed=100;}     // ab 70 volle püulle
        //}
        }  
    if ( modus == 0 && TempWasserMax >  105) {water_pump_speed = 100;}
    if ( modus == 0 && TempWasserMax <= 102) {water_pump_speed = 0;  }
    
}//ende waserp
/*


*/