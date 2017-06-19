/* * * * * * * * * * * * * * * * * * * * * * * * * *
 * ProjectionBall Firmware For v3.X				   *
 * Copyright (c) 2017  							   *
 * K.Watanabe,Crescent 							   *
 * Released under the MIT license 				   *
 * http://opensource.org/licenses/mit-license.php  *
 * 17/06/16 v1.0 Initial Release                   *
 * 												   *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


/*MODE*/

//L298N DIP
//#define V1

//TB6612 DIP
//#define V2	

//TB6612 TQFP
#define V3




/*Select Encoder Type*/
#ifdef V3
	#define AS5047D
	
	#define PCEN0	10000
	#define PCEN1	6000
	
	
#else
	#define AS5048A
	
	#define PCEN0	5000
	#define PCEN1	5000
#endif

	#define PMAX0	PCEN0+2000
	#define PMIN0	PCEN0-2000
	#define PMAX1	PCEN1+2000
	#define PMIN1	PCEN1-2000


/*Select Driver Type*/
#ifdef V1
	#define L298N
#else
	#define TB6612
#endif



/*Eable Frame Mode*/
#ifndef V1
	#define FRAMEENABLE
#endif


/*Error Stop Function*/
#define ERROR_Stop_EN


#ifdef V3
	#define TQFP
#else
	#define DIP
#endif


#define USE_WETHER_FONT
#define USE_STRING_FONT
//#define USE_MARK_FONT
#define USE_Limted_MARK_FONT
#define USE_NUMBER_FONT



#ifdef USE_MARK_FONT
	#define USE_Limted_MARK_FONT
#endif

#define MAXDUTY 0x0400
#define LSD_Offset 4


#define POS_IN_MAX	50
#define POS_IN_MIN	-POS_IN_MAX


#define DATA_NUM		4
#define ALL_DATA_NUM	202

#define FRAME_NUM		19
#define ALL_FRAME_NUM	202

#define st 	1
#define w	1//1.2



//#define EN_PID_I

#define PseudoD
//#define DiffD


#define AMP				80

#define TRIFUNCLEN		120

#define CMPRATE			3
#define MAXCMD			(380/CMPRATE)

#define LSRMAXCMD		50

/*PD Control Gain Set*/
//P Gain (L298N FA130:11.8~12.2) 
//       (TB6612 FA130 5~7,7)
//		 (TB6612 CLESS 4)
//#define KP	5//5


//D Gain (L298N FA130:200~400)
//D Gain (TB6612 FA130:100~130,120)
//D Gain (TB6612 CLESS:60)
//#define KD	50//120
//I Gain (FA130:0)
//(1/2)


#ifdef	TB6612

		#define KP	4//3,4,5,3
		#define KD	100//60,80,120,60
		#define KI  0

#endif

#ifdef	L298N

	#define KP	12
	#define KD	280
	#define KI  0
	
#endif

#pragma once		
enum{
	enum_ANIMATION,	//0
	enum_ROTATION,	//1
	enum_ONESTROKE,	//2
	enum_ALWAYS,	//3
	enum_AWATCH,	//4
	enum_DWATCH,	//5
	enum_DATE,		//6
	enum_STRING		//7
};
#pragma once		
enum{
	enum_STAR_DATA0,
	enum_ARROW_DATA1,
	enum_MAIL_DATA2,
	enum_SMILE_DATA3,
	enum_SUN_DATA4,
	enum_CLOUD_DATA5,
	enum_RAIN_DATA6,
	enum_SNOW_DATA7,
	enum_THUNDER_DATA8,
	enum_HEART_DATA
	
		
};

/*  Uart String Definition  */
#define Str_Switch_Mode		"swm"
#define Str_Uart_Mode		"urm"

#define Str_Start			"srt"
#define Str_Stop			"stp"

#define Str_Frame_Mode		"frm"
#define Str_SW0				"sw0"
#define Str_SW1				"sw1"
#define Str_SW2				"sw2"
#define Str_SW3				"sw3"
#define Str_SW4				"sw4"
#define Str_SW5				"sw5"
#define Str_SW6				"sw6"
#define Str_SW7				"sw7"
#define Str_SW8				"sw8"
#define Str_SW9				"sw9"

#define Str_Rotation		"rot"
#define Str_Stroke			"str"
#define Str_Always			"alw"
#define Str_AnalogWatch		"awt"
#define Str_DigitalWatch	"dwt"
#define Str_Day				"dym"
#define Str_StringMode		"stm"


#define Str_SD_Mode			"sdm"
#define Str_Internal_Mode	"inm"

#define Str_Center_Pos		"cen="
#define Str_Angle_Deg		"deg="
#define Str_Debug_Mode		"dbg"
#define Str_Restart			"rst"
#define Str_FrameNo			"fno="
#define Str_FramePause		"fpa"
#define Str_GetString		"stg="

#define Str_SetTime			"tim="
#define Str_GetTime			"tim?"
#define Str_SetDate			"day="
#define Str_GetDate			"day?"


#define Find_String			"=,"


#define GetStringMaxLen	6
