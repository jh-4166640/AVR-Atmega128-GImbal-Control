/*
 * gy25.c
 *
 * Created: 2025-11-25 오후 12:54:11
 *  Author: user
 */ 

#include "gy25.h"


ISR(USART0_RX_vect)
{
	uint8_t temp = UDR0;
	if(temp == FRAME_START)
	{
		gy25_idx = 0;
		gy25_buffer[gy25_idx++] = temp;
	}
	else if(gy25_idx < 8)
	{
		gy25_buffer[gy25_idx++] = temp;
		if(gy25_idx == 8) gy25_idx =0;
	}
	else gy25_idx = 0;
}

void GY25_Init(void)
{
	gy25_idx=0;
	UART0_Init_Intcon();
	
	uint16_t init_ms = 4000;
	
	LCD_print(0x80, MSG_FIRST_CORR0);
	LCD_print(0x10, MSG_FIRST_CORR1);
	_delay_ms(4000);
	
	/* Correction pitch/roll */
	LCD_print(0x80, MSG_PITCH_CORR);
	LCD_print(0x10, MSG_WAIT);
	_delay_ms(800);
	
	LCD_print(0x10, MSG_CALIBRATING);
	UART0_putch(COMMAND_START); // GY-25로 메세지 전송 시작
	UART0_putch(CORRECTION_MODE_PITCH); // pitch 보정
	_delay_ms(4000);
	
	/* Correction YAW */
	LCD_print(0x80,MSG_YAW_CORR);
	LCD_print(0x10, MSG_WAIT);
	_delay_ms(800);
	
	LCD_print(0x10, MSG_CALIBRATING);
	UART0_putch(COMMAND_START);	// GY-25로 메세지 전송 시작
	UART0_putch(CORRECTION_MODE_YAW);	// yaw(hedding) 보정
	_delay_ms(4000);
}
static inline void update_Angle_GY25(gy25_t* new_ang, gy25_t* cur_ang)
{
	cur_ang->pitch = new_ang->pitch;
	cur_ang->yaw   = new_ang->yaw;
	cur_ang->roll  = new_ang->roll;
}
gy25_t read_GY25(void)
{
	gy25_t data;
	uint8_t buf[8];

	UART0_putch(COMMAND_START);
	UART0_putch(QUERY_MODE);
	_delay_ms(20);
	for(int8_t i = 0;i<8;i++)
	{
		buf[i] = gy25_buffer[i];
	}
	if(buf[FRAME_S] == FRAME_START && buf[FRAME_E] == FRAME_END) // 데이터 정상 수신 완료
	{
		data.yaw = ((buf[YAW_H] << 8) | buf[YAW_L]) / 100;
		data.pitch = ((buf[PITCH_H] << 8) | buf[PITCH_L]) / 100;
		data.roll = ((buf[ROLL_H] << 8) | buf[ROLL_L]) / 100;	
	}
	else
	{
		data.pitch = ERROR_VAL;
		data.roll = ERROR_VAL;
		data.yaw = ERROR_VAL;
	}
	_delay_ms(40);
	return data;
}

gy25_t Delta_angle_Calc_GY25(gy25_t* new_ang, gy25_t* cur_ang) 
{
	gy25_t delta;
	int16_t dp = new_ang->pitch - cur_ang->pitch;
	int16_t dy = new_ang->yaw	- cur_ang->yaw;
	int16_t dr = new_ang->roll	- cur_ang->roll;
	
	delta.pitch = abs(dp) > 10? dp:0;
	delta.yaw	= abs(dy) > 10? dy:0;
	delta.roll	= abs(dr) > 10? dr:0;
	
	update_Angle_GY25(new_ang, cur_ang);
	
	return delta;
}
/*
void GY25_Sample_code(void)
{
	gy25_t angle;
	char str[32];
	
	// LPF
	// new = alpha * cur + angle * (1-alpha)
	cur_angle = new_angle;
	raw_angle = read_GY25();
	// new_angle = alpha * cur_angle + angle * (1-alpha);
	new_angle.pitch = (LPF_alpha * cur_angle.pitch + raw_angle.pitch * (LPF_scale-LPF_alpha)) / LPF_scale;
	new_angle.yaw   = (LPF_alpha * cur_angle.yaw   + raw_angle.yaw   * (LPF_scale-LPF_alpha)) / LPF_scale;
	new_angle.roll  = (LPF_alpha * cur_angle.roll  + raw_angle.roll  * (LPF_scale-LPF_alpha)) / LPF_scale;
	
	sprintf(str,"Y: %3d P: %3d",new_angle.yaw,new_angle.pitch);
	LCD_print(0x00,str);
	sprintf(str,"R: %3d",new_angle.roll	);
	LCD_print(0x10,str);
	delay_ms(50);
}
*/