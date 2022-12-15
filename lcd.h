

#ifndef ECU_CHR_LCD_H
#define	ECU_CHR_LCD_H

/* Section : Includes */
#include "../../mcal/GPIO/hal_gpio.h"

/* Section : Macro Declarations */
#define LCD_CLEAR                               0X01

#define LCD_RETURN_HOME                         0x02

#define LCD_ENTRY_MODE_DEC_SHIFT_OFF            0x04
#define LCD_ENTRY_MODE_DEC_SHIFT_ON             0x05
#define LCD_ENTRY_MODE_INC_SHIFT_OFF            0x06
#define LCD_ENTRY_MODE_INC_SHIFT_ON             0x07

#define LCD_CURSOR_MOVE_SHIFT_LEFT              0x10
#define LCD_CURSOR_MOVE_SHIFT_RIGHT             0x14

#define LCD_DISPLAY_SHIFT_LEFT                  0x18
#define LCD_DISPLAY_SHIFT_RIGHT                 0x1C

#define LCD_DISPLAY_ON_UNDERLINE_OFF_CURSOR_OFF 0x0C
#define LCD_DISPLAY_ON_UNDERLINE_OFF_CURSOR_ON  0x0D
#define LCD_DISPLAY_ON_UNDERLINE_ON_CURSOR_OFF  0x0E
#define LCD_DISPLAY_ON_UNDERLINE_ON_CURSOR_ON   0x0F
#define LCD_DISPLAY_OFF_CURSOR_OFF              0x08

#define LCD_8BIT_MODE_2_LINE                    0x38
#define LCD_4BIT_MODE_2_LINE                    0x28

#define LCD_CGRAM_START                         0x40
#define LCD_DDRAM_START                         0x80

#define ROW1 1
#define ROW2 2
#define ROW3 3
#define ROW4 4

/* Section : Macro Functions Declarations */


/* Section : Data Types Declarations */
typedef struct{
    pin_config_t lcd_rs;
    pin_config_t lcd_en;
    pin_config_t lcd_data[4];
}chr_lcd_4bit_t;

typedef struct{
    pin_config_t lcd_rs;
    pin_config_t lcd_en;
    pin_config_t lcd_data[8];
}chr_lcd_8bit_t;

/* Section : Function Declarations */
Std_ReturnType lcd_4bit_initialize(const chr_lcd_4bit_t *);
Std_ReturnType lcd_4bit_send_command(const chr_lcd_4bit_t * , uint8 );
Std_ReturnType lcd_4bit_send_char_data(const chr_lcd_4bit_t * , uint8 );
Std_ReturnType lcd_4bit_send_char_data_pos(const chr_lcd_4bit_t * , uint8  , uint8  , uint8 );
Std_ReturnType lcd_4bit_send_string(const chr_lcd_4bit_t * , uint8 *);
Std_ReturnType lcd_4bit_send_char_str_pos(const chr_lcd_4bit_t * , uint8  , uint8  , uint8 *);
Std_ReturnType lcd_4bit_send_custom_char(const chr_lcd_4bit_t * , uint8  , uint8  , const uint8 chr[] , uint8 );

Std_ReturnType lcd_8bit_initialize(const chr_lcd_8bit_t *);
Std_ReturnType lcd_8bit_send_command(const chr_lcd_8bit_t * , uint8 );
Std_ReturnType lcd_8bit_send_char_data(const chr_lcd_8bit_t * , uint8 );
Std_ReturnType lcd_8bit_send_char_data_pos(const chr_lcd_8bit_t * , uint8  , uint8  , uint8 );
Std_ReturnType lcd_8bit_send_string(const chr_lcd_8bit_t * , uint8 *);
Std_ReturnType lcd_8bit_send_char_str_pos(const chr_lcd_8bit_t * , uint8  , uint8  , uint8 *);
Std_ReturnType lcd_8bit_send_custom_char(const chr_lcd_8bit_t * , uint8  , uint8  , const uint8 chr[] , uint8 );

Std_ReturnType uint8_to_string(uint8  , uint8 *);
Std_ReturnType uint16_to_string(uint32  , uint8 *);
Std_ReturnType uint32_to_string(uint32  , uint8 *);


#endif	/* ECU_CHR_LCD_H */

