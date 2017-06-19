/* * * * * * * * * * * * * * * * * * * * * * * * * *
 * ProjectionBall Firmware For v3.X				   *
 * Copyright (c) 2017  							   *
 * K.Watanabe,Crescent 							   *
 * Released under the MIT license 				   *
 * http://opensource.org/licenses/mit-license.php  *
 * 17/06/16 v1.0 Initial Release                   *
 * 												   *
 * * * * * * * * * * * * * * * * * * * * * * * * * */




_FBS(BWRP_WRPROTECT_OFF & BSS_NO_BOOT_CODE & RBS_NO_BOOT_RAM & SWRP_WRPROTECT_OFF & SSS_NO_SEC_CODE & RSS_NO_SEC_RAM);

_FGS(GWRP_OFF & GSS_OFF);

_FOSCSEL(FNOSC_FRCPLL & IESO_ON);

_FOSC(POSCMD_NONE & OSCIOFNC_ON & IOL1WAY_OFF & FCKSM_CSDCMD);

_FWDT(WDTPOST_PS32768 & WDTPRE_PR128 & WINDIS_OFF & FWDTEN_OFF);

_FPOR(FPWRT_PWR128 & ALTI2C_OFF );

_FICD(ICS_PGD1 & JTAGEN_OFF);




void init_uart();

void OSC_setting(void)
{
	
	PLLFBD = 41;				// M  = 40
	CLKDIVbits.PLLPOST = 0;		// N1 = 2
	CLKDIVbits.PLLPRE = 0;		// N2 = 2
	
	RCONbits.SWDTEN = 0;			
	while(OSCCONbits.LOCK != 1);	
	
	DISICNT = 0x0000;	
	SRbits.IPL = 1;		
}


void IO_init(void)
{

	__builtin_write_OSCCONL(OSCCON & (~(1<<6)));	
	
	#ifdef TQFP
		CNPU1=0x0000;
		CNPU2=0x01FE;
	#else
		CNPU1=0x0056;//28
		CNPU2=0x01E0;
	#endif
	
	
	//PWM OC 
	RPOR1bits.RP2R = 18;// OC1 RB2　
	RPOR1bits.RP3R = 19;// OC2 RB3　
	

	// SPI1 (SD)
	RPOR6bits.RP12R = 0b01000;		
	RPINR20bits.SDI1R = 13;			
	RPOR7bits.RP14R = 0b00111;		

	init_uart();
	__builtin_write_OSCCONL(OSCCON | (1<<6));		
	while(OSCCONbits.LOCK!=1) {};		
}
/*
void init_i2c(void){

	//I2C
 
	unsigned int I2Cconfig1 = I2C1_ON & I2C1_IDLE_CON & I2C1_CLK_HLD & I2C1_IPMI_DIS & I2C1_7BIT_ADD & I2C1_SLW_DIS
							& I2C1_SM_DIS &I2C1_GCALL_DIS & I2C1_STR_DIS & I2C1_NACK & I2C1_ACK_DIS & I2C1_RCV_DIS
							& I2C1_STOP_DIS & I2C1_RESTART_DIS;		
	 
	unsigned int I2Cconfig2 = 38;    
  
 	 OpenI2C1(I2Cconfig1,I2Cconfig2);
	 ConfigIntI2C1(MI2C1_INT_OFF & SI2C1_INT_OFF);
	 IdleI2C1();



}
*/


//SPI　初期化

 void init_SPI(void){
 
	
	unsigned int spi_config1 = ENABLE_SCK_PIN 
							 & ENABLE_SDO_PIN 
							 & SPI_MODE16_ON
							 & SPI_SMP_ON//SPI_SMP_ON
							 & SPI_CKE_OFF//ON 
							 & SLAVE_ENABLE_OFF
							 & CLK_POL_ACTIVE_HIGH
							 & MASTER_ENABLE_ON0
							 & SEC_PRESCAL_4_1
							 & PRI_PRESCAL_1_1;

    unsigned int spi_config2 = FRAME_ENABLE_OFF & FRAME_SYNC_OUTPUT &
								FRAME_POL_ACTIVE_HIGH & FRAME_SYNC_EDGE_COINCIDE;

    unsigned int spi_config3  = SPI_ENABLE & SPI_IDLE_CON & SPI_RX_OVFLOW_CLR;

	__builtin_write_OSCCONL(OSCCON & (~(1<<6)));	
	
	TRISB = TRISB | 0b0010000000000000;//RB13DI
					 //432109876543210
#ifdef TQFP
	TRISC = TRISC | 0b0000000000010000;//RC4DI
					 //432109876543210
#endif				 
	
#ifndef TQFP
	RPINR22bits.SDI2R = 13;// RP13 DI
	RPOR7bits.RP14R	  = 10;// RP14 DO
	RPOR6bits.RP12R	  = 11;// RP12 CLK
#else
	RPINR22bits.SDI2R = 20;// RP20 DI
	RPOR10bits.RP21R  = 10;// RP21 DO
	RPOR9bits.RP19R   = 11;// RP19 CLK
#endif
		
	OpenSPI2(spi_config1,spi_config2,spi_config3);
	ConfigIntSPI2(SPI_INT_DIS);
	__builtin_write_OSCCONL(OSCCON | (1<<6));		
	while(OSCCONbits.LOCK!=1) {};		
}	


void SD_SPI_speedup(void){

	SPISTATbits.SPIEN = 0;		
	__delay_us(10);
	SPICON1bits.SPRE = 5;		// 2nd Pre = 8 - <SPRE>
	SPICON1bits.PPRE = 3;		// 1st Pre = 4^(3 - <PPRE>)
	SPISTATbits.SPIEN = 1;		// SPI


}





	


 void init_ADC(void){
 


	unsigned int ADCON1_con
				=ADC_MODULE_ON
				&ADC_IDLE_CONTINUE
				&ADC_AD12B_12BIT
				&ADC_FORMAT_INTG
				&ADC_CLK_AUTO
				&ADC_SIMULTANEOUS 
				&ADC_AUTO_SAMPLING_OFF
				&ADC_SAMP_OFF;
				
	unsigned int ADCON2_con
				=ADC_VREF_AVDD_AVSS
				&ADC_SCAN_OFF
				&ADC_SAMPLES_PER_INT_1
				&ADC_ALT_BUF_OFF
				&ADC_ALT_INPUT_OFF;	
				
	unsigned int ADCON3_con
				=ADC_SAMPLE_TIME_1//2->1 
				&ADC_CONV_CLK_SYSTEM
				&ADC_CONV_CLK_16Tcy;
				
	
	unsigned int ADCON4_con
				=0;
				
#ifdef SIA		
	unsigned int configport_l
				=ENABLE_AN0_ANA
				&ENABLE_AN1_ANA
				&ENABLE_AN2_ANA;
#else
	unsigned int configport_l
				=ENABLE_AN0_ANA
				&ENABLE_AN1_ANA;

#endif
	unsigned int configport_h=0;

	unsigned int configscan_h=0xFF;;

	unsigned int configscan_l=0xFF;
	
	OpenADC1(ADCON1_con,ADCON2_con,ADCON3_con,ADCON4_con,configport_l,configport_h,configscan_h,configscan_l);
	ConfigIntADC1(ADC_INT_DISABLE & ADC_INT_PRI_5);

}			


int readADC0(void){
		
		int ad_data;
		
		AD1CHS0bits.CH0SA=0;		
		AD1CON1bits.SAMP=1;

		ConvertADC1();
		while(!AD1CON1bits.DONE);
		AD1CON1bits.DONE=0;
		ad_data=ReadADC1(0);

		return ad_data;
}

int readADC1(void){
		
		int ad_data;
			
		AD1CHS0bits.CH0SA=1;		
		AD1CON1bits.SAMP=1;

		ConvertADC1();
		while(!AD1CON1bits.DONE);
		AD1CON1bits.DONE=0;
		ad_data=ReadADC1(0);//K1

		return ad_data;
}

int readADC2(void){
		
		int ad_data;
			
		AD1CHS0bits.CH0SA=2;		
		AD1CON1bits.SAMP=1;

		ConvertADC1();
		while(!AD1CON1bits.DONE);
		AD1CON1bits.DONE=0;
		ad_data=ReadADC1(0);//K1

		return ad_data;
}

 void init_TIMER(void){

	
	

	unsigned int t1period=0x8000;//
	unsigned int t2period=0x0400;//
	unsigned int t3period=0x01EE;//
		
		
		
			//TIM1	UART DISP	
	unsigned int T1CON_config
			=T1_ON
			&T1_IDLE_STOP
			&T1_GATE_OFF
			&T1_PS_1_256
			&T1_SYNC_EXT_OFF
			&T1_SOURCE_INT;
			
			
		//TIM2	PWM
	unsigned int T2CON_config
			=T2_ON
			&T2_IDLE_STOP
			&T2_GATE_OFF
			&T2_PS_1_1
			//&T2_PS_1_8
			&T2_SOURCE_INT;	
	
	
	
	
		//TIM3用 Control
	unsigned int T3CON_config
			=T3_ON
			&T3_IDLE_STOP
			&T3_GATE_OFF
			&T3_PS_1_8
			&T3_SOURCE_INT;
			
		//uart
	OpenTimer1(T1CON_config,t1period);
	ConfigIntTimer1(T1_INT_PRIOR_2 & T1_INT_ON);
	
		//pwm
	OpenTimer2(T2CON_config,t2period);	
	ConfigIntTimer2(T2_INT_PRIOR_3 & T2_INT_ON);
	
		//con
	OpenTimer3(T3CON_config,t3period);
	ConfigIntTimer3(T3_INT_PRIOR_5 & T3_INT_ON);
	
	
			
}	
				
void init_PWM(void){

	//Setup Output Compare to PWM mode
	OpenOC1(OC_IDLE_CON & OC_TIMER2_SRC & OC_PWM_FAULT_PIN_DISABLE, 0, 0);
	//Setup Output Compare to PWM mode
	OpenOC2(OC_IDLE_CON & OC_TIMER2_SRC & OC_PWM_FAULT_PIN_DISABLE, 0, 0);

	
	printf("Initialize PWM\n");


}		
	

 void init_uart(void)
{
 
	 unsigned int config1 = UART_EN & UART_IDLE_CON & UART_IrDA_DISABLE & UART_MODE_SIMPLEX & 
	 UART_UEN_00 & UART_DIS_WAKE & UART_DIS_LOOPBACK & UART_DIS_ABAUD &
	 UART_UXRX_IDLE_ONE & UART_BRGH_SIXTEEN & UART_NO_PAR_8BIT & UART_1STOPBIT;
	 
	 unsigned int config2 = UART_INT_TX & UART_IrDA_POL_INV_ZERO & UART_SYNC_BREAK_DISABLED &
	 UART_TX_ENABLE & UART_INT_RX_CHAR & UART_ADR_DETECT_DIS & UART_RX_OVERRUN_CLEAR;
 

	TRISB = TRISB & 0xFF7F;//RB7-U1RX RB6-U1TX　　　　
	RPINR18bits.U1RXR = 7;// RP7 U1RX
	RPOR3bits.RP6R = 3;// RP6 U1TX
	OpenUART1(config1,config2, 246);	
	ConfigIntUART1(UART_RX_INT_PR3 & UART_RX_INT_EN & UART_TX_INT_DIS);

 }

