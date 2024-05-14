#include <avr/interrupt.h>


#define A -14.6337
#define B 4791.842
#define C -115334.0

#define adcRes 1023.0

float Vref = 5.0;
int smokePin = A0;
int tempPin = A1;
int thermoPin = A2;
int tempPinTwo = A3;
String message = "";
volatile int RPM = 0;
volatile int RPMReadings[10] = {0,0,0,0,0,0,0,0,0,0}; 
float R0 = 10;                                                     



float tempResistance(int pin){
  float dividerResist = 10000.0;
  float refVoltage = 5.0;
  float scale = 1023.0;
  float tempValue = analogRead(pin);
  float tempResis = dividerResist/(scale/tempValue-1);
  float temperature = 1/(.00335 + (1/3977.0)*log(tempResis/10000));
  return(temperature-270.15);
}
float readThermoCouple(){
  float rawValue = 0;
  for(int i = 0;i<10;i++){
    rawValue += analogRead(thermoPin);
    delay(10);
  }
  rawValue=rawValue/10.0;
  float coupleTemp = (rawValue/adcRes)*Vref;
  coupleTemp = coupleTemp/(100*.000041);
  return (coupleTemp);
  
}
void calibrateMQ2(){
  int numSamples = 50;
  float calibration_total=0;
  for(int i= 0 ; i < numSamples ; i++){
    calibration_total += (5.0*(1023-analogRead(smokePin))/analogRead(smokePin));//equation to find the resistance value of the sensor based on ADC
    delay(500) ;
  }
  R0 = calibration_total/(numSamples*9.8); 
}
float readMQ2(){
  int numSamples = 5;
  float totalData = 0;
  for(int i = 0;i<numSamples;i++){
    totalData += (5.0*(1023-analogRead(smokePin))/analogRead(smokePin));
    delay(100);
  }
  float rsro =  totalData/(numSamples*R0);
  return rsro;
}

void setup() {
  //Enabling Interrupts (We want Port B)
  EICRA = (1<<ISC01)|(0 << ISC00);  // sense falling edge on the INT0 pin
  EIMSK = 0 << INT0;   // enable INT0 interrupt
  TCCR1A = 0; 
  TCCR1B = 0; 
  TCNT1 = 0; 
  TCCR1A = (1<<COM1A1)|(1<<COM1B1)|(1<<WGM11); 
  TCCR1B = (1<<WGM13)|(1<<CS10); 
 
  ICR1 = 320; // TOP = 320
  OCR1A = 300;//14 is 50%speed
 
  Serial.begin(9600);
  pinMode(smokePin,INPUT);
  pinMode(tempPin, INPUT);
  pinMode(thermoPin, INPUT);
  pinMode(10, OUTPUT);
  pinMode(9,OUTPUT);
  
  calibrateMQ2();
  pinMode(2,INPUT);
  pinMode(5,OUTPUT);
  
  sei();
}
ISR(INT0_vect){
  RPM++;
}
void readRPM(){
  RPM=0;
  EIMSK = 1 << INT0;   // enable INT0 interrupt
  delay(900);
  EIMSK = 0 << INT0; //disable interrupt on pin
  RPM = RPM * 60.0/2.0;
  RPM = averageRPM();
  
}
int averageRPM(){
  for(int i=0;i<9;i++){
    RPMReadings[i+1]=RPMReadings[i];
  }
  RPMReadings[0] = RPM;
  float RPMtotal = 0;
  for(int i=0;i<=9;i++){
    RPMtotal+=RPMReadings[i];
  }
  RPMtotal = RPMtotal/10; 
  return(RPMtotal);
}
void readCommand(){
  if(Serial.available()){
    //char mess = Serial.read();
    message = Serial.readString();
    message = message.substring(0,message.length() -2); //purge \0\n
    //Serial.println(message);
    if(message == "z"){
      OCR1A = 0;
      //Serial.println("fan to zero");
    }
    else if(message == "h"){
      OCR1A = 14;
    }
    else if(message == "f"){
      OCR1A = 300;
    }
  }
}

void loop() {
  
  readCommand();
  float smokeValue = readMQ2();
 
  float PPM = pow(10,((log10(smokeValue)-.53)/-.45)+2.3);

  float temperature = tempResistance(tempPin);
  float temp_two = tempResistance(tempPinTwo);
  float thermoCouple = readThermoCouple()+temperature;
  
  readRPM();
  
  
  Serial.print(PPM);
  Serial.print(" ");
  
  Serial.print(temperature);
  Serial.print(" ");
  
  Serial.print(thermoCouple,4);
  Serial.print(" ");
  
  Serial.print(temp_two);
  Serial.print(" ");

  Serial.println(RPM);
  
  delay(100);
  
}

