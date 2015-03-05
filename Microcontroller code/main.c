/* This code runs on pic16f877a microcntroller.
It receives data from UART
*/






#include <htc.h>
#include "pwm&timer@20MHz.c"

__CONFIG(HS & BORDIS & PWRTEN & WDTDIS & LVPDIS);

 signed char rdatal =0, Ts = 61, rdatar = 0,maxerr= 15,minerr= -15,b,minpos=-80,maxpos=100;
signed char pulsel=0,pulser=0, pl = 0, pr= 0,check=0;
unsigned char WML,WMR,posl=0,posr=0;
bit send_flag, setPWM;
signed char setPointl = 4,setPointr=4;
signed char err_l=5,err_r=5;
char ctr = 0,left=0,right,a;

//Initializing UART

void init_uart(void)
{	
	TRISC6 = 0;
	TRISC7 = 1;
	SYNC = 0;			//configuring asynchronous mode....
	
	BRGH = 1;			//asynchrous mode..
	SPBRG = 129;			//for setting baud rate 9.600 kb..
	SPEN = 1;			// enables RX & TX as serial ports..
	GIE = 1;			//GIE and PEIE
	PEIE = 1;			// enabling the interrupts..
	RCIE = 1;			// interrupt enable bit.....
	RX9 = 0;			// 8-bit reception..
	TXIE = 0;
	TXEN = 0;
	TX9 = 0;
	
}

void uart_send(signed char data)
{
	TXREG = data;
	TXEN = 1;
	while(!TRMT);
}

void sampler_init()
{
	T0CS=0;
	PSA=0;
	PS2=1;PS1=1;PS0=0;
	TMR0IF=0;
	TMR0IE=1;
	TMR0=Ts;
}

void main(void)
{ 
     delay_Nms(1000 );
	init_uart();
	TRISB0=1;
	TRISB1=1;
	INTF=0;
	INTE=1;
    RBIE=1;
	RBIF=0;
	INTEDG=1;
	TRMT=0;
	TRISC1 = 0;
        TRISC2 = 0;
        TRISC0 = 0;
        pwm();
       WML=WMR= PWMR = PWML = 100;
        send_flag = setPWM = 0;
        RC0 = 0;
        sampler_init();
	while(1)
	{
	
	CREN = 1;   	//Continuous Receive Enable bit...$$
	if(send_flag)
		{
            
		   // err=setPoint - pulse;
			if(err_l <=  minerr)err_l=minerr;                 
			if(err_l>=maxerr)err_l=maxerr; 
            if(err_r <=  minerr)err_r=minerr;                 
			if(err_r>=maxerr)err_r=maxerr; 
            //if(posl <= minpos)posl=minpos;   //to prevent data from reaching -86                
			//if(posl>=maxpos)posl=maxpos; 
          //  if(posr <= minpos)posr=minpos;                 
		//	if(posr>=maxpos)posr=maxpos;             
            uart_send(-86);
            uart_send(err_l);
            uart_send(err_r);
           // uart_send(posl);
            //uart_send(posr);
              
            send_flag = 0;
		}
           

        
if(setPWM)
{
 PWML=rdatal;
PWMR =rdatar;
setPWM=0;
}		

		

		 
	}	
}


void interrupt isr(void)
{
	
	if(RCIF)
	{
		check = RCREG;
         
          if(check!=-86  && right==1)

       {
          rdatar=RCREG;
          right=0;
          setPWM=1;
          check=0;

       }
      	
      else if(check !=-86 && left==1)
       {
          rdatal=RCREG;
          left=0;
          right=1;
          
        }
      
       
        else if(check==-86){left=1;}
		
        RCIF = 0;
	
     }
	
if(INTF==1)
	{
		//quadrature phase decoding
		if(RB0)
		{
			if(RB1)
			{
				pl=-1;
			}
			else
			{
				pl=1;
			}
		}
	
     	pulsel+=pl;

  	    if(RB6)
		{
			if(RB7)
			{
				pr=1;
			}
			else
			{
				pr=-1;
			}
		}
		pulser+=pr;
        b=PORTB;      	
     	RBIF=0;
		INTF=0;	
	}
	
		if(TMR0IF)
		{
			ctr++;
			if(ctr ==4)
			{    
                 //posl=pulsel;  pulsel=setpoint-err_L in qt
                 err_l=setPointl - pulsel;
                 pulsel=0;
                
                 err_r=setPointr - pulser;
                 pulser=0;
			     send_flag = 1;
               	 ctr = 0;
               
			}
             
			TMR0IF=0;
			TMR0=Ts;
		}	
}	
	


