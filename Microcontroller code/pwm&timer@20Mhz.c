/*****@20MHz CRYSTAL FREQUENCY*****/



#define PWM_FREQ 140	//pwm period=((PR2+1)*4*Tosc*prescaler factor	
#define HALT_PWM 70
#define PWMR CCPR1L
#define PWML CCPR2L



void delay_Nms(int n )
{
   	while(n>0)
  	 {
		TMR1H=0xEC;	// EC77H=60535d
		TMR1L=0x77;
	
		T1CKPS1=0;
		T1CKPS0=0;
	
		TMR1CS=0;
		TMR1IF=0;
		TMR1ON=1;
		while(!TMR1IF)  { }
		TMR1IF=0;
		n--;
	}
}



void pwm(void)
{
	PR2 = PWM_FREQ;				
	
	CCPR2L = HALT_PWM;   	
	CCPR1L = HALT_PWM;	
	
	

	T2CKPS1 = 0;
	T2CKPS0 = 0;
	TMR2ON =  1;

	CCP2M3 = 1;
	CCP2M2 = 1;
	CCP2M1 = 0;
	CCP2M0 = 0;
	
	CCP1M3 = 1; 
	CCP1M2 = 1;
	CCP1M1 = 0;
	CCP1M0 = 0;
}


