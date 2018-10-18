/*
 * Edge.c
 *
 *  Created on: 17/10/2018
 *      Author: Fer Muñoz
 */


/**
	\file
	\brief
		This is the starter file of FlexTimer.
		In this file the FlexTimer is configured in overflow mode.
	\author J. Luis Pizano Escalante, luispizano@iteso.mx
	\date	7/09/2014
	\todo
	    Add configuration structures.
 */



#include "FlexTimer.h"
#include "MK64F12.h"

void FTM0_ISR()
{
	FTM0->SC &= ~FLEX_TIMER_TOF;
	GPIOD->PDOR ^= 0xFF;
}


void FlexTimer_updateCHValue(sint16 channelValue)
{
	/**Assigns a new value for the duty cycle*/
	FTM0->CONTROLS[0].CnV = channelValue;
}


void FlexTimer_Init()
{
	/** Clock gating for the FlexTimer 0*/
		SIM->SCGC6 |= FLEX_TIMER_0_CLOCK_GATING;
		/**When write protection is enabled (WPDIS = 0), write protected bits cannot be written.
		* When write protection is disabled (WPDIS = 1), write protected bits can be written.*/
		FTM0->MODE |= FLEX_TIMER_WPDIS;
		/**Enables the writing over all registers*/
		FTM0->MODE &= ~FLEX_TIMER_FTMEN;
		/**Assigning a default value for modulo register*/
		FTM0->MOD = 0x00FF;
		/**Selects the Edge-Aligned PWM mode mode*/
		FTM0->CONTROLS[0].CnSC = FLEX_TIMER_MSB | FLEX_TIMER_ELSB;
		/**Assign a duty cycle of 50%*/
		FTM0->CONTROLS[0].CnV = FTM0->MOD/2;
		/**Configure the times*/
		FTM0->SC = FLEX_TIMER_CLKS_1|FLEX_TIMER_PS_128;
}


void PWMOutput_CenterAlignment(void)
{
	SIM -> SCGC6 |= 0x03000000; //enable FTM0 and FTM0 module clock //MODIFIQUE LA ESTRCUCTURA SIMILAR A LA DE ARRIBA
	SIM -> SCGC5 = SIM -> SCGC5|0x3E00; //enable port A/B/C/D/E clock
	FTM0 -> CONF = 0xC0; //set up BDM in 11
	FTM0 -> FMS = 0x00; //clear the WPEN so that WPDIS is set in FTM0_MODE reg
	FTM0 -> MODE|=0x05; //enable write the FTM CnV register
	FTM0 -> MOD = 1000;
    FTM0 -> CONTROLS[0].CnSC = 0x28; ////center-alignment, PWM begins with High //QUÉ ES C0SC
    FTM0 -> CONTROLS[1].CnSC = 0x28; //PWM waveform is high-low-high
    FTM0 -> COMBINE = 0x02; //complementary mode for CH0&CH1 of FTM0
    FTM0 -> COMBINE |= 0x10; // dead timer insertion enabled in complementary mode for //CH0&CH1 of

	FTM0 -> DEADTIME = 0x1F; //dead time is 16 system clock cycles
    FTM0 -> CONTROLS[1].CnV = 500;
    FTM0 -> CONTROLS[0].CnV = 500;
    FTM0 -> CNTIN = 0x00;
    FTM0 -> CONTROLS[2].CnSC = 0x28;
    FTM0 -> CONTROLS[3].CnSC = 0x28;
    FTM0 -> COMBINE |= 0x0200;
    FTM0 -> COMBINE |= 0x1000;
    FTM0 -> CONTROLS[3].CnV = 250;
    FTM0 -> CONTROLS[2].CnV = 250;
    FTM0 -> SC = 0x68;
}

void PWMOutput_EdgeAlignment(void)
{
	SIM -> SCGC6 |= 0x03000000; //enable FTM0 and FTM0 module clock
	FTM0 -> CONF = 0xC0; //set up BDM in 11
	FTM0 -> FMS = 0x00; //clear the WPEN so that WPDIS is set in FTM0_MODE register
	FTM0 -> MODE |= 0x05; //enable write the FTM CnV register
	FTM0 -> MOD = 1000;
	FTM0 -> CONTROLS[0].CnSC = 0x28; //edge-alignment, PWM initial state is High, becomes low //afteR match
	FTM0 -> CONTROLS[1].CnSC = 0x28;
	FTM0 -> COMBINE = 0x02; //complementary mode for CH0&CH1 of FTM0
	FTM0 -> COMBINE |= 0x10; //dead timer insertion enabled in complementary mode for //CH0&CH1 of
	FTM0 -> CONTROLS[1].CnV = 500;
	FTM0 -> CONTROLS[0].CnV = 500;
	FTM0 -> CONTROLS[2].CnSC = 0x28;
	FTM0 -> CONTROLS[3].CnSC = 0x28;
	FTM0 -> COMBINE |= 0x0200;
	FTM0 -> COMBINE |= 0x1000;
	FTM0 -> DEADTIME = 0x00;
	FTM0 -> CONTROLS[3].CnV = 250;
	FTM0 -> CONTROLS[2].CnV = 250;
	FTM0 -> CNTIN = 0x00;
	FTM0 -> SC = 0x08; //PWM edge_alignment, system clock driving, dividing by 1
}
