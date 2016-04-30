/***************************************************************/
/* Program:   433 remote control                               */
/*                                                             */
/* File name: switch.cpp                                       */
/* Author:    Andrej Zavadsky                                  */
/* Comment:                                                    */
/*    Based on wiringPi librarby by Gerth                      */
/***************************************************************/
/* History:                                                    */
/* 2014-04-19 AZa   Initial version.                           */
/***************************************************************/


/******************************************/
/*    Includes                            */
/******************************************/

#include "switch.hpp"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>



/******************************************/
/*    Functions / Methods                 */
/******************************************/



/******************************************/
/*    Class  cTransciever433MHz           */
/*  Hndles 433MHz transmitter and         */
/*  receiver in order to control 433MHz   */
/*  devices                               */
/******************************************/

//Constructor

cTransciever433MHz::cTransciever433MHz(void)
{

}

// Initiate GPIO 17   pin 11 (RPI Version A)

int cTransciever433MHz::Init(void)
{
	if (wiringPiSetup () != RTC_OK) return RTC_NOK;

   pinMode (OUTPUT_PIN, OUTPUT) ;
}


// Endcode address to 433MHz RC code
int cTransciever433MHz::encode(short device_address, short device_number)
{
   switch(device_address)
   {
      case (1):
         switch(device_number)
         {
            case (1):
                        strcpy(szOn, "000010001000100011101000111010001110100010001000111010001110100011101000111010001110100011101000111010000");
								strcpy(szOff,"000010001000100011101000111010001110100010001000111010001110100011101000111010001110100011101000100010000");           
               break;
            case (2):
 								strcpy(szOn, "000010001000100011101000111010001110100011101000100010001110100011101000111010001110100011101000111010000");
								strcpy(szOff,"000010001000100011101000111010001110100011101000100010001110100011101000111010001110100011101000100010000");
              break;
            case (3):
								strcpy(szOn, "000010001000100011101000111010001110100011101000111010001000100011101000111010001110100011101000111010000");
								strcpy(szOff,"000010001000100011101000111010001110100011101000111010001000100011101000111010001110100011101000100010000");
               break;
            case (4):
								strcpy(szOn, "000010001000100011101000111010001110100011101000111010001110100010001000111010001110100011101000111010000");
								strcpy(szOff,"000010001000100011101000111010001110100011101000111010001110100010001000111010001110100011101000100010000");
               break;
            default: return RTC_DEVICE_NUMBER_OUT_OF_RANGE;
         }
         break;
      case (2):
         // not yet defined
      case (3):
         // not yet defined
      case (4):
         // not yet defined

      default: return RTC_DEVICE_ADDRESS_OUT_OF_RANGE;
   }


  return RTC_OK;
}


// Function to send the output code to the RF transmitter connected to GPIO 17.
void cTransciever433MHz::Send(char* szCode)
{

	for (int iSend = 0 ; iSend < 10 ; iSend++)
	{
		sleeptime.tv_sec = 0;
		sleeptime.tv_nsec = CONRAD_RSL366R_CHAR_DELAY_NSEC; 

		for (int i = 0 ; i < strlen(szCode) ; i++)
		{
			if (szCode[i] == '1')
			{
				digitalWrite (OUTPUT_PIN, HIGH) ;                        				
			}
			else
			{
				digitalWrite (OUTPUT_PIN, LOW) ;				
			}
			nanosleep(&sleeptime,&remtime);
		}
		sleeptime.tv_nsec = 10000000; //10ms
		nanosleep(&sleeptime,&remtime);
	}
}


int cTransciever433MHz::SendCode(short device_address, short device_number, short switch_position)
{
	int rtc;	
	rtc = encode(device_address, device_number);
	if (rtc!= RTC_OK)
	{
		switch (rtc)
		{
		case RTC_DEVICE_ADDRESS_OUT_OF_RANGE:
			printf("Device address out of range.\n");
			break;
		case RTC_DEVICE_NUMBER_OUT_OF_RANGE:
			printf("Device number out of range.\n");
			break;
		default: printf("Unexpected error has occured during encoding data.\n");
			break;
		}
		return RTC_NOK;
	}


	switch (switch_position)
	{
		case SW_ON:
			printf("Sending %s\n", szOn);
			Send(szOn);
			break;
		case SW_OFF:
			printf("Sending %s\n", szOff);
			Send(szOff);
			break;
		default:	
			printf("Switch position needs to be 0 (off) or 1 (on).\n");
			return RTC_NOK;
		
	}
     

}



/******************************************/
/*    Class  cTranscieverIR               */
/*  Hndles IR transmitter and             */
/*  receiver in order to control Infra Red*/
/*  devices                               */
/******************************************/


//Constructor

cTranscieverIR::cTranscieverIR(void)
{
	shift_tolerance = 0;

	for (int i=0; i<64; i++)
	{
		codeIR[i] = 0;
		remote_map[0][i] = '\0';
	}
		

	
}

// Initiate GPIO 17   pin 11 (RPI Version A)

int cTranscieverIR::Init(void)
{
	if (wiringPiSetup () != RTC_OK) return RTC_NOK;

   pinMode (OUTPUT_PIN, INPUT) ;
}

int cTranscieverIR::receive_word(void)
{
	sleeptime.tv_nsec = 1000; //1us   = 1 tick
	int pulse_cnt=0;
	long ticks_on=0;
	long ticks_off=0;	
	int code;
	
	while (ticks_on < TICK_DELAY)
	{
		code = digitalRead(IR_PIN);
		if (code == 0)
		{
			ticks_off += 1;
			if (ticks_on > 0)	
			{
				codeIR[pulse_cnt] = ticks_on;
				pulse_cnt++;
				ticks_on = 0;				
			}				
		}
		if (code == 1)
		{
			ticks_on += 1;			
			if (ticks_off > 0)	
			{
				codeIR[pulse_cnt] = ticks_off;
				pulse_cnt++;
				ticks_off = 0;
			}			
		}
		
		//nanosleep(&sleeptime,&remtime);
	}	

	return pulse_cnt;
}
int cTranscieverIR::receive(void)
{
	int code;
	int pulse_cnt =0;
	int len =0;


	
	while ( code = (digitalRead(IR_PIN)) ||
			  (len > TICK_DELAY)					)
	{
		if (len<TICK_DELAY) len++;
	}

   // Set our thread to real time priority
	


	pulse_cnt = receive_word();

	
	return pulse_cnt;
}

int cTranscieverIR::validate_code(int length)
{
	
	int high_value=0, 
		 low_value=32000;	
	float average=0.0;
		 
	for (int i=0;i<length;i++)
	{
		if (codeIR[i] > high_value) 
			high_value = codeIR[i];
		if (codeIR[i] < low_value)
			low_value = codeIR[i];
			
		average += codeIR[i];
	}

	average = average / length;
	

	shift_tolerance = (int) (low_value / 3); 
	printf("min %d, max %d, average %d, tolerance %d\n", low_value, high_value, shift_tolerance,(int)(average));

	if (shift_tolerance < (high_value / 2 - low_value))
		return RTC_NOK;
	else
		return RTC_OK;
	
}



int cTranscieverIR::print_codeIR(int length)
{
	int cnt=0;
   for (int i=0; i < length; i++)
	{
		printf(" %d:%d", (i % 2)?1:0, codeIR[i]);
	}
	
   while ((remote_map[cnt][0] != '\0') && cnt<64)
	{	
		printf("%d: %s\n", cnt, remote_map[cnt]);
		cnt++;
	}


}

int cTranscieverIR::code_recognition(int length)
{
	char key[64];
	int cnt=0;
	int cFound = -32000;

	
	for (int i=0; i<length; i++)
	{
		if (codeIR[i] > 5000)
		{
			key[cnt] = (i % 2)?'1':'0';
			cnt++;
			key[cnt] = (i % 2)?'1':'0' ;
		}
		else
		{
			key[cnt] = (i % 2)?'1':'0';
		}
		cnt++;
			
	}
	key[cnt] = '\0';

	cnt=0;
	while (cFound == -32000 && cnt != 64)
	{
		if (!strcmp(key, remote_map[cnt]))
		{
			cFound = cnt;			
		}			
		if (remote_map[cnt][0] == '\0')
		{
			strcpy(remote_map[cnt], key);		
			cFound = -1;
		}		
		cnt++;
	}

	
	return cFound;
}

int cTranscieverIR::store_remote_interface()
{
	FILE * fd=NULL;
	int cnt=0;
	
	const char * filename = "remote_philips.conf";
	fd = fopen ( filename, "w" );

	while (cnt < 64 && remote_map[cnt][0] != '\0')
	{
		fprintf(fd, "Code %d:<%s>\n", cnt, remote_map[cnt]);
		cnt++;
	}

	fclose(fd);
	
}

int cTranscieverIR::load_remote_interface()
{
	FILE * fd=NULL;
	int cnt=0;
	char key[64];
	char line[100];
	short record=0;
	int code_amount=0;
	
	const char * filename = "remote_philips.conf";
	fd = fopen ( filename, "r" );

	while (!feof(fd))
	{
		fgets(line, 100, fd);
		if (!feof(fd)) 
		{
			cnt=0;
			record=0;
			for (int i=0; i<100; i++)
			{
				if (line[i] == '\0') break;
				if (line[i] == '<') 
				{
					record=1;
					i++;
				}
				if (line[i] == '>') 
				{
					record=0;
					key[cnt] = '\0';
				}
				if (record) 
				{
					key[cnt] = line[i];
					cnt++;
				}

			}
			strcpy(remote_map[code_amount], key);
			code_amount++;
		}
	}
	fclose(fd);

}
