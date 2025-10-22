/*! @mainpage Proyecto Integrador
 *
 * \section genDesc General Description
 *
 * Proyecto integrador de la cátedra Electrónica Programable en la que se realiza un prototipado de una 
 * luz de dentista que sea ajustable automaticamente en intensidad y apertura de iris de luz dependiendo
 * de la distancia al paciente
 *
 * \section hardConn Hardware Connection
 *
 * |   	MRFC522		|   ESP-EDU		|
 * |:--------------:|:--------------|
 * | 	SDO/MISO 	|	GPIO_22		|
 * | 	3V3		 	| 	3V3			|
 * | 	SCK		 	| 	GPIO_20		|
 * | 	SDI/MOSI 	| 	GPIO_21		|
 * | 	RESET	 	| 	GPIO_18		|
 * | 	CS		 	| 	GPIO_9		|
 * | 	GND		 	| 	GND			|
 *
 * @section changelog Changelog
 *
 * |   Date	    | Description                                    |
 * |:----------:|:-----------------------------------------------|
 * | 22/10/2025 | Document creation		                         |
 *
 * @author Gastón Jair Díaz (diazgastonj@gmail.com)
 * @author Juan Ignacio Oliva (juaaaaanioliiiva@gmail.com)
 */

/*==================[inclusions]=============================================*/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led.h"
#include "uart_mcu.h"
#include "rfid_utils.h"
#include "hc_sr04.h"
#include "timer_mcu.h"
/*==================[macros and definitions]=================================*/
#define CONFIG_BLINK_PERIOD_US 1000 * 1000
/*==================[internal data definition]===============================*/
unsigned int last_user_ID;
// RFID structs
MFRC522Ptr_t mfrcInstance;
/*==================[internal functions declaration]=========================*/
uint16_t M;
bool act_Med = false;

void FuncTimerA(void* param){
    vTaskNotifyGiveFromISR(medir_task_handle, pdFALSE);    /* Envía una notificación a la tarea Medir asociada al sensor */
	
}

static void Medir(void *pvParameter)
{
    while (true)
    {
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);    /* La tarea espera en este punto hasta recibir una notificación */
        if (act_Med == true)
            M = HcSr04ReadDistanceInCentimeters(); // funcion del ultrasonido que mide

    }
}

static void Regular_intensidad_luz(void *pvParameter){

	

}
/**
 * Executed every time the card reader detects a user in
 */
/* void userTapIn() {

//	show card UID
	UartSendString(UART_PC,"\nCard uid bytes: ");
	for (uint8_t i = 0; i < mfrcInstance->uid.size; i++) {
		UartSendString(UART_PC," 0X");
		UartSendString(UART_PC, (char*)UartItoa(mfrcInstance->uid.uidByte[i], 16));
		UartSendString(UART_PC," ");
	}
	UartSendString(UART_PC,"\n\r");
	// Convert the uid bytes to an integer, byte[0] is the MSB
	last_user_ID =
		(int)mfrcInstance->uid.uidByte[3] |
		(int)mfrcInstance->uid.uidByte[2] << 8 |
		(int)mfrcInstance->uid.uidByte[1] << 16 |
		(int)mfrcInstance->uid.uidByte[0] << 24;

	UartSendString(UART_PC,"Card Read user ID: ");
	UartSendString(UART_PC, (char*)UartItoa(last_user_ID, 10));
	UartSendString(UART_PC,"\n\r");


} */
/*==================[external functions definition]==========================*/
void app_main(void){

	HcSr04Init(GPIO_3, GPIO_2); // trigger : pulso cuando dispara el sonido //Echo pulso que recibe

	 /* Inicialización de timers */
    timer_config_t timer_led_1 = {
        .timer = TIMER_A,
        .period = CONFIG_BLINK_PERIOD_US,
        .func_p = FuncTimerA,
        .param_p = NULL
    };
    
	TimerInit(&timer_led_1);
	xTaskCreate(&Medir, "MEDIR", 512, NULL, 5, &medir_task_handle);
	 
	/* Inicialización del conteo de timers */
    TimerStart(timer_led_1.timer);
/* 	LedsInit();
	serial_config_t UART_USB;
	UART_USB.baud_rate = 115200;
	UART_USB.port = UART_PC;
	UartInit(&UART_USB);
	setupRFID(&mfrcInstance);

	UartSendString(UART_PC,"Init MRFC522 test.\r\n"); */
	/*
    while(true){
		UartSendString(UART_PC,"Reading... \r\n");
		if (PICC_IsNewCardPresent(mfrcInstance)) {
			if (PICC_ReadCardSerial(mfrcInstance)) {
				LedOn(LED_1);
				userTapIn();
				LedOff(LED_1);
			}
		}
		vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
	}
		*/
}
/*==================[end of file]============================================*/
