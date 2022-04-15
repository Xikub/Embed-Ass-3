/* Code by Colin Tipton H00300274
 * 
 *  Exercise to use RTOS to control timing for an ESP32 microcontroller
 *  
 * 
 * 
 */

// Define the Pin names to be used later in code
//#define wDWaveform 22
#define ledOutput 16 
#define buttonInput 23
#define potInput 2
#define squareWave 15

// Initial settin of variables
volatile int executionsCount = 0;
int potValue;
int voltage[4];
int error_code = 0;
int buttonValue = 0;
int hertz;
int led = 0;
float squareFreq;
float average;
float sum = 0.0;
float voltValue = 0.0;
float averageVolts;
 
void setup() {
 
  Serial.begin(19200);
  // Set pin modes for initialisation as input/output and what output to start at
  pinMode(ledOutput, OUTPUT); 
  pinMode(buttonInput, INPUT);
  pinMode(potInput, INPUT);
  pinMode(squareWave, INPUT);
  digitalWrite(ledOutput, LOW);
//  digitalWrite(wDWaveform, LOW);

  // Create the list of tasks for RTOS to control timing
  xTaskCreate(MyTask1, "Task1", 1000, NULL, 1, NULL);
  xTaskCreate(MyTask2, "Task2", 1000, NULL, 1, NULL);
  xTaskCreate(MyTask3, "Task3", 1000, NULL, 1, NULL);
  xTaskCreate(MyTask4, "Task4", 1000, NULL, 1, NULL);
  xTaskCreate(MyTask5, "Task5", 1000, NULL, 1, NULL);
  xTaskCreate(MyTask6, "Task6", 1000, NULL, 1, NULL);
  xTaskCreate(MyTask7, "Task7", 1000, NULL, 1, NULL);
  xTaskCreate(MyTask8, "Task8", 1000, NULL, 1, NULL);
  xTaskCreate(MyTask9, "Task9", 1000, NULL, 1, NULL);
  xTaskCreate(MyTask10, "Task10", 1000, NULL, 1, NULL);

  // Define all array values as 0 to begin with
  voltage[0] = 0;
  voltage[1] = 0;
  voltage[2] = 0;
  voltage[3] = 0;
}

static void MyTask1(void* pvParameters){
   (void) pvParameters;
   for(;;){
//    Serial.println("Task 1");
//    digitalWrite(wDWaveform, HIGH);
//    delayMicroseconds(50);
//    digitalWrite(wDWaveform, LOW);
    vTaskDelay(200);
  }
}

static void MyTask2(void* pvParameters){
  (void) pvParameters;
   for(;;){
//    Serial.println("Task 2");
    buttonValue = digitalRead(buttonInput);
//    Serial.println(buttonValue);
    vTaskDelay(200);
  }
}

static void MyTask3(void* pvParameters){
  (void) pvParameters;
   for(;;){
//    Serial.println("Task 3");
//    squareFreq = pulseIn(squareWave);
    squareFreq = ((squareFreq /4095) * 3.3);
//    Serial.println(squareFreq);
    vTaskDelay(995);
  }
}

static void MyTask4(void* pvParameters){
  (void) pvParameters;
   for(;;){
//    Serial.println("Task 4");
    potValue = analogRead(potInput);
    voltValue = (3.3/4095) * potValue;
    for (int i = 1; i < 4; i++){
      voltage[i - 1] = voltage [i];
    }
    voltage[3] = voltValue;
    
//    Serial.println(voltValue);
    vTaskDelay(42);
  }
}

static void MyTask5(void* pvParameters){
  (void) pvParameters;
   for(;;){
//    Serial.println("Task 5");
    for (int i = 0; i < 4; i++){
      sum += voltage[i];
      average = (sum / 4);
      averageVolts = ((3.3/4095) * average);
  }
//    Serial.println(averageVolts);
    sum = 0;
    vTaskDelay(42);
  }
}

static void MyTask6(void* pvParameters){
  (void) pvParameters;
   for(;;){
//    Serial.println("Task 6");
    for(int i=0;i<1000; i++){
      __asm__ __volatile__ ("nop");
  }
    vTaskDelay(100);
  }
}

static void MyTask7(void* pvParameters){
  (void) pvParameters;
   for(;;){
//    Serial.print("Task 7  ");
    
    float average_analogue_in = average;
//    Serial.println(average_analogue_in);
    if (average_analogue_in >= 3.3/2)
      error_code = 1;
    else
      error_code = 0;
    
//    Serial.print("Error Code ");
//    Serial.println(error_code);
    vTaskDelay(333);
  }
}

static void MyTask8(void* pvParameters){
  (void) pvParameters;
   for(;;){
//    Serial.println("Task 8");
//    Serial.println(error_code);
    
    if(error_code == 0)
      digitalWrite(ledOutput, LOW);
    else
      digitalWrite(ledOutput, HIGH);
    led = digitalRead(ledOutput);
//    Serial.print("LED state ");
//    Serial.println(led);
    vTaskDelay(333);
  }
}

static void MyTask9(void* pvParameters){
  (void) pvParameters;
   for(;;){
    Serial.println("Task 9");
//    Serial.println(buttonValue);
//    Serial.println(squareFreq);
//    Serial.println(potInput);
    vTaskDelay(5000);
  }
}

static void MyTask10(void* pvParameters){
  (void) pvParameters;
   for(;;){
    if (buttonValue == 1){
      Serial.println(buttonValue);
      Serial.println(squareFreq);
      Serial.println(potInput);
//      Serial.println("Task 10");
    }
  }
} 
void loop() {
}
