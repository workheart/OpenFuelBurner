
unsigned long oldtimer =0;
int sellerieinput, TWasserdiff;
bool wassermarsch;
unsigned long timer = 0;
char serialInput[12]; //00,00,00,00,11
int32_t SerialinArry[4] = {};  // as many variables -1
int counter = 0;
String incommingcmd;


void Serial_displ()  // makes the Display disp
    {
        timer = millis();
    if(timer- oldtimer >= 500 ){        
    oldtimer = timer;                    //timer zurücksetzen

wassermarsch = water_pump_speed;


       // am ende sollte n Array? rauskommen das so aussieht: 
    //      [Flam  Wasser Volt pumpe fuel(int) modus fehlercode]
    //         1     2      3    4     5         6      7  
if (modus == 6){   
    Serial.print(int(TempFlame0));       Serial.print(","); //  [0]
    Serial.print(int(TempWasser0));      Serial.print(","); //  [1]
    Serial.print(int(Board_Voltage));    Serial.print(","); //  [2]     
    Serial.print(AirExtra);              Serial.print(","); //  [3]     
    Serial.print(FuelExtra);             Serial.print(","); //  [4] 
    Serial.print(fuel_pump_speed);       Serial.print(","); //  [5]
    Serial.print(Fan_speed);             Serial.print(","); //  [6]
    Serial.print(modus);                 Serial.print(","); //  [7]
    Serial.println(int(slowdown));                          //  [8]
    Serial.flush();
    } // nomral
else{
    Serial.print(int(TempFlame0));       Serial.print(","); //  [0]
    Serial.print(cleanTempWasserMax);      Serial.print(","); //  [1]
    Serial.print(cleanBoard_Voltage);    Serial.print(","); //  [2]     
    Serial.print(sleep_mode);            Serial.print(","); //  [3]
    Serial.print(wassermarsch);          Serial.print(","); //  [4]
    Serial.print(glowing);               Serial.print(","); //  [5]
    Serial.print(modus);                 Serial.print(","); //  [6]
    Serial.print(Fan_speed);             Serial.print(","); //  [7]
    Serial.print(fehlercode);            Serial.print(","); //  [8]
    Serial.print(fuel_pump_speed);       Serial.print(","); //  [9]
    if     (modus == 1) {Serial.println(seconds);}          //  [10]
    else if(modus == 3) {Serial.println(coolseconds);}      //  [10]
    else                {Serial.println(  int(254) );}      //  [10]
    Serial.flush();
    } // nomral
    }    //<timer ende
    }       // <function end

void Serial_recive_frdispl(){

 if (Serial.available()) { 
    incommingcmd = Serial.readStringUntil('\n');
    incommingcmd.trim();
    //Array zu  variablen 
if (incommingcmd.equals     ("ON")){modus = 1;}
else if (incommingcmd.equals("OF")){modus = 0;}
else if (incommingcmd.equals("SP")){sleep_mode = !sleep_mode;}
else if (incommingcmd.equals("TN")){                        // switch tuning mode on or burn off
        if (modus == 6){modus = 3;}
            else{modus = 6;}
        }
else if (incommingcmd.equals("AP") && modus == 6){AirExtra = AirExtra +5;} // mehr luft
else if (incommingcmd.equals("AM") && modus == 6){AirExtra = AirExtra -5;} // weniger luft
else if (incommingcmd.equals("FP") && modus == 6){FuelExtra = FuelExtra +5;} // mehr sprit
else if (incommingcmd.equals("FM") && modus == 6){FuelExtra = FuelExtra -5;} // weniger sprit
else if (incommingcmd.equals("TP") && modus == 6){FuelExtra = slowdown  +5;} // boot kurve flacher
else if (incommingcmd.equals("TM") && modus == 6){FuelExtra = slowdown  -5;} // boot kurve steiler
else if (incommingcmd.equals("DF") && modus == 6){FuelExtra = 80; AirExtra = 100;} //back to "normal"
else {Serial.print(incommingcmd); Serial.println(" unknown!");}

}

}//ende funktion
