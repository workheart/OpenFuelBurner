// Jurs NTC rechner

float MAX_ADC=1023.0;
unsigned long Tstartmillis= 0;              //only workes here now
unsigned long Tstartmillis2= 0;              //only workes here now

    

float lese_NTC(float R0, float R1, float VA_VB,float RV=1000,float T1=80, float T0=0) {//widerstand bei0, wiederstand bei 80, spannungsverhältniss

 float abs0=-273.15;
 T0+=-abs0;   
 T1+=-abs0;  // umwandeln Celsius in absolute Temperatur
 float B= (T0 * T1)/ (T1-T0) * log(R0/R1); // Materialkonstante B
 float RN=RV*VA_VB / (1-VA_VB); // aktueller Widerstand des NTC
 return T0 * B / (B + T0 * log(RN / R0))-273.15;
  // T0           : Nenntemperatur des NTC-Widerstands in °C
  // R0           : Nennwiderstand des NTC-Sensors in Ohm
  // T1           : erhöhte Temperatur des NTC-Widerstands in °C
  // R1           : Widerstand des NTC-Sensors bei erhöhter Temperatur in Ohm
  // Vorwiderstand: Vorwiderstand in Ohm  
  // VA_VB        : Spannungsverhältnis "Spannung am NTC / Betriebsspannung bzw messung durch maximum"
  // Rückgabewert : Temperatur
  }

float lese_PT1000(int F0_eingang) {           // liest den ADC-Eingang und rechnet auf Grad Celsius um

  int ADC_wert;
  float temperatur;
    digitalWrite(V_ref_flame_pin, HIGH);                   // Vref einschalten 
    analogRead(F0_eingang);                               // ADC in Betrieb nehmen warum 2mal?;
    ADC_wert = analogRead(F0_eingang);                       // und Kanal messen
    digitalWrite(V_ref_flame_pin, LOW);                     // (um erwärmung zu vermeiden)
    if(ADC_wert < MAX_ADC) {                             // nur gueltige Werte auswerten
      float PT_x = RVFlame * ADC_wert / (MAX_ADC-ADC_wert);   // Widerstand ausrechnen         
      PT_x = PT_x - 1000.0;                              // Offset fuer 0 Grad abziehen 
      temperatur = PT_x / 3.85;                        // und auf Grad C skalieren
    } else temperatur = 9999;                          // falls PT1000-Zuleitung offen  
      return temperatur;                                // return adc wert
// nach einem Post https://www.mikrocontroller.net/attachment/highlight/242914 das mal ausprobieren

  }


float lese_Spannung(int ad_eingang, float R1 = 0, float R2 =1) {  //Analogpin, R1 on high side R2 gnd to  analogpin return in 100mV (125 = 12.5V)

  unsigned long sum_ADC = 0;                  //initialisieren der sumer
  int sampe_count=5;                // mach 3-5 messungen zum glätten
  unsigned long VoltageADC;
  unsigned long Voltage;
  // 1= 0-5V no resistor dependent on resistornetwork 
  for (int i = 0; i < sampe_count; i++) // über die anzahl der samples messen
   {
    sum_ADC+=analogRead(ad_eingang);
    }
  {
  VoltageADC = ((sum_ADC/sampe_count)*50)/MAX_ADC;  //5000 is Ref_voltage  und durch die anzahl der samples teilen
  }
  return Voltage = (VoltageADC/ (R2/(R1+R2))); //tune ist ein verzweifelter versuch genauer zu messen
 }
 
void update_Temp(){              //Update Temp Volt
 unsigned long currentmillis = millis(); 
    if (currentmillis - Tstartmillis2 >= (temp_update_intervall*4U))               //alle 100ms
    
      { Tstartmillis2 = currentmillis;
        TempFlame0    = lese_PT1000(flame0_pin);}
  
    if (currentmillis - Tstartmillis >= temp_update_intervall)                   // alle 25ms
    {
      Tstartmillis = currentmillis;
    
      Board_Voltage = lese_Spannung(car_mesure_pin,RVoltagediv2,RVoltagediv1);
      TempWasser0   = lese_NTC(R0Wasser, R1Wasser,(analogRead(wasser0_pin)/MAX_ADC), RVWasser, T1Wasser, T0Wasser);
      TempWasser1   = lese_NTC(R0Wasser, R1Wasser,(analogRead(wasser1_pin)/MAX_ADC), RVWasser, T1Wasser, T0Wasser);
      new_temp = 1;
    }
 }
 void fake_Temp(){              //Update Temp Volt Current every loop
    
    Board_Voltage = rand() %2 +125;
    //current_total = rand() %10;
    TempFlame0    = rand() %20 +210;
    TempWasser0   = rand() %5  + 20; 
    TempWasser1   = rand() %5  + 20;
    }
 

