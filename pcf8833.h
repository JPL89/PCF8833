/*
	@File 		pcf8833
	@Author		JOSIMAR PEREIRA LEITE
	@country	Brazil
	@Date		26/06/2021
	

    Copyright (C) 2021  JOSIMAR PEREIRA LEITE

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
*/
#ifndef PCF8833_H_
#define PCF8833_H_

// FONTS
#include "Fonts/Arial_10x10.h"
#include "Fonts/Arial_12x12.h"
#include "Fonts/Arial_13x13.h"
#include "Fonts/Arial_16x16.h"

#include "Fonts/Arial_Bold_10x11.h"
#include "Fonts/Arial_Bold_16x17.h"
#include "Fonts/Times_New_Roman_Bold_12x11.h"

#include "Fonts/Calibri_9x10.h"
#include "Fonts/Tahoma_10x11.h"
#include "Fonts/Times_New_Roman_11x12.h"

#include "Fonts/Ms_Serif.h"
#include "Fonts/Verdana_11x11.h"
#include "Fonts/Arial_Italic_11x12.h"

#define PCF8833_CS						PC0
#define PCF8833_MOSI					PC1
#define PCF8833_CLK						PC2
#define PCF8833_RS						PC3
	
#define PCF8833_CS_LOW					PORTC &= (unsigned char)~(1<<PCF8833_CS)
#define PCF8833_CS_HIGH					PORTC |= (1<<PCF8833_CS)

#define PCF8833_MOSI_LOW				PORTC &= (unsigned char)~(1<<PCF8833_MOSI)
#define PCF8833_MOSI_HIGH				PORTC |= (1<<PCF8833_MOSI)

#define PCF8833_CLK_LOW					PORTC &= (unsigned char)~(1<<PCF8833_CLK)
#define PCF8833_CLK_HIGH				PORTC |= (1<<PCF8833_CLK)

#define PCF8833_RS_LOW					PORTC &= (unsigned char)~(1<<PCF8833_RS)
#define PCF8833_RS_HIGH					PORTC |= (1<<PCF8833_RS)

#define PCF8833_RGB(R, G, B)			(R * 7 / 255) << 5 | (G * 7 / 255) << 2 | (B * 3 / 255)

#define PCF8833_COLOR_LIGHT_BLUE		0X96
#define PCF8833_COLOR_WHITE 			0XFF
#define PCF8833_COLOR_BLACK 			0X00
#define PCF8833_COLOR_RED 				0XE0
#define PCF8833_COLOR_LIGHT_CYAN		0XDF
#define PCF8833_COLOR_ORANGE			0XE4
#define PCF8833_COLOR_SILVER			0XB6
#define PCF8833_COLOR_GRAY				0X6D
#define PCF8833_COLOR_YELLOW			0XFC
#define PCF8833_COLOR_PURPLE 			0XE3
#define PCF8833_COLOR_BLUE 				0X03
#define PCF8833_COLOR_GREEN 			0X1C
#define PCF8833_COLOR_WHITE_GHOST_WHITE 0XDB
#define PCF8833_COLOR_WHITE_SMOKE 		0XDA
#define PCF8833_COLOR_HOT_PINK			0XEA
#define PCF8833_COLOR_DEEP_PINK			0XE2
#define PCF8833_COLOR_PINK				0XF6
#define PCF8833_COLOR_PAPAYA_WHIP		0XFA
#define PCF8833_COLOR_VIOLET			0XCE
#define PCF8833_COLOR_GAINSBORO 		0XDA
#define PCF8833_COLOR_SNOW		 		0XFA

class PCF8833
{
	public:
		void Serial_Write(unsigned char data);
		void Serial_Command(unsigned char cmd);
		void Serial_Data(unsigned char data);
		void Set_Window(unsigned int xs,unsigned int xe,unsigned int ys,unsigned int ye);
		void Set_Pixel(unsigned int x,unsigned int y,unsigned int color);
		void Draw_Image_8Bits(const char *buffer,unsigned char x, unsigned char y,unsigned int IMAGE_W,unsigned int IMAGE_H);
		void Clear(unsigned int x,unsigned int y,unsigned int width, unsigned int height, unsigned int color);
		void Fill(unsigned int color);
		void Init();	
		
		void WriteChar(unsigned char data, unsigned int x, unsigned int y, const unsigned char Font[], unsigned int Color);
		void WriteString(const char *data, unsigned int x, unsigned int y, const unsigned char Font[], unsigned int Color);
		
		unsigned int RGB(unsigned char R, unsigned char G, unsigned char B);
};

#endif /* PCF8833_H_ */