#include <stdio.h>
#include <Ticker.h>
#include <stdbool.h>
#include <math.h>

Ticker periodicTicker; 

// Define the Pin names to be used later in code
#define wDWaveform 22
#define ledOutput 16 
#define buttonInput 23
#define potInput 2
#define squareWave 21

// Initial settin of variables
volatile int executionsCount = 0;
int potValue;
int voltage[4];
int error_code = 0;
int buttonValue = 0;
int hertz;
float squareFreq;
float average;
float sum = 0.0;
float voltValue = 0.0;
 
void setup() {
 
  Serial.begin(19200);
  // Set pin modes for initialisation as input/output and whart output to start at
  pinMode(ledOutput, OUTPUT); 
  pinMode(buttonInput, INPUT);
  pinMode(potInput, INPUT);
  pinMode(squareWave, OUTPUT);
  digitalWrite(ledOutput, LOW);
  digitalWrite(wDWaveform, LOW);
//  digitalWrite(squareWave, LOW);

  // Define all array values as 0 to begin with
  voltage[0] = 0;
  voltage[1] = 0;
  voltage[2] = 0;
  voltage[3] = 0;
  // Call the function tickerFunction once every millisecond using the ticker
  periodicTicker.attach_ms(1, tickerFunction);
}

void tickerFunction(){
// Execute tasks when count reaches a value thsat gives the stated modulus to time all functions at a constant rate
  task_1();    
  if(executionsCount%200==0)
    task_2();
  if(executionsCount%1000==1)
    task_3();
  if(executionsCount%42==0)
    task_4();
  if(executionsCount%42==1)
    task_5();
  if(executionsCount%100==0)
    task_6();
  if(executionsCount%333==0)
    task_7();
  if(executionsCount%333==1)
    task_8();
  if(executionsCount%500==0)
    task_9();
  executionsCount++;
  Serial.println(executionsCount);
}

void task_1(){
  digitalWrite(wDWaveform, HIGH);
  delayMicroseconds(50);
  digitalWrite(wDWaveform, LOW);
//  Serial.println("Performed Task 1");
}

void task_2(){
  buttonValue = digitalRead(buttonInput);
  Serial.println(buttonValue);
//  Serial.println("Performed Task 2");
}

void task_3(){
   float high;
   high = pulseIn(squareWave, LOW);
   squareFreq = 1000000.0 / (high * 2);
   Serial.print("sad face");
//   Serial.println("Performed Task 3);
}

void task_4(){
  
  for (int i = 0; i < 4; i++){
    voltage[i] = analogRead(potInput);
    potValue = analogRead(potInput);
    voltValue = (3.3/4095) * potValue;
    voltage[i - 1] = voltage [i];
  }
    Serial.print(voltValue);
//  Serial.print("Performed Task 4");
}

void task_5(){
  for (int i = 0; i < 4; i++){
    sum += voltage[i];
    average = (sum / 4);
//    Serial.println("Performed Task 5");
  }
  Serial.print((3.3/4095) * average);
  sum = 0;
}

void task_6(){
  for(int i=0;i<1000; i++){
    __asm__ __volatile__ ("nop");
  }
//  Serial.println("Performed Task 6");
}

void task_7(){
  float average_analogue_in = average;
  if (average_analogue_in > (3.3/2))
    error_code = 1;
  else
    error_code = 0;
//  Serial.println("Performed Task 7");
}

void task_8(){
  if(error_code=1)
    digitalWrite(ledOutput, HIGH);
  else
    digitalWrite(ledOutput, LOW);
//  Serial.println("Performed Task 8");
//  Serial.print(error_code);
}

void task_9(){
  Serial.println(buttonValue);
  Serial.println(squareFreq);
  Serial.println(potInput);
//  Serial.println("Performed Task 9");
}
 
void loop() {
}
