/**
 * @file    Test_lib_pac9633_main.c 
 * @author 	Alexis ROLLAND
 * @date	2022-03
 * @brief 	Test app for PCA9633 driver
 *  
 *
 */

#include "lib_test_lib_pca9633.h"   // Inclusion du fichier .h "Applicatif" renommé

/**
 * Insérer Ici les bits de configuration pour le MCU 						  
 * Copier / Coller depuis le fichier Config_Pic24.txt					  
 */
/*--------------------------------------------------------------------------*/
/* PIC24FJ128GA010															*/
/*--------------------------------------------------------------------------*/
#pragma config POSCMOD = HS         // Primary Oscillator Select (EC, XT, HS, NONE)
#pragma config OSCIOFNC = ON        // Primary Oscillator Output Function (ON, OFF)
#pragma config FCKSM = CSDCMD       // Clock Switching and Monitor (CSECME, CSECMD, CSDCMD)
#pragma config FNOSC = PRI          // Oscillator Select (FRC, FRCPLL, PRI, PRIPLL, SOSC, LPRC, FRCDIV)
#pragma config IESO = OFF           // Internal External Switch Over Mode (ON, OFF)
#pragma config WDTPS = PS256        // Watchdog Timer Postscaler (PS1, PS2, PS4, PS8,..., PS32768)
#pragma config FWPSA = PR32         // WDT Prescaler (PR32, PR128)
#pragma config WINDIS = OFF         // Watchdog Timer Window (ON, OFF)
#pragma config FWDTEN = OFF         // Watchdog Timer Enable (ON, OFF)
#pragma config ICS = PGx2           // Comm Channel Select (PGx1, PGx2)
#pragma config BKBUG = OFF          // Background Debug (ON, OFF)
#pragma config GWRP = OFF           // General Code Segment Write Protect (ON, OFF)
#pragma config GCP = OFF            // General Code Segment Code Protect (ON, OFF)
#pragma config JTAGEN = OFF         // JTAG Port Enable (ON, OFF)
//----------------------------------------------------------------------------
/* Directives de compilation - Macros		*/

/* Déclarations des variables globales 	*/
extern  pca9633_desc_t  MyPca9633;

/* Programme Principal			*/

int main(void)
{
// Variables locales au main
    pca9633_err_t Res = 0;
    pca9633_pwm_t PwmValues={0,0,0,0};
    pca9633_osc_status_t  OscStatus;

    Initialiser();		// Appel fonction d'initialisation

    while(1)
    {
        __delay_ms(1000);
                
        PwmValues.Pwm0 = 0x00;      // Blue
        PwmValues.Pwm1 = 0x00;      // Green
        PwmValues.Pwm2 = 0xFF;      // Red
        
        Res = pca9633_setPWM(&MyPca9633,&PwmValues);
        if (Res != PCA9633_OK) error_handler();
        
        Res = pca9633_get_osc_status(&MyPca9633, &OscStatus);
        if (Res != PCA9633_OK) error_handler();
        
        if (OscStatus == PCA9633_OSC_IS_ON) Res = pca9633_osc_off(&MyPca9633);
        else Res = pca9633_osc_on(&MyPca9633);
        if (Res != PCA9633_OK) error_handler();
        LATAbits.LATA0 = OscStatus;
    }
}					
