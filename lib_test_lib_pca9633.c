/**
 * @file 
 * @author 	Alexis ROLLAND
 * @date	
 * @brief 	
 */


#include "lib_test_lib_pca9633.h"

/* Directives de compilation - Macros		*/

/* Déclarations des variables globales 	*/
pca9633_desc_t  MyPca9633;
i2c_desc_t      I2CModule;

/*	Implémentation du code */
//------------------------------------------------------------------------------
void Initialiser(void)
{
    pca9633_err_t   Res;
    pca9633_config_t    pcaCfg;
    
    // Leds
    TRISA &= 0xFF00;
    LATA = 0;
            
    // PCA9633
    pcaCfg.pi2c = &I2CModule;
    pcaCfg.initType = INIT_WITH_I2C1;
    pcaCfg.i2c_Address = PCA9633_DEFAULT_I2C_ADDR;
    
    Res = pca9633_init(&MyPca9633,&pcaCfg);
    if (Res != PCA9633_OK) error_handler();
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void    error_handler(void)
{
    LATAbits.LATA0 = 0;
    while(1){
        LATAbits.LATA0 = ~LATAbits.LATA0;
        __delay_ms(50);
    }
}
