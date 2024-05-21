/**
 * @file    
 * @author 	
 * @date	
 * @brief 	
 */

#ifndef	__LIB_PCA9633_H__
#define	__LIB_PCA9633_H__
#include <xc.h>

#include "lib_i2c_pic24_ll.h"

// Default I2C address (for 8 pins devices)
#define PCA9633_DEFAULT_I2C_ADDR    0x62

// internal regs
#define PCA9633_REG_MODE1       0x00
#define PCA9633_REG_MODE2       0x01
#define PCA9633_REG_PWM0        0x02
#define PCA9633_REG_PWM1        0x03
#define PCA9633_REG_PWM2        0x04
#define PCA9633_REG_PWM3        0x05
#define PCA9633_REG_GRPPWM      0x06
#define PCA9633_REG_GRPFREQ     0x07
#define PCA9633_REG_LEDOUT      0x08

/*
 Masks and reg values
 */
#define MODE1_REG_INIT_VALUE    0x00    /**<  Normal mode, oscillator ON    */
#define MODE2_REG_INIT_VALUE    0x00    /**< Dimming Group Control, Output logic state not inverted, Output changes on STOP, Output driver as OD */
#define LEDOUT_REG_INIT_VALUE   0xFF    /**< LED driver x individual brightness can be controlled through its PWMx register and group dimming/blinking can be controlled through its PWMx register and the GRPPWM registers. */

#define NORMAL_MODE             0x00    /**< Mode control bit is bit4 (SLEEP) os MODE1 register */
#define SLEEP_MODE              0x10    /**< Mode control bit is bit4 (SLEEP) os MODE1 register */

#define DMBLNK_MASK             0x20    /**< Mask to access the DMBLNK bit (MODE2[5])   */

typedef enum    {   
    PCA9633_OK,     /**< Success Value          */
    PCA9633_ERROR,  /**< Non specific error     */
} pca9633_err_t;
                    
typedef enum    {   
    INIT_WITH_I2C1,     /**< Initialize the PCA9633 and I2C1 module    */
    INIT_WITH_I2C2,     /**< Initialize the PCA9633 and I2C2 module    */
    INIT_ALREADY_DONE   /**< Initialize the PCA9633 but don't touch I2C module config */
} pca9633_i2c_init_type_t;

#define BUF_SIZE    16  // Max I2C (Tx/Rx) buffer size                    

typedef struct {
    i2c_desc_t  *pi2c;                      /**< I2C descriptor initialized or not    */
    pca9633_i2c_init_type_t    initType;    /**< Initializing scheme (with I2C1, with I2C2 or init already done) */
    uint8_t i2c_Address;                    /**< I2C (7bits) Adrress of the PCA9633 device          */
} pca9633_config_t;                    

typedef struct  {
    i2c_desc_t  *pi2c;          /**< Fully intialized I2C descriptor    */
    uint8_t     i2c_Address;    /**< Real (7 bits) address of the used PCA9633  */
} pca9633_desc_t; 
    
typedef struct {
    uint8_t Pwm0;   /**< Value to set to PWM0 channel    */   
    uint8_t Pwm1;   /**< Value to set to PWM1 channel    */   
    uint8_t Pwm2;   /**< Value to set to PWM2 channel    */   
    uint8_t Pwm3;   /**< Value to set to PWM3 channel    */   
} pca9633_pwm_t;
 
typedef enum {PCA9633_OSC_IS_ON, PCA9633_OSC_IS_OFF} pca9633_osc_status_t;
typedef enum {PCA9633_DIMMING_GROUP_CONTROL, PCA9633_BLINKING_GROUP_CONTROL} pca9633_group_control_mode_t; 
   
/**
 * @brief       Initializes the PCA9633 device descriptor
 *              Init state is osc PCA9633 Powered on, no SUB or ALL calls, 
 *              Dimming group control, Output logic state not inverted, 
 *              output drivers are open drain.
 *              LED driver x individual brightness can be controlled through 
 *              its PWMx register.
 * 
 * @param[out]  pPca9633        Address of the PAC9633 device descriptor
 * @param[in]   pPca9633CFG     Address of the PCA9633 init structure	
 * 
 * @return      PCA9633_OK on success
 * @return      PCA9633_ERROR on error
 */
 pca9633_err_t pca9633_init (pca9633_desc_t *pPca9633, const pca9633_config_t *pPca9633CFG);
  
 /**
 * @brief       Sets ON or OFF the internal oscillator
 * 
 * @param[in]   pPca9633        Address of the PAC9633 device descriptor   	
 * 
 * @return      PCA9633_OK on success
 * @return      PCA9633_ERROR on error
 */
 pca9633_err_t pca9633_osc_on(const pca9633_desc_t *pPca9633);
 pca9633_err_t pca9633_osc_off(const pca9633_desc_t *pPca9633);

 /**
 * @brief       Gets the oscillator status of the PCA9633 
 * 
 * @param[in]   pPca9633        Address of the PAC9633 device descriptor 
 * @param[out]  pOscStatus      Address where to write the status
 * 
 * @return      PCA9633_OK on success
 * @return      PCA9633_ERROR on error
 */
 pca9633_err_t pca9633_get_osc_status(const pca9633_desc_t *pPca9633, pca9633_osc_status_t *pOscStatus);
 
 /**
 * @brief       Sets the 4 PWM          
 * 
 * @param[in]   pPca9633        Address of the PCA9633 device descriptor
 * @param[in]   pPwmValues      Address of a pca9633_pwm_t structure holding the 4 values
 * 
 * @return      PCA9633_OK on success
 * @return      PCA9633_ERROR on error
 */
 pca9633_err_t pca9633_setPWM(const pca9633_desc_t *pPca9633,const pca9633_pwm_t *pPwmValues);
 
 
 /**
 * @brief       Gets the group control mode (dimming or blinking)  
 * 
 * @param[in]   pPca9633        Address of the PCA9633 device descriptor
 * @param[in]   pmode           Address where to write the result (actual mode)
 *  
 * @return      PCA9633_OK on success
 * @return      PCA9633_ERROR on error   
 */
 pca9633_err_t pca9633_get_group_control_mode(const pca9633_desc_t *pPca9633, pca9633_group_control_mode_t *pmode);
 
 /**
 * @brief       Sets the group control mode (dimming or blinking)  
 * 
 * @param[in]   pPca9633        Address of the PCA9633 device descriptor
 * @param       mode            Mode to switch to
 *  
 * @return      PCA9633_OK on success
 * @return      PCA9633_ERROR on error   
 */
 pca9633_err_t pca9633_set_group_control_mode(const pca9633_desc_t *pPca9633, pca9633_group_control_mode_t mode);
 
 /**
 * @brief  
 * 
 * @param	
 * 
 * @return   
 *
 * @attention :     Dimming mode -> sets the PWM duty Cycle
 *                  Bliking mode -> sets the blinking duty cycle (Tblinking fixed by pca9633_set_group_freq) 
 */
 pca9633_err_t pca9633_set_group_duty_cycle(const pca9633_desc_t *pPca9633, uint8_t dutyCyle);
 
 /**
 * @brief  
 * 
 * @param	
 * 
 * @return   
 *
 */
 pca9633_err_t pca9633_get_group_duty_cycle(const pca9633_desc_t *pPca9633, uint8_t *pdutyCyle);
 
 /**
 * @brief  
 * 
 * @param	
 * 
 * @return   
 *
 * @attention : Actual blinking period is ((period + 1) / 24) in seconds
 */
 pca9633_err_t pca9633_set_group_freq(const pca9633_desc_t *pPca9633, uint8_t period);
 
 /**
 * @brief  
 * 
 * @param	
 * 
 * @return   
 *
 * @attention : Actual blinking period is ((period + 1) / 24) in seconds
 */
 pca9633_err_t pca9633_get_group_freq(const pca9633_desc_t *pPca9633, uint8_t *pperiod);
 
 
 //-----------------------------------------------------------------------------
 //-----------------------------------------------------------------------------
 
 /**
  * Low level functions - Not part of the API 
  */
  
/**
 * @brief       Writes a byte into a PCA9633 register   
 * 
 * @param[in]   pPca9633        Address of the PCA9633 device descriptor
 * @param       RegAddr         Address of the PCA9633 target register
 * @param       RegValue        Value of the byte to be written in the target register
 * 
 * @return      PCA9633_OK on success
 * @return      PCA9633_ERROR on error 
 * 
 * @Attention Low Level Function - Should not be used by app 
 *
 */
 pca9633_err_t pca9633_write_reg(const pca9633_desc_t *pPca9633,uint8_t RegAddr, uint8_t RegValue);
 
 /**
 * @brief       Reads a byte from a PCA9633 register   
 * 
 * @param[in]   pPca9633        Address of the PCA9633 device descriptor
 * @param       RegAddr         Address of the PCA9633 target register
 * @param[out]  pRegValue       Address of the byte receiving the register value
 * 
 * @return      PCA9633_OK on success
 * @return      PCA9633_ERROR on error 
 * 
 * @Attention Low Level Function - Should not be used by app 
 *
 */
 pca9633_err_t pca9633_read_reg(const pca9633_desc_t *pPca9633,uint8_t RegAddr, uint8_t *pRegValue);
 
 
         
 /**
 * @brief  
 * 
 * @param	
 * 
 * @return   
 *
 */
 
 

#endif


