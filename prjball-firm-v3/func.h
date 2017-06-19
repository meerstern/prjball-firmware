/* * * * * * * * * * * * * * * * * * * * * * * * * *
 * ProjectionBall Firmware For v3.X				   *
 * Copyright (c) 2017  							   *
 * K.Watanabe,Crescent 							   *
 * Released under the MIT license 				   *
 * http://opensource.org/licenses/mit-license.php  *
 * 17/06/16 v1.0 Initial Release                   *
 * 												   *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#include "FSIO.h"
#include "variable.h"

#define SPI_CS1 LATBbits.LATB10
#define SPI_CS2 LATBbits.LATB11
#define SD_CS	LATBbits.LATB15
#define LSD	LATBbits.LATB5//Laser ON/OFF

#define CK	PORTBbits.RB9//For test

#ifndef TQFP
	#define SW (PORTB&0x0003)
#else
	#define SW ((PORTC&0x00C0)>>6)
#endif
	
	
#ifdef FRAMEENABLE
	#ifndef TQFP
		#define PATTN ((PORTB&0x0300)>>8)
	#else
		#define PATTN ((PORTC&0x0300)>>8)
	#endif
		
#else
	#define PATTN (3)
#endif







//Motor1 CCW/CW OutPort
// #ifdef TQFP
	// #define M1IN1_ON	LATA|= 0x0008
	// #define M1IN1_OFF	LATA&=~0x0008
	// #define M1IN2_ON    LATA|= 0x0004
	// #define M1IN2_OFF   LATA&=~0x0004
// #else 
	#define M1IN1_ON	LATA|= 0x0004
	#define M1IN1_OFF	LATA&=~0x0004
	#define M1IN2_ON    LATA|= 0x0008
	#define M1IN2_OFF   LATA&=~0x0008
// #endif

//Motor2 CCW/CW OutPort
// #ifdef TQFP
	// #define M2IN4_ON	LATA|= 0x0010
	// #define M2IN4_OFF	LATA&=~0x0010
	// #define M2IN3_ON	LATB|= 0x0010
	// #define M2IN3_OFF	LATB&=~0x0010
// #else
	#define M2IN4_ON	LATB|= 0x0010
	#define M2IN4_OFF	LATB&=~0x0010
	#define M2IN3_ON	LATA|= 0x0010
	#define M2IN3_OFF	LATA&=~0x0010
// #endif
//SPI_CS1
#define SPICS1_ON	LATB|= 0x0400
#define SPICS1_OFF	LATB&=~0x0400

//SPI_CS2
#define SPICS2_ON	LATB|= 0x0800
#define SPICS2_OFF	LATB&=~0x0800

//Motor Driver STBY
#ifdef TQFP
	#define STBY_ON		LATA|= 0x0100
	#define STBY_OFF	LATA&=~0x0100
#endif

//ERROR LED
#define ERROR_LED_ON	LATA|= 0x0200

unsigned int AS5048_0(void);
unsigned int AS5048_1(void);
unsigned int AS5047_0(void);
unsigned int AS5047_1(void);

int FSfgetc(FSFILE *fp);
char *FSfgets(char *s, int n, FSFILE * stream);
    
void setpath(struct path* pR, char* str, unsigned int num);
void setpath2(struct fpath* pR, char* str, unsigned int num);
void PosCmdChk(int *cmd);
void LsrCmdChk(int *cmd, unsigned int num);
void SetDirctnM1(void);
void SetDirctnM2(void);

void Chkxres0(void);

void Chkxres1(void);

void delay_us(int usec);
void delay_ms(int msec);
	
void GetStrPos(	 char *str, 
				unsigned char len, 
				unsigned char strcnt, 
				int cnt, 
				char *lsr, 
				int *xpos, 
				int *ypos	);
void ChkStrCnt(	 char *STR, 
				unsigned char ALLSTRLEN,
				unsigned char *ALLSTRCNT,
				int *CNT0,
				int *CNT1);
