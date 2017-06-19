/* * * * * * * * * * * * * * * * * * * * * * * * * *
 * ProjectionBall Firmware For v3.X				   *
 * Copyright (c) 2017  							   *
 * K.Watanabe,Crescent 							   *
 * Released under the MIT license 				   *
 * http://opensource.org/licenses/mit-license.php  *
 * 17/06/16 v1.0 Initial Release                   *
 * 												   *
 * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <p33Fxxxx.h>

#include "timer.h"
#include "delay.h"
#include "outcompare.h"
#include "adc.h"
#include "uart.h"
#include "spi_cstm.h"

//#include "i2c.h"

//SD Card
#define FCY 39613750UL	
#define __dsPIC33E__
#define MEDIA_SOFT_DETECT


#include <libpic30.h>	
#include "HardwareProfile.h"
#include "SD-SPI.c"
#include "FSIO.c"
#define FLAG_TRUE		1
#define FLAG_FALSE		0

#include "main.h"
#include "sysint.h"

#include "font.h"
#include "variable.h"
#include "func.h"




/*Main Control Loop(Interrupt 80~150us)*/
void __attribute__ ((__interrupt__,auto_psv)) _T3Interrupt(void){


	//IFS0bits.ADIF=0;
	IFS0bits.T3IF=0;
	
	time_us++;
	if(time_us>10000-1){time_s++;time_us=0;}
	if(time_s>60-1){time_m++;time_s=0;}
	if(time_m>60-1){time_h++;time_m=0;}
	if(time_h>24-1){date_d++;time_h=0;}
	
	
			
			CK=1;
	//#ifdef DiffD		
			//dx buffer
			x_bit6=x_bit4;
			x_bit7=x_bit5;
			
			x_bit4=x_bit2;
			x_bit5=x_bit3;
			
			x_bit2=x_bit0;
			x_bit3=x_bit1;
	//#endif			
			
	//read encoder data
	#ifdef AS5048A
			x_bit0=(AS5048_0());			
			x_bit1=(AS5048_1());			
	#endif
	
	#ifdef AS5047D
			x_bit0=(AS5047_0());
			x_bit1=(AS5047_1());
	#endif
	
			x_res0=(x_bit0+x_bit2+x_bit4+x_bit6)/4;			
			x_res1=(x_bit1+x_bit3+x_bit5+x_bit7)/4;

			
				
			//Laser ON/OFF Setting
			//XYCommand
			if(PAT_Status==enum_STRING){
					
					if(allstrlen!=0){
						char lsrtmp;
						GetStrPos(	(char*)&allstr, 
									allstrlen, 
									allstrcnt, 
									cnt0, 
									&lsrtmp, 
									(int*)&p_cmd0, 
									(int*)&p_cmd1	);
						LSD=lsrtmp;					
				    }					
			}
			else if(PAT_Status==enum_AWATCH){
						if(0< time_h && time_h <=12)wat_h=120-10*time_h;
						else if(12< time_h && time_h <24)wat_h=240-10*time_h;
						else wat_h=0;
						if(0< time_m && time_m <30)wat_m=120-(time_m*2);
						else if(30<= time_m && time_m <60){
							wat_m=120-(time_m*2); 
							if(wat_h>5)wat_h=wat_h-5;
							else wat_h=wat_h+115;
						}
						else wat_m=0;						
						
						if(allwatcnt==0){
							p_cmd0=awatch_frame_x[cnt0];
							p_cmd1=awatch_frame_y[cnt0];
							LSD=awatch_frame_l[cnt0];
						}
						else if(allwatcnt==1){
							p_cmd0=awatch_min_x[cnt0]*cose[(int)wat_m]
									-awatch_min_y[cnt0]*sine[(int)wat_m];
							p_cmd1=awatch_min_x[cnt0]*sine[(int)wat_m]
									+awatch_min_y[cnt0]*cose[(int)wat_m];
							LSD=awatch_min_l[cnt0];							
						}else {
							p_cmd0=awatch_hour_x[cnt0]*cose[(int)wat_h]
									-awatch_hour_y[cnt0]*sine[(int)wat_h];
							p_cmd1=awatch_hour_x[cnt0]*sine[(int)wat_h]
									+awatch_hour_y[cnt0]*cose[(int)wat_h];
							LSD=awatch_hour_l[cnt0];							
						}					
			}
			else if(PAT_Status==enum_DWATCH){
					if(time_h<10){
						dwatchstrlen=4;
						dwatchstr[0]='0'+(char)time_h;
						dwatchstr[1]=':';
						dwatchstr[2]='0'+(char)time_m/10;
						dwatchstr[3]='0'+(char)time_m%10;						
					}
					else{
						dwatchstrlen=5;
						dwatchstr[0]='0'+(char)time_h/10;
						dwatchstr[1]='0'+(char)time_h%10;
						dwatchstr[2]=':';
						dwatchstr[3]='0'+(char)time_m/10;
						dwatchstr[4]='0'+(char)time_m%10;							
					}
					if(dwatchstrlen!=0){
						char lsrtmp;
						GetStrPos(	(char*)&dwatchstr, 
									dwatchstrlen, 
									dwatchstrcnt, 
									cnt0, 
									&lsrtmp, 
									(int*)&p_cmd0, 
									(int*)&p_cmd1	);
						LSD=lsrtmp;					
				    }					
			}
			else if(PAT_Status==enum_DATE){
					if(date_m<10 && date_d<10){
						dwatchstrlen=3;
						dwatchstr[0]='0'+(char)date_m;
						dwatchstr[1]='/';
						dwatchstr[2]='0'+(char)date_d;											
					}
					else if(date_m>=10 && date_d<10){
						dwatchstrlen=4;
						dwatchstr[0]='0'+(char)date_m/10;
						dwatchstr[1]='0'+(char)date_m%10;
						dwatchstr[2]='/';
						dwatchstr[3]='0'+(char)date_d;								
					}else if(date_m<10 && date_d>=10){
						dwatchstrlen=4;
						dwatchstr[0]='0'+(char)date_m;
						dwatchstr[1]='/';
						dwatchstr[2]='0'+(char)date_d/10;	
						dwatchstr[3]='0'+(char)date_d%10;						
					}else{
						dwatchstrlen=5;
						dwatchstr[0]='0'+(char)date_m/10;
						dwatchstr[1]='0'+(char)date_m%10;
						dwatchstr[2]='/';
						dwatchstr[3]='0'+(char)date_d/10;
						dwatchstr[4]='0'+(char)date_d%10;						
					}					
					if(dwatchstrlen!=0){
						char lsrtmp;
						GetStrPos(	(char*)&dwatchstr, 
									dwatchstrlen, 
									dwatchstrcnt, 
									cnt0, 
									&lsrtmp, 
									(int*)&p_cmd0, 
									(int*)&p_cmd1	);
						LSD=lsrtmp;					
				    }					
			}
			else if(SD_mode==1){
				
				if(PAT_Status==enum_ANIMATION){
					
					p_cmd0=f[(int)frameNum][cnt0].x*CMPRATE;
					p_cmd1=f[(int)frameNum][cnt0].y*CMPRATE;
			
					if(0<(cnt0-LSD_Offset)){
						LSD=f[(int)frameNum][cnt0-LSD_Offset].pow;
					}
					else if(0<cnt0-LSD_Offset+allf_num[(int)frameNum]-1){
						LSD=f[(int)frameNum][cnt0-LSD_Offset+allf_num[(int)frameNum]-1].pow;
					}
					else{
						 LSD=f[(int)frameNum][cnt0].pow;
					}
				}else{
					
					p_cmd0=p[SW_Status][cnt0].x*CMPRATE;
					p_cmd1=p[SW_Status][cnt0].y*CMPRATE;					
				
					if(0<(cnt0-LSD_Offset)){
						LSD=p[SW_Status][cnt0-LSD_Offset].pow;
					}
					else if(0<(cnt0-LSD_Offset+allp_num[SW_Status]-1)){
						LSD=p[SW_Status][cnt0-LSD_Offset+allp_num[SW_Status]-1].pow;
					}
					else{
						LSD=p[SW_Status][cnt0].pow;
					}
				}				
			}
			else{//Internal 
						
				if(PAT_Status==enum_ANIMATION){
				
					if(cnt0>TRIFUNCLEN)cnt0=cnt0-TRIFUNCLEN;
					if(cnt2>TRIFUNCLEN)cnt2=cnt2-TRIFUNCLEN;					
					p_cmd0=AMP*sine[cnt0];
					p_cmd1=AMP*cose[cnt2];					
					LSD=1;
				}
				else if(SW_Status==enum_STAR_DATA0){
					p_cmd0=(int)(starx[cnt0]/3);
					p_cmd1=(int)(stary[cnt0]/3);
					LSD=1;					
				}
				else if(SW_Status==enum_ARROW_DATA1){
					p_cmd0=(((int)arrowx[cnt0]));//+x_cmd2)/2;
					p_cmd1=(((int)arrowy[cnt0]));//+x_cmd3)/2;
					LSD=1;
				}
				else if(SW_Status==enum_MAIL_DATA2){
					p_cmd0=(((int)mailx[cnt0]));//+x_cmd2)/2;
					p_cmd1=(((int)maily[cnt0]));//+x_cmd3)/2;
					LSD=maill[cnt0];
				}
				else if(SW_Status==enum_SMILE_DATA3){
					p_cmd0=((int)nikox[cnt0])/2;
					p_cmd1=((int)nikoy[cnt0])/2;
					LSD=nikol[cnt0];
				}
				else if(SW_Status==enum_HEART_DATA){
					p_cmd0=((int)mk_heart_x[cnt0])/1;
					p_cmd1=((int)mk_heart_y[cnt0])/1;
					LSD=mk_heart_l[cnt0];
				}
#ifdef USE_WETHER_FONT
				else if(SW_Status==enum_SUN_DATA4){
					p_cmd0=mk_sun_x[cnt0];
					p_cmd1=mk_sun_y[cnt0];
					LSD=mk_sun_l[cnt0];
				}
				else if(SW_Status==enum_CLOUD_DATA5){
					p_cmd0=mk_cloud_x[cnt0];
					p_cmd1=mk_cloud_y[cnt0];
					LSD=mk_cloud_l[cnt0];
				}
				else if(SW_Status==enum_RAIN_DATA6){
					p_cmd0=mk_rain_x[cnt0];
					p_cmd1=mk_rain_y[cnt0];
					LSD=mk_rain_l[cnt0];
				}
				else if(SW_Status==enum_SNOW_DATA7){
					p_cmd0=mk_snow_x[cnt0];
					p_cmd1=mk_snow_y[cnt0];
					LSD=mk_snow_l[cnt0];
				}
				else if(SW_Status==enum_THUNDER_DATA8){
					p_cmd0=mk_thunder_x[cnt0];
					p_cmd1=mk_thunder_y[cnt0];
					LSD=mk_thunder_l[cnt0];
				}
#endif //USE_WETHER_FONT				
				
			}		
			
			if(Start_EN!=1)LSD=0;
			
			
			
			//PAT_Status==2 -> LSR Drowing Mode
			if(PAT_Status==2&&LSD==1&&(LSD_num>cnt0)){
				LSD=LSD;
			}
			else if(PAT_Status==2&&LSD==1&&(LSD_num<=cnt0)){
				LSD=0;
			}
			
			if(Init_Count<900)LSD=0;
	
			
			p_cmd2=p_cmd0;
			p_cmd3=p_cmd1;			
		
			p_cmd0=p_cmd2*cose[u_angle]-p_cmd3*sine[u_angle];
			p_cmd1=p_cmd2*sine[u_angle]+p_cmd3*cose[u_angle];	
			
			//dxcmd buffer
			//x_cmd2=x_cmd0;
			//x_cmd3=x_cmd1;
				
				
			//PAT_Status==1 -> Rotate Mode
			if(PAT_Status==1){
				x_cmd0=(int)(x_cen0+p_cmd0*cose[rdeg]);
				x_cmd1=(int)(x_cen1+p_cmd1);
			}
			else{
				x_cmd0=x_cen0+p_cmd0;
				x_cmd1=x_cen1+p_cmd1;
			}
			
			dx_cmd0=0;//x_cmd0-x_cmd2;//0;
			dx_cmd1=0;//x_cmd1-x_cmd3;//0;
		
		
			x_err0=x_cmd0-x_res0;		
			x_err1=x_cmd1-x_res1;
			
	#ifdef EN_PID_I
			x_sum0+=x_err0;
			x_sum1+=x_err1;
	#endif	
			
	#ifdef ERROR_Stop_EN
			if(Start_EN==1){
				if(x_err0>300||x_err0<-300){
					Err_cnt0++;
				}
				else{Err_cnt0=0;} 
				if(x_err1>300||x_err1<-300){
					Err_cnt1++;
				}
				else{Err_cnt1=0;}
			}else{
				Err_cnt0=0;
				Err_cnt1=0;
			}
	#endif
			
			
			
			tau_cmd0=KP*x_err0+KI*x_sum0+KD*(dx_cmd0-dx_res0);
			tau_cmd1=KP*x_err1+KI*x_sum1+KD*(dx_cmd1-dx_res1);
		
			
			OUT0=tau_cmd0;
			OUT1=tau_cmd1;


			
			//Repeat Count
			if(POS_IN_MIN<x_err0&&x_err0<POS_IN_MAX &&POS_IN_MIN<x_err1&&x_err1<POS_IN_MAX){
				cnt1++;
				cnt3++;
			}
			//cnt1++;
			
			//Repeat Check -> Reset or not
			//Point Count RESET
			if(PAT_Status==enum_STRING){
						
						if(cnt1>=4){cnt0++;cnt1=0;x_sum0=0;x_sum1=0;}
						ChkStrCnt(	( char*)&allstr, 
									allstrlen,
									(unsigned char*)&allstrcnt,
									(int*)&cnt0,
									(int*)&cnt1);
						
			
			}
			else if(PAT_Status==enum_AWATCH){
				
				if(cnt1>=4){cnt0++;cnt1=0;x_sum0=0;x_sum1=0;}
				if(allwatcnt==0){
					if(cnt0>=(sizeof(awatch_frame_l)/sizeof(int))){allwatcnt++;cnt0=0;cnt1=-6;}
				}
				else if(allwatcnt==1){
					if(cnt0>=(sizeof(awatch_min_x)/sizeof(int))){allwatcnt++;cnt0=0;cnt1=-6;}
					
				}else{							
					if(cnt0>=(sizeof(awatch_hour_x)/sizeof(int))){allwatcnt=0;cnt0=0;cnt1=-6;}
				}
				
				//if(allstrlen<=allstrcnt){allstrcnt=0;cnt1=-10;}						
			}
			else if(PAT_Status==enum_DWATCH){
				
				if(cnt1>=4){cnt0++;cnt1=0;x_sum0=0;x_sum1=0;}
				ChkStrCnt(	( char*)&dwatchstr, 
							dwatchstrlen,
							(unsigned char*)&dwatchstrcnt,
							(int*)&cnt0,
							(int*)&cnt1);					
			}
			else if(PAT_Status==enum_DATE){
				if(cnt1>=4){cnt0++;cnt1=0;x_sum0=0;x_sum1=0;}
				ChkStrCnt(	( char*)&dwatchstr, 
							dwatchstrlen,
							(unsigned char*)&dwatchstrcnt,
							(int*)&cnt0,
							(int*)&cnt1);					
			}
			else if(SD_mode==1){
			
				if(PAT_Status==enum_ANIMATION){
				
					if(NextFlag==1){
						if(Frame_EN==1){
							frameNum++;
						}
						if(FRAME_NUM<=frameNum){
							frameNum=0;
						}else if(allf_num[(int)frameNum]==0){
							frameNum=0;
						}				
						NextFlag=0;
					}
								
					if(cnt1>=f[(int)frameNum][0].pow){cnt0++;cnt1=0;x_sum0=0;x_sum1=0;}
					if(cnt0>=allf_num[(int)frameNum])cnt0=1;
				}
				else{
				
					if(NextFlag==1){
						if(PAT_Status==1){
							rdeg=rdeg+3;
							if(rdeg>=TRIFUNCLEN-10)rdeg=0;
							
						}
						else if(PAT_Status==2){
							LSD_num=LSD_num+5;
							if(LSD_num>allp_num[SW_Status]+20)LSD_num=0;
						
						}else{
							rdeg=0;						
						}
						NextFlag=0;
					}
				
					if(cnt1>=p[SW_Status][0].pow){cnt0++;cnt1=0;x_sum0=0;x_sum1=0;}
					if(cnt0>=allp_num[SW_Status])cnt0=1;
				}		
			
			}//Internal Mode
			else{
			
				if(PAT_Status==enum_ANIMATION){
				
					if(NextFlag==1){
					
						if(rdeg>12){
							deg0=(rand()%36)*2;
							deg1=(rand()%18)*3;
							rdeg=0;
						}
						else{
								rdeg++;
						}
						NextFlag=0;
					}
						
				
					if(cnt1>=6){cnt0=cnt0+1+deg0;cnt1=0;x_sum0=0;x_sum1=0;}
					if(cnt3>=6){cnt2=cnt2+1+deg1;cnt3=0;x_sum0=0;x_sum1=0;}
						
					if(cnt0>TRIFUNCLEN-10)cnt0=cnt0-TRIFUNCLEN+10;
					if(cnt2>TRIFUNCLEN-10)cnt2=cnt2-TRIFUNCLEN+10;				
				
					if(cnt0>=TRIFUNCLEN-10)cnt0=0;
					if(cnt2>=TRIFUNCLEN-10)cnt2=0;
				}
				else{
				
					if(NextFlag==1){
						if(PAT_Status==enum_ROTATION){
							rdeg=rdeg+3;
							if(rdeg>=TRIFUNCLEN-10)rdeg=0;
						}
						else if(PAT_Status==enum_ONESTROKE){
						
							if(SW_Status==enum_STAR_DATA0){
								LSD_num=LSD_num+5;
								if(LSD_num>sizeof(starx)/sizeof(starx[0])+20)LSD_num=0;
							}
							else if(SW_Status==enum_ARROW_DATA1){
								LSD_num=LSD_num+5;
								if(LSD_num>sizeof(arrowx)/sizeof(arrowx[0])+20)LSD_num=0;
							}
							else if(SW_Status==enum_MAIL_DATA2){
								LSD_num=LSD_num+5;
								if(LSD_num>sizeof(mailx)/sizeof(mailx[0])+20)LSD_num=0;
							}
							else if(SW_Status==enum_SMILE_DATA3){
								LSD_num=LSD_num+5;
								if(LSD_num>sizeof(nikox)/sizeof(nikox[0])+20)LSD_num=0;
							}
							else if(SW_Status==enum_HEART_DATA){
								LSD_num=LSD_num+5;
								if(LSD_num>sizeof(mk_heart_x)/sizeof(mk_heart_x[0])+20)LSD_num=0;
							}
#ifdef USE_WETHER_FONT
							else if(SW_Status==enum_SUN_DATA4){
								LSD_num=LSD_num+5;
								if(LSD_num>sizeof(mk_sun_l)/sizeof(mk_sun_l[0])+20)LSD_num=0;
								
							}
							else if(SW_Status==enum_CLOUD_DATA5){
								LSD_num=LSD_num+5;
								if(LSD_num>sizeof(mk_cloud_l)/sizeof(mk_cloud_l[0])+20)LSD_num=0;
							}
							else if(SW_Status==enum_RAIN_DATA6){
								LSD_num=LSD_num+5;
								if(LSD_num>sizeof(mk_rain_l)/sizeof(mk_rain_l[0])+20)LSD_num=0;
							}
							else if(SW_Status==enum_SNOW_DATA7){
								LSD_num=LSD_num+5;
								if(LSD_num>sizeof(mk_snow_l)/sizeof(mk_snow_l[0])+20)LSD_num=0;
							}
							else if(SW_Status==enum_THUNDER_DATA8){
								LSD_num=LSD_num+5;
								if(LSD_num>sizeof(mk_thunder_l)/sizeof(mk_thunder_l[0])+20)LSD_num=0;
							}	
#endif //USE_WETHER_FONT							
						}
						else{
							LSD_num=0;
							rdeg=0;
						}
						NextFlag=0;
					}
				
					
					if(SW_Status==enum_STAR_DATA0){
						if(cnt1>=4){cnt0++;cnt1=0;x_sum0=0;x_sum1=0;}
						if(cnt0>=sizeof(starx)/sizeof(starx[0]))cnt0=0;
					}
					else if(SW_Status==enum_ARROW_DATA1){
						if(cnt1>=5){cnt0++;cnt1=0;x_sum0=0;x_sum1=0;}//5		
						if(cnt0>=sizeof(arrowx)/sizeof(arrowx[0]))cnt0=0;
					}
					else if(SW_Status==enum_MAIL_DATA2){
						if(cnt1>=5){cnt0++;cnt1=0;x_sum0=0;x_sum1=0;}//5			
						if(cnt0>=sizeof(mailx)/sizeof(mailx[0]))cnt0=0;
					}
					else if(SW_Status==enum_SMILE_DATA3){
						if(cnt1>=4){cnt0++;cnt1=0;x_sum0=0;x_sum1=0;}//4
						if(cnt0>=sizeof(nikox)/sizeof(nikox[0]))cnt0=0;
					}
					else if(SW_Status==enum_HEART_DATA){
						if(cnt1>=4){cnt0++;cnt1=0;x_sum0=0;x_sum1=0;}//4
						if(cnt0>=sizeof(mk_heart_x)/sizeof(mk_heart_x[0]))cnt0=0;
					}
#ifdef USE_WETHER_FONT
					else if(SW_Status==enum_SUN_DATA4){
						if(cnt1>=4){cnt0++;cnt1=0;x_sum0=0;x_sum1=0;}//4
						//if(cnt0>=197)cnt0=0;
						if(cnt0>=sizeof(mk_sun_x)/sizeof(mk_sun_x[0])){cnt0=0;}
					}
					else if(SW_Status==enum_CLOUD_DATA5){
						if(cnt1>=4){cnt0++;cnt1=0;x_sum0=0;x_sum1=0;}//4
						if(cnt0>=sizeof(mk_cloud_l)/sizeof(mk_cloud_l[0]))cnt0=0;
					}
					else if(SW_Status==enum_RAIN_DATA6){
						if(cnt1>=4){cnt0++;cnt1=0;x_sum0=0;x_sum1=0;}//4
						if(cnt0>=sizeof(mk_rain_l)/sizeof(mk_rain_l[0]))cnt0=0;
					}
					else if(SW_Status==enum_SNOW_DATA7){
						if(cnt1>=4){cnt0++;cnt1=0;x_sum0=0;x_sum1=0;}//4
						if(cnt0>=sizeof(mk_snow_l)/sizeof(mk_snow_l[0]))cnt0=0;
					}
					else if(SW_Status==enum_THUNDER_DATA8){
						if(cnt1>=4){cnt0++;cnt1=0;x_sum0=0;x_sum1=0;}//4
						if(cnt0>=sizeof(mk_thunder_l)/sizeof(mk_thunder_l[0]))cnt0=0;
					}
#endif //USE_WETHER_FONT
				}		
			}
			
							
			SetDirctnM1();
			SetDirctnM2();

			Chkxres0();
			Chkxres1();
			

			SetDCOC1PWM(duty0);

			
			
			
			if(Init_Count<500){
				SetDCOC2PWM(0);
				Err_cnt0=0;
				Err_cnt1=0;
			}
			else{
				SetDCOC2PWM(duty1);
			}
			
			if(Init_Count<1000){
				Init_Count++;
				
			}
			

	#ifdef DiffD		

			dx_res0=(x_bit0-x_bit2);
			dx_res1=(x_bit1-x_bit3);
	#endif	

	#ifdef PseudoD
			dx_tmp0 += dx_res0*st;
			dx_res0 = w*(x_res0 - dx_tmp0);
			
			dx_tmp1 += dx_res1*st;
			dx_res1 = w*(x_res1 - dx_tmp1);
	#endif			
	
			CK=0;
				

}







/*Display Interrupt & Animation Control()*/
void __attribute__ ((__interrupt__,auto_psv)) _T1Interrupt(void){

	
	IFS0bits.T1IF=0;

	//Display
	uart_out=1;	
	NextFlag=1;
	

	if(date_m==2 && date_d>28 && date_y%4!=0){date_m++;date_d=1;}
	else if(date_m==2 && date_d>29){date_m++;date_d=1;}
	else if(date_m==4 && date_d>30){date_m++;date_d=1;}
	else if(date_m==6 && date_d>30){date_m++;date_d=1;}
	else if(date_m==9 && date_d>30){date_m++;date_d=1;}
	else if(date_m==11 && date_d>30){date_m++;date_d=1;}
	else if(date_m==12 && date_d>31){date_y++;date_d=1;date_m=1;}
	else if(date_d>31){date_m++;date_d=1;}
	
	
	if(SD_mode==1){
	
	}else{
	
		if(PAT_Status==0){
			if(rdeg>12){				
				printf("Lissajous orbit X:sin(%d/72), Y: cos(%d/72)\n",cnt0,cnt2);
			}
	
		}
			
	}

	//Status Update
	if(UART_mode==0){
		if(SW_Status!=SW){
			SW_Status=SW;
			//Init_Count=0;
		}
		if(PAT_Status!=PATTN){
			PAT_Status=PATTN;
			//Init_Count=0;
		}
	}
	
	
	
}

/*PWM Interrupt(36.86kHz,10bit)*/
void __attribute__ ((__interrupt__,auto_psv)) _T2Interrupt(void){
//void _ISR _T2Interrupt(void){
	//割り込みフラグクリア
	IFS0bits.T2IF=0;

}


 



void __attribute__ ((interrupt, no_auto_psv)) _U1RXInterrupt(void) {
   
   char RxData;
    _U1RXIF = 0;
    if(U1STAbits.OERR){                 // 受信エラーなら
        U1STAbits.OERR = 0;             //    エラークリア
        RxWptr = 0;                     //    既存データはすべて廃棄
		
    }else{                              // 正常受信なら
        RxData = U1RXREG;
		switch(RxData){                 // CRLFを受信したら、
			case 0x0A:                      //   それまでの受信データを
			case 0x0D:                      //   RxRbufに転送する
				if(RxWptr){                 //   ただし、CRLFは転送しない
					//memcpy((char*)RxRbuf, (char*)RxWbuf, RxWptr);
					memcpy((char*)RxRbuf, (char*)RxWbuf, RxWptr);
					RxRptr = RxWptr;        //   バッファ転送し受信数保存
					RxWptr = 0;             //   ポインタリセット
				}
				break;
			default:                        // 一般文字なら
				RxWbuf[RxWptr++] = RxData;  //  受信バッファに保存
				if(RxWptr >= 30)RxWptr = 0; //  容量超えはすべて廃棄
				break;
        }
    }
}

	
int main(void)
{
restart:
	init_variable();
	
	char str[20];
	char Name_tmp[15];
	init_path();

	unsigned int j=0;
	unsigned int k=0;
	UART_mode=0;
	Start_EN=1;
	Frame_EN=1;
	Init_Count=0;
	Debug_mode=0;
	
	NextFlag=0;
	
	LSD_num=0;
	
	rdeg=0;
	rflag=0;
	angle=0;
	u_angle=0;
	
	Err_cnt0=0;
	Err_cnt1=0;
	
	allstrlen=5;
	watflg=1;
	wat_m=0;
	wat_h=0;
	date_d=1;
	date_m=1;
	date_y=17;
	
	strcpy((char*)allstr,"hello");

	
	// SD
	FSFILE * fp;

	//cnt0=36;

	//1: input mode, 0: output mode
	
	TRISA=0b0000000000000011;
	TRISB=0b0000001110000011;
	
	PORTA=0b0000000000000000;
	PORTB=0b1000110000000000;//B10,B11 ENCODER CS
	      //5432109876543210
	
#ifdef TQFP
	TRISC=0b0000001111010000;
	PORTC=0b0000000000000000;
		  //5432109876543210
#endif
	
	AD1PCFGL =0b1111111111110000;
			  

	SPI_CS1=1;
	SPI_CS2=1;


	OSC_setting();
	IO_init();

	__delay_ms(10);	
	
	printf("Initialize Success\n");


	CloseTimer2();
	CloseTimer3();
	CloseTimer1();
	
	__delay_ms(1);	
		
	
	while ( !MDD_MediaDetect() )
	{
		printf("No SD Card. Draw from internal. \n");
		SD_mode=0;//0:Internal memory 1:SD memory
		goto start;
	}

	// SD Initialize
	while (FSInit() == FLAG_FALSE)
	{
        printf("Cannot initialize SD.\n");
		SD_mode=0;//0:Internal memory 1:SD memory
		goto start;
	}
	SD_mode=1;//0:Internal memory 1:SD memory
	//SD_SPI_speedup();

	__delay_ms(2);
	
	
	//!!!!!!!!!!!Load SD Data!!!!!!!!!!!!!
	for(k=0;k<DATA_NUM;k++){
		j=0;		
		
		sprintf(Name_tmp,"data%d.csv",k);
		
		fp = FSfopen( Name_tmp, "r");
		//printf("SD_mode Load data%d.csv \n",k);
		__delay_ms(1);
		while (FSfgets(str,16, fp) != NULL) {
			
			setpath((struct path *)&p[k][j], str, j);
			j++;
			if(j>=ALL_DATA_NUM)break;
		}		
		allp_num[k]=j;
		FSfclose(fp);
		printf("SD_mode data%d num:%d \n",k,j);
		delay_ms(10);
	}
	
	
	//!!!!!!!!!!!Load Frame Data!!!!!!!!!!!!!
	for(k=0;k<FRAME_NUM;k++){
		j=0;	
		
		sprintf(Name_tmp,"frame%d.csv",k);
		
		fp = FSfopen( Name_tmp, "r");
		//printf("SD_mode Load Frame%d \n",k);
		__delay_ms(1);
		while (FSfgets(str,16, fp) != NULL) {
			
			setpath2((struct fpath *)&f[k][j], str, j);
			j++;
			if(j>=ALL_FRAME_NUM)break;
		}		
		allf_num[k]=j;
		FSfclose(fp);
		printf("SD_mode No.%d Frame num:%d \n",k,j);
	}
	
	//while(1);

		
start:
			
		__delay_ms(1);
		//while(1);
	
		init_SPI();
		init_ADC();
		init_PWM();
		
		//CNPU1=0xFFFF;
		//CNPU2=0x0001;
		
#ifdef	AS5047D
		//ADC Center Position Read
		x_cen0=PCEN0+(readADC0()/3-682);//12000
		x_cen1=PCEN1+(readADC1()/3-682);//12000
		printf("Center Pos X:%d, Y:%d\n",x_cen0,x_cen1);
#endif	

#ifdef	AS5048A
		//ADC Center Position Read
		x_cen0=PCEN0+(readADC0()/2-1024);
		x_cen1=PCEN1+(readADC1()/2-1024);
		printf("Center Pos X:%d, Y:%d\n",x_cen0,x_cen1);
#endif	


	
	//Status Update
	if(UART_mode==0){
		SW_Status=SW;
		PAT_Status=PATTN;	
	}
	
	srand((x_cen0+x_cen1)%100);
	
	//printf("Initialize Timer\n");
	init_TIMER();
	

#ifdef TQFP
	STBY_ON;
#endif
	


		
	while(1){
	
	
	
	
	#ifdef ERROR_Stop_EN
		if(Err_cnt0>900||Err_cnt1>900){
		
		#ifdef TQFP
			STBY_OFF;
		#endif
			CloseTimer2();
			CloseTimer3();
			CloseTimer1();
			LSD=0;
			M1IN1_OFF;
			M1IN2_OFF;
			M2IN3_OFF;
			M2IN4_OFF;
			printf("\n Safety Stop!! Check Device!!\n");
			Err_cnt0=0;
			Err_cnt1=0;
			ERROR_LED_ON;
		
		}
	#endif
			
			
		if(RxRptr!=0){                   
         
			printf(" %s ",RxRbuf);  
		   
			if(strcmp((char*)RxRbuf,Str_Switch_Mode) == 0){
				UART_mode=0;
				printf("**Switch Mode** \n");
			}
			else if(strcmp((char*)RxRbuf,Str_Uart_Mode) == 0){
				UART_mode=1;
				printf("**UART Mode** \n");
			}
			else if(strcmp((char*)RxRbuf,Str_Start) == 0){
				Start_EN=1;
				Init_Count=0;
				printf("**Start** \n");
			}
			else if(strcmp((char*)RxRbuf,Str_Stop) == 0){
				Start_EN=0;
				Init_Count=0;
				printf("**STOP** \n");
			}
			else if(strcmp((char*)RxRbuf,Str_Frame_Mode) == 0){
				PAT_Status=enum_ANIMATION;				
				printf("**Frame Mode** \n");
			}
			else if(strcmp((char*)RxRbuf,Str_SW0) == 0){
				SW_Status=enum_STAR_DATA0;
				printf("**SW 0** \n");
			}
			else if(strcmp((char*)RxRbuf,Str_SW1) == 0){
				SW_Status=enum_ARROW_DATA1;
				printf("**SW 1** \n");
			}
			else if(strcmp((char*)RxRbuf,Str_SW2) == 0){
				SW_Status=enum_MAIL_DATA2;
				printf("**SW 2** \n");
			}
			else if(strcmp((char*)RxRbuf,Str_SW3) == 0){
				SW_Status=enum_SMILE_DATA3;
				printf("**SW 3** \n");
			}
			else if(strcmp((char*)RxRbuf,Str_SW4) == 0){
				SW_Status=enum_SUN_DATA4;
				printf("**SW 4** \n");
			}
			else if(strcmp((char*)RxRbuf,Str_SW5) == 0){
				SW_Status=enum_CLOUD_DATA5;
				printf("**SW 5** \n");
			}
			else if(strcmp((char*)RxRbuf,Str_SW6) == 0){
				SW_Status=enum_RAIN_DATA6;
				printf("**SW 6** \n");
			}
			else if(strcmp((char*)RxRbuf,Str_SW7) == 0){
				SW_Status=enum_SNOW_DATA7;
				printf("**SW 7** \n");
			}
			else if(strcmp((char*)RxRbuf,Str_SW8) == 0){
				SW_Status=enum_THUNDER_DATA8;
				printf("**SW 8** \n");
			}
			else if(strcmp((char*)RxRbuf,Str_SW9) == 0){
				SW_Status=9;
				printf("**SW 9** \n");
			}
			else if(strcmp((char*)RxRbuf,Str_Rotation) == 0){
				PAT_Status=enum_ROTATION;
				printf("**Rotation** \n");
			}
			else if(strcmp((char*)RxRbuf,Str_Stroke) == 0){
				PAT_Status=enum_ONESTROKE;
				printf("**Stroke** \n");
			}
			else if(strcmp((char*)RxRbuf,Str_Always) == 0){
				PAT_Status=enum_ALWAYS;
				printf("**Always** \n");
			}
			else if(strcmp((char*)RxRbuf,Str_AnalogWatch) == 0){
				PAT_Status=enum_AWATCH;
				printf("**AnalogWatch** \n");
			}
			else if(strcmp((char*)RxRbuf,Str_DigitalWatch) == 0){
				PAT_Status=enum_DWATCH;
				printf("**DigitalWatch** \n");
			}
			else if(strcmp((char*)RxRbuf,Str_Day) == 0){
				PAT_Status=enum_DATE;
				printf("**Day** \n");
			}
			else if(strcmp((char*)RxRbuf,Str_StringMode) == 0){
				PAT_Status=enum_STRING;
				printf("**String** \n");
			}			
			else if(strcmp((char*)RxRbuf,Str_SD_Mode) == 0){
				SD_mode=1;
				printf("**SD Mode** \n");
			}
			else if(strcmp((char*)RxRbuf,Str_Internal_Mode) == 0){
				SD_mode=0;
				printf("**Internal Mode** \n");
			}
			else if(strcmp((char*)RxRbuf,Str_Debug_Mode) == 0){
				if(Debug_mode==0)Debug_mode=1;
				else Debug_mode=0;
				printf("**Debug Mode** \n");
			}			
			else if(strcmp((char*)RxRbuf,Str_FramePause) == 0){
				if(Frame_EN==0)Frame_EN=1;
				else Frame_EN=0;
				printf("**Frame Pause  Status: %d ** \n",Frame_EN);
			}
			else if(strcmp((char*)RxRbuf,Str_Restart) == 0){
				Start_EN=0;				
				printf("**Restart** \n");
				for(k=0;k<20;k++){
					RxRbuf[k]=0;
				}
				RxRptr = 0;               
				RxWptr = 0;
				goto restart;
			}
			else if(strncmp((char*)RxRbuf,Str_Center_Pos,4) == 0){
				char *tok;
				char find_num=0;
				tok = strtok( (char*)RxRbuf, Find_String);
				//printf("tok1: %s\n",tok);
				printf("**Center Pos X:%d, Y:%d ->",x_cen0,x_cen1);
				while( tok != NULL ){
					//printf( "%s\n", tok );
					tok = strtok( NULL, Find_String);
					//printf("tok2: %s\n",tok);
					if(find_num==0)x_cen0=atoi(tok)+x_cen0;
					if(find_num==1)x_cen1=atoi(tok)+x_cen1;
					find_num++;					
				}				
				printf(" X:%d, Y:%d **\n",x_cen0,x_cen1);
			}
			else if(strncmp((char*)RxRbuf,Str_FrameNo,4) == 0){
				char *p;
				int tmp_no=0;
				p=strpbrk((char*)RxRbuf, "1234567890" );				
				printf("**Frame No : %d -> ",frameNum);
				if ( p != NULL ) tmp_no = atoi( p );
				
				if( tmp_no < 0 )frameNum=0;
				else if(FRAME_NUM<=frameNum)frameNum=0;
				else frameNum=tmp_no;				
				printf("%d  **\n",frameNum);	
			
			}
			else if(strncmp((char*)RxRbuf,Str_Angle_Deg,4) == 0){
				char *p;
				int tmp_ang=0;
				p=strpbrk( (char*)RxRbuf, "1234567890" );
				
				printf("**Angle : %d degree -> ",angle);
				if ( p != NULL ) tmp_ang = atoi( p );
				
				if(0<=tmp_ang && tmp_ang<360){
					angle=tmp_ang/((int)360/TRIFUNCLEN);
					angle=angle*((int)360/TRIFUNCLEN);
				}
				else angle=0;
				
				u_angle=(int)angle/((int)360/TRIFUNCLEN);
						
				
				printf("%d degree **\n",angle);
			}
			else if(strncmp((char*)RxRbuf,Str_GetString,4) == 0){
				char *p;
				p=(char*)RxRbuf;//strtok(allstr, "=");
				for(k=0;k<32;k++){
					allstr[k]=0;
				}
				char tmplen=strlen((char*)RxRbuf)-4;//p);
				if(tmplen>0)strncpy((char*)allstr,(char*)&RxRbuf[4],tmplen);
				
				if(GetStringMaxLen<tmplen)tmplen=GetStringMaxLen;
				allstrlen=tmplen;
				printf("**GetString Len:%d, STR:\"%s\" **",tmplen,allstr);
				
			}
			else if(strncmp((char*)RxRbuf,Str_SetTime,4) == 0){
				char *p;
				long time=0;
				unsigned int h,m,s;
				p=strpbrk((char*) RxRbuf, "1234567890" );
				if ( p != NULL ) time = atol( p );
				//printf("TIME: %ld",time);
				h=time/10000;
				m=(time/100)%100;
				s=time%100;
				if(0<=h && h <24)time_h=h;
				if(0<=m && m <60)time_m=m;
				if(0<=s && s <60)time_s=s;							
				printf("**Get: %02d:%02d:%02d NOW: %02d:%02d:%02d **\n\r",
						h,m,s,time_h,time_m,time_s);
				
				
			}
			else if(strncmp((char*)RxRbuf,Str_SetDate,4) == 0){
				char *p;
				long date=0;
				unsigned int y,m,d;
				p=strpbrk( (char*)RxRbuf, "1234567890" );
				if ( p != NULL ) date = atol( p );
				y=date/10000;
				m=(date/100)%100;
				d=date%100;
				if(0<=y && y <99)date_y=y;
				if(0<=m && m <13)date_m=m;
				if(0<=d && d <32)date_d=d;							
				printf("**Get: %02d/%02d/%02d NOW: %02d/%02d/%02d **\n\r",
						y,m,d,date_y,date_m,date_d);			
			}		
			else if(strncmp((char*)RxRbuf,Str_GetTime,4) == 0){
				printf("**NOW: %02d:%02d:%02d **\n\r",time_h,time_m,time_s);
			}
			else if(strncmp((char*)RxRbuf,Str_GetDate,4) == 0){
				printf("**NOW: 20%02d/%02d/%02d **\n\r",date_y,date_m,date_d);
			}
			
			for(k=0;k<20;k++){
				RxRbuf[k]=0;
			}
		 	RxRptr = 0;               
			RxWptr = 0; 			
            printf("\r\n");         
		}
		

			
		if(uart_out==1&& Debug_mode==1){
			

	
			printf("x_cmd0: %d\t x_res0: %d\t x_cmd1: %d\t x_res1: %d ERR: %d\n",
					x_cmd0,x_res0,x_cmd1,x_res1,Err_cnt0+Err_cnt1);

			uart_out=0;
		}
			
			
	}
}
 
