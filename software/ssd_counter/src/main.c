// Header Files
#include <stdio.h>
#include "xbasic_types.h"
#include "xparameters.h"
#include "xgpio.h"
#include <math.h>

// Parameter Definitions
#define BTNS_DEVICE_ID		   XPAR_AXI_GPIO_0_DEVICE_ID // Push Buttons ID

Xuint32 *ssd_base_addr_ptr = (Xuint32 *)XPAR_CUSTOM_SSD_IP_0_S00_AXI_BASEADDR;
XGpio BTNInst; // GPIOs instances

static int btn_value;      // Global Variable to hold the button value
static int old_btn_value;  // Global Variable to hold the previous button value
static int count;		   // Holds the true/current count
static int count_copy;     // Holds the count copy for the program to calculate and use for SSD count calculation
static int count_ones;     // Holds the count/digit for the right side (ones place) of the SSD
static int count_tens;     // Holds the count/digit for the left side  (tens place) of the SSD

//----------------------------------------------------
// PROTOTYPE FUNCTIONS
//----------------------------------------------------
static void ssd_display_ones_place(int num);
static void ssd_display_tens_place(int num);

// Function is responsible for the right side of the SSD
// The function will light up the appropriate segments based on the digit given.
static void ssd_display_ones_place(int num){

	switch(num){
		case 0:
			*(ssd_base_addr_ptr+0)=0x3F;
			break;
		case 1:
			*(ssd_base_addr_ptr+0)=0x6;
			break;
		case 2:
			*(ssd_base_addr_ptr+0)=0x5B;
			break;
		case 3:
			*(ssd_base_addr_ptr+0)=0x4F;
			break;
		case 4:
			*(ssd_base_addr_ptr+0)=0x66;
			break;
		case 5:
			*(ssd_base_addr_ptr+0)=0x6D;
			break;
		case 6:
			*(ssd_base_addr_ptr+0)=0x7D;
			break;
		case 7:
			*(ssd_base_addr_ptr+0)=0x7;
			break;
		case 8:
			*(ssd_base_addr_ptr+0)=0x7F;
			break;
		case 9:
			*(ssd_base_addr_ptr+0)=0x6F;
			break;
		default:
			*(ssd_base_addr_ptr+0)=0x00;
	}

}

// Function is responsible for the left side of the SSD
// The function will light up the appropriate segments based on the digit given.
// Difference between the ssd_display_ones_place and ssd_display_tens_place is that the function writes a 1 to the 8th bit hence the + 128.
static void ssd_display_tens_place(int num){

	switch(num){
		case 0:
			*(ssd_base_addr_ptr+0)=0x3F + 128;
			break;
		case 1:
			*(ssd_base_addr_ptr+0)=0x6 + 128;
			break;
		case 2:
			*(ssd_base_addr_ptr+0)=0x5B + 128;
			break;
		case 3:
			*(ssd_base_addr_ptr+0)=0x4F + 128;
			break;
		case 4:
			*(ssd_base_addr_ptr+0)=0x66 + 128;
			break;
		case 5:
			*(ssd_base_addr_ptr+0)=0x6D + 128;
			break;
		case 6:
			*(ssd_base_addr_ptr+0)=0x7D + 128;
			break;
		case 7:
			*(ssd_base_addr_ptr+0)=0x7 + 128;
			break;
		case 8:
			*(ssd_base_addr_ptr+0)=0x7F + 128;
			break;
		case 9:
			*(ssd_base_addr_ptr+0)=0x6F + 128;
			break;
		default:
			*(ssd_base_addr_ptr+0)=0x00;
	}

}

int main (void){

	int status; // main method variable for status

	//----------------------------------------------------
	// INITIALIZE THE PERIPHERALS & SET DIRECTIONS OF GPIO
	//----------------------------------------------------

	// Initialise Push Buttons
	status = XGpio_Initialize(&BTNInst, BTNS_DEVICE_ID);
	if(status != XST_SUCCESS) return XST_FAILURE;

	// Set all buttons direction to inputs
	// Setting Pins for buttons to be inputs.
	XGpio_SetDataDirection(&BTNInst, 1, 0xFF);

	// Initialize Count and Button Values
	count = 0;
	btn_value = 0;

	// Run a loop forever
	while(1){

		// Reading value from buttons
		btn_value = XGpio_DiscreteRead(&BTNInst, 1);

		// if a new button press
		if (btn_value != old_btn_value){
			// new button value is old
			old_btn_value = btn_value;

			// Update count
			count = count + btn_value;
		}

		// if count is 100 or greater
		if (count >= 100){
			// subtract the value by 100
			// the subtraction is a new count
			count = count-100;
		}

		// Convert Count
		count_copy = count;
		if (count_copy >= 10) {
			count_ones = 0;
			while ( (count_copy % 10) != 0 ){
				count_copy--;
				count_ones++;
			}
			count_tens = count_copy / 10;
		}
		else{
			count_tens = 0;
			count_ones = count;
		}

		// SSD Display Ones Place
		ssd_display_ones_place(count_ones);

		// SSD Display Tens Place
	    ssd_display_tens_place(count_tens);
	}

	return 0;
}
