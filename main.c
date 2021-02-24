/////////////////////////Notes for the user////////////////////////////////
///////////////////////////////////////////////////////////////////////////
//This application is a simple calculator./////////////////////////////////
//It performs addition, subtraction, multiplication and division.//////////
//Due to the time constraint, this application is not perfect./////////////
//A few problems are disregarded, they include:////////////////////////////
//1- Bouncing has not been addressed; no debounce has been applied.////////
//2- The number of characters to be displayed on the screen has been///////
//limited to avoid solving the problem of overflow. The limitation was/////
//done by limiting the number of digits per number under operation to 5.///
//This will cause the maximum number obtained by multiplication to be a////
//10-digit number at max.//////////////////////////////////////////////////
//3- It can only do one operation at a time.///////////////////////////////
//4- It does not provide functionality for negative numbers.///////////////
//5- It is not optimized.//////////////////////////////////////////////////
//6 - A few delays in the code were required for proper hardware operation/
//however, due to the time constraint, I was not able to apply them all.///
//////////////////////////////Hardware/////////////////////////////////////
//The software is built of the Tiva C board (tm4c123gh6pm)/////////////////
//It also utilizes a 4x4 key pad with the circuitry found in the report.///
//It also utilizes a 16x1 LCD module with a HD44780 controller.////////////
//The clock of operation has been left as the default 16MHz of the board.//
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
#include "tm4c123gh6pm.h"
///////////////////////////////////////////////////////////////////////////
//Definition of functions
void keypad_init(void);
void LCD_init(void);
void delayMs(int n);
void delayUs(int n);
void lcdcmd(unsigned char value);
void display(unsigned char value);
unsigned char get_key(void);
unsigned concatenate(unsigned x, unsigned y);
unsigned add(unsigned x, unsigned y);
unsigned sub(unsigned x, unsigned y);
unsigned mult(unsigned x, unsigned y);
unsigned div(unsigned x, unsigned y);
//////////////////////////////////////////////////////////////////////////
//This is the main function which is also the answer to question:
//Q2-IV
int main()
{	
  unsigned char key;
	unsigned digits;
	unsigned digits2;
	unsigned num;
	unsigned num2;
	unsigned ans;
	char op;
	
  
	//Calling initialization functions for keypad and LCD
  keypad_init();
  LCD_init();
 
	//The continuous loop
  while(1)
  {
		//Reset the temperary values for the next operation
		digits = 0;
		digits2 = 0;
		num = 0;
		num2 = 0;
		//Clear display for next operation
		lcdcmd(0x01);
		//First loop
		while(digits < 5)
		{
			if (get_key() == 'c')
			{
				lcdcmd(0x01);//Clear the display
				digits = 0;//Reset number of digits of first operand
			}
			else if (get_key()=='A')
			{
				if (digits == 0)
				{
					//Do nothing
				}
				else
				{
					display(num);//Display the current number
					break;//Break out of the loop and go to the next loop to get operator
				}
			}
			else if (get_key()==('+'))
			{
				if (digits == 0)
				{
					//Do nothing
				}
				else
				{
					display('+');//Display the operator
					op = '+';//Set operator to +
				}
			}
			else if (get_key()==('-'))
			{
				if (digits == 0)
				{
					//Do nothing
				}
				else
				{
					display('-');
					op = '-';
				}
			}
			else if (get_key()==('*'))
			{
				if (digits == 0)
				{
					//Do nothing
				}
				else
				{
					display('*');
					op = '*';
				}
			}
			else if (get_key()==('/'))
			{
				if (digits == 0)
				{
					//Do nothing
				}
				else
				{
					display('/');
					op = '/';
				}
			}
			else if ((digits == 0)&(get_key()==0))
			{
				//Do nothing
			}
			else
			{
				digits++;//Increase the digits counter by 1
				key = get_key();//Fetch the number
				num = concatenate(num, key);//Shift the current number and add it to it
				display(num);//Display the number
			}
			return num;
		}
		
///////////////////////////////////////////////////////////////////////////////////
		///////////Get operator if we don't have it already//////////////////////
		
		//Second loop
		while(op != ('0'|'1'|'2'|'3'))
			if (digits == 0)
			{
				break;
			}
			else
			{
				if (get_key() == 0)
				{
					break;
				}
				else if (get_key()==('1'|'2'|'3'|'4'|'5'|'6'|'7'|'8'|'9'))
				{
					//Do nothing
				}
				else if (get_key()=='c')
				{
					lcdcmd(0x01);//Clear display
					digits = 0;
					digits2 = 0;
					break;
				}
				else if (get_key()=='A')
				{
					break;
				}
				else
				{
					if(op == ('+' | '-' | '*' | '/'))
					{
						break;
					}
					else if (get_key()=='+')
					{
						display('+');
						op = 0;
					}
					else if (get_key()=='-')
					{
						display('-');
						op = 1;
					}
					else if (get_key()=='*')
					{
						display('*');
						op = 2;
					}
					else if (get_key()=='/')
					{
						display('/');
						op = 3;
					}
				}
			}
			
//////////////////////////////////////////////////////////////////////
		//////////////Perform the operation////////////////////////
		
			if (op != ('0'|'1'|'2'|'3'))
		{
			break;
		}
		else
			{
			while(digits2 < 5)
			{
				if (get_key() == ('+' | '-' | '*' | '/'))
				{
					//Do nothing
				}
				else if (get_key()==('c'))
				{
					lcdcmd(0x01);
					digits = 0;
					digits2 = 0;
					break;
				}
				else if ((get_key()=='A')&(digits2 == 0))
				{
					break;
				}
				else
				{
					digits2++;
					key = get_key();
					num2= concatenate(num2, key);
				}
				return num2;
				}
			}
		if((op ==('0'|'1'|'2'|'3'))&(digits !=0)&(digits2 != 0))
		{
			if(op == '0')
			{
				ans = add(num,num2);
				lcdcmd(0x01);
				display (ans);
			}
			else if (op =='1')
			{
				ans = sub(num, num2);
				lcdcmd(0x01);
				display(ans);
			}
			else if (op =='2')
			{
				ans = mult(num, num2);
				lcdcmd(0x01);
				display(ans);
			}
			else if (op == '3')
			{
				ans = div(num, num2);
				lcdcmd(0x01);
				display(ans);
			}
			else
			{
				break;
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////////
////////////Below are the functions called in the main function///////////////
//////////////////////////////////////////////////////////////////////////////

//The function to initialize the keypad
//This is also the answer to
//Q2-III
void keypad_init(void){
	//On the hardware, we've connected:
	//Column0 to PE0, Column1 to PE1, Column2 to PE2, Column3 to PE3.
	//Row0 to PE4, Row1 to PE5, Row2 to PE6, Row3 to PE7.
	//Since we're reading the values on the rows and outputting on the columns;
	//PE0-3 are outputs and PE4-7 are inputs
	//PA:0,PB:1,PC:2,PD:3,PE:4,PF:5 <- Port clock bits
	SYSCTL_RCGC2_R |= (1 << 4); //Enable clock for port E
	//Create a 4 cycle delay for clock to stabilize
	volatile unsigned long delay;
	delay = SYSCTL_RCGC2_R;
	//Set directions for port E, 0 is input, 1 is output
	//By default on tm4c123gh6pm, they're all set to 0
	GPIO_PORTE_DIR_R |= ((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3));
	//Set port pins to digital, since they're switches (on/off)
	GPIO_PORTE_DEN_R |= (0xFFFF);	
	//Disable analogue functionality and alternative function of pins
	GPIO_PORTE_AFSEL_R &= ~0xFF; //Disable alternative functions for all pins in port E
  GPIO_PORTE_AMSEL_R &= ~0XFF; //Disable analogue function for all pins in port E
}

//This is the function to initialize the LCD
//This is also the answer to
//Q2-II
void LCD_init(void){
	//Keep in mind that the HD44780 controller automatically
	//initializes the LCD once the power is supplied to it
	//The following initialization will interface the Tiva c
	//board to the LCD and make sure it is ready to display
	//On the hardware, we've connected:
	//PF0 to Rs, PF1 to En, PF2 to R/W
	//PD0-PD7 to Data0-Data7, respectively
	//Since we're only writing to the LCD, all of these pins are outputs
	//PA:0,PB:1,PC:2,PD:3,PE:4,PF:5 <- Port clock bits
	SYSCTL_RCGC2_R |= ((1 << 3) | (1 << 5)); //Enable clock for ports D and F
	//Set directions for port E, 0 is input, 1 is output
	//By default on tm4c123gh6pm, they're all set to 0
	GPIO_PORTD_DIR_R |= ((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7));
	GPIO_PORTE_DIR_R |= ((1 << 0) | (1 << 1) | (1 << 2));
	//Configure the pins as digital
	GPIO_PORTD_DEN_R |= ((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7));
	GPIO_PORTE_DIR_R |= ((1 << 0) | (1 << 1) | (1 << 2));
	//Making sure the LCD is going to display, just in case
	lcdcmd(0x0E);//Turn on display with cursor blinking
	lcdcmd(0x01);//Clear display
	lcdcmd(0x80);//Force cursor to beginning of first line
}

//Delay in milliseconds
void delayMs(int n) //16 MHz CPU Clock assumed
{
  for(int i=0; i<n; i++)
  {
    for(int j=0; j<3180; j++)
    {
    }
  }
}

//Delay in microseconds
void delayUs(int n)
{
  for(int i=0; i<n; i++)
  {
    for(int j=0; j<3; j++)
    {
    }
  }
}

//The function to get the pressed key
//It grounds the columns, column by column
//Then it checks every row if it is grounded or not.
//When the column and row have intersected, then this is the pressed key.
unsigned char get_key(void)
{
	unsigned char key;
	//We check if there is a key pressed in the 1st column
  GPIO_PORTE_DATA_R &= ~(1<< 0);
	GPIO_PORTE_DATA_R |= ((1 << 1)|(1<<2)|(1<<3));
	
		if ((GPIO_PORTE_DATA_R & (1 << 4)) == 0)
	{
		key = 1;
	}
	else if((GPIO_PORTE_DATA_R & (1 << 5)) == 0)
	{
		key = 4;
	}
	else if ((GPIO_PORTE_DATA_R & (1 << 6)) == 0)
	{
		key = 7;
	}
	else if ((GPIO_PORTE_DATA_R & (1 << 7)) == 0)
	{
		key = 'c';
	}
	else
	{
		//Key was not in the 1st column
		//Now we check the 2nd column
		GPIO_PORTE_DATA_R &= ~(1 << 1);
		GPIO_PORTE_DATA_R |= ((1<<0)|(1<<2)|(1<<3));
		if((GPIO_PORTE_DATA_R & (1 << 4)) == 0)
			{
				key = 2;
			}
		else if((GPIO_PORTE_DATA_R & (1<<5))==0)
			{
				key = 5;
			}
		else if((GPIO_PORTE_DATA_R & (1<<6))==0)
			{
				key = 8;
			}
		else if((GPIO_PORTE_DATA_R & (1<<7))==0)
			{
				key = 0;
			}
		else
		{
			//Key was not int he 2nd column
			//Now we check the 3rd column
			GPIO_PORTE_DATA_R &= ~(1 << 2);
			GPIO_PORTE_DATA_R |= ((1<<0)|(1<<1)|(1<<3));
			if((GPIO_PORTE_DATA_R & (1<<4))==0)
			{
				key = 3;
			}
			else if ((GPIO_PORTE_DATA_R & (1<<5))==0)
			{
				key = 6;
			}
			else if ((GPIO_PORTE_DATA_R & (1<<6))==0)
			{
				key = 9;
			}
			else if ((GPIO_PORTE_DATA_R & (1<<7))==0)
			{
				key = 'A';
			}
			else
			{
				//Key was not in the 3rd column
				//Now we check the 4th column
				GPIO_PORTE_DATA_R &= ~(1<<3);
				GPIO_PORTE_DATA_R |= ((1<<0)|(1<<1)|(1<<2));
				if((GPIO_PORTE_DATA_R & (1<<4))==0)
				{
					key = '+';
				}
				else if ((GPIO_PORTE_DATA_R &(1<<5))==0)
				{
					key = '-';
				}
				else if((GPIO_PORTE_DATA_R & (1<<6))==0)
				{
					key = '*';
				}
				else if ((GPIO_PORTE_DATA_R & (1<<7))==0)
				{
					key = '/';
				}
			}
		}
	}

    return key;
}

//Function for sending Commands to the command register of LCD
void lcdcmd(unsigned char value)  
{	
	//Commands are used to perform special functions; other than display on LCD
	//The command that will be used in this assignment is:
	//0x0E to turn on display and cause cursor to blink
	//0x01 to clear display
	//0x80 to force cursor to beginning of the line
	GPIO_PORTD_DATA_R |= value;
	GPIO_PORTF_DATA_R &= ~((1 << 0) | (1 << 2));//rs = 0 and rw = 0
	GPIO_PORTF_DATA_R |= (1 << 1);//en = 1
	delayUs(50);
	GPIO_PORTF_DATA_R &= ~(1 << 1);//en = 0
return;
}
//Function for sending data to the data register of LCD
void display(unsigned char value)  
{
	GPIO_PORTD_DATA_R |= value;
	GPIO_PORTF_DATA_R &= ~(1 << 2);//rw = 0
	GPIO_PORTF_DATA_R |= ((1 << 0) | (1 << 1));//rs = 1 and en = 1;
	delayUs(500);
	GPIO_PORTF_DATA_R &= ~(1 << 1);//en = 0
	delayUs(500);
return;
}
unsigned concatenate(unsigned x, unsigned y)
{
	unsigned shifted;
	x = x*10;
	shifted = x + y;
	return shifted;
}
unsigned add(unsigned x, unsigned y)
{
	unsigned z;
	z = x + y;
	return z;
}
unsigned sub(unsigned x, unsigned y)
{
	unsigned z;
	z = x - y;
	return z;
}

unsigned mult(unsigned x, unsigned y)
{
	unsigned z;
	z = x*y;
	return z;
}
unsigned div(unsigned x, unsigned y)
{
	unsigned z;
	z = x/y;
	return z;
}
