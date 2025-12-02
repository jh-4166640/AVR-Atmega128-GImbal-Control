/*
 * Sphere_Control.c
 *
 * Created: 2025-11-11 10:02:30AM 
 * Author : Jiheon
 */ 

#include "main.h"

int main(void)
{
	LCD_Init();
	GY25_Init();
	Servo_Init();
	sei();
	DDRD = 0x00;
	//Servo_sample_code();
	
	uint8_t str[32];
	LCD_Clear();
	cur_angle = read_GY25();
	servo_status.pitch_deg = 90;
	servo_status.yaw_deg = 90;
	set_PitchServo(DEG2OCR(servo_status.pitch_deg));
	set_YawServo(DEG2OCR(servo_status.yaw_deg));
	delay_ms(1000);
    while (1) 
    {
		raw_angle = read_GY25();
		// LPF
		// new_angle = alpha * cur_angle + angle * (1-alpha);
		new_angle.pitch = (LPF_alpha * cur_angle.pitch + raw_angle.pitch * (LPF_scale-LPF_alpha)) / LPF_scale;
		new_angle.yaw   = (LPF_alpha * cur_angle.yaw   + raw_angle.yaw   * (LPF_scale-LPF_alpha)) / LPF_scale;
		new_angle.roll  = (LPF_alpha * cur_angle.roll  + raw_angle.roll  * (LPF_scale-LPF_alpha)) / LPF_scale;
		// 변화량으로 제어
		//gy25_t delta = Delta_angle_Calc_GY25(&new_angle, &cur_angle);
		//set_Servo(&servo_status, delta.pitch, delta.yaw);
		// 값으로 제어
		set_PitchServo(DEG2OCR(new_angle.pitch + 90));
		set_YawServo(DEG2OCR(new_angle.yaw + 90));
		
		sprintf(str,"Y:%3d   P:%3d",cur_angle.yaw,cur_angle.pitch);
		LCD_print(0x80,str);
		//sprintf(str,"YSM:%3d PSM:%3d",servo_status.yaw_deg, servo_status.pitch_deg);
		//sprintf(str,"Y:%3d   P:%3d",delta.yaw,delta.pitch);
		LCD_print(0x10,str);
		delay_ms(300);
    }
}

