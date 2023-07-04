#define botao1 2 // Botões de interrupção
#define botao2 3
#define led1 4
#define led2 5

TaskHandle_t contadorTaskHandle;

void contadorTask(void* parameter) {
  int count = 0;

  while (1) {
    Serial.print("Contador: ");
    Serial.println(count);

    count++;

    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void setup() {
  pinMode(botao1, INPUT_PULLUP);
  pinMode(botao2, INPUT_PULLUP);

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);

  Serial.begin(115200);

  xTaskCreatePinnedToCore(contadorTask, "ContadorTask", 1024, NULL, configMAX_PRIORITIES - 1, &contadorTaskHandle, 1);
}

void loop() {

}