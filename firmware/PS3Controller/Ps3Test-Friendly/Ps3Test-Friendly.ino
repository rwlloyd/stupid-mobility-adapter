#include <Ps3Controller.h>

//variable to save the esp bluetooth address useful for debugging.
String esp_address; 
 
void onEvent()
{
   if(Ps3.event.analog_changed.button.l1){
       Serial.print("L1 New value: ");
       Serial.println(Ps3.data.analog.button.l1);
   }
    
   if(Ps3.event.analog_changed.button.l2){
       Serial.print("L2 New value: ");
       Serial.println(Ps3.data.analog.button.l2);
   }
 
   if(Ps3.event.analog_changed.button.r1){
       Serial.print("R1 New value: ");
       Serial.println(Ps3.data.analog.button.r1);
   }
 
   if(Ps3.event.analog_changed.button.r2){
       Serial.print("R2 New value: ");
       Serial.println(Ps3.data.analog.button.r2);
   }
    
   if(Ps3.event.analog_changed.button.cross){
       Serial.print("Cross New value: ");
       Serial.println(Ps3.data.analog.button.cross);
   }
 
   if(Ps3.event.analog_changed.button.triangle){
       Serial.print("Triangle New value: ");
       Serial.println(Ps3.data.analog.button.triangle);
   }
 
   if(Ps3.event.analog_changed.button.square){
       Serial.print("Square New value: ");
       Serial.println(Ps3.data.analog.button.square);
   }
 
   if(Ps3.event.analog_changed.button.circle){
       Serial.print("Circle New value: ");
       Serial.println(Ps3.data.analog.button.circle);
   }
 
   if(Ps3.event.analog_changed.button.up){
       Serial.print("Up New value: ");
       Serial.println(Ps3.data.analog.button.up);
   }
 
   if(Ps3.event.analog_changed.button.down){
       Serial.print("Down New value: ");
       Serial.println(Ps3.data.analog.button.down);
   }
 
   if(Ps3.event.analog_changed.button.left){
       Serial.print("Left New value: ");
       Serial.println(Ps3.data.analog.button.left);
   }
 
   if(Ps3.event.analog_changed.button.right){
       Serial.print("Right New value: ");
       Serial.println(Ps3.data.analog.button.right);
   }
}
 
void setup()
{
    Serial.begin(115200);
 
    esp_address = String(Ps3.getAddress());
    // Ps3.begin("24:62:AB:FC:BB:4A"); animatronic head addr
//    Ps3.begin("FC:F5:C4:00:FA:C6");
    Ps3.begin("10:52:1c:76:30:42");

 
    Serial.println("Initialization finished.");
    Serial.println(esp_address);
     
    Ps3.attach(onEvent);
 
}
 
void loop()
{
  vTaskDelete(NULL);
}
