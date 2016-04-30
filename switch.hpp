/***************************************************************/
/* Program:   433 remote control                               */
/*                                                             */
/* File name: switch.hpp                                       */
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

#include <wiringPi.h>
#include <time.h>


/******************************************/
/*    Definitions                         */
/******************************************/
//Internal codes
#define RTC_OK          0               // return code
#define RTC_NOK         -1              // return code

#define RTC_DEVICE_ADDRESS_OUT_OF_RANGE  1
#define RTC_DEVICE_NUMBER_OUT_OF_RANGE   2

//GPIO settings
#define	OUTPUT_PIN	0               // 0  -> GPIO17 / pin11
#define  IR_PIN 0							// Pin used for IR receive GPIO17 / pin11


//Switch possitions
#define SW_OFF             0
#define SW_ON              1

#define TICK_DELAY		10000 //0,1us


//Transmition frequency

#define CONRAD_RSL366R_CHAR_DELAY_NSEC	357000 //Conrad 433MHz transmitter


/******************************************/
/*    Declarations                        */
/******************************************/



/******************************************/
/*    Class  cTransciever433MHz           */
/*  Hndles 433MHz transmitter and         */
/*  receiver in order to control 433MHz   */
/*  devices                               */
/******************************************/


class cTransciever433MHz
{
   protected:
   char szOn[500];
   char szOff[500];
   
   timespec sleeptime;
   timespec remtime;
   
   int encode(short device_address, short device_number);
	void Send(char* szCode);
	
   
   public:
   cTransciever433MHz(void);
	int Init(void);      
   int SendCode(short device_address, short device_number, short switch_position);
   
   private:
   
};

/******************************************/
/*    Class  cTranscieverIR               */
/*  Hndles IR transmitter and             */
/*  receiver in order to control Infra Red*/
/*  devices                               */
/******************************************/


class cTranscieverIR
{
	protected:
   timespec sleeptime;
   timespec remtime;	

	int shift_tolerance;

	int codeIR[64];		//IR code

	char remote_map[64][64];   // button codes
	
	public:
	cTranscieverIR(void);
	int Init(void);

	int receive(void);
	int receive_word(void);
	int validate_code(int length);

	int store_remote_interface(void);
	int load_remote_interface(void);
	
	int print_codeIR(int length);
	int code_recognition(int length);

	
	

	private:
};


