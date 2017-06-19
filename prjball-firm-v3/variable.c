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
#include "variable.h"
	
	//UART Display
	volatile unsigned char uart_out;
	volatile unsigned char SW_Status;
	volatile unsigned char PAT_Status;
	volatile unsigned int Init_Count;

	
	volatile int duty0, duty1,duty2,duty3;
	volatile int OUT0, OUT1;
	volatile unsigned int x_bit0,x_bit1,x_bit2,x_bit3,x_bit4,x_bit5,x_bit6,x_bit7;
	
	
	
	volatile int x_res0,dx_res0,x_err0,x_sum0,x_cmd0,x_cmd2,dx_cmd0;
	volatile int x_res1,dx_res1,x_err1,x_sum1,x_cmd1,x_cmd3,dx_cmd1;
	volatile int dx_tmp0;
	volatile int dx_tmp1;
	
	volatile int cnt0,cnt1,cnt2,cnt3;
	
	//int max_val;
	
	volatile int tau_cmd0;
	volatile int tau_cmd1;
	
	volatile int x_cen0;
	volatile int x_cen1;
	
	
	//float amp;
	
	volatile unsigned char RxWptr,RxRptr;
	volatile char RxWbuf[32],RxRbuf[32];
	
	
	volatile unsigned char allp_num[DATA_NUM];
	// int allq_num;
	// int allr_num;
	// int alls_num;
	volatile unsigned char allf_num[FRAME_NUM];
	
	volatile unsigned char  LSD_num;
	//int LSD_flag;

	volatile int deg0;
	volatile int deg1;
	volatile int rdeg;
	volatile char rflag;
	volatile int angle;
	volatile int u_angle;
	volatile char SD_mode;//0:Internal memory 1:SD memory
	volatile char UART_mode;//0:SW Mode 1:Uart Mode
	volatile char Start_EN;//0:OFF 1:ON
	volatile char Debug_mode;//0:OFF,1:ON
	volatile char Frame_EN;//0:OFF,1:ON

	volatile unsigned char frameNum;
	volatile unsigned char NextFlag;

	volatile int p_cmd0;
	volatile int p_cmd1;
	volatile int p_cmd2;
	volatile int p_cmd3;

	volatile unsigned int Err_cnt0;
	volatile unsigned int Err_cnt1;	
	
	volatile char time_h;
	volatile char time_m;
	volatile char time_s;
	volatile char date_d;
	volatile char date_m;
	volatile char date_y;
	volatile int time_us;//10000
	
void init_variable(void){


	//char uartdata[7];
	//int count=0;
	unsigned char i=0;
	duty0=0, duty1=0; 
	OUT0=0, OUT1=0;
	x_bit0=0,x_bit1=0,x_bit2=0,x_bit3=0;

	x_res0=0,dx_res0=0,x_err0=0,x_sum0=0,x_cmd0=0,dx_cmd0=0;x_cmd2=0;
	x_res1=0,dx_res1=0,x_err1=0,x_sum1=0,x_cmd1=0,dx_cmd1=0;x_cmd3=0;
	//max_val=0;
	cnt0=0;	cnt1=0;
	//amp=1.0;
	
	tau_cmd0=0;//tau_dob_tmp0=0;tau_dob_tmp2=0;tau_dob_tmp4=0;
	tau_cmd1=0;//tau_dob_tmp1=0;tau_dob_tmp3=0;tau_dob_tmp5=0;
	
	for(i=0;i<20;i++){
		allf_num[i]=0;
	}
	

}
	
	

	const float sine[120]={	0.0f,	0.052f,	0.104f,	0.156f,	0.207f,	
							0.258f,	0.309f,	0.358f,	0.406f,	0.453f,	
							0.5f,	0.544f,	0.587f,	0.629f,	0.669f,	
							0.707f,	0.743f,	0.777f,	0.809f,	0.838f,	
							0.866f,	0.891f,	0.913f,	0.933f,	0.951f,	
							0.965f,	0.978f,	0.987f,	0.994f,	0.998f,	
							1.0f,	0.998f,	0.994f,	0.987f,	0.978f,	
							0.965f,	0.951f,	0.933f,	0.913f,	0.891f,	
							0.866f,	0.838f,	0.809f,	0.777f,	0.743f,	
							0.707f,	0.669f,	0.629f,	0.587f,	0.544f,	
							0.5f,	0.453f,	0.406f,	0.358f,	0.309f,	
							0.258f,	0.207f,	0.156f,	0.104f,	0.052f,	
							0.0f,	-0.052f,	-0.104f,	-0.156f,	-0.207f,	
							-0.258f,	-0.309f,	-0.358f,	-0.406f,	-0.453f,	
							-0.5f,	-0.544f,	-0.587f,	-0.629f,	-0.669f,	
							-0.707f,	-0.743f,	-0.777f,	-0.809f,	-0.838f,	
							-0.866f,	-0.891f,	-0.913f,	-0.933f,	-0.951f,	
							-0.965f,	-0.978f,	-0.987f,	-0.994f,	-0.998f,	
							-1.0f,	-0.998f,	-0.994f,	-0.987f,	-0.978f,	
							-0.965f,	-0.951f,	-0.933f,	-0.913f,	-0.891f,	
							-0.866f,	-0.838f,	-0.809f,	-0.777f,	-0.743f,	
							-0.707f,	-0.669f,	-0.629f,	-0.587f,	-0.544f,	
							-0.5f,	-0.453f,	-0.406f,	-0.358f,	-0.309f,	
							-0.258f,	-0.207f,	-0.156f,	-0.104f,	-0.052f};					 
						 
						
						 
						 
	const float cose[120]={ 1.0f,	0.998f,	0.994f,	0.987f,	0.978f,	
							0.965f,	0.951f,	0.933f,	0.913f,	0.891f,	
							0.866f,	0.838f,	0.809f,	0.777f,	0.743f,	
							0.707f,	0.669f,	0.629f,	0.587f,	0.544f,	
							0.5f,	0.453f,	0.406f,	0.358f,	0.309f,	
							0.258f,	0.207f,	0.156f,	0.104f,	0.052f,	
							0.0f,	-0.052f,	-0.104f,	-0.156f,	-0.207f,	
							-0.258f,	-0.309f,	-0.358f,	-0.406f,	-0.453f,	
							-0.5f,	-0.544f,	-0.587f,	-0.629f,	-0.669f,	
							-0.707f,	-0.743f,	-0.777f,	-0.809f,	-0.838f,	
							-0.866f,	-0.891f,	-0.913f,	-0.933f,	-0.951f,	
							-0.965f,	-0.978f,	-0.987f,	-0.994f,	-0.998f,	
							-1.0f,	-0.998f,	-0.994f,	-0.987f,	-0.978f,	
							-0.965f,	-0.951f,	-0.933f,	-0.913f,	-0.891f,	
							-0.866f,	-0.838f,	-0.809f,	-0.777f,	-0.743f,	
							-0.707f,	-0.669f,	-0.629f,	-0.587f,	-0.544f,	
							-0.5f,	-0.453f,	-0.406f,	-0.358f,	-0.309f,	
							-0.258f,	-0.207f,	-0.156f,	-0.104f,	-0.052f,	
							0.0f,	0.052f,	0.104f,	0.156f,	0.207f,	
							0.258f,	0.309f,	0.358f,	0.406f,	0.453f,	
							0.5f,	0.544f,	0.587f,	0.629f,	0.669f,	
							0.707f,	0.743f,	0.777f,	0.809f,	0.838f,	
							0.866f,	0.891f,	0.913f,	0.933f,	0.951f,	
							0.965f,	0.978f,	0.987f,	0.994f,	0.998f};	
					 


	
	
	
	
		
const int starx[168]={   0,  -9, -16, -25, -29, -36, -42, -49, -57, -66,
					   -74, -81, -88, -94,-103,-110,-118,-125,-132,-141,
					  -148,-155,-164,-173,-180,-188,-194,-203,-212,-219,
					  -226,-233,-243,-217,-197,-175,-154,-135,-111, -88,
					   -67, -40, -13,  16,  36,  53,  72,  98, 123, 145,
					   168, 188, 210, 226, 247, 269, 295, 317, 336, 353,
					   373, 398, 367, 342, 318, 294, 277, 254, 227, 199,
					   177, 152, 128, 104,  80,  50,  27,  -1, -28, -48,
					   -71, -91,-116,-137,-165,-194,-211,-240,-261,-294,
					  -314,-333,-352,-379,-400,-371,-347,-326,-312,-291,
					  -273,-255,-238,-220,-202,-185,-160,-141,-116, -96,
					   -74, -55, -36, -11,   7,  26,  43,  63,  80,  99,
					   118, 137, 154, 168, 190, 208, 228, 248, 239, 234,
					   225, 218, 210, 206, 200, 192, 185, 179, 171, 165,
					   159, 152, 145, 138, 129, 125, 121, 115, 111, 105,
					    98,  94,  84,  77,  73,  68,  61,  56,  50,  44,
						37,  34,  26,  20,  16,  13,   7,   0};
						
const int stary[168]={ 375, 354, 330, 305, 286, 268, 249, 228, 205, 178,
					   153, 134, 112,  89,  65,  42,  18,  -1, -23, -50,
					   -74, -98,-123,-149,-174,-196,-221,-245,-269,-293,
					  -314,-336,-371,-356,-341,-326,-311,-298,-281,-266,
					  -250,-230,-212,-190,-178,-165,-153,-133,-115, -97,
					   -81, -66, -50, -38, -23,  -6,  11,  28,  41,  52,
					    69,  90,  90,  90,  90,  90,  90,  90,  90,  90,
						90,  90,  90,  90,  90,  90  ,90,  90,  90,  90,
						90,  90,  90,  90,  90,  90,  90,  90,  90,  90,
						90,  90,  90,  90,  90,  70,  55,  41,  27,  15,
						 1, -10, -25, -38, -47, -62, -77, -93,-107,-127,
				      -138,-154,-169,-182,-200,-215,-229,-240,-255,-267,
					  -279,-293,-304,-319,-334,-346,-360,-375,-345,-320,
					  -298,-274,-257,-236,-217,-195,-176,-158,-132,-118,
					   -97, -74, -54, -35, -11,   7,  28,  43,  57,  74,
					    91, 104, 127, 149, 168, 182, 203, 219, 231, 253,
					   275, 292, 307, 319, 338, 352, 368, 375};

const int maill[164]={0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
					  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
					  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
					  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
					  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
					  1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
					  0,0,0,0,0,0,0,0,0,0,0,0,0,0};

const int mailx[164]={-105,-104,-104,-104,-104,-104,-104,-104,-104,-104,
					  -104,-104,-104,-104,-104,-104,-104,-104,-104,-105,
					  -105,-105,-105, -97, -89, -81, -73, -65, -57, -49,
					   -41, -33, -25, -17,  -9,  -1,   7,  15,  23,  31,
					    39,  47,  55,  63,  71,  79,  87,  95, 104, 104,
					   104, 104, 104, 104, 104, 104, 104, 104, 104, 104,
					   104, 104, 104, 104, 104, 104, 104, 103, 103, 103,
					   103,  95,  87,  79,  71,  63,  55,  47,  39,  31,
					    23,  15,   1,   1,   1,   1,   1,   1,  -7, -15,
					   -23, -31, -39, -47, -55, -63, -71, -79, -87, -95,
					  -105,-105,-105,-105,-105,-105, -97, -89, -81, -73,
					   -65, -57, -49, -41, -33, -25, -17,  -9,  -1,   7,
					    15,  23,  31,  39,  47,  55,  63,  71,  79,  87,
						95, 114, 114, 112, 112, 108, 108,  95,  87,  79,
						71,  63,  55,  47,  39,  31,  23,  15,   7,  -1,
						-9, -17, -25, -33, -41, -49, -57, -65, -73, -81,
					   -89,-104,-104,-104};//165

const int maily[164]={	72,  72,  72,  72,  64,  56,  48,  40,  32,  24,
						16,   8,   0,  -8, -16, -24, -32, -40, -48, -63,
					   -62, -62, -62, -62, -62, -62, -62, -62, -62, -62,
					   -62, -62, -62, -62, -62, -62, -62, -62, -62, -62,
					   -62, -62, -62, -62, -62, -62, -62, -62, -64, -64,
					   -64, -64, -56, -48, -40, -32, -24, -16,  -8,   0,
					     8,  16,  24,  32,  40,  48,  56,  71,  71,  71,
						71,  65,  59,  53,  47,  41,  35,  29,  23,  17,
						11,   5,  -1,  -1,  -1,  -1,  -1,  -1,   4,   9,
						14,  19,  24,  29,  34,  39,  44,  49,  54,  59,
						72,  72,  72,  72,  72,  72,  72,  72,  72,  72,
						72,  72,  72,  72,  72,  72,  72,  72,  72,  72,
						72,  72,  72,  72,  72,  72,  72,  72,  72,  72,
						72,  71,  71,  71,  72,  72,  72,  72,  72,  72, 
						72,  72,  72,  72,  72,  72,  72,  72,  72,  72,
						72,  72,  72,  72,  72,  72,  72,  72,  72,  72,
						72,  74,  74,  74};//165
						

	
	
const int nikox[125]={   0, -15, -30, -47, -63, -79, -95,-111,-129,-144,
					  -156,-171,-182,-192,-201,-209,-215,-220,-222,-223,
					  -223,-220,-215,-208,-201,-191,-181,-169,-157,-142,
					  -129,-116,-103, -89, -74, -57, -39, -24,  -6,  14,
					    33,  51,  69,  86, 103, 121, 135, 149, 164, 175,
					   186, 195, 202, 208, 212, 214, 215, 214, 213, 208,
					   200, 192, 183, 174, 163, 151, 139, 126, 112, 100,
					    86,  70,  57,  41,  23,   2, -12, -26, -38, -47,
					   -57, -66, -72, -77, -81, -83, -83, -83, -83, -83,
					   -83, -82, -80, -77, -75, -70, -57, -41, -25,  -6,
					    13,  31,  48,  65,  83,  88,  92,  91,  88,  85,
						81,  80,  81,  81,  81,  80,  75,  70,  66,  60,
						51,  39,  23,   2};//120



const int nikoy[125]={ 186, 190, 190, 186, 180, 176, 170, 163, 154, 144,
					   134, 121, 108,  95,  80,  65,  49,  32,  16,  -3,
					   -22, -40, -57, -74, -90,-105,-119,-131,-144,-155,
					  -163,-170,-177,-183,-187,-192,-195,-197,-199,-198,
					  -194,-190,-185,-178,-170,-162,-151,-140,-129,-113,
					   -99, -84, -71, -54, -36, -18,   1,  18,  35,  51,
					    67,  82,  96, 108, 120, 132, 141, 149, 158, 164,
					   171, 176, 182, 188, 205, 205, 195, 176, 167, 155,
					   144, 128, 113,  96,  81,  62,  47,  33,  19,   1,
					   -18, -36, -56, -74, -91,-107,-116,-123,-126,-129,
					  -128,-126,-122,-116,-107, -90, -70, -50, -30, -15,
					     3,  17,  35,  50,  64,  83, 101, 119, 135, 150,
					   160, 175, 182,186};//120

const char nikol[125]={0,0,0,0,0,0,1,1,1,1,
					   1,1,1,1,1,1,1,1,1,1,
					   1,1,1,1,1,1,1,1,1,1,
					   1,1,1,1,1,1,1,1,1,1,
					   1,1,1,1,1,1,1,1,1,1,
					   1,1,1,1,1,1,1,1,1,1,
					   1,1,1,1,1,1,1,1,1,1,
					   1,1,1,1,1,1,1,1,1,1,
					   1,1,0,0,0,0,0,0,0,0,
					   1,1,1,1,0,0,0,0,0,0,
					   0,0,1,1,1,1,1,1,1,1,
					   1,0,0,0,0,0,1,1,1,1,
					   0,0,0,0};//120

const int arrowx[108]={   0,   0,   0,   0,   0,  -8, -16, -24, -32, -40, 
					    -48, -56, -64, -72, -80, -88, -96,-109,-109,-109,
					   -109,-109,-101, -93, -85, -77, -69, -61, -50, -50,
					    -50, -50, -50, -50, -50, -50, -50, -50, -50, -50,
						-50, -50, -50, -50, -50, -50, -50, -50, -50, -50,
						-42, -34, -26, -18, -10,  -2,   6,  14,  22,  30,
						 38,  49,  49,  49,  49,  49,  49,  49,  49,  49,
						 49,  49,  49,  49,  49,  49,  49,  48,  48,  48,
						 48,  48,  56,  64,  72,  80,  88,  96, 108, 108,
						108, 108, 108, 101,  94,  87,  80,  73,  66,  59,
						 52,  45,  38,  31,  24,  17,   1,   1};


						 
					


					
const int arrowy[108]={ 108, 108, 108, 108, 108, 100,  92,  84,  76,  68,
						 60,  52,  44,  36,  28,  20,  10,  -5,  -5,  -5,
						 -5,  -5,  -5,  -5,  -5,  -5,  -5,  -5,  -8,  -8,
						 -8,  -8,  -8, -10, -16, -24, -32, -40, -48, -56,
						-64, -72, -80, -88, -96,-110,-110,-110,-110,-110,
					   -110,-110,-110,-110,-110,-110,-110,-110,-110,-110,
					   -110,-110,-110,-110,-110,-110,-102, -94, -86, -78,
					    -70, -62, -54, -46, -38, -30, -22, -8, -8, -8,
						-8, -8, -8, -8, -8, -8, -8, -8, -8, -4,
						-4, -4, -4,  -3,   5,  13,  21,  29,  37,  45,
						 53,  61,  69,  77,  85,  93, 108, 108};

						 
						 
						 

					   
					   
					   
const int StrOffset[7][6]={	{0,		0,		0,		0,		0,		0},
							{0,		0,		0,		0,		0,		0},
							{-50,	50,		0,		0,		0,		0},
							{-100,	0,		100,	0,		0,		0},
							{-140,	-45,	45,		140,	0,		0},
							{-160,	-80,	0,		80,		160,	0},
							{-200,	-120,	-40,	40,		120,	200},
							};					   


						
						
 volatile  char allstr[32];
 volatile unsigned char allstrlen;
 volatile unsigned char allstrcnt;
 
 volatile char dwatchstr[8];
 volatile unsigned char dwatchstrlen;
 volatile unsigned char dwatchstrcnt;
 
 
 volatile unsigned char allwatcnt;
 
 volatile char watflg; 
 volatile char wat_h;
 volatile char wat_m;
					   


volatile struct path p[DATA_NUM][ALL_DATA_NUM]__attribute__((far));


volatile struct fpath f[FRAME_NUM][ALL_FRAME_NUM]__attribute__((far));


	
void init_path(void){
		
		unsigned char i=0;
		unsigned char j=0;
		
		for(i=0;i<DATA_NUM;i++){
			for(j=0;j<ALL_DATA_NUM;j++){
				p[i][j].x=0;
				p[i][j].y=0;
				p[i][j].pow=0;
			}
		}
		
		for(i=0;i<FRAME_NUM;i++){
			for(j=0;j<ALL_FRAME_NUM;j++){
				p[i][j].x=0;
				p[i][j].y=0;
				p[i][j].pow=0;
			}
		}
		
		
		
}
	
	
	
	
