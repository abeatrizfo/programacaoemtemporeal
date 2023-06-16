#include <FreeRTOS.h>
#include <task.h>
#include <timers.h>

#define MOTOR1_LED_PIN 21
#define MOTOR2_LED_PIN 14
#define SUBMOTOR1_PIN 26
#define SUBMOTOR2_PIN 27
#define BUTTON_PIN 22
#define POTENTIOMETER_PIN 36

TaskHandle_t xTask1;
TimerHandle_t xTimer1, xTimer2;

void vTask1(void *pvParameters);
void callBackTimer1(TimerHandle_t xTimer);
void callBackTimer2(TimerHandle_t xTimer);

void setup() {
  Serial.begin(115200);
  pinMode(MOTOR1_LED_PIN, OUTPUT);
  pinMode(MOTOR2_LED_PIN, OUTPUT);
  pinMode(SUBMOTOR1_PIN, OUTPUT);
  pinMode(SUBMOTOR2_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  xTimer1 = xTimerCreate("TIMER1", pdMS_TO_TICKS(1000), pdTRUE, 0, callBackTimer1);
  xTimer2 = xTimerCreate("TIMER2", pdMS_TO_TICKS(10000), pdFALSE, 0, callBackTimer2); 

  xTaskCreate(vTask1, "TASK1", configMINIMAL_STACK_SIZE + 1024, NULL, 1, &xTask1);

  xTimerStart(xTimer1, 0);
}

void loop() {
  // Vazio
}

void vTask1(void *pvParameters) {
  uint8_t debouncingTime = 0;

  while (1) {
    if ((digitalRead(BUTTON_PIN) == LOW) && (xTimerIsTimerActive(xTimer2) == pdFALSE)) {
      debouncingTime++;
      if (debouncingTime >= 10) {
        debouncingTime = 0;
        int potentiometerValue = analogRead(POTENTIOMETER_PIN);
        int motorSet = (potentiometerValue < 512) ? 1 : 2;

        if (motorSet == 1) {
          digitalWrite(MOTOR1_LED_PIN, HIGH);
          digitalWrite(SUBMOTOR1_PIN, HIGH);
          xTimerStart(xTimer2, 0);
          xTimerStop(xTimer1, 0);
          Serial.println("Iniciando o Timer 2 (Conjunto 1)...");
        } else if (motorSet == 2) {
          digitalWrite(MOTOR2_LED_PIN, HIGH);
          digitalWrite(SUBMOTOR2_PIN, HIGH);
          xTimerStart(xTimer2, 0);
          xTimerStop(xTimer1, 0);
          Serial.println("Iniciando o Timer 2 (Conjunto 2)...");
        }
      }
    } else {
      debouncingTime = 0;
    }
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

void callBackTimer1(TimerHandle_t xTimer) {
  digitalWrite(MOTOR1_LED_PIN, !digitalRead(MOTOR1_LED_PIN));
  digitalWrite(SUBMOTOR1_PIN, !digitalRead(SUBMOTOR1_PIN));
  Serial.println("Timer 1 (Conjunto 1)...");
}

void callBackTimer2(TimerHandle_t xTimer) {
  digitalWrite(MOTOR1_LED_PIN, LOW);
  digitalWrite(MOTOR2_LED_PIN, LOW);
  digitalWrite(SUBMOTOR1_PIN, LOW);
  digitalWrite(SUBMOTOR2_PIN, LOW);
  Serial.println("Desligando o Timer 2...");
  xTimerStart(xTimer1, 0);
}