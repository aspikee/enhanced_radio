/***************************************************************/
/* Program:   Enhanced Radio                                   */
/*                                                             */
/* File name: radio.cpp                                        */
/* Author:    Andrej Zavadsky                                  */
/* Comment:                                                    */
/*    Based on wiringPi librarby by Gerth                      */
/***************************************************************/
/* History:                                                    */
/* 2014-04-19 AZa   Initial version.                           */
/***************************************************************/

#include <stdio.h>
#include <string.h>
#include "switch.hpp"
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <sys/mman.h>

int main(int argc, char **argv)
{ 
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
	
   if(pthread_setschedparam(pthread_self(), SCHED_FIFO, &sp))
   {
      printf("WARNING: Failed to set stepper thread"
                              "to real-time priority\n");
   }	
      pthread_setschedparam(pthread_self(), SCHED_FIFO, &sp);



	
	return RTC_OK;
}