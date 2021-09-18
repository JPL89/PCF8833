/*
	@File 		pcf8833.cpp
	@Author		JOSIMAR PEREIRA LEITE
	@country	Brazil
	@Date		26/06/2021
	

    Copyright (C) 2021  JOSIMAR PEREIRA LEITE

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
*/
#include "pcf8833.h"
#include <avr/io.h>
#include <avr/pgmspace.h>

unsigned int PCF8833::RGB(unsigned char R, unsigned char G, unsigned char B)
{
	return ((R & 0xE0) | (G & 0x1C) | (B & 0x03));
}

void PCF8833::Serial_Write(unsigned char data)
{
	//	transmission byte 
	//	may be a command OR a data byte
	//	Send MSB first: MSB..LSB (B7..B0)
	
	for (int i=7; i>=0; i--)
	{
		PCF8833_CLK_LOW;
		
		if (data & (1 << i))
			PCF8833_MOSI_HIGH;
		else
			PCF8833_MOSI_LOW;
		
		PCF8833_CLK_HIGH;
	}
}

void PCF8833::Serial_Command(unsigned char cmd)
{
	PCF8833_CS_HIGH;		// CS: No Active
	
	PCF8833_MOSI_LOW;		// SDI: 0 -> Command write
	
	PCF8833_CLK_LOW;		// At the falling edge of SCE: SCLK must be LOW
	
	PCF8833_CS_LOW;			// Chip Select: Active low

	PCF8833_CLK_HIGH;		// SDIN: is sampled at the rising edge of SCLK
	
	PCF8833::Serial_Write(cmd);	// Send command
		
	PCF8833_CS_HIGH;		// CS: No Active
}

void PCF8833::Serial_Data(unsigned char data)
{
	PCF8833_CS_HIGH;		// CS: No Active
	
	PCF8833_MOSI_HIGH;		// SDI: 1 -> Data write
					
	PCF8833_CLK_LOW;		// At the falling edge of SCE: SCLK must be LOW
	
	PCF8833_CS_LOW;			// Chip Select: Active low

	PCF8833_CLK_HIGH;		// SDIN: is sampled at the rising edge of SCLK
	
	PCF8833::Serial_Write(data);	// Send command
	
	PCF8833_CS_HIGH;		// CS: No Active
	
}

// xs: left
// xe: right
// ys: top
// ye: bottom
void PCF8833::Set_Window(unsigned int xs,unsigned int xe,unsigned int ys,unsigned int ye)
{
	PCF8833::Serial_Command(0x2A);
	PCF8833::Serial_Data(xs);
	PCF8833::Serial_Data(xe);

	PCF8833::Serial_Command(0x2B);
	PCF8833::Serial_Data(ys);
	PCF8833::Serial_Data(ye);
	
	PCF8833::Serial_Command(0x2C);
}

void PCF8833::Set_Pixel(unsigned int x,unsigned int y,unsigned int color)
{
	PCF8833::Set_Window(x, x+1, y, y+1);
	
	PCF8833::Serial_Data(color);
	PCF8833::Serial_Data(color);
}

void PCF8833::Clear(unsigned int x,unsigned int y,unsigned int width, unsigned int height, unsigned int color)
{
	PCF8833::Set_Window(x, x+width, y, y+height);
	
	for (unsigned int h=0; h<width; h++)
	{
		for (unsigned int w=0; w<height; w++)
		{
			PCF8833::Serial_Data(color);
		}
	}
}

void PCF8833::WriteChar(unsigned char data, unsigned int x, unsigned int y, const unsigned char Font[], unsigned int Color)
{
	unsigned char width				= pgm_read_byte( &Font[0] );	// font width
	unsigned char page				= pgm_read_byte( &Font[2] );	// font pages
	unsigned char st				= pgm_read_byte( &Font[3] );	// font start
	unsigned char ch				= 0;							// character
	unsigned char fh				= 5;							// font file header size
	unsigned char col				= 0;							// col
	unsigned char i					= 0;							// width * width ...
	unsigned int row				= 0;							// rows
	unsigned int j					= 0;
	
	int id = ( ( ( data - st) * ( (width+1) * 2) + fh ) );

	for (i=0; i<page; i++)
	{
		for (; col < (width + j); col++)
		{
			ch = pgm_read_byte( &Font[ (id + col) + 2 ] );
			
			for(row=0; row<8; row++)
			{
				if (ch & (1 << row))
				{
					PCF8833::Serial_Command(0x2A);
					PCF8833::Serial_Data(x + col);
					PCF8833::Serial_Data(ch);

					PCF8833::Serial_Command(0x2B);
					PCF8833::Serial_Data(y + row);
					PCF8833::Serial_Data(8);
					
					PCF8833::Serial_Command(0x2C);
					
					PCF8833::Serial_Data(Color);
				}
			}
		}
		
		y += 8;
		x -= width;
		j += width;
	}
}

void PCF8833::WriteString(const char *data, unsigned int x, unsigned int y, const unsigned char Font[], unsigned int Color)
{
	while(*data)
	{
		WriteChar(*data, x, y, Font, Color);
		
		unsigned char width				= pgm_read_byte( &Font[0] );	// font width
		unsigned char st				= pgm_read_byte( &Font[3] );	// font start
		unsigned char fh				= 5;							// font file header size
		int i							= ( ( ( *data++ - st) * ( (width+1) * 2) + fh ) );
		
		x								+= pgm_read_byte( &Font[ i ] ) + 1;
	}
}


void PCF8833::Draw_Image_8Bits(const char *buffer,unsigned char x, unsigned char y,unsigned int IMAGE_W,unsigned int IMAGE_H)
{
	PCF8833::Set_Window(x, (x+IMAGE_W)-1, y, (y+IMAGE_H)-1);
	
	for(int i=0; i<(int)(IMAGE_W*IMAGE_H)-1; i++)
	{
		PCF8833::Serial_Data(pgm_read_byte(&buffer[i]));
	}
}

void PCF8833::Fill(unsigned int color)
{
	PCF8833::Set_Window(0,131,0,131);
	
	for (unsigned char h=0; h<131; h++)
	{
		for (unsigned char w=0; w<131; w++)
		{
			PCF8833::Serial_Data(color);
		}
	}
}

void PCF8833::Init()
{
	DDRC |= (1 << PCF8833_MOSI);
	DDRC |= (1 << PCF8833_CLK);
	DDRC |= (1 << PCF8833_CS);
	DDRC |= (1 << PCF8833_RS);
	
	PCF8833_CLK_LOW;
	PCF8833_CS_HIGH;
	PCF8833_MOSI_HIGH;
	
	PCF8833_RS_HIGH;
	
	PCF8833::Serial_Command(0x11);		// Sleep Out
	
	PCF8833::Serial_Command(0x36);		// MADCTL: 0x36
	PCF8833::Serial_Data(0);
	
	PCF8833::Serial_Command(0x3A);		// COLMOD: 0x3A
	PCF8833::Serial_Data(0x02);		// 8-Bits mode
	
	PCF8833::Serial_Command(0x29);		// Display on
}