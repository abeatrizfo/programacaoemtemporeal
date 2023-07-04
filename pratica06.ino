#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <semphr.h>

int currentSignal = 0;
bool pedestrianRequested = false;

TaskHandle_t vehicleTask;
TaskHandle_t pedestrianTask;
SemaphoreHandle_t pedestrianSemaphore;
SemaphoreHandle_t signalMutex;

void vehicleTaskFunction(void* parameter);
void pedestrianTaskFunction(void* parameter);
void setVehicleSignal();
void setPedestrianSignal();

void setup() {
  Serial.begin(115200);

  pedestrianSemaphore = xSemaphoreCreateBinary();
  signalMutex = xSemaphoreCreateMutex();

  xTaskCreate(vehicleTaskFunction, "VehicleTask", 1000, NULL, 1, &vehicleTask);
  xTaskCreate(pedestrianTaskFunction, "PedestrianTask", 1000, NULL, 1, &pedestrianTask);
}

void loop() {
  
}

void vehicleTaskFunction(void* parameter) {
  while (1) {
    if (Serial.available() > 0) {
      char command = Serial.read();

      if (command == 'P') {
        xSemaphoreGive(pedestrianSemaphore);
      }
    }

    delay(100);
  }
}

void pedestrianTaskFunction(void* parameter) {
  while (1) {
    xSemaphoreTake(pedestrianSemaphore, portMAX_DELAY);

    xSemaphoreTake(signalMutex, portMAX_DELAY);

    if (currentSignal == 0) {
      setPedestrianSignal();
    }

    xSemaphoreGive(signalMutex);
  }
}

void setVehicleSignal() {
  xSemaphoreTake(signalMutex, portMAX_DELAY);

  Serial.println("Sinal para veículos: Vermelho");

  currentSignal = 0;

  xSemaphoreGive(signalMutex);
}

void setPedestrianSignal() {
  xSemaphoreTake(signalMutex, portMAX_DELAY);

  Serial.println("Sinal para pedestres: Verde");

  currentSignal = 1;

  xSemaphoreGive(signalMutex);

  delay(5000);

  setVehicleSignal();

  pedestrianRequested = false;
}
