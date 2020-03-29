#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
void PortF_Init(void);

static void vTask1( void *pvParameters );
static void vTask2( void *pvParameters );
static void vTask3( void *pvParameters );
static void vTask4( void *pvParameters );
static void vTask5( void *pvParameters );

xTaskHandle xTask2Handle;
xTaskHandle xTask1Handle;
xTaskHandle xTask4Handle;
xTaskHandle xTask5Handle;
xTaskHandle xTask3Handle;

int state = 0 ; 
int isPressed = 0; 
int setmax = 0; 

int main(void){    
  PortF_Init();        // Call initialization of port PF3, PF2, PF1    
	xTaskCreate( vTask1, (const portCHAR *)"Task1", configMINIMAL_STACK_SIZE, NULL, 3, &xTask1Handle );
	xTaskCreate( vTask2, (const portCHAR *)"Task2", configMINIMAL_STACK_SIZE, NULL, 4, &xTask2Handle );
	xTaskCreate( vTask4, (const portCHAR *)"Task4", configMINIMAL_STACK_SIZE, NULL, 2, &xTask4Handle);
	xTaskCreate( vTask3, (const portCHAR *)"Task3", configMINIMAL_STACK_SIZE, NULL, 5, &xTask3Handle);

		/* Start the scheduler. */
	vTaskStartScheduler();

}

void vApplicationIdleHook(void){
	GPIO_PORTF_DATA_R = 0x0E;       // LED is white
}
//-----------------------------------------------------------------MASTER-----------------------------------------------------------------------------
static void vTask3( void *pvParameters )
{
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	for( ;; )
	{
		if(setmax==1){
			vTaskDelayUntil( &xLastWakeTime, (12000));	
			setmax=0;
		xTaskCreate( vTask1, (const portCHAR *)"Task1", configMINIMAL_STACK_SIZE, NULL, 3, &xTask1Handle );
		xTaskCreate( vTask2, (const portCHAR *)"Task2", configMINIMAL_STACK_SIZE, NULL, 4, &xTask2Handle );
		xTaskCreate( vTask4, (const portCHAR *)"Task4", configMINIMAL_STACK_SIZE, NULL, 2, &xTask4Handle);
			
		}
	 if(isPressed==1){
		 
			isPressed=0 ;	
			vTaskPrioritySet(xTask4Handle,4);
			vTaskDelayUntil( &xLastWakeTime, (15000));
			vTaskPrioritySet(xTask4Handle,1);
		}		
	 
		vTaskPrioritySet(xTask2Handle, 2);
		vTaskPrioritySet(xTask1Handle, 3);
		vTaskDelayUntil( &xLastWakeTime, (5000));
		
		 if(isPressed==1){
			 
			isPressed=0 ;	
			vTaskPrioritySet(xTask4Handle,4);
			vTaskDelayUntil( &xLastWakeTime, (15000));
			vTaskPrioritySet(xTask4Handle,1);
		}
		 
		vTaskPrioritySet(xTask1Handle, 2);
		vTaskPrioritySet(xTask2Handle, 3);
		vTaskDelayUntil( &xLastWakeTime, (10000));
			
	}
}
//-----------------------------------------------------------------RED-----------------------------------------------------------------------------
static void vTask2( void *pvParameters )
{
	for( ;; )
	{
    GPIO_PORTF_DATA_R = 0x02;       // LED is RED
		
		state = GPIO_PORTF_DATA_R & 0x11;
    if(state == 0x10) //sw2  on right to reset
       {
				isPressed = 1; 
       }
		else if (state == 0x01) {
			xTaskCreate( vTask5, (const portCHAR *)"Task5", configMINIMAL_STACK_SIZE, NULL, 6, &xTask5Handle );	
			 }
	}
}
//-----------------------------------------------------------------GREEN-----------------------------------------------------------------------------
static void vTask4 ( void *pvParameters )
{
	for( ;; )
	{
   GPIO_PORTF_DATA_R = 0x08;       // LED is Green
	state = GPIO_PORTF_DATA_R & 0x11;
		 if (state == 0x01) {
			 	xTaskCreate( vTask5, (const portCHAR *)"Task5", configMINIMAL_STACK_SIZE, NULL, 6, &xTask5Handle );	
			
	}
}
}
//-----------------------------------------------------------------WHITE----------------------------------------------------------------------------
static void vTask5 ( void *pvParameters )
{
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	
	for( ;; )
	{
	
		vTaskDelete(xTask4Handle);
		vTaskDelete(xTask2Handle);
		vTaskDelete(xTask1Handle);	
		vTaskDelete(xTask3Handle);
		setmax =1 ;
		xTaskCreate( vTask3, (const portCHAR *)"Task3", configMINIMAL_STACK_SIZE, NULL, 5, &xTask3Handle);
		vTaskDelete(NULL);
	

	}
	
}
//-----------------------------------------------------------------BLUE-----------------------------------------------------------------------------
static void vTask1( void *pvParameters )
{
	for( ;; )
	{
    GPIO_PORTF_DATA_R = 0x04;       // LED is Blue
		
		state = GPIO_PORTF_DATA_R & 0x11;
    if(state == 0x10) //sw2  on right to reset
       {
				isPressed = 1 ; 
       }
			 	else if (state == 0x01) {
			xTaskCreate( vTask5, (const portCHAR *)"Task5", configMINIMAL_STACK_SIZE, NULL, 6, &xTask5Handle );	
		
			 }
	}
}

void PortF_Init(void){ 
        
  SYSCTL_RCGCGPIO_R |= 0x00000020; // activate clock for port F
	GPIO_PORTF_LOCK_R   =       0x4c4f434b;
  GPIO_PORTF_CR_R     =       0x11;
  GPIO_PORTF_DIR_R |= 0x0E; 
	GPIO_PORTF_DIR_R &= ~0x11;	
	GPIO_PORTF_PUR_R |= 0X11;
  GPIO_PORTF_DEN_R |= 0x1f;     // enable digital I/O on PF3-PF1 
  GPIO_PORTF_AMSEL_R = 0x00;        // 3) disable analog function
  GPIO_PORTF_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL  
  GPIO_PORTF_AFSEL_R = 0x00;        // 6) no alternate function	
}

