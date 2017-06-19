/* * * * * * * * * * * * * * * * * * * * * * * * * *
 * ProjectionBall Firmware For v3.X				   *
 * Copyright (c) 2017  							   *
 * K.Watanabe,Crescent 							   *
 * Released under the MIT license 				   *
 * http://opensource.org/licenses/mit-license.php  *
 * 17/06/16 v1.0 Initial Release                   *
 * 												   *
 * * * * * * * * * * * * * * * * * * * * * * * * * */	
	#include "main.h"
	
	//UART Display
	extern volatile unsigned char uart_out;
	extern volatile unsigned char SW_Status;
	extern volatile unsigned char PAT_Status;
	extern volatile unsigned int Init_Count;
		
	extern volatile int duty0, duty1,duty2,duty3;
	extern volatile int OUT0, OUT1;
	extern volatile unsigned int x_bit0,x_bit1,x_bit2,x_bit3,x_bit4,x_bit5,x_bit6,x_bit7;
	
	
	extern volatile int x_res0,dx_res0,x_err0,x_sum0,x_cmd0,x_cmd2,dx_cmd0;
	extern volatile int x_res1,dx_res1,x_err1,x_sum1,x_cmd1,x_cmd3,dx_cmd1;
	extern volatile int dx_tmp0;
	extern volatile int dx_tmp1;
	
	extern volatile int cnt0,cnt1,cnt2,cnt3;
	
	//extern volatile int max_val;
	
	extern volatile int tau_cmd0;
	extern volatile int tau_cmd1;
	
	extern volatile int x_cen0;
	extern volatile int x_cen1;
	
	
	//extern volatile float amp;
	
	
	extern volatile unsigned char RxWptr,RxRptr;
	extern volatile char RxWbuf[32],RxRbuf[32];
	
	
	extern volatile unsigned char allp_num[DATA_NUM];
	extern volatile unsigned char allf_num[FRAME_NUM];
	
	extern volatile unsigned char  LSD_num;
	//extern volatile int LSD_flag;

	extern volatile int deg0;
	extern volatile int deg1;
	extern volatile int rdeg;
	extern volatile char rflag;
	extern volatile int angle;
	extern volatile int u_angle;
	
	extern volatile char SD_mode;//0:Internal memory 1:SD memory
	extern volatile char UART_mode;//0:SW Mode 1:Uart Mode
	extern volatile char Start_EN;//0:OFF 1:ON
	extern volatile char Debug_mode;//0:OFF,1:ON
	extern volatile char Frame_EN;//0:OFF,1:ON
	extern volatile unsigned char frameNum;
	extern volatile unsigned char NextFlag;

	extern volatile int p_cmd0;
	extern volatile int p_cmd1;
	extern volatile int p_cmd2;
	extern volatile int p_cmd3;
	
	extern volatile unsigned int Err_cnt0;
	extern volatile unsigned int Err_cnt1;
	
	extern volatile char time_h;
	extern volatile char time_m;
	extern volatile char time_s;
	extern volatile char date_d;
	extern volatile char date_m;
	extern volatile char date_y;
	extern volatile int time_us;//10000


	extern void init_variable(void);

	extern  const float sine[120];
 	extern  const float cose[120];
						 
						 
					 


	
	
	
	
		
extern  const int starx[168];
						
extern  const int stary[168];


extern  const int maill[164];
extern  const int mailx[164];

extern  const int maily[164];
				
	
	
extern  const int nikox[125];//120



extern  const int nikoy[125];//120

extern  const char nikol[125];//120

extern  const int arrowx[108];
					
extern  const int arrowy[108];

extern  const int StrOffset[7][6];


extern  volatile char allstr[32];
extern  volatile unsigned char allstrlen;
extern  volatile unsigned char allstrcnt;


extern  volatile char dwatchstr[8];
extern  volatile unsigned char dwatchstrlen;
extern  volatile unsigned char dwatchstrcnt;


extern  volatile char watflg; 
extern  volatile unsigned char allwatcnt;
extern  volatile char wat_h;
extern  volatile char wat_m;

					   
#include "main.h"					   
					   
					   
#pragma once				   
struct path{
	//Laser ON/OFF
	char pow;
	//X
	char x;
	//Y
	char y;
};
#pragma once
struct fpath{
	//Laser ON/OFF
	char pow;
	//X
	char x;
	//Y
	char y;
}; 



extern volatile struct path p[DATA_NUM][ALL_DATA_NUM]__attribute__((far));

extern volatile struct fpath f[FRAME_NUM][ALL_FRAME_NUM]__attribute__((far));
	
extern  void init_path(void);
	
	
	
