hw_timer_t * timer = NULL;

void onTimer(){
    Serial.print("timer ");
    Serial.println(millis());
}

// start/stop the timer
void toggleTimer(){
    if(timer){
        timerEnd(timer);
        timer = NULL;
    } else {
        timer = timerBegin(3, 80, 1);//div 80
        timerAttachInterrupt(timer, &onTimer, 1);
        timerAlarmWrite(timer, 1000000, true);//1000ms
        timerAlarmEnable(timer);
    }
}
