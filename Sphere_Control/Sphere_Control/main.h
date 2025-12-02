/*
 * main.h
 *
 * Created: 2025-11-11 10:03:57 AM 
 *  Author: Jiheon 
 */ 

#ifndef MAIN_H_
#define MAIN_H_


#define F_CPU 14745600UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <math.h>
#include <stdlib.h>
#include "lcd.h"
#include "servo.h"
#include "gy25.h"

ISR(TIMER1_COMPA_vect)
{
	asm("nop");
}

servo_t servo_status; //Current Servo motor angle status
gy25_t raw_angle, new_angle, cur_angle;
int16_t LPF_alpha = 80, LPF_scale = 100; // LPF의 alpha: LPF_alpha / LPF_scale = 0.8


inline void delay_ms(uint16_t ms);




#endif /* MAIN_H_ */