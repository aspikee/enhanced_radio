/***************************************************************/
/* Program:   433 remote control                               */
/*                                                             */
/* File name: switch_test.cpp                                  */
/* Author:    Andrej Zavadsky                                  */
/* Comment:                                                    */
/*    Based on wiringPi librarby by Gerth                      */
/***************************************************************/
/* History:                                                    */
/* 2014-04-19 AZa   Initial version.                           */
/***************************************************************/

#include <stdio.h>
#include "switch.hpp"
#include <pthread.h>
#include <sys/mman.h>

int main(int argc, char **argv)
{ 
	/*
	int device_address, device_number;
	int on_off;

	if (argc != 4)
	{
		printf("usage: address 1-4 switch 1-4 1-on/0-off\n");
	}
	else
	{
		cTransciever433MHz *light_sw = new (cTransciever433MHz);

		if (light_sw->Init() != RTC_OK) 
		{
			printf("Swtich could not be initialized.\n");
			return -1;
		}

		device_address = atoi(argv[1]);
		device_number  = atoi(argv[2]);
		on_off			= atoi(argv[3]);

		light_sw->SendCode(device_address, device_number,on_off);

	}

	*/


	int code=0;
	long ms_on=0;
	long ms_off=0;
	int length;
	
	cTranscieverIR *IRrec = new (cTranscieverIR);

	if (IRrec->Init() != RTC_OK) 
	{
		printf("IRrec could not be initialized.\n");
		return -1;
	}

   // Lock memory to ensure no swapping is done.
   if(mlockall(MCL_FUTURE|MCL_CURRENT)){
			 fprintf(stderr,"WARNING: Failed to lock memory\n");
   }
   struct sched_param sp;
   sp.sched_priority = 30;
	
   if(pthread_setschedparam(pthread_self(), SCHED_FIFO, &sp)){
 			 printf("WARNING: Failed to set stepper thread"
 						"to real-time priority\n");
   }	
	pthread_setschedparam(pthread_self(), SCHED_FIFO, &sp);


	IRrec->load_remote_interface();
	IRrec->print_codeIR(0);
/*	
	while (1)
	{
		length = IRrec->receive();
		//IRrec->print_codeIR(length);
		printf("\n\n");

		IRrec->print_codeIR(length);
		code = IRrec->code_recognition(length);	
		switch (code)
		{
			case 0: printf("detected 0\n"); break;
			case 1: printf("detected 1\n"); break;
			case 2: printf("detected 2\n"); break;
			case 3: printf("detected 3\n"); break;
			case 4: printf("detected 4\n"); break;
			case 5: printf("detected 5\n"); break;
			case 6: printf("detected 6\n"); break;
			case 7: printf("detected 7\n"); break;
			case 8: printf("detected 8\n"); break;
			case 9: printf("detected 9\n"); break;
			case -1: printf("stored\n "); break;
			default: printf("not defined\n"); break;
		}

		
		delay(500);
		//IRrec->store_remote_interface();
	}
*/	
	//IRrec->normalize_code(length);
	//IRrec->print_codeIR(length);
	
	
	
	return 0;
} // main
