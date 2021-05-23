// Jurs NTC rechner

float MAX_ADC=1023.0; //bei int. adc


float lese_NTC(float R0, float R1, float VA_VB,float RV=1000,float T1=80, float T0=0) {//widerstand bei0, wiederstand bei 80, spannungsverhältniss

 float abs0=-273.15;
 T0+=-abs0;   
 T1+=-abs0;  // umwandeln Celsius in absolute Temperatur
 float B= (T0 * T1)/ (T1-T0) * log(R0/R1); // Materialkonstante B
 float RN=RV*VA_VB / (1-VA_VB); // aktueller Widerstand des NTC
 return T0 * B / (B + T0 * log(RN / R0))-273.15;
 }
  // T0           : Nenntemperatur des NTC-Widerstands in °C
  // R0           : Nennwiderstand des NTC-Sensors in Ohm
  // T1           : erhöhte Temperatur des NTC-Widerstands in °C
  // R1           : Widerstand des NTC-Sensors bei erhöhter Temperatur in Ohm
  // Vorwiderstand: Vorwiderstand in Ohm  
  // VA_VB        : Spannungsverhältnis "Spannung am NTC zu Betriebsspannung"
  // Rückgabewert : Temperatur

  //PT1000 mit internem ADwandler lesen

float lese_PT1000(int ad_eingang) {           // liest den ADC-Eingang und rechnet auf Grad Celsius um

  int ADC_wert;
  float temperatur;
    digitalWrite(V_ref_flame_pin, HIGH);                   // Vref einschalten 
    analogRead(ad_eingang);                                // ADC in Betrieb nehmen
    ADC_wert = analogRead(ad_eingang);                       // und Kanal messen
    digitalWrite(V_ref_flame_pin, LOW);                     // (um erwärmung zu vermeiden)
    if(ADC_wert < MAX_ADC) {                             // nur gueltige Werte auswerten
      float PT_x = RVFlame * ADC_wert / (MAX_ADC-ADC_wert);   // Widerstand ausrechnen         
      PT_x = PT_x - 1000;                              // Offset fuer 0 Grad abziehen 
      temperatur = PT_x / 3.85;                        // und auf Grad C skalieren
    } else temperatur = 9999;                          // falls PT1000-Zuleitung offen  
      return temperatur;
  }

float lese_Spannung(int ad_eingang, float R1 = 0, float R2 =1) {  //Analogpin, R1 on high side R2 gnd to  analogpin

  int sum_ADC = 0;                  //initialisieren der sumer
  int sampe_count=10;                // mach 3-5 messungen zum glätten
  float VoltageADC;
  float Voltage;
  // 1= 0-5V no resistor dependent on resistornetwork 
  for (int i = 0; i < sampe_count; i++) // über die anzahl der samples messen
   {
    sum_ADC+=analogRead(ad_eingang);
    }
  {
  VoltageADC = ((sum_ADC/sampe_count)*5.0)/MAX_ADC;  //5.0 is Ref_voltage  und durch die anzahl der samples teilen
  }
  return Voltage = (VoltageADC/ (R2/(R1+R2))); //tune ist ein verzweifelter versuch genauer zu messen
 }
 
float currentCurrent_read(int sampe_count = 50)  {     // read and clen Current OF ACS sensor 1ms/sample!
 
    /* Sens laut Datenblatt 66mv/A bei 30A variante
                            100mv/A bei 20A
                            185mV/A bei 5A
    */

  float Current, current_sum = 0;
  for(int i=0;i< sampe_count;i++) {
    float VoltageADC =  analogRead(current_mesure_pin) * 5.0 / 1023.0; // Messwert auslesen in Volt
    delay(1);                                                          //acs delay
    VoltageADC =  VoltageADC - 2.5;                    // Nulldurchgang (vcc/2) abziehen
    float currentCurrent =  VoltageADC*100.0;                                            // votl in mAmpere*1000 berechnen
    current_sum += currentCurrent;                               // Summieren
  }
  return Current = fabs(current_sum/sampe_count);
  } //ende current current


void update_Temp(){              //Update Temp Volt Current every loop
 
    unsigned long startmillis= 0;              //only workes here now
    unsigned long currentmillis = millis(); 
    
    if (currentmillis - startmillis >= user_delay)
     {startmillis = currentmillis;
    Board_Voltage = lese_Spannung(car_mesure_pin,908,299);//
    Extern_Voltage = lese_Spannung(Vmesure_pin,908,300);//
    current_total = currentCurrent_read(2);
    TempFlame0    = lese_PT1000(flame0_pin);
    TempWasser0   = lese_NTC(R0Wasser, R1Wasser,analogRead(wasser0_pin)/MAX_ADC);
    TempWasser1   = lese_NTC(R0Wasser, R1Wasser,analogRead(wasser0_pin)/MAX_ADC);
    if (TempWasser0 > TempWasser1){TempWasserMax = TempWasser0;}    // immer den hoereen wert nehmen
                else {TempWasserMax = TempWasser1;}
    }
 }
 void fake_Temp(){              //Update Temp Volt Current every loop
    
    Board_Voltage = rand() %2 +12.5;
    //current_total = rand() %10;
     current_total = currentCurrent_read(30);
    TempFlame0    = rand() %20 +210;
    TempWasser0   = rand() %5  + 20; 
    TempWasser1   = rand() %5  + 20;
    if (TempWasser0 > TempWasser1){TempWasserMax = TempWasser0;}    // immer den hoereen wert nehmen
                else {TempWasserMax = TempWasser1;}
    }
 