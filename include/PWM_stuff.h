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
  int pump_PWM_value = map(water_pump_speed, 0, 100, 0, 254); 
  analogWrite(pump_pin, pump_PWM_value);
  }



void glow_plug()                                                // Glühkerze
{
  	if (millis() > gtimer) {gtimer = millis();}                 // timer
      if (gtimer - gtimerold >= 1000UL){
      gtimerold = gtimer;
      if (glow_time > 0){glow_time--;}
   
 if (glow_time > 0 && dry) {                                    // drying modus dry is bool turn on to just warm fuel
    analogWrite(glow_pin, 20);                                  // 255 = 14V --> 8-9V 150 
      glowing = true;}
        else { digitalWrite(glow_pin, LOW);                    //invertet logic = High aus nomral Low aus 
                glowing = false;}

  
  if (glow_time > 0 && !dry) {        
    analogWrite(glow_pin, 195);
        glowing = true;}                                // 255 = ~14V --> 8-9V 150
        else { digitalWrite(glow_pin, LOW);                    //invertet logic 
                glowing = false;}
    }
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

    impulse_delay = 10000UL/fuel_pump_speed; //speed in Hz

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
analogWrite (fuel_pin, 200);
delay(1000);
analogWrite (fuel_pin, 0);
delay(1000);
analogWrite (pump_pin, 150);
delay(1000);
analogWrite (pump_pin, 0);
delay(1000);
analogWrite (glow_pin, 150);
delay(1000);
analogWrite (glow_pin, 0);
delay(1000);
analogWrite (fan0_pin, 100);
delay(1000);
analogWrite (fan0_pin, 0);
delay(1000);
}