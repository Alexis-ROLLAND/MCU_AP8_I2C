/**
 * @file 
 * @author 	
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
void Initialiser(void){
    pca9633_err_t   Res;
    pca9633_config_t    pcaCfg;
    
    // Leds (Explorer 16/32 onboard leds)
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
#if CURRENT_TEST == TEST_SET_GET_OSC_STATUS
void    mainTask(void){
    pca9633_err_t   Res;
    static  pca9633_osc_status_t OscStatus;
    
    __delay_ms(1000);
    
    Res = pca9633_get_osc_status(&MyPca9633, &OscStatus);   /**< Get Oscillator Status  */
    if (Res != PCA9633_OK) error_handler();
    
    if (OscStatus == PCA9633_OSC_IS_ON){
        Res = pca9633_osc_off(&MyPca9633);
        if (Res != PCA9633_OK) error_handler();
        LATAbits.LATA0 = 0;
    }
    else{
        Res = pca9633_osc_on(&MyPca9633);
        if (Res != PCA9633_OK) error_handler();
        LATAbits.LATA0 = 1;
    }
    
}
#endif

#if CURRENT_TEST == TEST_SET_PWM
pca9633_pwm_t PwmValues={0,0,0,0};  /**< Global variable to not overload the stack  */
void    mainTask(void){
    pca9633_err_t   Res;
        
    __delay_ms(1000);
    PwmValues.Pwm0 = 0x00;      // Blue
    PwmValues.Pwm1 = 0x00;      // Green
    PwmValues.Pwm2 = 0xFF;      // Red
    
    Res = pca9633_setPWM(&MyPca9633,&PwmValues);
    if (Res != PCA9633_OK) error_handler();
    
    LATA = 0x01;    /**< LED3 must be on when  LCD led is Red   */ 
    
    __delay_ms(1000);
    PwmValues.Pwm0 = 0x00;      // Blue
    PwmValues.Pwm1 = 0xFF;      // Green
    PwmValues.Pwm2 = 0x00;      // Red
    
    Res = pca9633_setPWM(&MyPca9633,&PwmValues);
    if (Res != PCA9633_OK) error_handler();
    
    LATA = 0x02;    /**< LED4 must be on when  LCD led is green   */
    
    __delay_ms(1000);
    PwmValues.Pwm0 = 0xFF;      // Blue
    PwmValues.Pwm1 = 0x00;      // Green
    PwmValues.Pwm2 = 0x00;      // Red
    
    Res = pca9633_setPWM(&MyPca9633,&PwmValues);
    if (Res != PCA9633_OK) error_handler();

    LATA = 0x04;    /**< LED5 must be on when  LCD led is blue   */
}
#endif
//------------------------------------------------------------------------------
void    error_handler(void){
    LATAbits.LATA0 = 0;
    while(1){
        LATAbits.LATA0 = ~LATAbits.LATA0;
        __delay_ms(50);
    }
}
