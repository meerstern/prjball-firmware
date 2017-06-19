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
#include "func.h"
#include "main.h"

#include "spi_cstm.h"
#include <libpic30.h>	
#include "HardwareProfile.h"
#include "SD-SPI.h"
#include "FSIO.h"
#include "font.h"
#include "variable.h"



unsigned int AS5048_0(void){


	unsigned int data;
	
	//SPI_CS1=0;
	SPICS1_OFF;
	putcSPI2(0xFFFF);
	
	while(SPI2STATbits.SPITBF);
	while(!SPI2STATbits.SPIRBF);
	//while(!DataRdySPI1());
			
	data=getcSPI2()&0x3FFF;
	SPICS1_ON;
	//SPI_CS1=1;
	
	return data;
	
}

unsigned int AS5048_1(void){


	unsigned int data;
	
	//SPI_CS2=0;
	SPICS2_OFF;
	putcSPI2(0xFFFF);
	
	while(SPI2STATbits.SPITBF);
	while(!SPI2STATbits.SPIRBF);
	//while(!DataRdySPI1());
			
	data=getcSPI2()&0x3FFF;
	//SPI_CS2=1;
	SPICS2_ON;
	return data;

}


unsigned int AS5047_0(void){


	unsigned int data;
	
	//SPI_CS1=0;
	SPICS1_OFF;
	putcSPI2(0x3FFF);
	
	while(SPI2STATbits.SPITBF);
	while(!SPI2STATbits.SPIRBF);
	//while(!DataRdySPI1());
			
	data=getcSPI2()&0x3FFF;
	SPICS1_ON;
	//SPI_CS1=1;
	
	return data;
	
}

unsigned int AS5047_1(void){


	unsigned int data;
	
	//SPI_CS2=0;
	SPICS2_OFF;
	putcSPI2(0x3FFF);
	
	while(SPI2STATbits.SPITBF);
	while(!SPI2STATbits.SPIRBF);
	//while(!DataRdySPI1());
			
	data=getcSPI2()&0x3FFF;
	//SPI_CS2=1;
	SPICS2_ON;
	return data;

}


int FSfgetc(FSFILE *fp){
	char c;
	//volatile char c;
	if(FSfread(&c,1,1,fp) != 1){
		//while(1);
	}
	return (int)c;
}

//char *FSfgets(volatile char *s,volatile int n,FSFILE * stream){
char *FSfgets(char *s, int n, FSFILE * stream){
          
    char *const head = s;       
    for (; n > 1; n--) {
		int c =FSfgetc(stream);    
        if(c == EOF)break;
        *s = c;                   
        s++;                  
        if (c == '\n')break;
    }
    
    if (s == head || FSerror()){
		return NULL;
		
	}
    *s = NULL;
    return head;
   
}








void setpath(struct path* pR, char* str, unsigned int num) {
	unsigned char i = 0;
	int temp=0;
	
	char* pStr = strtok(&str[0], "\",\"");
	while (pStr) {
		switch (i) {
		case 0:
			//Laser ON/OFF
			temp = atoi(pStr);
			LsrCmdChk(&temp, num);
			pR->pow = temp;
			break;
		case 1:
			//X
			temp = atoi(pStr)/CMPRATE;
			PosCmdChk(&temp);
			pR->x = temp;
			break;
		case 2:
			//Y
			temp = atoi(pStr)/CMPRATE;
			PosCmdChk(&temp);
			pR->y = temp;
			break;
		default:
			break;
		}
		i++;
	
		pStr = strtok(NULL, "\",\"");
		if (pStr == NULL ) {
			
			pStr = strtok(NULL, "\"\n");
		}
	}
 
}

void setpath2(struct fpath* pR, char* str, unsigned int num) {
	unsigned char i = 0;
	int temp=0;
	
	char* pStr = strtok(&str[0], "\",\"");
	while (pStr) {
		switch (i) {
		case 0:
			//Laser ON/OFF
			temp = atoi(pStr);
			LsrCmdChk(&temp, num);
			pR->pow = temp;
			break;
		case 1:
			//X
			temp = atoi(pStr)/CMPRATE;
			PosCmdChk(&temp);
			pR->x = temp;
			break;
		case 2:
			//Y
			temp = atoi(pStr)/CMPRATE;
			PosCmdChk(&temp);
			pR->y = temp;
			break;
		default:
			break;
		}
		i++;
		
		pStr = strtok(NULL, "\",\"");
		if (pStr == NULL ) {
			
			pStr = strtok(NULL, "\"\n");
		}
	}
 
}

void PosCmdChk(int *cmd){
	
	if(*cmd>MAXCMD)*cmd=MAXCMD;
	if(*cmd<-MAXCMD)*cmd=-MAXCMD;
}

void LsrCmdChk(int *cmd, unsigned int num){
	
	if(num==0){
		if(*cmd>LSRMAXCMD)*cmd=LSRMAXCMD;
		if(*cmd<1)*cmd=1;
	
	}
	else{
		
		if(*cmd>2)*cmd=1;
		if(*cmd<0)*cmd=0;
	
	}
	
	
}


void SetDirctnM1(void){

			if(OUT0>0){
				duty0=OUT0;
				M1IN1_ON;
				M1IN2_OFF;
			}
			else if(OUT0<0){
				duty0=-OUT0;
				M1IN1_OFF;
				M1IN2_ON;
			}
			else{
				duty0=0;
				M1IN1_OFF;
				M1IN2_OFF;
			}

			if(duty0>MAXDUTY){duty0=MAXDUTY;}
			if(duty0<0)duty0=0;
			
			if(Start_EN!=1)duty0=0;


}




void SetDirctnM2(void){

			if(OUT1>0){
				duty1=OUT1;
				M2IN3_ON;
				M2IN4_OFF;

			}
			else if(OUT1<0){
				duty1=-OUT1;
				M2IN3_OFF;
				M2IN4_ON;

			}
			else{
				duty1=0;
				M2IN3_OFF;
				M2IN4_OFF;

			}

			if(duty1>MAXDUTY){duty1=MAXDUTY;}
			if(duty1<0)duty1=0;
			
			if(Start_EN!=1)duty1=0;
			
}


void Chkxres0(void){


#ifdef AS5048A
			if(PMIN0<x_res0&& x_res0<PMAX0){
			
			}
			else{
				//duty0=0;
				duty0=duty0/3;
				Err_cnt0++;
			}
#endif

#ifdef AS5047D
			if(PMIN0<x_res0&& x_res0<PMAX0){
				
			}//11500,13500
			else{
				//duty0=0;
				duty0=duty0/3;
				Err_cnt0++;
			}
#endif		
			

}

void Chkxres1(void){

#ifdef AS5048A
			if(PMIN1<x_res1&& x_res1<PMAX1){}
			else{
				//duty1=0;
				duty1=duty1/3;
				Err_cnt1++;
			}
#endif

#ifdef AS5047D
			if(PMIN1<x_res1&& x_res1<PMAX1){}//11500,13500
			else{
				//duty1=0;
				duty1=duty1/3;
				Err_cnt1++;
			}
#endif

}



void GetStrPos(	char *str, 
				unsigned char strlen, 
				unsigned char strcnt, 
				int cnt, 
				char *lsr, 
				int *xpos, 
				int *ypos	){
	
	char strng=*(str+strcnt);//allstr[0];//*(str+strcnt);
	int l=0,x=0,y=0;
	
	/*NUMBER*/
#ifdef USE_NUMBER_FONT
	if(strng=='0'){
		l=num_zero_l[cnt];		x=num_zero_x[cnt];		y=num_zero_y[cnt];		
	}
	else if(strng=='1'){
		l=num_one_l[cnt];		x=num_one_x[cnt];		y=num_one_y[cnt];		
	}
	else if(strng=='2'){
		l=num_two_l[cnt];		x=num_two_x[cnt];		y=num_two_y[cnt];		
	}
	else if(strng=='3'){
		l=num_three_l[cnt];		x=num_three_x[cnt];		y=num_three_y[cnt];		
	}
	else if(strng=='4'){
		l=num_four_l[cnt];		x=num_four_x[cnt];		y=num_four_y[cnt];		
	}
	else if(strng=='5'){
		l=num_five_l[cnt];		x=num_five_x[cnt];		y=num_five_y[cnt];		
	}
	else if(strng=='6'){
		l=num_six_l[cnt];		x=num_six_x[cnt];		y=num_six_y[cnt];		
	}
	else if(strng=='7'){		
		l=num_seven_l[cnt];		x=num_seven_x[cnt];		y=num_seven_y[cnt];		
	}
	else if(strng=='8'){
		l=num_eight_l[cnt];		x=num_eight_x[cnt];		y=num_eight_y[cnt];		
	}
	else if(strng=='9'){
		l=num_nine_l[cnt];		x=num_nine_x[cnt];		y=num_nine_y[cnt];		
	}
#endif //USE_NUMBER_FONT
	
	/*MARK*/
#ifdef USE_Limted_MARK_FONT
	if(strng==':'){
		l=mk_colon_l[cnt];		x=mk_colon_x[cnt];		y=mk_colon_y[cnt];		
	}
	else if(strng=='/'){
		l=mk_over_l[cnt];		x=mk_over_x[cnt];		y=mk_over_y[cnt];		
	}
	else if(strng==','){
		l=mk_comma_l[cnt];		x=mk_comma_x[cnt];		y=mk_comma_y[cnt];		
	}	
	else if(strng=='.'){
		l=mk_period_l[cnt];		x=mk_period_x[cnt];		y=mk_period_y[cnt];		
	}
#endif
#ifdef USE_MARK_FONT
	else if(strng=='!'){
		l=mk_exclamation_l[cnt];x=mk_exclamation_x[cnt];y=mk_exclamation_y[cnt];		
	}	
	else if(strng=='#'){
		l=mk_hash_l[cnt];		x=mk_hash_x[cnt];		y=mk_hash_y[cnt];		
	}	
	else if(strng=='$'){
		l=mk_dollars_l[cnt];		x=mk_dollars_x[cnt];		y=mk_dollars_y[cnt];		
	}	
	else if(strng=='%'){
		l=mk_percent_l[cnt];		x=mk_percent_x[cnt];		y=mk_percent_y[cnt];		
	}	
	else if(strng=='&'){
		l=mk_ampersand_l[cnt];		x=mk_ampersand_x[cnt];		y=mk_ampersand_y[cnt];		
	}
	else if(strng=='('){
		l=mk_parenl_l[cnt];		x=mk_parenl_x[cnt];		y=mk_parenl_y[cnt];		
	}	
	else if(strng==')'){
		l=mk_parenr_l[cnt];		x=mk_parenr_x[cnt];		y=mk_parenr_y[cnt];		
	}	
	else if(strng=='*'){
		l=mk_asterisk_l[cnt];		x=mk_asterisk_x[cnt];		y=mk_asterisk_y[cnt];		
	}	
	
	else if(strng=='?'){
		l=mk_question_l[cnt];		x=mk_question_x[cnt];		y=mk_question_y[cnt];		
	}	
	else if(strng=='@'){
		l=mk_at_l[cnt];		x=mk_at_x[cnt];		y=mk_at_y[cnt];		
	}	
	else if(strng=='['){
		l=mk_bracketl_l[cnt];		x=mk_bracketl_x[cnt];		y=mk_bracketl_y[cnt];		
	}
	else if(strng==']'){
		l=mk_bracketr_l[cnt];		x=mk_bracketr_x[cnt];		y=mk_bracketr_y[cnt];		
	}		
	else if(strng=='^'){
		l=mk_hat_l[cnt];		x=mk_hat_x[cnt];		y=mk_hat_y[cnt];		
	}
	else if(strng=='_'){
		l=mk_underscore_l[cnt];		x=mk_underscore_x[cnt];		y=mk_underscore_y[cnt];		
	}	
	else if(strng=='|'){
		l=mk_verticalbar_l[cnt];	x=mk_verticalbar_x[cnt];	y=mk_verticalbar_y[cnt];		
	}	
	else if(strng=='~'){
		l=mk_tilde_l[cnt];		x=mk_tilde_x[cnt];		y=mk_tilde_y[cnt];		
	}	
	else if(strng=='\''){
		l=mk_singlequotation_l[cnt];	x=mk_singlequotation_x[cnt];	y=mk_singlequotation_y[cnt];		
	}	
	else if(strng=='\"'){
		l=mk_doublequotation_l[cnt];	x=mk_doublequotation_x[cnt];	y=mk_doublequotation_y[cnt];		
	}	
	else if(strng=='\\'){
		l=mk_yen_l[cnt];		x=mk_yen_x[cnt];		y=mk_yen_y[cnt];		
	}	
	else if(strng=='+'){
		l=mk_plus_l[cnt];		x=mk_plus_x[cnt];		y=mk_plus_y[cnt];		
	}	
	else if(strng=='<'){
		l=mk_lessthan_l[cnt];		x=mk_lessthan_x[cnt];		y=mk_lessthan_y[cnt];		
	}	
	else if(strng=='='){
		l=mk_equal_l[cnt];		x=mk_equal_x[cnt];		y=mk_equal_y[cnt];		
	}
	else if(strng=='>'){
		l=mk_graterthan_l[cnt];		x=mk_graterthan_x[cnt];		y=mk_graterthan_y[cnt];		
	}
	else if(strng=='-'){
		l=mk_dash_l[cnt];		x=mk_dash_x[cnt];		y=mk_dash_y[cnt];		
	}
#endif //USE_MARK_FONT
	
	/*STRING*/
#ifdef USE_STRING_FONT
	if(strng==' '){
		l=str_space_l[cnt];		x=str_space_x[cnt];		y=str_space_y[cnt];		
	}
	else if(strng=='a'){
		l=str_a_l[cnt];		x=str_a_x[cnt];		y=str_a_y[cnt];		
	}
	else if(strng=='b'){
		l=str_b_l[cnt];		x=str_b_x[cnt];		y=str_b_y[cnt];		
	}
	else if(strng=='c'){
		l=str_c_l[cnt];		x=str_c_x[cnt];		y=str_c_y[cnt];			
	}
	else if(strng=='d'){
		l=str_d_l[cnt];		x=str_d_x[cnt];		y=str_d_y[cnt];		
	}
	else if(strng=='e'){
		l=str_e_l[cnt];		x=str_e_x[cnt];		y=str_e_y[cnt];		
	}	
	else if(strng=='f'){
		l=str_f_l[cnt];		x=str_f_x[cnt];		y=str_f_y[cnt];			
	}
	else if(strng=='g'){
		l=str_g_l[cnt];		x=str_g_x[cnt];		y=str_g_y[cnt];			
	}
	else if(strng=='h'){
		l=str_h_l[cnt];		x=str_h_x[cnt];		y=str_h_y[cnt];			
	}
	else if(strng=='i'){
		l=str_i_l[cnt];		x=str_i_x[cnt];		y=str_i_y[cnt];		
	}
	else if(strng=='j'){
		l=str_j_l[cnt];		x=str_j_x[cnt];		y=str_j_y[cnt];			
	}
	else if(strng=='k'){
		l=str_k_l[cnt];		x=str_k_x[cnt];		y=str_k_y[cnt];			
	}	
	else if(strng=='l'){
		l=str_l_l[cnt];		x=str_l_x[cnt];		y=str_l_y[cnt];			
	}	
	else if(strng=='m'){
		l=str_m_l[cnt];		x=str_m_x[cnt];		y=str_m_y[cnt];			
	}
	else if(strng=='n'){
		l=str_n_l[cnt];		x=str_n_x[cnt];		y=str_n_y[cnt];			
	}
	else if(strng=='o'){
		l=str_o_l[cnt];		x=str_o_x[cnt];		y=str_o_y[cnt];			
	}
	else if(strng=='p'){
		l=str_p_l[cnt];		x=str_p_x[cnt];		y=str_p_y[cnt];			
	}
	else if(strng=='q'){
		l=str_q_l[cnt];		x=str_q_x[cnt];		y=str_q_y[cnt];			
	}
	else if(strng=='r'){
		l=str_r_l[cnt];		x=str_r_x[cnt];		y=str_r_y[cnt];			
	}
	else if(strng=='s'){
		l=str_s_l[cnt];		x=str_s_x[cnt];		y=str_s_y[cnt];			
	}
	else if(strng=='t'){
		l=str_t_l[cnt];		x=str_t_x[cnt];		y=str_t_y[cnt];			
	}
	else if(strng=='u'){
		l=str_u_l[cnt];		x=str_u_x[cnt];		y=str_u_y[cnt];			
	}
	else if(strng=='v'){
		l=str_v_l[cnt];		x=str_v_x[cnt];		y=str_v_y[cnt];			
	}	
	else if(strng=='w'){
		l=str_w_l[cnt];		x=str_w_x[cnt];		y=str_w_y[cnt];			
	}	
	else if(strng=='x'){
		l=str_x_l[cnt];		x=str_x_x[cnt];		y=str_x_y[cnt];			
	}	
	else if(strng=='y'){
		l=str_y_l[cnt];		x=str_y_x[cnt];		y=str_y_y[cnt];			
	}	
	else if(strng=='z'){
		l=str_z_l[cnt];		x=str_z_x[cnt];		y=str_z_y[cnt];			
	}	
#endif //USE_STRING_FONT
	
	*lsr=l;
	*xpos=x+StrOffset[(int)strlen][(int)strcnt];
	*ypos=y;
	
	
}

void ChkStrCnt(	char *STR, 
				unsigned char ALLSTRLEN,
				unsigned char *ALLSTRCNT,
				int *CNT0,
				int *CNT1){
					
					
	char strng=*(STR+*ALLSTRCNT);
	int length=0;
	/*NUMBER*/
#ifdef USE_NUMBER_FONT
	if(strng=='0'){
		length=sizeof(num_zero_l)/sizeof(char);
	}
	else if(strng=='1'){
		length=sizeof(num_one_l)/sizeof(char);	
	}
	else if(strng=='2'){
		length=sizeof(num_two_l)/sizeof(char);	
	}
	else if(strng=='3'){
		length=sizeof(num_three_l)/sizeof(char);			
	}
	else if(strng=='4'){
		length=sizeof(num_four_l)/sizeof(char);		
	}
	else if(strng=='5'){
		length=sizeof(num_five_l)/sizeof(char);		
	}
	else if(strng=='6'){
		length=sizeof(num_six_l)/sizeof(char);	
	}
	else if(strng=='7'){		
		length=sizeof(num_seven_l)/sizeof(char);			
	}
	else if(strng=='8'){
		length=sizeof(num_eight_l)/sizeof(char);			
	}
	else if(strng=='9'){
		length=sizeof(num_nine_l)/sizeof(char);			
	}
#endif 
	/*MARK*/
#ifdef USE_Limted_MARK_FONT
	if(strng==':'){
		length=sizeof(mk_colon_l)/sizeof(char);			
	}
	else if(strng=='/'){
		length=sizeof(mk_over_l)/sizeof(char);				
	}
	else if(strng==','){
		length=sizeof(mk_comma_l)/sizeof(char);				
	}
	else if(strng=='.'){
		length=sizeof(mk_period_l)/sizeof(char);				
	}
#endif
#ifdef USE_MARK_FONT
	else if(strng=='!'){
		length=sizeof(mk_exclamation_l)/sizeof(char);				
	}
	else if(strng=='#'){
		length=sizeof(mk_hash_l)/sizeof(char);				
	}
	else if(strng=='$'){
		length=sizeof(mk_dollars_l)/sizeof(char);				
	}
	else if(strng=='%'){
		length=sizeof(mk_percent_l)/sizeof(char);				
	}
	else if(strng=='&'){
		length=sizeof(mk_ampersand_l)/sizeof(char);				
	}
	else if(strng=='('){
		length=sizeof(mk_parenl_l)/sizeof(char);				
	}
	else if(strng==')'){
		length=sizeof(mk_parenr_l)/sizeof(char);				
	}
	else if(strng=='*'){
		length=sizeof(mk_asterisk_l)/sizeof(char);				
	}	
	else if(strng=='?'){
		length=sizeof(mk_question_l)/sizeof(char);				
	}
	else if(strng=='@'){
		length=sizeof(mk_at_l)/sizeof(char);				
	}
	else if(strng=='['){
		length=sizeof(mk_bracketl_l)/sizeof(char);				
	}
	else if(strng==']'){
		length=sizeof(mk_bracketr_l)/sizeof(char);				
	}
	else if(strng=='^'){
		length=sizeof(mk_hat_l)/sizeof(char);				
	}
	else if(strng=='_'){
		length=sizeof(mk_underscore_l)/sizeof(char);				
	}
	else if(strng=='|'){
		length=sizeof(mk_verticalbar_l)/sizeof(char);				
	}
	else if(strng=='~'){
		length=sizeof(mk_tilde_l)/sizeof(char);				
	}
	else if(strng=='\''){
		length=sizeof(mk_singlequotation_l)/sizeof(char);				
	}
	else if(strng=='\"'){
		length=sizeof(mk_doublequotation_l)/sizeof(char);				
	}
	else if(strng=='\\'){
		length=sizeof(mk_yen_l)/sizeof(char);				
	}
	else if(strng=='+'){
		length=sizeof(mk_plus_l)/sizeof(char);				
	}
	else if(strng=='<'){
		length=sizeof(mk_lessthan_l)/sizeof(char);				
	}
	else if(strng=='='){
		length=sizeof(mk_equal_l)/sizeof(char);				
	}
	else if(strng=='>'){
		length=sizeof(mk_graterthan_l)/sizeof(char);				
	}
	else if(strng=='-'){
		length=sizeof(mk_dash_l)/sizeof(char);				
	}
#endif //USE_MARK_FONT
	/*STRING*/
	
#ifdef USE_STRING_FONT
	if(strng==' '){
		length=sizeof(str_space_l)/sizeof(char);				
	}
	else if(strng=='a'){
		length=sizeof(str_a_l)/sizeof(char);				
	}
	else if(strng=='b'){
		length=sizeof(str_b_l)/sizeof(char);				
	}
	else if(strng=='c'){
		length=sizeof(str_c_l)/sizeof(char);				
	}
	else if(strng=='d'){
		length=sizeof(str_d_l)/sizeof(char);				
	}
	else if(strng=='e'){
		length=sizeof(str_e_l)/sizeof(char);				
	}
	else if(strng=='f'){
		length=sizeof(str_f_l)/sizeof(char);				
	}
	else if(strng=='g'){
		length=sizeof(str_g_l)/sizeof(char);				
	}
	else if(strng=='h'){
		length=sizeof(str_h_l)/sizeof(char);				
	}
	else if(strng=='i'){
		length=sizeof(str_i_l)/sizeof(char);				
	}
	else if(strng=='j'){
		length=sizeof(str_j_l)/sizeof(char);				
	}
	else if(strng=='k'){
		length=sizeof(str_k_l)/sizeof(char);				
	}
	else if(strng=='l'){
		length=sizeof(str_l_l)/sizeof(char);				
	}
	else if(strng=='m'){
		length=sizeof(str_m_l)/sizeof(char);				
	}
	else if(strng=='n'){
		length=sizeof(str_n_l)/sizeof(char);				
	}
	else if(strng=='o'){
		length=sizeof(str_o_l)/sizeof(char);				
	}
	else if(strng=='p'){
		length=sizeof(str_p_l)/sizeof(char);				
	}
	else if(strng=='q'){
		length=sizeof(str_q_l)/sizeof(char);				
	}
	else if(strng=='r'){
		length=sizeof(str_r_l)/sizeof(char);				
	}
	else if(strng=='s'){
		length=sizeof(str_s_l)/sizeof(char);				
	}
	else if(strng=='t'){
		length=sizeof(str_t_l)/sizeof(char);				
	}
	else if(strng=='u'){
		length=sizeof(str_u_l)/sizeof(char);				
	}
	else if(strng=='v'){
		length=sizeof(str_v_l)/sizeof(char);				
	}
	else if(strng=='w'){
		length=sizeof(str_w_l)/sizeof(char);				
	}
	else if(strng=='x'){
		length=sizeof(str_x_l)/sizeof(char);				
	}
	else if(strng=='y'){
		length=sizeof(str_y_l)/sizeof(char);				
	}
	else if(strng=='z'){
		length=sizeof(str_z_l)/sizeof(char);				
	}
#endif //USE_STRING_FONT
		
	
	if(*CNT0>=length){
		*ALLSTRCNT=*ALLSTRCNT+1;
		*CNT0=0;
		*CNT1=-5;
	}
	if(ALLSTRLEN<=*ALLSTRCNT){
		*ALLSTRCNT=0;
		*CNT1=-10;
	}	
	
}



void delay_us(int usec){
	int i;
	while(usec){
		for(i=0;i<4;i++){
			asm("NOP");
		}
		usec--;
	}
}

void delay_ms(int msec){
	int i;
	for(i=0;i<msec;i++){
		delay_us(1000);
	}
}

