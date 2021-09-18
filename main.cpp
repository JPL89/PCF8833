/*
	@File 		main.cpp
	@Author		JOSIMAR PEREIRA LEITE
	@country	Brazil
	@Date		07/06/2021
	

    Copyright (C) 2021  JOSIMAR PEREIRA LEITE

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
*/

#define F_CPU 16000000UL

#include <avr/io.h>

#include "pcf8833.h"

PCF8833 pcf8833;


int main()
{
	pcf8833.Init();
	
	pcf8833.Clear(0, 0, 132, 132, PCF8833_COLOR_WHITE);
	
	pcf8833.WriteString("Nokia",2,2,Font_Arial_16x16, PCF8833_COLOR_RED);
	
	while(1);
}