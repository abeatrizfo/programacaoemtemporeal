struct sensor {
  int deviceId;
  int measurementType;
  float value;
};

QueueHandle_t queue;

void task1(void* parameter) {
  for (int i = 0; i < 10; i++) {
    struct sensor lm35Sensor;
    lm35Sensor.deviceId = i;
    lm35Sensor.measurementType = 1;
    lm35Sensor.value = random(0, 100) / 10.0;

    xQueueSend(queue, &lm35Sensor, portMAX_DELAY);
  }

  vTaskDelete(NULL);
}

void task2(void* parameter) {
  for (int i = 0; i < 10; i++) {
    struct sensor ldrSensor;
    ldrSensor.deviceId = i;
    ldrSensor.measurementType = 2;
    ldrSensor.value = random(0, 1024);

    xQueueSend(queue, &ldrSensor, portMAX_DELAY);
  }

  vTaskDelete(NULL);
}

void setup() {
  Serial.begin(115200);
  queue = xQueueCreate(10, sizeof(struct sensor));

  if (queue == NULL) {
    Serial.println("Error creating the queue");
  }

  xTaskCreate(task1, "Task1", 1000, NULL, 1, NULL);
  xTaskCreate(task2, "Task2", 1000, NULL, 1, NULL);
}

void loop() {
    
}