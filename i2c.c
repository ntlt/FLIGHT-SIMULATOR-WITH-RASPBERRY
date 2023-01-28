#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <wiringSerial.h>
#include <stdio.h>
#include <stdint.h> // su dung so nguyen int16_t
#include <math.h> // gcc....-lm
// define MPU register address define
#define device_address 	    0x68

#define SMPLRT_DIV 			25
#define CONFIG 				26
#define GYRO_CONFIG 		27
#define ACCEL_CONFIG   	 	28
#define INT_ENABLE     		56
#define PWR_MGMT_1 			107

#define ACCEL_XOUT_H 		59
#define ACCEL_YOUT_H 		61
#define ACCEL_ZOUT_H 		63

#define GYRO_XOUT_H 		67
#define GYRO_YOUT_H 		69
#define GYRO_ZOUT_H 		71


// GPIO pin define
#define INC_SPEED 		    26
#define DEC_SPEED	 	    19
#define OPEN_FLAP 		    13
#define CLOSE_FLAP 		    21
#define LANDING_GEAR 	    20
#define CENTER_PLANE 	    12
#define L_BRAKE 	        6
#define R_BRAKE      	    5

int mpu, serial;
int inc_speed, dec_speed, open_flap, close_flap,landing_gear,r_brake,l_brake,center_plane;
// checking button
void inc_speed_1(void)
{
    inc_speed = 1;
}

void inc_speed_0(void)
{
    inc_speed = 0;
}

void dec_speed_1(void)
{
    dec_speed = 1;
}

void dec_speed_0(void)
{
    dec_speed = 0;
}

void open_flap_1(void)
{
    open_flap = 1;
}

void open_flap_0(void)
{
    open_flap = 0;
}
void close_flap_1(void)
{
    close_flap = 1;
}

void close_flap_0(void)
{
    close_flap = 0;
}
void landing_gear_1(void)
{
	landing_gear = 1;
}
void landing_gear_0(void)
{
	landing_gear = 0;
}
void center_plane_1(void)
{
    center_plane = 1;
}

void center_plane_0(void)
{
    center_plane = 0;
}

void left_brake_1(void)
{
    l_brake = 1;
}

void left_brake_0(void)
{
    l_brake = 0;
}

void right_brake_1(void)
{
    r_brake = 1;
}

void right_brake_0(void)
{
    r_brake = 0;
}
////////////////////////////////////////

void MPU6050_Init(void)
 {
	// cau hinh che do hoat bao gom cac register 25->28,56,107
	//tham so dau tien la mpu(thiet bi can giao tiep
	//tham so thu 2 dia chi register muon giao tiep
	//tham so thu 3 la gia tri muon gui xuogn thanh ghi
	//wiringPiI2CWriteReg8 (tham so 1, thamh so 2, tham so 3);
	wiringPiI2CWriteReg8 (mpu, SMPLRT_DIV, 15);	
	//khogn su dung xung ngoai , tat bo loc so DLFP
	wiringPiI2CWriteReg8 (mpu, CONFIG, 0);	
	
	//gyro Fs nam trong khoang 500 do/giay
	//wiringPiI2CWriteReg8 (mpu, GYRO_CONFIG, 0x08);
	wiringPiI2CWriteReg8 (mpu, GYRO_CONFIG, 0);
	//acc fs +-8g
	wiringPiI2CWriteReg8 (mpu, ACCEL_CONFIG, 0x10);		
	wiringPiI2CWriteReg8 (mpu, INT_ENABLE, 0x01);	
	wiringPiI2CWriteReg8 (mpu, PWR_MGMT_1, 0x01);	

 }
 
////////////////////////////////////////////////////
int16_t read_data(unsigned char addr)
{
	/*
	//doc gia tri do
	int16_t high_byte, low_byte, data;
	high_byte = wiringPiI2CReadReg8(mpu, Acc_X);
	
	//59 la 8 bit muc cao( +1)= 60 <la 8 bit muc thap>
	low_byte  = wiringPiI2CReadReg8(mpu, Acc_X + 1);
	
	//ghep data lai de thu ve gia tri 16 bit
	data = (high<<8) | low; 
	*/
	
	int16_t high_byte, low_byte, data;
	high_byte = wiringPiI2CReadReg8(mpu, addr);
	
	//59 la 8 bit muc cao( +1)= 60 <la 8 bit muc thap>
	low_byte  = wiringPiI2CReadReg8(mpu, addr + 1);
	
	//ghep data lai de thu ve gia tri 16 bit
	data = (high_byte<<8) | low_byte; 
	return data;
	
}

//////////////////////////////////////////////////////

int main(void)
{
	//set up i2c communication 
	mpu = wiringPiI2CSetup(device_address); //datasheet
	//setup serial communication
	serial = serialOpen("/dev/ttyAMA0", 115200);
	//theit lap che do dau vao cho nut nhan 
	wiringPiSetupGpio();
	pinMode(INC_SPEED, 		INPUT);     pinMode(DEC_SPEED,		INPUT);
	pinMode(OPEN_FLAP ,		INPUT);     pinMode(CLOSE_FLAP, 	INPUT);
    pinMode(LANDING_GEAR,	INPUT);	    pinMode(CENTER_PLANE, 	INPUT);
    pinMode(L_BRAKE, 	    INPUT);     pinMode(R_BRAKE, 	    INPUT);

	//thiet lap che do do cho mpy6050
	MPU6050_Init();
	while(1)
	{
		
		//thiet lap chan doc tin hieu (INPUT)
		inc_speed        = digitalRead(INC_SPEED);		    dec_speed	 	= digitalRead(DEC_SPEED);
		open_flap 		= digitalRead(OPEN_FLAP);		    close_flap 	    = digitalRead(CLOSE_FLAP);
		landing_gear	= digitalRead(LANDING_GEAR);		center_plane 	= digitalRead(CENTER_PLANE);
        l_brake 		= digitalRead(L_BRAKE);             r_brake 	= digitalRead(R_BRAKE);

		//read value of MPU from accelerometer data
		//for range of +-2g, thi chia cho 16384 theo datasheet 4096.0
        //RAW VALUE
		float xAccel = (float)read_data(ACCEL_XOUT_H);
		float yAccel = (float)read_data(ACCEL_XOUT_H);
		float zAccel = (float)read_data(ACCEL_XOUT_H);
		//READ GYRO RAW VALUE
	    float gyro_x = read_data(GYRO_XOUT_H);
	    float gyro_y = read_data(GYRO_YOUT_H);
	    float gyro_z = read_data(GYRO_ZOUT_H);
		//calculate roll and pitch from the accelerometer data
		//pitch=accTilt1, roll=accTilt2
		float pitch = atan2f(xAccel,sqrt(pow(yAccel,2)+pow(zAccel,2)))*180/M_PI; //tilt angle from accelerometer ( in degrees)
		float roll  = atan2f(yAccel,sqrt(pow(xAccel,2)+pow(zAccel,2)))*180/M_PI;
	
////////////////////////////////////////////////////////////////////////
		
		//read value of MPU from gyro data
		// for a 250deg/s thi chia cho 131.0 theo datasheet
		//hien thi gia tri len terminal raspberry 
		printf("speedu: %d; Speeddw: %d; Open flap: %d; Close flap: %d; Landing gear: %d; Center plane: %d; Left brake: %d; Right brake: %d; Roll: %.0f; Pitch: %.0f",  inc_speed, dec_speed, open_flap, close_flap, landing_gear, center_plane, l_brake, r_brake, roll, pitch);
        //hien thi gia  tri len pc via serial port
        serialPrintf(serial, "%d %d %d %d %d %d %d %d %.0f %.0f",inc_speed, dec_speed, open_flap, close_flap, landing_gear, center_plane, l_brake, r_brake, roll, pitch); 
        //hien thi per 50s
        delay(50); 
	}
	serialClose(serial);
	return 0;
}