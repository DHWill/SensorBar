const int selectPins[3] = {2, 3, 4}; // S0~2, S1~3, S2~4
const int zInput = A0; // Connect common (Z) to A0 (analog input)
//----------------------------------------------------------------
void setup() 
{
  Serial.begin(9600); // Initialize the serial port
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
int inThresh = 100;
int nearsetSensor = -1;
int16_t toSend[21];


void analogReads(int nMux, int sensPMux){
  int last = 0;
  int count = 0;
  toSend[0] = (int16_t) -1;
  for(int a = 0; a < nMux; a++){
    for(int i = 0; i < sensPMux; i++){
      int16_t _sensorDistance = getMuxPin(i, a);  //get mux_i from demuxer0
      toSend[count+1] = _sensorDistance;
      count ++;
    /*
    if(sensorDistance > inThresh){
      if(sensorDistance > last){
        last = sensorDistance;
        nearsetSensor = (sensPMux * a) + i; 
        }
      }
     */
    }
  }
  Serial.write((char*)toSend, sizeof(toSend));
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
