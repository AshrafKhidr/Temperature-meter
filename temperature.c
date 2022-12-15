
#include "application.h"

uint16 temperatue;

int main() {
   Std_ReturnType ret = E_NOT_OK;
   
   adc_config_t adc1 = {
        .ADC_InterruptHandler = NULL,
        .acquisition_time = ADC_12_TAD,
        .adc_channel = ADC_CHANNEL_AN0,
        .conversion_clock = ADC_CONVERSION_CLOCK_FOSC_DIV_16,
        .result_format = ADC_RESULT_RIGHT,
        .volt_reference = ADC_VOLT_REFERENCE_DISABLED
    };
   
   pin_config_t heater = {
     .direction = GPIO_DIRECTION_OUTPUT,
     .logic = GPIO_LOW,
     .port = PORTD_INDEX,
     .pin = GPIO_PIN1
   };
   pin_config_t conditioner = {
     .direction = GPIO_DIRECTION_OUTPUT,
     .logic = GPIO_LOW,
     .port = PORTD_INDEX,
     .pin = GPIO_PIN0
   };
   
   chr_lcd_8bit_t lcd_user = {
      .lcd_en.direction = GPIO_DIRECTION_OUTPUT ,
      .lcd_en.logic = GPIO_LOW , 
      .lcd_en.port = PORTB_INDEX , 
      .lcd_en.pin = GPIO_PIN0,
      
      .lcd_rs.direction = GPIO_DIRECTION_OUTPUT , 
      .lcd_rs.logic = GPIO_LOW , 
      .lcd_rs.port = PORTB_INDEX , 
      .lcd_rs.pin = GPIO_PIN1,
      
      .lcd_data[0].direction = GPIO_DIRECTION_OUTPUT , .lcd_data[0].logic = GPIO_LOW , .lcd_data[0].port = PORTD_INDEX , .lcd_data[0].pin = GPIO_PIN2,
      .lcd_data[1].direction = GPIO_DIRECTION_OUTPUT , .lcd_data[1].logic = GPIO_LOW , .lcd_data[1].port = PORTA_INDEX , .lcd_data[1].pin = GPIO_PIN1,
      .lcd_data[2].direction = GPIO_DIRECTION_OUTPUT , .lcd_data[2].logic = GPIO_LOW , .lcd_data[2].port = PORTA_INDEX , .lcd_data[2].pin = GPIO_PIN2,
      .lcd_data[3].direction = GPIO_DIRECTION_OUTPUT , .lcd_data[3].logic = GPIO_LOW , .lcd_data[3].port = PORTA_INDEX , .lcd_data[3].pin = GPIO_PIN3,
      .lcd_data[4].direction = GPIO_DIRECTION_OUTPUT , .lcd_data[4].logic = GPIO_LOW , .lcd_data[4].port = PORTC_INDEX , .lcd_data[4].pin = GPIO_PIN7,
      .lcd_data[5].direction = GPIO_DIRECTION_OUTPUT , .lcd_data[5].logic = GPIO_LOW , .lcd_data[5].port = PORTA_INDEX , .lcd_data[5].pin = GPIO_PIN5,
      .lcd_data[6].direction = GPIO_DIRECTION_OUTPUT , .lcd_data[6].logic = GPIO_LOW , .lcd_data[6].port = PORTB_INDEX , .lcd_data[6].pin = GPIO_PIN2,
      .lcd_data[7].direction = GPIO_DIRECTION_OUTPUT , .lcd_data[7].logic = GPIO_LOW , .lcd_data[7].port = PORTB_INDEX , .lcd_data[7].pin = GPIO_PIN3,
    };
    
    gpio_pin_initialize(&heater);
    gpio_pin_initialize(&conditioner);
    adc_init(&adc1);
    lcd_8bit_initialize(&lcd_user);
    ret = lcd_8bit_send_string(&lcd_user , "temperature=");
    
   while(1){
       ret = adc_convert_from_channel_blocking(&adc1 , ADC_CHANNEL_AN0 , &temperatue );
       temperatue = temperatue * 4.88f;
       temperatue /= 10; 
       
		if(temperatue<10){   
            lcd_8bit_send_char_data_pos(&lcd_user , 1 , 13 , 30); //sending 0
            lcd_8bit_send_char_data(&lcd_user , (temperatue%10)+48);
            lcd_8bit_send_char_data(&lcd_user , 0xDF); // sending degree symbol
            lcd_8bit_send_char_data(&lcd_user , 'c');	
		}
		else if( temperatue<100){
            lcd_8bit_send_char_data_pos(&lcd_user , 1 , 13 , (temperatue/10)+48); //sending 0
            lcd_8bit_send_char_data(&lcd_user , (temperatue%10)+48);
            lcd_8bit_send_char_data(&lcd_user , 0xDF); // sending degree symbol
            lcd_8bit_send_char_data(&lcd_user , 'c');
			if(temperatue<20)
			{
				gpio_pin_write_logic(&heater , GPIO_HIGH);
			}
			else if (temperatue>30)
			{
				gpio_pin_write_logic(&conditioner , GPIO_HIGH);
			}
			else
			{
				gpio_pin_write_logic(&heater , GPIO_LOW);
				gpio_pin_write_logic(&conditioner , GPIO_LOW);
			}
		}		
		else
		{
		
		}
        
   }

    return (EXIT_SUCCESS);
}

