#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "freertos/queue.h"

TaskHandle_t myTaskHandle = NULL;
TaskHandle_t myTaskHandle2 = NULL;
QueueHandle_t queue;

int messageIndex = 1;
int maxMessages = 5;

void Demo_Task(void *arg)
{
    char txBuffer[50];
    queue = xQueueCreate(5, sizeof(txBuffer)); 
    if (queue == 0)
    {
        printf("Failed to create queue = %p\n", queue);
    }

    while (1) {
        sprintf(txBuffer, "Hello from Demo_Task %d", messageIndex++);
        xQueueSend(queue, (void*)txBuffer, (TickType_t)0);

        if (messageIndex > maxMessages) {
            messageIndex = 1;
        }

        vTaskDelay(1000/ portTICK_RATE_MS);
    }
}

void Demo_Task2(void *arg)
{
    char rxBuffer[50];
    while(1){
        if (xQueueReceive(queue, &(rxBuffer), (TickType_t)5))
        {
            printf("Received data from queue == %s\n", rxBuffer);
            vTaskDelay(1000/ portTICK_RATE_MS);
        }
    }
}

void setup(void)
{
    Serial.begin(115200);
    Serial.println("teste");
    xTaskCreate(Demo_Task, "Demo_Task", 4096, NULL, 10, &myTaskHandle);
    xTaskCreatePinnedToCore(Demo_Task2, "Demo_Task2", 4096, NULL, 10, &myTaskHandle2, 1);
}

void loop(){

}