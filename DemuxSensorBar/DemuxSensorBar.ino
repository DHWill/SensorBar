const int selectPins[3] = {2, 3, 4}; // S0~2, S1~3, S2~4
const int zInput = A0; // Connect common (Z) to A0 (analog input)
float vSpeed = 0.05;
float posX, dirX, destX, velocity;
//----------------------------------------------------------------
void setup() 
{
  Serial.begin(115200); // Initialize the serial port
  // Set up the select pins as outputs:
  for (int i=0; i<3; i++)
  {
    pinMode(selectPins[i], OUTPUT);
    digitalWrite(selectPins[i], HIGH);
  }
  pinMode(zInput, INPUT); // Set up Z as an input
}
//----------------------------------------------------------------
void loop(){
  analogReads(4, 5);

}
//------------------------------------------------------------------
bool debug = false;
int inThresh = 100;
int nearsetSensor = -1;
int16_t toSend[21];


void analogReads(int nMux, int sensPMux){
  int last = 0;
  int count = 0;
  toSend[0] = (int16_t) -1;
  for(int a = 0; a < nMux; a++){
    for(int i = 0; i < sensPMux; i++){
      int16_t sensorDistance = getMuxPin(i, a);  //get mux_i from demuxer0
      int index = (sensPMux * a) + i;
      toSend[index+1] = sensorDistance;
      //toSend[index+1] = count;
      //Serial.print(String(sensorDistance) + "\t");
      if(sensorDistance > inThresh){
        if(sensorDistance > last){
          last = sensorDistance;
          nearsetSensor = index; 
        }
      }
    }
  }
  //Serial.write((char*)toSend, sizeof(toSend));
  
  destX = nearsetSensor / 20.;    //20 sensors
  destX *=2.;
  destX -= 1.;
  velocity = destX - posX;
  posX += (velocity * vSpeed);
  

  float posXI = posX +1;
  posXI /=2;
  posXI = 1. - posXI;
  //posXI *= 255;    //255
  
  
  
  Serial.write((char *)&posXI, sizeof(posXI));
  
  if(debug){
      for(int i = 0; i < 56; i ++){   //56 screens
        if(int(posXI) == i){
          Serial.print("||");
        }
        else{Serial.print("o");}
      }
      Serial.print(posXI);
      Serial.println("  NEAREST IS ->" + String(destX) + "posX IS ->" + String(posX));
      
  }
}
//----------------------------------------------------------------
int getMuxPin(byte pin, int analogPin)
{
  for (int i=0; i<3; i++)
  {
    if (pin & (1<<i))
      digitalWrite(selectPins[i], HIGH);
    else
      digitalWrite(selectPins[i], LOW);
  }
  delayMicroseconds(1);   // (Address to Signal OUT < 720 ns) + (Propogation Delay Time < 60ns) = 0.78 microsecond
  return analogRead(analogPin);
}
