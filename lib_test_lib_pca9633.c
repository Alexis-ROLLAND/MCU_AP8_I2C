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
#if CURRENT_TEST == TEST_SET_GET_GROUP_CONTROL_MODE
void    mainTask(void){
    pca9633_err_t   Res;
    pca9633_group_control_mode_t mode;
    
    __delay_ms(1000);
    
    Res = pca9633_get_group_control_mode(&MyPca9633, &mode);
    if (Res != PCA9633_OK) error_handler();
    
    if (mode == PCA9633_DIMMING_GROUP_CONTROL){
        Res = pca9633_set_group_control_mode(&MyPca9633, PCA9633_BLINKING_GROUP_CONTROL);
        if (Res != PCA9633_OK) error_handler();
        LATAbits.LATA0 = 1;
    }
    else {
        Res = pca9633_set_group_control_mode(&MyPca9633, PCA9633_DIMMING_GROUP_CONTROL);
        if (Res != PCA9633_OK) error_handler();
        LATAbits.LATA0 = 0;
    }
    
}

#endif
//------------------------------------------------------------------------------
#if CURRENT_TEST == TEST_GET_SET_GRP_DUTY_CYCLE
const pca9633_pwm_t BlueValues={255,0,0,0};  /**< Global const variable to not overload the stack nor the RAM */
void    mainTask(void){
    pca9633_err_t   Res;
    static uint8_t  doOnce = 0;
    static uint8_t  alpha = 255;
    uint8_t ralpha;
    
    if (!doOnce){
        Res = pca9633_set_group_control_mode(&MyPca9633, PCA9633_DIMMING_GROUP_CONTROL);
        if (Res != PCA9633_OK) error_handler();
        
        Res = pca9633_setPWM(&MyPca9633,&BlueValues);
        if (Res != PCA9633_OK) error_handler();
        
        doOnce = 1;
    }
    
    __delay_ms(500);
    Res = pca9633_set_group_duty_cycle(&MyPca9633, alpha--);
    if (Res != PCA9633_OK) error_handler();
    
    Res = pca9633_get_group_duty_cycle(&MyPca9633, &ralpha);
    if (Res != PCA9633_OK) error_handler();
    
    LATA = ralpha;
    
}
#endif
//------------------------------------------------------------------------------
#if CURRENT_TEST == TEST_GET_SET_GRP_FREQ
const pca9633_pwm_t RedValues={0,0,255,0};  /**< Global const variable to not overload the stack nor the RAM */
void    mainTask(void){
    pca9633_err_t   Res;
    static uint8_t  doOnce = 0;
    static uint8_t  Period = 0;
    static uint8_t  Alpha = 128;
    uint8_t rperiod;
    
    if (!doOnce){
        Res = pca9633_set_group_control_mode(&MyPca9633, PCA9633_BLINKING_GROUP_CONTROL);
        if (Res != PCA9633_OK) error_handler();
        
        Res = pca9633_setPWM(&MyPca9633,&RedValues);
        if (Res != PCA9633_OK) error_handler();
        
        Res = pca9633_set_group_duty_cycle(&MyPca9633, Alpha);  // Blinking Duty Cycle (50%)
        if (Res != PCA9633_OK) error_handler();
        
        Res = pca9633_set_group_freq(&MyPca9633, Period);       // Blinking Period
        if (Res != PCA9633_OK) error_handler();
        
        doOnce = 1;
    }
    
    __delay_ms(5000);
    
    Res = pca9633_set_group_freq(&MyPca9633, Period++);       // Blinking Period
        if (Res != PCA9633_OK) error_handler();
    
    Res = pca9633_get_group_freq(&MyPca9633, &rperiod);
    if (Res != PCA9633_OK) error_handler();
    
    
    LATA = rperiod;
    
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
