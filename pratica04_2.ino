QueueHandle_t queue;

void task1(void* parameter) {
  int sum = 0;
  int element;
  
  for (int i = 0; i < 10; i++) {
    xQueueReceive(queue, &element, portMAX_DELAY);
    if (element % 2 != 0) {
      sum += element;
    }
  }

  Serial.print("Sum of odd numbers: ");
  Serial.println(sum);

  vTaskDelete(NULL);
}

void task2(void* parameter) {
  for (int i = 0; i <= 30; i++) {
    xQueueSend(queue, &i, portMAX_DELAY);
  }

  vTaskDelete(NULL);
}

void setup() {
  Serial.begin(115200);
  queue = xQueueCreate(10, sizeof(int));

  if (queue == NULL) {
    Serial.println("Error creating the queue");
  }

  xTaskCreate(task1, "Task1", 1000, NULL, 1, NULL);
  xTaskCreate(task2, "Task2", 1000, NULL, 1, NULL);
}

void loop() {
}