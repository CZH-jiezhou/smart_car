#include "key.h"


/*��д�жϻص�������*/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	switch (GPIO_Pin)
	{
	case GPIO_PIN_3://key1
		kp_tracking++;
		break;
	case GPIO_PIN_4://key2
		stop = 0;

		break;
	case GPIO_PIN_5://key3
		stop = 1;
		 


		break;
	}
}
