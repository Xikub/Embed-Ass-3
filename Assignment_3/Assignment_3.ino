/* Code by Colin Tipton H00300274
 * 
 *  Exercise to use RTOS to control timing for an ESP32 microcontroller
 *  
 *======================= Task 1 =======================
 * Output a digital watchdog waveform with length and 
 * period of the "normal" operation of signal B from
 * assignment 1
 *======================= Task 2 =======================
 * Monitor one digital input connected to a button,
 * inline with a 3v3 signal
 *======================= Task 3 =======================
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 */

// Define the Pin names to be used later in code
#define wDWaveform 22
#define ledOutput 16 
#define buttonInput 23
#define potInput 2
#define squareWave 15

// Prototype of tasks
void task_1(void *pvParameters);
void task_2(void *pvParameters);
void task_3(void *pvParameters);
void task_4(void *pvParameters);
void task_5(void *pvParameters);
void task_6(void *pvParameters);
void task_7(void *pvParameters);
void task_8(void *pvParameters);
void task_9(void *pvParameters);

// Initial settin of variables
//volatile int executionsCount = 0;
int potValue;
float voltage[4];
int error_code = 0;
//int buttonValue = 0;
int hertz;
int led = 0;
float squareFreq;
float average;
float sum = 0.0;
float voltValue = 0.0;
float averageVolts;
static QueueHandle_t queue;
static QueueHandle_t queueVolt;
static SemaphoreHandle_t dataSemaphore;

bool buttonValue;

struct taskSemaphore{
  bool button = false;
  float frequencyIn;
  float averageVoltage;
};
taskSemaphore taskData;

 
void setup() {
 
  Serial.begin(19200);
  // Set pin modes for initialisation as input/output and what output to start at
  pinMode(ledOutput, OUTPUT);
  pinMode(wDWaveform, OUTPUT); 
  pinMode(buttonInput, INPUT);
  pinMode(potInput, INPUT);
  pinMode(squareWave, INPUT);
  digitalWrite(ledOutput, LOW);
//  digitalWrite(wDWaveform, LOW);

  // Create queue for tasks 5 and 7
  queue = xQueueCreate(1, sizeof(float));
  queueVolt = xQueueCreate(1, sizeof(float));

  // Create semaphore/flag to hold data safely
  dataSemaphore = xSemaphoreCreateMutex();

  // Create the list of tasks for RTOS to control timing
  xTaskCreate(MyTask1, "Task1", 1000, NULL, 3, NULL);
  xTaskCreate(MyTask2, "Task2", 1000, NULL, 2, NULL);
  xTaskCreate(MyTask3, "Task3", 1000, NULL, 2, NULL);
  xTaskCreate(MyTask4, "Task4", 1000, NULL, 3, NULL);
  xTaskCreate(MyTask5, "Task5", 1000, NULL, 1, NULL);
  xTaskCreate(MyTask6, "Task6", 1000, NULL, 2, NULL);
  xTaskCreate(MyTask7, "Task7", 1000, NULL, 3, NULL);
  xTaskCreate(MyTask8, "Task8", 1000, NULL, 2, NULL);
  xTaskCreate(MyTask9, "Task9", 1000, NULL, 3, NULL);
  xTaskCreate(MyTask10, "Task10", 1000, NULL, 3, NULL);

}

static void MyTask1(void* pvParameters){
   (void) pvParameters;
   for(;;){
  // Define all array values as 0 to begin with

//    Serial.println("Task 1");
    digitalWrite(wDWaveform, HIGH);
    delayMicroseconds(50);
    digitalWrite(wDWaveform, LOW);
    vTaskDelay(200);
  }
}

static void MyTask2(void* pvParameters){
  (void) pvParameters;
   for(;;){  
//  Serial.println("Task 2");
    if(xSemaphoreTake(dataSemaphore, portMAX_DELAY) == pdTRUE){
      taskData.button = digitalRead(buttonInput);
      buttonValue = taskData.button;
      xSemaphoreGive(dataSemaphore);
    }
    vTaskDelay(200);
  }
}

static void MyTask3(void* pvParameters){
  (void) pvParameters;
  for(;;){
    if(xSemaphoreTake(dataSemaphore, portMAX_DELAY) == pdTRUE){
      squareFreq = pulseIn(squareWave, HIGH);
      taskData.frequencyIn = 1000000.0/(squareFreq*2);
      xSemaphoreGive(dataSemaphore);
    }
//    Serial.println("Task 3");
//    Serial.println(taskData.frequencyIn);
    vTaskDelay(995);
  }
}

static void MyTask4(void* pvParameters){
  (void) pvParameters;  
  for(;;){
    float x = 0;
    voltage[0] = 0;
    voltage[1] = 0;
    voltage[2] = 0;
    voltage[3] = 0;
    x = analogRead(potInput);
    xQueueSend(queue, &x, 100);
    vTaskDelay(42);
  }
}

static void MyTask5(void* pvParameters){
  taskData.averageVoltage = 0;
  (void) pvParameters;
//  Serial.println("Task 5");
   for(;;){
    float c = 0;
    if(xQueueReceive(queue, &c, 100)){
      for (int i = 1; i < 4; i++){
        voltage[i - 1] = voltage [i];
      }
      voltage[3] = c * (3.3/4095);
//      Serial.print("v3: ");
//      Serial.println(voltage[3]);       
    }

    if(xSemaphoreTake(dataSemaphore, portMAX_DELAY) == pdTRUE){
      for (int i = 1; i < 4; i++){
        taskData.averageVoltage = taskData.averageVoltage + voltage[i];
      }
      taskData.averageVoltage = taskData.averageVoltage / 4;
      c = taskData.averageVoltage;
      xQueueSend(queueVolt, &c, 100);
      xSemaphoreGive(dataSemaphore); 
    }
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
//    Serial.println("Task 7");
    float c = 0;
    if(xQueueReceive(queueVolt, &c, 100)){
      if(c > (3.3 / 2)){
        error_code = 1;
      }
      else{
        error_code = 0;
      }
    }
//    Serial.println(error_code);
    vTaskDelay(333);
  }
}

static void MyTask8(void* pvParameters){
  (void) pvParameters;
   for(;;){
//    Serial.println("Task 8");
    digitalWrite(ledOutput, error_code);
    vTaskDelay(333);
  }
}

static void MyTask9(void* pvParameters){
  (void) pvParameters;
   for(;;){
//    Serial.println("Task 9");
    Serial.println(taskData.button);
    Serial.println(taskData.frequencyIn);
    Serial.println(taskData.averageVoltage * 3);
    vTaskDelay(5000);
  }
}

static void MyTask10(void* pvParameters){
  (void) pvParameters;
   for(;;){
    if(xSemaphoreTake(dataSemaphore, portMAX_DELAY) == pdTRUE){   
      if(buttonValue == true) {
        Serial.println(taskData.button);
        Serial.println(taskData.frequencyIn);
        Serial.println(taskData.averageVoltage * 3);
      }
      xSemaphoreGive(dataSemaphore);
    }   
    vTaskDelay(100); 
  }
} 
void loop() {
}
