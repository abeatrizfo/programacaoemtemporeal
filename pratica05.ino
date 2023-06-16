#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

char c;
char buf[50];
int idx = 0;
static volatile uint8_t flag = 0;
int tam;

static const uint8_t buf_len = 255;

char b[50];
char e[50];
char d[50];

SemaphoreHandle_t mutex;

void ordenar(char *x) {
  int temp = 0;
  int t = tam - 1;
  for (int i = 0; i < t; i++) {
    for (int j = i + 1; j < t; j++) {
      if (x[i] > x[j]) {
        temp = x[i];
        x[i] = x[j];
        x[j] = temp;
      }
    }
  }
}

void receber(void *arg) {
  while (1) {
    while (Serial.available() > 0) {
      c = Serial.read();
      xSemaphoreTake(mutex, portMAX_DELAY);
      if (idx < 50 - 1) {
        buf[idx] = c;
        idx++;
      }
      if (c == '\n') {
        buf[idx - 1] = '\0';
        tam = idx;
        idx = 0;
        flag = 1;
        Serial.print("Enviando: ");
        for (int i = 0; i < tam; i++) {
          Serial.print(buf[i]);
        }
        Serial.println();
      }
      xSemaphoreGive(mutex);
    }
  }
}

void enviar(void *arg) {
  while (1) {
    if (flag == 1) {
      xSemaphoreTake(mutex, portMAX_DELAY);
      Serial.println("Recebendo: ");
      for (int i = 0; i < tam; i++) {
        Serial.print(b[i]);
      }
      Serial.println();
      Serial.println("Recebendo: ");
      for (int i = 0; i < tam; i++) {
        Serial.print(e[i]);
      }
      Serial.println();
      Serial.println("Recebendo: ");
      for (int i = 0; i < tam; i++) {
        Serial.print(d[i]);
      }
      Serial.println();
      flag = 0;
      xSemaphoreGive(mutex);
    }
  }
}

void separacao(void *arg) {
  while (1) {
    if (flag == 1) {
      xSemaphoreTake(mutex, portMAX_DELAY);
      for (int i = 0; i < 50; i++) {
        if (buf[i] >= 65 && buf[i] <= 90) {
          b[i] = buf[i];
        }
        if (buf[i] >= 97 && buf[i] <= 122) {
          e[i] = buf[i];
        }
        if (buf[i] >= 48 && buf[i] <= 57) {
          d[i] = buf[i];
        }
      }
      xSemaphoreGive(mutex);
    }
  }
}

void ordenacao(void *arg) {
  while (1) {
    xSemaphoreTake(mutex, portMAX_DELAY);
    ordenar(b);
    ordenar(e);
    ordenar(d);
    xSemaphoreGive(mutex);
  }
}

void setup() {
  Serial.begin(115200);
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  mutex = xSemaphoreCreateMutex();
  xTaskCreatePinnedToCore(enviar, "enviar", 1024, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(receber, "Receber", 1024, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(separacao, "Separacao", 1024, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(ordenacao, "Ordenacao", 1024, NULL, 1, NULL, 1);
}

void loop() {
}