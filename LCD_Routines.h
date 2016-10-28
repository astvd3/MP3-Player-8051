#ifndef _LCD_Routines_H
#define _LCD_Routines_H

#include "Main.H"

#define COMMAND (0)
#define DDRAM (1)

#define function_set_8bit (0x30)
#define function_set_4bit (0x20)
#define one_line (0x00)
#define two_line (0x08)
#define set_5x8 (0x00)
#define set_5x11 (0x40)

#define display_control (0x08)
#define display_off (0x00)
#define display_on (0x40)
#define cursor_off (0x00)
#define cursor_on (0x20)
#define blink_off (0x00)
#define blink_on (0x01)

#define display_mode (0x10)
#define shift_right (0x08)
#define shift_left (0x00)
#define shift_cursor (0x00)
#define shift_display (0x04)

#define set_cg_addr (0x40)
#define set_ddram_addr (0x80)
#define line1 (0x00)
#define tab1 (0x07)
#define line2 (0x40)
#define tab2 (0x47)

#define entry_mode (0x04)
#define inc_right (0x02)
#define dec_left (0x00)
#define cursor (0x00)
#define display (0x01)

#define return_home (0x02)
#define clear_display (0x01)




void LCD_Write(bit RegSelect, uint8_t LCD_Data);

void LCD_Init(void);

void LCD_Print(uint8_t num_bytes, uint8_t * array_name);


#endif