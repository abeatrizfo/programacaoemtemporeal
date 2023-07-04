#define MAX_MESSAGE_LENGTH 256

TaskHandle_t taskAHandle;
TaskHandle_t taskBHandle;
SemaphoreHandle_t messageSemaphore;
char* messageBuffer = NULL;

void taskA(void* parameter) {
  char inputBuffer[MAX_MESSAGE_LENGTH];
  int bufferIndex = 0;

  while (1) {
    if (Serial.available() > 0) {
      char receivedChar = Serial.read();

      if (receivedChar == '\n') {
        inputBuffer[bufferIndex] = '\0';

        size_t messageLength = strlen(inputBuffer);
        messageBuffer = (char*)pvPortMalloc((messageLength + 1) * sizeof(char));
        if (messageBuffer != NULL) {
          strcpy(messageBuffer, inputBuffer);

          xSemaphoreGive(messageSemaphore);
        }

        bufferIndex = 0;
      } else {
        inputBuffer[bufferIndex++] = receivedChar;
        if (bufferIndex >= MAX_MESSAGE_LENGTH) {
          bufferIndex = 0;
        }
      }
    }

    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

void taskB(void* parameter) {
  while (1) {
    if (xSemaphoreTake(messageSemaphore, portMAX_DELAY) == pdTRUE) {
      if (messageBuffer != NULL) {
        Serial.println(messageBuffer);

        vPortFree(messageBuffer);
        messageBuffer = NULL;
      }
    }
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("--- FreeRTOS Memory Demo ---");

  messageSemaphore = xSemaphoreCreateBinary();

  if (messageSemaphore == NULL) {
    Serial.println("Error creating semaphore");
    while (1);
  }

  xTaskCreatePinnedToCore(taskA, "TaskA", 2048, NULL, 1, &taskAHandle, 0);

  xTaskCreatePinnedToCore(taskB, "TaskB", 2048, NULL, 1, &taskBHandle, 1);

  vTaskDelete(NULL);
}

void loop() {

}