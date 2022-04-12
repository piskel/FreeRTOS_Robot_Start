
#include "MKL46Z4.h"

/*--------------------------------------------------
 * Function:  portCONFIGURE_TIMER_FOR_RUNT_TIME_STATS
 *
 * Description: defined as a function rather than a MACRO - does not require changing the FreeRTOSConfig.h file.
 *
 * This function configures timer 3 with 16 microsecond increments.  TCNT3 can be used as the counter value
 * to return (see portGET_RUN_TIME_COUNTER_VALUE() function).
 *-----------------------------------------------------------*/
void xconfigTimerForRunTimeStats(void)
{
//	// Enable LPTMR0 clock
//	SIM->SCGC5|=SIM_SCGC5_LPTMR_MASK;
//	// Enable OSC clock as LPTMR clock source
//	OSC0->CR|=OSC_CR_ERCLKEN_MASK;
//
//	// Low Power Timer Control Status Register (LPTMRx_CSR)
//	// Timer interrupt disabled.
//	// CNR counter is reset whenever TCF is set (compare)
//	// Time Counter mode
//	// LPTMR is disable
//	LPTMR0->CSR=0;
//
//	// Low Power Timer Prescale Register (LPTMRx_PSR)
//	// Source clock external osc 8MHz divide by 2
//	LPTMR0->PSR=0|LPTMR_PSR_PCS(3);
//
//	// Low Power Timer Compare Register (LPTMRx_CMR)
//	LPTMR0->CMR=64; // 16us at 4MHz
//
//	// Low Power Timer Control Status Register (LPTMRx_CSR)
//	// LPTMR start
//	LPTMR0->CSR|=LPTMR_CSR_TEN_MASK;
		// System Options Register 2 (SIM_SOPT2)
			// Choix de l'horloge source pour le TPM
			// Source du clock MCGPLLCLK/2, donc 24MHz
			// 24MHz/20kHz=1200, duty 50% -> 600
			// 24MHz/400Hz=60000, duty 50% -> 30000

			SIM->SOPT2|=SIM_SOPT2_TPMSRC(1)|SIM_SOPT2_PLLFLLSEL_MASK;

			// Enable du clock du TPM
			// System Clock Gating Control Register 6 (SIM_SCGC6)
			SIM->SCGC6|=SIM_SCGC6_TPM2_MASK;	// IC

//			//--------------------------------------------------------------
//			// TPM2
//			//--------------------------------------------------------------
//			// Status and Control (TPMx_SC)
//			// Disables DMA transfers
//			// Disable TOF interrupts. Use software polling or DMA request
//			// TPM counter operates in up counting mode
//			// TPM counter increments on every TPM counter clock
//			// Prescale Factor Selection: Divide by 1 --> 24MHz/1=24MHz --> 1 step toutes les 41.66 ns
//			TPM2->SC=0|(TPM_SC_CMOD(1)|TPM_SC_PS(0));
//
//			// Channel (n) Status and Control (TPMx_CnSC)
//			// OC --> toggle output et enable de l'interruption
//			TPM2->CONTROLS[0].CnSC=0|(TPM_CnSC_ELSA_MASK|TPM_CnSC_MSA_MASK);	// OC
//
//			// Modulo (TPMx_MOD)
//			// Systicks
//			TPM2->MOD=65535;
}

/*--------------------------------------------------
 * Function:  portGET_RUN_TIME_COUNTER_VALUE
 *
 * Description: Defined as an inline function rather than a MACRO - does not require changing the FreeRTOSConfig.h file.
 *
 * This function configures timer 3 with 16 microsecond increments.  TCNT3 can be used as the counter value
 * to return (see portGET_RUN_TIME_COUNTER_VALUE() function).
 *-----------------------------------------------------------*/
//inline uint16_t portGET_RUN_TIME_COUNTER_VALUE()  { return TCNT1; }
