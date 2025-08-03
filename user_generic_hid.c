/********************************************************************
 *
 *			                High Res (1024 step) PCTx
 *
 ********************************************************************
 *
 * Author               Date        Comment
 ********************************************************************
 * Phillip Pickett     10.5.08
 * Phillip Pickett     10.5.08		Added HR to the USB device name
 ********************************************************************/

#include <p18cxxx.h>
#include <usart.h>
#include <delays.h>
#include "typedefs.h"
#include "usb.h"
#include "io_cfg.h"             // I/O pin mapping
#include "user_generic_hid.h"

/** V A R I A B L E S ********************************************************/
#pragma udata

byte number_of_bytes_read;
unsigned char transmit_buffer[EP0_BUFF_SIZE];
unsigned char receive_buffer[EP0_BUFF_SIZE];

int count;
int i;
unsigned int pulse1, pulse2, pulse3, pulse4, pulse5, pulse6, pulse7, pulse8, pulse9;
unsigned int mul1, mul2, mul3, mul4, mul5, mul6, mul7, mul8, mul9;
void step(unsigned char);
void ReportLoopback(void);
void HandleControlOutReport(void);

/** D E C L A R A T I O N S **************************************************/
#pragma code
void UserInit(void) {
	TRISCbits.TRISC0 = 0; //pin c0 output

	//pre/post scale registers from right to left in increasing order
	// Timer2 prescale = 16
	T2CONbits.T2CKPS0 = 0;
	T2CONbits.T2CKPS1 = 1;
	
	// Timer2 postscale = 16
	T2CONbits.T2OUTPS0 = 1;
	T2CONbits.T2OUTPS1 = 1;
	T2CONbits.T2OUTPS2 = 1;
	T2CONbits.T2OUTPS3 = 1;

	// Timer period
	PR2 = 0xE9;  //233
	//83.3*16*16*(233+1)= 5ms

	//count = 2;
	//120 = 1ms
	//240 = 2ms
	//36 = .3ms

	//vista needs this to be small otherwise err code 10 in OS
	pulse1 = -1; //a check for vista
	pulse2 = 1;
	pulse3 = 1;
	pulse4 = 1;
	pulse5 = 1;
	pulse6 = 1;
	pulse7 = 1;
	pulse8 = 1;
	pulse8 = 1;
	pulse9 = 1;

	mul1 = 0;
	mul2 = 0;
	mul3 = 0;
	mul4 = 0;
	mul5 = 0;
	mul6 = 0;
	mul7 = 0;
	mul8 = 0;
	mul9 = 0;

	// Timer2 is on
	T2CONbits.TMR2ON = 1;

    // Enable Timer2 interrupt
    PIE1bits.TMR2IE = 1;
}


void ProcessIO(void) {   
    if((usb_device_state < CONFIGURED_STATE)||(UCONbits.SUSPND==1)) { return; }
    
	ReportLoopback();
	if (pulse1 != -1){ //fixed the error code 10 message in vista
	    if (PIR1bits.TMR2IF) {			// if the timer interrupt bit is set
				T2CONbits.TMR2ON = 0; 	//turn off the timer
	
				LATCbits.LATC0 = 0;
				Delay100TCYx(36);
				//Delay100TCYx(240);
	
				//C0
	    		LATCbits.LATC0 = 1; 	
				Delay100TCYx(90);
				step(pulse1);
				if(mul1 == 1){
					step(255);
				}
				else if( mul1 == 2) {
					step(255);
					step(255);
				}
				else if( mul1 == 3) {
					step(255);
					step(255);
					step(255);
				}
	    		LATCbits.LATC0 = 0; 
	
				Delay100TCYx(36);
	
	    		LATCbits.LATC0 = 1; 	
				Delay100TCYx(90);
				step(pulse2);
				if(mul2 == 1){
					step(255);
				}
				else if( mul2 == 2) {
					step(255);
					step(255);
				}
				else if( mul2 == 3) {
					step(255);
					step(255);
					step(255);
				}	
	    		LATCbits.LATC0 = 0; 
	
				Delay100TCYx(36);
	
	    		LATCbits.LATC0 = 1; 	
				Delay100TCYx(90);
				step(pulse3);
				if(mul3 == 1){
					step(255);
				}
				else if( mul3 == 2) {
					step(255);
					step(255);
				}
				else if( mul3 == 3) {
					step(255);
					step(255);
					step(255);
				}	
	    		LATCbits.LATC0 = 0;
	
				Delay100TCYx(36);
	
	    		LATCbits.LATC0 = 1; 	
				Delay100TCYx(90);
				step(pulse4);
				if(mul4 == 1){
					step(255);
				}
				else if( mul4 == 2) {
					step(255);
					step(255);
				}
				else if( mul4 == 3) {
					step(255);
					step(255);
					step(255);
				} 	
	    		LATCbits.LATC0 = 0;
	
				Delay100TCYx(36);
	
	    		LATCbits.LATC0 = 1; 	
				Delay100TCYx(90);
				step(pulse5);
				if(mul5 == 1){
					step(255);
				}
				else if( mul5 == 2) {
					step(255);
					step(255);
				}
				else if( mul5 == 3) {
					step(255);
					step(255);
					step(255);
				}	
	    		LATCbits.LATC0 = 0; 
	
				Delay100TCYx(36);
	
	    		LATCbits.LATC0 = 1; 	
				Delay100TCYx(90);
				step(pulse6);
				if(mul6 == 1){
					step(255);
				}
				else if( mul6 == 2) {
					step(255);
					step(255);
				}
				else if( mul6 == 3) {
					step(255);
					step(255);
					step(255);
				}	
	    		LATCbits.LATC0 = 0; 
	
				Delay100TCYx(36);
	
	    		LATCbits.LATC0 = 1; 	
				Delay100TCYx(90);
				step(pulse7);
				if(mul7 == 1){
					step(255);
				}
				else if( mul7 == 2) {
					step(255);
					step(255);
				}
				else if( mul7 == 3) {
					step(255);
					step(255);
					step(255);
				}	
	    		LATCbits.LATC0 = 0;
	
				Delay100TCYx(36);
	
	    		LATCbits.LATC0 = 1; 	
				Delay100TCYx(90);
				step(pulse8);
				if(mul8 == 1){
					step(255);
				}
				else if( mul8 == 2) {
					step(255);
					step(255);
				}
				else if( mul8 == 3) {
					step(255);
					step(255);
					step(255);
				} 	
	    		LATCbits.LATC0 = 0;
	
				Delay100TCYx(36);
	
	    		LATCbits.LATC0 = 1; 	
				Delay100TCYx(90);
				step(pulse9);
				if(mul9 == 1){
					step(255);
				}
				else if( mul9 == 2) {
					step(255);
					step(255);
				}
				else if( mul9 == 3) {
					step(255);
					step(255);
					step(255);
				} 	
	    		LATCbits.LATC0 = 0;
	
				Delay100TCYx(36);
				LATCbits.LATC0 = 1;		

				Delay100TCYx(240);
	
		        PIR1bits.TMR2IF = 0; 	//reset the timer
				T2CONbits.TMR2ON = 1; 	//turn the timer on 
	    } 
	}
}


void ReportLoopback(void) {
	byte count;

	// Find out if an Output report has been received from the host.
	number_of_bytes_read = HIDRxReport(receive_buffer, HID_OUTPUT_REPORT_BYTES);

	if (number_of_bytes_read > 0) {
		// An Output report was received.
		pulse1 = receive_buffer[0];  //servo1
		mul1 = receive_buffer[1]; 	 //servo1
		pulse2 = receive_buffer[2];  //servo2
		mul2 = receive_buffer[3]; 	 //servo2
		pulse3 = receive_buffer[4];  //servo3
		mul3 = receive_buffer[5]; 	 //servo3
		pulse4 = receive_buffer[6];  //servo4
		mul4 = receive_buffer[7]; 	 //servo4
		pulse5 = receive_buffer[8];  //servo5
		mul5 = receive_buffer[9];    //servo5
		pulse6 = receive_buffer[10]; //servo6
		mul6 = receive_buffer[11]; 	 //servo6
		pulse7 = receive_buffer[12]; //servo7
		mul7 = receive_buffer[13]; 	 //servo7
		pulse8 = receive_buffer[14]; //servo8
		mul8 = receive_buffer[15]; 	 //servo8
		pulse9 = receive_buffer[16]; //servo9
		mul9 = receive_buffer[17]; 	 //servo9

		for (count = 1; count <= HID_OUTPUT_REPORT_BYTES; count = count + 1) {				
//			transmit_buffer[count-1] = receive_buffer[count-1];			
			transmit_buffer[count-1] = 0;			
		}

		// If necessary, wait until the interrupt IN endpoint isn't busy.
		while(mHIDTxIsBusy()) {
			// Service USB interrupts.
			USBDriverService(); 
		}
		// The report will be sent in the next interrupt IN transfer.
   	    HIDTxReport(transmit_buffer, HID_INPUT_REPORT_BYTES);
	}
}


void HandleControlOutReport()
{
	byte count;

	// Find out if an Output or Feature report has arrived on the control pipe.
	// Get the report type from the Setup packet.

	switch (MSB(SetupPkt.W_Value))
    {
		case 0x02: // Output report 

			// Get the report ID from the Setup packet.

    		switch(LSB(SetupPkt.W_Value))
		    {
				case 0: // Report ID 0

					// This example application copies the Output report data 
					// to hid_report_in. 
					// (Assumes Input and Output reports are the same length.)
					// A "real" application would do something more useful with the data.

				    // wCount holds the number of bytes read in the Data stage.
					// This example assumes the report fits in one transaction.	
				
					for (count = 1; count <= HID_OUTPUT_REPORT_BYTES; count = count + 1)
					{
						hid_report_in[count-1] = hid_report_out[count-1];
					}				
					// The number of bytes in the report (from usbcfg.h).
		
					break;		
			} // end switch(LSB(SetupPkt.W_Value))

		case 0x03: // Feature report 

			// Get the report ID from the Setup packet.

    		switch(LSB(SetupPkt.W_Value))
		    {
				case 0: // Report ID 0

				// The Feature report data is in hid_report_feature.
				// This example application just sends the data back in the next
				// Get_Report request for a Feature report.			
			
			    // wCount holds the number of bytes read in the Data stage.
				// This example assumes the report fits in one transaction.	
		
				break;
			} // end switch(LSB(SetupPkt.W_Value))		

	} // end switch(MSB(SetupPkt.W_Value))

} // end HandleControlOutReport


void GetInputReport0(void)
{
	byte count;

	// Set pSrc.bRam to point to the report.
	// 

	pSrc.bRam = (byte*)&hid_report_in;

	// The number of bytes in the report (from usbcfg.h).

	LSB(wCount) = HID_INPUT_REPORT_BYTES;

} // end GetInputReport0


void GetFeatureReport0(void)
{
	byte count;

	// Set pSrc.bRam to point to the report.

	pSrc.bRam = (byte*)&hid_report_feature;

	// The number of bytes in the report (from usbcfg.h).

    LSB(wCount) = HID_FEATURE_REPORT_BYTES;

} // end GetFeatureReport0



//12TCY
void step(unsigned char i){
	 do{
	_asm
	   nop
	   nop
	_endasm
	} while (i--);
}

/*
Math

12MIPS = 12,000,000 ip second
1sec = 1000ms
12,000,000/1000 = 12,000 ip ms

12,000/128 = 93.75 ip step
12,000/256 = 46.875 ip step
12,000/512 = 23.4375 ip step
12,000/1024 = 11.71875 ip step
12,000/2048 = 5.859375 ip step
*/