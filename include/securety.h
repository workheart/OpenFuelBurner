 #include <Arduino.h>

int counter_securety = 0;
int counter_flame = 0;
int clean_securety =5;                // durch welchen schnitt sollen messwerte geglättet werden
unsigned long securetystartMillis1 =0;
unsigned long flamestartMillis = 0;
int sumTempWasser0, sumTempWasser1, sumTempFlame0, sumBoard_Voltage;             //messwert summe
int cleanTempWasser0, cleanTempWasser1, cleanTempFlame0;    //bereinigte messwerte
float cleanBoard_Voltage;
int FlameCheck, oldFlameCheck;

 void SecuretyTempCheck() { //chek temp for iregulare
    
    unsigned long currentMillis = millis();

    if (currentMillis - securetystartMillis1 >= 300UL ){       //werte Sammeln und glätten
        securetystartMillis1 = currentMillis; 
        counter_securety++;
        //summen addieren bis...
        sumTempWasser0 = sumTempWasser0 + TempWasser0;
        sumTempWasser1 = sumTempWasser1 + TempWasser1;
        sumTempFlame0 =  sumTempFlame0 + TempFlame0;
        sumBoard_Voltage = sumBoard_Voltage + Board_Voltage;
        
        }
    if (counter_securety>=clean_securety){                                                       //wenn genug werte (clean sec) da glätten
        counter_securety = 0;
        // ... es clean_securety (5) summen gibt und dadurch teilen
        cleanTempWasser0 = sumTempWasser0/clean_securety;
        cleanTempWasser1 = sumTempWasser1/clean_securety;
        cleanTempFlame0 =  sumTempFlame0/clean_securety;
        cleanBoard_Voltage = sumBoard_Voltage/clean_securety;
        // and repea
        sumTempWasser0 = 0;
        sumTempWasser1 = 0;
        sumTempFlame0 =  0;
        sumBoard_Voltage = 0;


        }
   
    
    
    if (millis()>usr_securety_delay+2000UL){         //Alles erst nach delay 
    //sind werte plausiebel       
    if (cleanTempFlame0 < -100 || cleanTempFlame0 > 1000 || cleanTempWasser0 > 200 || cleanTempWasser0 < -25 || cleanTempWasser1 >200 || cleanTempWasser1 < -25)
        {
         major_fail = 1;
         Serial.println("fuhler werte unplausiebel");
        }
    //ist spannung sicher
    if (cleanBoard_Voltage > 15.00 || cleanBoard_Voltage < 10.00)
        {
         major_fail = 1;
         Serial.println("spannung unsicher");
        }
    //ist Backup wasser fühler realistisch *temp diff etc
    if ( (cleanTempWasser0 - cleanTempWasser1) > 30 || (cleanTempWasser0 - cleanTempWasser1)< -30)
        {
         major_fail = 1;
         Serial.println("temp differenz");
        }
    // zu viele startversucher?
    if (ignit_fail >= 3)
        {
        major_fail= 1;
        Serial.println("zu viele startversuche");
        ignit_fail = 0;
        }
    // is flamme noch an
    if (modus == 2)
        {      // einfach zu Kalt
        if ( cleanTempFlame0 < 100)
            {
            major_fail = 1;
            Serial.println  ("flamme unter 100");
            Flame = 0;
            }
         
            }
        }
       }  //ende der securety Funktion    
void SecuretyPartCheck(){  ///// alle sein und messen
    double glow_current, fan_current, pump_current;

    Serial.print(currentCurrent_read(50)); Serial.println(" ruhe strom in A");
    analogWrite(glow_pin, 255);
    delay(10*extratime);
    glow_current = currentCurrent_read(50);
    digitalWrite(glow_pin, HIGH);
    Serial.print(glow_current); Serial.println("A : glow current");

    analogWrite(fan0_pin, 255);
    delay(200*extratime);
    fan_current = currentCurrent_read(100);
    digitalWrite(fan0_pin, LOW);
    Serial.print(fan_current); Serial.println("A : fan current");

    analogWrite(pump_pin, 255);
    delay(200*extratime);
    pump_current = currentCurrent_read(50);
    digitalWrite(pump_pin, LOW);
    Serial.print(pump_current); Serial.println("A : Pump current");

    // analogWrite(fuel_pin, 50);
    // delay(50*extratime);
    // fuel_current = currentCurrent_read(50);
    // analogWrite(fuel_pin, LOW);
    // Serial.print(fuel_current); Serial.println("A : fuel current");
    


 }// ende securetypartychecker
