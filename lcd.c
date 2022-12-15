#include "ecu_chr_lcd.h"

static Std_ReturnType lcd_send_4bits(const chr_lcd_4bit_t * , uint8 );
static Std_ReturnType lcd_4bit_send_enable(const chr_lcd_4bit_t *);
static Std_ReturnType lcd_8bit_send_enable(const chr_lcd_8bit_t *);
static Std_ReturnType lcd_4bit_set_cursor(const chr_lcd_4bit_t * , uint8  , uint8 );
static Std_ReturnType lcd_8bit_set_cursor(const chr_lcd_8bit_t * , uint8  , uint8 );

static Std_ReturnType lcd_send_4bits(const chr_lcd_4bit_t *lcd , uint8 data_command){
    Std_ReturnType ret = E_OK;
    ret = gpio_pin_write_logic(&(lcd->lcd_data[0]) , (data_command >> 0) & (uint8)0x01);
    ret = gpio_pin_write_logic(&(lcd->lcd_data[1]) , (data_command >> 1) & (uint8)0x01);
    ret = gpio_pin_write_logic(&(lcd->lcd_data[2]) , (data_command >> 2) & (uint8)0x01);
    ret = gpio_pin_write_logic(&(lcd->lcd_data[3]) , (data_command >> 3) & (uint8)0x01);

    return ret;
}

static Std_ReturnType lcd_4bit_send_enable(const chr_lcd_4bit_t *lcd){
    Std_ReturnType ret = E_OK;
    ret = gpio_pin_write_logic(&(lcd->lcd_en), GPIO_HIGH);
    __delay_us(5);
    ret = gpio_pin_write_logic(&(lcd->lcd_en), GPIO_LOW);
    return ret;
}

static Std_ReturnType lcd_8bit_send_enable(const chr_lcd_8bit_t *lcd){
    Std_ReturnType ret = E_OK;
    ret = gpio_pin_write_logic(&(lcd->lcd_en), GPIO_HIGH);
    __delay_us(5);
    ret = gpio_pin_write_logic(&(lcd->lcd_en), GPIO_LOW);
    return ret;
}

static Std_ReturnType lcd_8bit_set_cursor(const chr_lcd_8bit_t *lcd , uint8 row , uint8 col){
    Std_ReturnType ret = E_OK;
    col--;
    switch(row){
        case ROW1:
            ret = lcd_8bit_send_command(lcd , LCD_DDRAM_START + col);
            break;
        case ROW2:
            ret = lcd_8bit_send_command(lcd , 0xC0 + col);
            break;
        case ROW3:
            ret = lcd_8bit_send_command(lcd , 0x94 + col);
            break;
        case ROW4:
            ret = lcd_8bit_send_command(lcd , 0xD4 + col);
            break;
            default: ;
    }
    


    return ret;
}


static Std_ReturnType lcd_4bit_set_cursor(const chr_lcd_4bit_t *lcd , uint8 row , uint8 col){
    Std_ReturnType ret = E_OK;
    col--;
    switch(row){
        case ROW1:
            ret = lcd_4bit_send_command(lcd , 0x80 + col);
            break;
        case ROW2:
            ret = lcd_4bit_send_command(lcd , 0xC0 + col);
            break;
        case ROW3:
            ret = lcd_4bit_send_command(lcd , 0x94 + col);
            break;
        case ROW4:
            ret = lcd_4bit_send_command(lcd , 0xD4 + col);
            break;
            default: ;
    }
    


    return ret;
}


Std_ReturnType lcd_4bit_initialize(const chr_lcd_4bit_t *lcd){
    Std_ReturnType ret = E_OK;
    uint8 data_pins_conter = 0;
    if(NULL == lcd){
        ret = E_NOT_OK;
    }
    else{
        
        ret = gpio_pin_initialize(&(lcd->lcd_en));
        ret = gpio_pin_initialize(&(lcd->lcd_rs));
        for(data_pins_conter = 0 ; data_pins_conter < 4 ; data_pins_conter++){
            ret = gpio_pin_initialize(&(lcd->lcd_data[data_pins_conter]));
        }
        
        __delay_ms(20);
        ret = lcd_4bit_send_command(lcd , LCD_8BIT_MODE_2_LINE);
        __delay_ms(5);
        ret = lcd_4bit_send_command(lcd , LCD_8BIT_MODE_2_LINE);
        __delay_us(150);
        ret = lcd_4bit_send_command(lcd , LCD_8BIT_MODE_2_LINE);
        
        ret = lcd_4bit_send_command(lcd , LCD_CLEAR);
        ret = lcd_4bit_send_command(lcd , LCD_RETURN_HOME);
        ret = lcd_4bit_send_command(lcd , LCD_ENTRY_MODE_INC_SHIFT_OFF);
        ret = lcd_4bit_send_command(lcd , LCD_DISPLAY_ON_UNDERLINE_OFF_CURSOR_OFF);
        ret = lcd_4bit_send_command(lcd , LCD_4BIT_MODE_2_LINE);
        ret = lcd_4bit_send_command(lcd , LCD_DDRAM_START);
        
    }
    return ret;
}
Std_ReturnType lcd_4bit_send_command(const chr_lcd_4bit_t *lcd , uint8 command){
    Std_ReturnType ret = E_OK;
    if(NULL == lcd){
        ret = E_NOT_OK;
    }
    else{
       ret = gpio_pin_write_logic(&(lcd->lcd_rs) , GPIO_LOW); 
       ret = lcd_send_4bits(lcd , command >> 4);
       ret = lcd_4bit_send_enable(lcd);
       ret = lcd_send_4bits(lcd , command);
       ret = lcd_4bit_send_enable(lcd);
    }
    return ret;
}
Std_ReturnType lcd_4bit_send_char_data(const chr_lcd_4bit_t *lcd , uint8 data){
    Std_ReturnType ret = E_OK;
    if(NULL == lcd){
        ret = E_NOT_OK;
    }
    else{
       ret = gpio_pin_write_logic(&(lcd->lcd_rs), GPIO_HIGH); 
       ret = lcd_send_4bits(lcd , data >> 4);
       ret = lcd_4bit_send_enable(lcd);
       ret = lcd_send_4bits(lcd , data);
       ret = lcd_4bit_send_enable(lcd);
    }
    return ret;
}
Std_ReturnType lcd_4bit_send_char_data_pos(const chr_lcd_4bit_t *lcd , uint8 row , uint8 col , uint8 data){
    Std_ReturnType ret = E_OK;
    if(NULL == lcd){
        ret = E_NOT_OK;
    }
    else{
        
        ret = lcd_4bit_set_cursor(lcd , row , col);
        ret = lcd_4bit_send_char_data(lcd , data);
    }
    return ret;
}
Std_ReturnType lcd_4bit_send_string(const chr_lcd_4bit_t *lcd , uint8 *str){
    Std_ReturnType ret = E_OK;
    if((NULL == lcd) || (NULL == str) ){
        ret = E_NOT_OK;
    }
    else{
        while(*str){
            ret = lcd_4bit_send_char_data(lcd , *str++);
        }
        
    }
    return ret;
}
Std_ReturnType lcd_4bit_send_char_str_pos(const chr_lcd_4bit_t *lcd , uint8 row , uint8 col , uint8 *str){
    Std_ReturnType ret = E_OK;
    if((NULL == lcd) || (NULL == str) ){
        ret = E_NOT_OK;
    }
    else{
        ret = lcd_4bit_set_cursor(lcd , row ,col);
        while(*str){
            ret = lcd_4bit_send_char_data(lcd , *str++);
        }
    }
    return ret;
}
Std_ReturnType lcd_4bit_send_custom_char(const chr_lcd_4bit_t *lcd , uint8 row , uint8 col , const uint8 chr[] , uint8 mem_pos){
    Std_ReturnType ret = E_OK;
    uint8 counter = 0;
    if(NULL == lcd){
        ret = E_NOT_OK;
    }
    else{
        ret = lcd_4bit_send_command(lcd , LCD_CGRAM_START + (mem_pos*8) );
        for(counter = 0 ; counter < 8 ; ++counter){
            ret = lcd_4bit_send_char_data(lcd , chr[counter]);
        }
        ret = lcd_4bit_send_char_data_pos(lcd , row , col , mem_pos);
        
    }
    return ret;
}


Std_ReturnType lcd_8bit_initialize(const chr_lcd_8bit_t *lcd){
     Std_ReturnType ret = E_OK;
    uint8 data_pins_conter = 0;
    if(NULL == lcd){
        ret = E_NOT_OK;
    }
    else{
        
        ret = gpio_pin_initialize(&(lcd->lcd_en));
        ret = gpio_pin_initialize(&(lcd->lcd_rs));
        for(data_pins_conter = 0 ; data_pins_conter < 8 ; data_pins_conter++){
            ret = gpio_pin_initialize(&(lcd->lcd_data[data_pins_conter]));
        }
        
        __delay_ms(20);
        ret = lcd_8bit_send_command(lcd , LCD_8BIT_MODE_2_LINE);
        __delay_ms(5);
        ret = lcd_8bit_send_command(lcd , LCD_8BIT_MODE_2_LINE);
        __delay_us(150);
        ret = lcd_8bit_send_command(lcd , LCD_8BIT_MODE_2_LINE);
        
        ret = lcd_8bit_send_command(lcd , LCD_CLEAR);
        ret = lcd_8bit_send_command(lcd , LCD_RETURN_HOME);
        ret = lcd_8bit_send_command(lcd , LCD_ENTRY_MODE_INC_SHIFT_OFF);
        ret = lcd_8bit_send_command(lcd , LCD_DISPLAY_ON_UNDERLINE_OFF_CURSOR_OFF);
        ret = lcd_8bit_send_command(lcd , LCD_DDRAM_START);
    }
    return ret;
}
Std_ReturnType lcd_8bit_send_command(const chr_lcd_8bit_t *lcd , uint8 command){
    Std_ReturnType ret = E_OK;
    uint8 pin_counter = 0;
    if(NULL == lcd){
        ret = E_NOT_OK;
        
    }
    else{ 
        ret = gpio_pin_write_logic(&(lcd->lcd_rs) , GPIO_LOW);
        for(pin_counter = 0; pin_counter < 8; pin_counter++){
            ret = gpio_pin_write_logic(&(lcd->lcd_data[pin_counter]) , (command >> pin_counter) & (uint8)0x01);
        }
        ret = lcd_8bit_send_enable(lcd);
    }
    return ret;
}
Std_ReturnType lcd_8bit_send_char_data(const chr_lcd_8bit_t *lcd , uint8 data){
    Std_ReturnType ret = E_OK;
    uint8 pin_counter = 0;
    if(NULL == lcd){
        ret = E_NOT_OK;
        
    }
    else{ 
        ret = gpio_pin_write_logic(&(lcd->lcd_rs) , GPIO_HIGH);
        for(pin_counter = 0; pin_counter < 8; pin_counter++){
            ret = gpio_pin_write_logic(&(lcd->lcd_data[pin_counter]) , (data >> pin_counter) & (uint8)0x01);
        }
        ret = lcd_8bit_send_enable(lcd);
    }
    return ret;
}
Std_ReturnType lcd_8bit_send_char_data_pos(const chr_lcd_8bit_t *lcd , uint8 row , uint8 col , uint8 data){
    Std_ReturnType ret = E_OK;
    if(NULL == lcd){
        ret = E_NOT_OK;
    }
    else{
        
        ret = lcd_8bit_set_cursor(lcd , row , col);
        ret = lcd_8bit_send_char_data(lcd , data);
    }
    return ret;
}
Std_ReturnType lcd_8bit_send_string(const chr_lcd_8bit_t *lcd , uint8 *str){
    Std_ReturnType ret = E_OK;
    if((NULL == lcd) || (NULL == str) ){
        ret = E_NOT_OK;
    }
    else{
        while(*str){
            ret = lcd_8bit_send_char_data(lcd , *str++);
        }
        
    }
    return ret;
}
Std_ReturnType lcd_8bit_send_char_str_pos(const chr_lcd_8bit_t *lcd , uint8 row , uint8 col , uint8 *str){
    Std_ReturnType ret = E_OK;
    if((NULL == lcd) || (NULL == str) ){
        ret = E_NOT_OK;
    }
    else{
        ret = lcd_8bit_set_cursor(lcd , row ,col);
        while(*str){
            ret = lcd_8bit_send_char_data(lcd , *str++);
        }
    }
    return ret;
}
Std_ReturnType lcd_8bit_send_custom_char(const chr_lcd_8bit_t *lcd , uint8 row , uint8 col , const uint8 chr[] , uint8 mem_pos){
    Std_ReturnType ret = E_OK;
    uint8 counter = 0;
    if(NULL == lcd){
        ret = E_NOT_OK;
    }
    else{
        ret = lcd_8bit_send_command(lcd , LCD_CGRAM_START + (mem_pos*8) );
        for(counter = 0 ; counter < 8 ; ++counter){
            ret = lcd_8bit_send_char_data(lcd , chr[counter]);
        }
        ret = lcd_8bit_send_char_data_pos(lcd , row , col , mem_pos);
        
    }
    return ret;
}

Std_ReturnType uint8_to_string(uint8 value , uint8 *str){
    Std_ReturnType ret = E_OK;
    if(NULL == str){
        ret = E_NOT_OK;
    }
    else{
        memset(str ,' \0' , 4);
        sprintf(str , "%i" , value);
        
    }
    return ret;
}
Std_ReturnType uint16_to_string(uint32 value , uint8 *str){
    Std_ReturnType ret = E_OK;
    if(NULL == str){
        ret = E_NOT_OK;
    }
    else{
        memset(str ,' \0' , 6);
        sprintf(str , "%i" , value);
        
    }
    return ret;
}
Std_ReturnType uint32_to_string(uint32 value , uint8 *str){
    Std_ReturnType ret = E_OK;
    if(NULL == str){
        ret = E_NOT_OK;
    }
    else{
        memset(str ,' \0' , 11);
        sprintf(str , "%i" , value);
        
    }
    return ret;
}

