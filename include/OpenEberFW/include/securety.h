 #include <Arduino.h>

int counter_securety = 0, counter_securety2 = 0;
int counter_flame = 0;
unsigned int fehlercode;            
unsigned long securetystartMillis1 =0;
unsigned long flamestartMillis = 0;
int sumTempWasser0, sumTempWasser1,sumTempWasserMax, sumTempFlame0, sumBoard_Voltage;             //messwert summe
int FlameCheck, oldFlameCheck;

 void SecuretyTempCheck() { //chek temp for iregulare   and smooth out// TODO: circular buffer rewrite
     fehlercode = 0;
    if (firstrun)        // nur im ersten loop werte als clean annehmen 
        {
        cleanTempWasser0   = TempWasser0;
        cleanTempWasser1   = TempWasser1;
        cleanTempFlame0    = TempFlame0;
        cleanBoard_Voltage = Board_Voltage;
        if (cleanTempWasser0 > cleanTempWasser1)
             {cleanTempWasserMax = cleanTempWasser0;} 
        else {cleanTempWasserMax = cleanTempWasser1;}
      
            }
   
    if (new_temp){       //werte Sammeln (wenn neue da und glätten
        counter_securety++;
        counter_securety2++;

        sumTempWasser0 = sumTempWasser0 + TempWasser0;
        sumTempWasser1 = sumTempWasser1 + TempWasser1;
        sumTempFlame0 =  sumTempFlame0 + TempFlame0;
        sumBoard_Voltage = sumBoard_Voltage + Board_Voltage;
        new_temp = false;       //werte wurden geupdatestd
        }
    if (counter_securety == clean_securety){                                                       //wenn genug werte (clean sec) da glätten
        counter_securety = 0;
        
        cleanTempWasser0 = sumTempWasser0/clean_securety;
        cleanTempWasser1 = sumTempWasser1/clean_securety;
        cleanTempFlame0 =  sumTempFlame0/clean_securety;
        
        if (cleanTempWasser0 > cleanTempWasser1)
             {cleanTempWasserMax = cleanTempWasser0;} 
        else {cleanTempWasserMax = cleanTempWasser1;}
               
        sumTempWasser0 =   0;
        sumTempWasser1 =   0;
        sumTempFlame0 =    0;
    }

    if (counter_securety2 == (clean_securety*4)){                                                       //um spannungseinbrüche start etc zu sichren
        counter_securety2 = 0;

        cleanBoard_Voltage = sumBoard_Voltage/(clean_securety*4);
        sumBoard_Voltage = 0;

        }
   
    
    
    if (millis()>usr_securety_delay+2000UL){         //Alles erst nach delay 
   
    //sind werte plausiebel       
    if (cleanTempFlame0 < -100 || cleanTempFlame0 > 300 || cleanTempWasser0 > 110 || cleanTempWasser0 < -25 || cleanTempWasser1 >110 || cleanTempWasser1 < -25)
        {
         major_fail = 1;
         fehlercode =fehlercode+1;
         if(SerialDebug){Serial.println("fuhler werte unplausiebel");}
        }
    //ist spannung sicher
    if (cleanBoard_Voltage > 150 || cleanBoard_Voltage < 105 )
        {
         major_fail = 1;
         fehlercode = fehlercode+10;
         if(SerialDebug){Serial.println("spannung unsicher");}
        }
    //ist Backup wasser fühler realistisch *temp diff etc
    if ( (cleanTempWasser0 - cleanTempWasser1) > 30 || (cleanTempWasser0 - cleanTempWasser1)< -30)
        {
         major_fail = 1;
         fehlercode =fehlercode+100;
         if(SerialDebug){Serial.println("temp differenz");}
        }
    // zu viele startversucher?
    if (ignit_fail > 3)
        {
        major_fail= 1;
        fehlercode =fehlercode+1000;
        if(SerialDebug){Serial.println("zu viele startversuche");}
        ignit_fail = 0;
        }
    // is flamme noch an
    if (modus == 2 || modus >= 5 )
        {
        if ( cleanTempFlame0 < 150)
            {
            major_fail = 1;
            fehlercode = fehlercode+10000;
          if(SerialDebug){Serial.println  ("flamme unter 150");}
            Flame = 0;
            }
         
            }
        }
       }  //ende der securety Funktion    


/*
int circularFilter (int in, int count){






}
*/