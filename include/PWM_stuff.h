//#include <Arduino.h>

int glow_time; //ftimer statt einschalten ist sicherer
unsigned long gtimer;
unsigned long gtimerold;


void PWM_Map() // sets Fan sepped from fan speet to a PWM  siganla
{
 // pwm average of 13v for 10v = 0.7, 255 * 0.7 = 196
  int Fan_PWM_value = map(Fan_speed, 0, 100, 0, 190); 
  analogWrite(fan0_pin, Fan_PWM_value);

// waterpmp can run on 12V but percentage is easyer
  int pump_PWM_value = map(water_pump_speed, 0, 100, 0, 255); 
  analogWrite(pump_pin, pump_PWM_value);
  }



void glow_plug()                                                // Glühkerze
 {
  if (millis() > gtimer) {gtimer = millis();}
    if (gtimer - gtimerold >= user_delay){
    gtimerold = gtimer;
    glow_time--;
   
 if (glow_time > 0 && dry) {                                    // drying modus dry is bool turn on to just warm fuel
    analogWrite(glow_pin, 255-20); // 255 = 14V --> 8-9V 150   //255- weil is immernoich inverted
      glowing = true;}
        else { digitalWrite(glow_pin, HIGH);                    //invertet logic 
                glowing = false;}

  }
  if (glow_time > 0 && !dry) {        
    analogWrite(glow_pin, 255-160); // 255 = ~14V --> 8-9V 150   //255- weil is immernoich inverted MIN
      glowing = true;}
        else { digitalWrite(glow_pin, HIGH);                    //invertet logic 
                glowing = false;}
    }
 
  

/*              überlegung:
        o,5hz mindest pumpung 
        5hz  maximal
        pro takt 5-15ms impuls (websto shower hat 11)
*/
int impulse_delay;

void fuel_pump()
{
  static bool pulsing;                                  // 0 0der 1 
  static unsigned long ftimer;
  static unsigned long ftimerold;

  if (fuel_pump_speed > 0) {
     if (millis() > ftimer) {ftimer = millis();}

    impulse_delay = user_delay/fuel_pump_speed; //speed in Hz

    if (!pulsing && ftimer > ftimerold + impulse_delay){
      pulsing = 1;
      ftimerold = ftimer;
    }
    if (pulsing && ftimer > ftimerold+ pulse_lenght){
      pulsing = 0;
      ftimerold = ftimer;
    }
  digitalWrite(fuel_pin, HIGH * pulsing);
}
  if (fuel_pump_speed <= 0){ digitalWrite(fuel_pin, LOW);}
}

void pwm_test(){

analogWrite (pump_pin, 20);
analogWrite (fuel_pin, 10);
analogWrite (extern1 , 10);
analogWrite (glow_pin, 20);
analogWrite (fan0_pin, 20);

digitalWrite (relay , 1);
digitalWrite (V_ref_flame_pin, 1);
delay(1);
digitalWrite (relay , 0);
digitalWrite (V_ref_flame_pin, 0);
delay(3);

}