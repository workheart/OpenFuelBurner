
unsigned long oldtimer =0;
int sellerieinput;
unsigned long timer = 0;

void Serial_plot()  // makes the plotter plot
    {

        timer = millis();
    if(timer- oldtimer >= user_delay ){     
    oldtimer = timer;                       //timer zurücksetzen
    Serial.print(TempFlame0); Serial.print(",");
    Serial.print(TempWasser0); Serial.print(",");
    Serial.print(TempWasser1); Serial.print(",");
    Serial.print(Board_Voltage); Serial.print(",");
    Serial.print(water_pump_speed); Serial.print(",");
    Serial.print(Fan_speed); Serial.print(",");
    Serial.print(fuel_pump_speed);Serial.print(",");
    if (dry){Serial.print(glowing*30); Serial.print(",");}
    if (!dry){Serial.print(glowing*100); Serial.print(",");}
    Serial.print(seconds); Serial.print(",");
    Serial.println(current_total);

        }    
    }

void Serial_tuning(){

    if(Serial.available()){
        sellerieinput = Serial.parseInt();
                if(sellerieinput == 0) {return;}
                else if(sellerieinput == 1){
                    Serial.println("START");
                    modus = 1;
                    Serial.flush();}
                else if(sellerieinput == 11){
                    Serial.println("AUS");
                    modus= 0;
                    Serial.flush();}
                else if(sellerieinput == 9){
                    Serial.println("reset");
                    major_fail = 0;
                    Serial.flush();}                    
                else if(sellerieinput == 5){
                    if (sleep_mode > 0){Serial.println("sleep off");sleep_mode = 0;}
                    else{Serial.println("sleep on"); sleep_mode = 1;}
                    Serial.flush();}               
                else if(sellerieinput == 12){
                    Serial.println("foehn runter");
                    Fan_speed -= 5;
                    Serial.flush();}
                else if(sellerieinput == 18){
                    Serial.println("foehn hoch");
                    Fan_speed += 10;
                    Serial.flush();}
                else if(sellerieinput == 1){
                    Serial.println("ein");
                    modus = 1;
                    Serial.flush();}
                else if(sellerieinput == 99){
                    Serial.println("glüh 10");
                    glow_time = 10;
                    Serial.flush();}
                else if(sellerieinput == 55){
                    Serial.println("glow Off");
                    glow_time = 0;
                    Serial.flush();}
                else if(sellerieinput == 38){
                    Serial.println("merh sprit ");
                    fuel_pump_speed += 0.5;
                    Serial.flush();}
                else if(sellerieinput == 32){
                    Serial.println("weniger sprit");
                    if   (fuel_pump_speed > 0) {fuel_pump_speed -= 0.5;}
                    else {fuel_pump_speed = 0;}
                    Serial.flush();}
                else if(sellerieinput == 48){
                    Serial.println("tuning");
                    modus = 6;
                    Serial.flush();}
                else if(sellerieinput == 45){
                    Serial.println("spritr Aus!");
                    fuel_pump_speed = 0;
                    Serial.flush();}    
                else{Serial.println(sellerieinput);Serial.flush();}
                 }







    //end serial Tuning
}