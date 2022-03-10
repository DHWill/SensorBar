#include <ZSharpIR.h>
#define BAUDRATE 9600

// Define the number of sensors
#define NUMBER_OF_SENSORS 6
#define model 20150
#define N_SENSORS 4

ZSharpIR *sensors[N_SENSORS];
//ZSharpIR multiSensor(A0, model);


//------------------------------------------------------------------
void setup() {
  
  for(int i = 0; i < N_SENSORS; i ++){
    sensors[i] = new ZSharpIR(i, model);
  }
  Serial.begin(BAUDRATE);
  while(!Serial);
}

//------------------------------------------------------------------
int inThresh = 500;

void loop() {
  analogReads();
}

//------------------------------------------------------------------
void analogReads(){
  int last = 9999;
  int nearsetSensor = -1; 
  
  for(int i = 0; i < N_SENSORS; i++){
    int sensorDistance = sensors[i]-> distance();
    if(sensorDistance < inThresh){
      if(sensorDistance < last){
        last = sensorDistance;
        nearsetSensor = i;
        delay(20);
        }
      }
    }
  //Serial.println(nearsetSensor);
  Serial.println(sensors[0]-> distance());
  delay(20);
}
