/**
 * @file 
 * @author 	
 * @date	
 * @brief 	
 */
#include "lib_pca9633.h"

/* Directives de compilation - Macros		*/


/* Déclarations des variables globales 	*/
uint8_t     TxBuffer[BUF_SIZE];
uint8_t     RxBuffer[BUF_SIZE];


/*	Implémentation du code */
//------------------------------------------------------------------------------
pca9633_err_t pca9633_init (pca9633_desc_t *pPca9633, const pca9633_config_t *pPca9633CFG){
    i2c_config_t i2cCfg;
    i2c_err_t   Res;
    pca9633_pwm_t   PwmValues={0,0,0,0};
    if (pPca9633CFG->initType != INIT_ALREADY_DONE){
        i2cCfg.i2cBRG_value = KHZ100_MHZ4;
        
        switch(pPca9633CFG->initType){
            case INIT_WITH_I2C1:
                i2c_init(_I2C1, &i2cCfg, pPca9633CFG->pi2c);
                break;
            case INIT_WITH_I2C2:
                i2c_init(_I2C2, &i2cCfg, pPca9633CFG->pi2c);
                break;
            default: break;
            }
    }
    pPca9633->pi2c = pPca9633CFG->pi2c;
    pPca9633->i2c_Address = pPca9633CFG->i2c_Address;
    
    Res = pca9633_write_reg(pPca9633,PCA9633_REG_MODE1, MODE1_REG_INIT_VALUE);  /**< Oscillator ON (normal mode), no subcalls */
    if (Res != I2C_OK) return PCA9633_ERROR;
    
    Res = pca9633_write_reg(pPca9633,PCA9633_REG_MODE2, MODE2_REG_INIT_VALUE);  /**< Dimming Group Control, Output logic state not inverted, Output changes on STOP, Output driver as OD */
    if (Res != I2C_OK) return PCA9633_ERROR;
    
    Res = pca9633_write_reg(pPca9633,PCA9633_REG_LEDOUT, LEDOUT_REG_INIT_VALUE); /**< LED driver x individual brightness can be controlled through its PWMx register. */
    if (Res != I2C_OK) return PCA9633_ERROR;
    
    Res = pca9633_setPWM(pPca9633,&PwmValues);  /**< Set all the four PWM at 0%  */
    if (Res != I2C_OK) return PCA9633_ERROR;
    
    return PCA9633_OK;
}
//------------------------------------------------------------------------------
pca9633_err_t pca9633_setPWM(const pca9633_desc_t *pPca9633, const pca9633_pwm_t *pPwmValues){
   i2c_err_t   Res;
   
   TxBuffer[0] = 0xA2;      /**< Auto-Increment for individual brightness registers, starting at PWM0 Address */
   TxBuffer[1] = pPwmValues->Pwm0;
   TxBuffer[2] = pPwmValues->Pwm1;
   TxBuffer[3] = pPwmValues->Pwm2;
   TxBuffer[4] = pPwmValues->Pwm3;
   
   Res = i2c_write(pPca9633->pi2c, pPca9633->i2c_Address,TxBuffer,5);
   if (Res != I2C_OK) return PCA9633_ERROR;
   
   return PCA9633_OK; 
}
//------------------------------------------------------------------------------
pca9633_err_t pca9633_osc_on(const pca9633_desc_t *pPca9633){
    pca9633_err_t   Res;
    Res = pca9633_write_reg(pPca9633,PCA9633_REG_MODE1, NORMAL_MODE);
    if (Res != PCA9633_OK) return Res;
    return PCA9633_OK;
}
//------------------------------------------------------------------------------
 pca9633_err_t pca9633_osc_off(const pca9633_desc_t *pPca9633){
    pca9633_err_t   Res;
    Res = pca9633_write_reg(pPca9633,PCA9633_REG_MODE1, SLEEP_MODE);
    if (Res != PCA9633_OK) return Res;
    return PCA9633_OK;
 }
//------------------------------------------------------------------------------
 pca9633_err_t pca9633_get_osc_status(const pca9633_desc_t *pPca9633, pca9633_osc_status_t *pOscStatus){
    pca9633_err_t   Res;
    Res =  pca9633_read_reg(pPca9633,PCA9633_REG_MODE1, &RxBuffer[0]);
    if (Res != PCA9633_OK) return Res;
    
    if ( (RxBuffer[0] & SLEEP_MODE) == SLEEP_MODE) *pOscStatus = PCA9633_OSC_IS_OFF;
    else *pOscStatus = PCA9633_OSC_IS_ON;
    
    return PCA9633_OK;
 }
 //------------------------------------------------------------------------------
 
 //------------------------------------------------------------------------------
pca9633_err_t pca9633_write_reg(const pca9633_desc_t *pPca9633,uint8_t RegAddr, uint8_t RegValue){
    i2c_err_t   Res;
    TxBuffer[0] = RegAddr & 0x0F;   // Clear all auto-increment features
    TxBuffer[1] = RegValue;
     
    Res = i2c_write(pPca9633->pi2c, pPca9633->i2c_Address,TxBuffer,2);
    if (Res != I2C_OK) return PCA9633_ERROR;
    
    return PCA9633_OK;
}
//------------------------------------------------------------------------------        
pca9633_err_t pca9633_read_reg(const pca9633_desc_t *pPca9633,uint8_t RegAddr, uint8_t *pRegValue){
    i2c_err_t   Res;
    TxBuffer[0] = RegAddr & 0x0F;   // Clear all auto-increment features
    
    Res = i2c_write_then_read(pPca9633->pi2c, pPca9633->i2c_Address,TxBuffer,1,pRegValue,1);
    if (Res != I2C_OK) return PCA9633_ERROR;
        
    return PCA9633_OK;
}
//------------------------------------------------------------------------------
