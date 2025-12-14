/*
 * Sphere_Control.c
 *
 * Created: 2025-11-11 10:02:30AM 
 * Author : Jiheon
 */ 

#include "main.h"


/*
	pitch는 위로 올라가면 -
	yaw는 왼쪽이 +	
*/
#define StabilizeCNT 8


void Init_main(void)
{
	char msg[20];
	LCD_Init();
	MPU6050_Init();
	uint8_t res = MPU6050_Check_WHO_AM_I();
	if(res == 0)
	{
		LCD_print(0x80, "Pass");
		delay_ms(1000);
	}
	else
	{
		sprintf(msg, "Fail %d",res);
		LCD_print(0x80, msg);
		LCD_print(0x10,"Please Restart");
		while(1);
	}
	Servo_Init();
	servo_status.pitch = 90;
	servo_status.yaw= 90;
	//set_Servo(&servo_status, 0,0);
	set_YawServo(DEG2OCR(servo_status.yaw));
	set_PitchServo(DEG2OCR(servo_status.pitch));
}

int main(void)
{
	cli();
	Init_main();
	sei();

	uint8_t str[32];
	LCD_Clear();
	pitch_deg_stabilize = 0;
	yaw_deg_stabilize = 0;
	uint16_t scan_MPUcnt = 0;
	while (1)
	{
		if(data_ready_flg)
		{
			data_ready_flg = 0;
			if(scan_MPUcnt == StabilizeCNT)
			{
				pitch_deg_stabilize = pitch_deg;
				yaw_deg_stabilize = yaw_deg;
				scan_MPUcnt=0;
			}
			cli();
			//sprintf(str,"r:%d p:%d y:%d",(uint16_t)roll_deg,(uint16_t)pitch_deg,(uint16_t)yaw_deg);
			sprintf(str,"P:%d p:%d y:%d",(int16_t)pitch_deg_stabilize,(int16_t)pitch_deg,(int16_t)yaw_deg);
			LCD_print(0x80,str);
			
			//set_ServoPitch_Relative(&servo_status, pitch_deg, &old_pitch_deg_stabilize);
			//set_ServoYaw_Relative(&servo_status, yaw_deg, &old_yaw_deg_stabilize);
			////set_ServoYaw_Absolute(&servo_status, (uint16_t)yaw_deg);
			set_Servo_angle(&servo_status,pitch_deg,pitch_deg_stabilize,yaw_deg,yaw_deg_stabilize);
			
			sprintf(str,"Y:%d p:%d y:%d",(int16_t)yaw_deg_stabilize, servo_status.pitch, servo_status.yaw);
			LCD_print(0x10,str);	
			_delay_ms(100);
			sei();
			scan_MPUcnt++;
		}
		
		//delay_ms(300);
	}
}
//
////GY-25
//int main(void)
//{
	//LCD_Init();
	//GY25_Init();
	//Servo_Init();
	//sei();
	//DDRD = 0x00;
	////Servo_sample_code();
	//
	//uint8_t str[32];
	//LCD_Clear();
	//cur_angle = read_GY25();
	//servo_status.pitch_deg = 90;
	//servo_status.yaw_deg = 90;
	//set_PitchServo(DEG2OCR(servo_status.pitch_deg));
	//set_YawServo(DEG2OCR(servo_status.yaw_deg));
	//delay_ms(1000);
    //while (1) 
    //{
		//raw_angle = read_GY25();
		//// LPF
		//// new_angle = alpha * cur_angle + angle * (1-alpha);
		//new_angle.pitch = (LPF_alpha * cur_angle.pitch + raw_angle.pitch * (LPF_scale-LPF_alpha)) / LPF_scale;
		//new_angle.yaw   = (LPF_alpha * cur_angle.yaw   + raw_angle.yaw   * (LPF_scale-LPF_alpha)) / LPF_scale;
		//new_angle.roll  = (LPF_alpha * cur_angle.roll  + raw_angle.roll  * (LPF_scale-LPF_alpha)) / LPF_scale;
		//// 변화량으로 제어
		////gy25_t delta = Delta_angle_Calc_GY25(&new_angle, &cur_angle);
		////set_Servo(&servo_status, delta.pitch, delta.yaw);
		//// 값으로 제어
		//set_PitchServo(DEG2OCR(new_angle.pitch + 90));
		//set_YawServo(DEG2OCR(new_angle.yaw + 90));
		//
		//sprintf(str,"Y:%3d   P:%3d",cur_angle.yaw,cur_angle.pitch);
		//LCD_print(0x80,str);
		////sprintf(str,"YSM:%3d PSM:%3d",servo_status.yaw_deg, servo_status.pitch_deg);
		////sprintf(str,"Y:%3d   P:%3d",delta.yaw,delta.pitch);
		//LCD_print(0x10,str);
		//delay_ms(300);
    //}
//}

