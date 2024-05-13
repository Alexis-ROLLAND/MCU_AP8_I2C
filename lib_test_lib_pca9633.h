/**
 * @file  
 * @author 	
 * @date	
 * @brief 	
 *  
 *
 */
#ifndef	__LIB_APP_TEST_PCA9633_H__
#define	__LIB_APP_TEST_PCA9633_H__

#ifndef FCY
#define FCY 4000000UL
#endif

#include <libpic30.h>
#include <xc.h>

#include "lib_pca9633.h"

#define I2C_MODULE  _I2C1

//------------------------------------------------------------------------------
#define     TEST_SET_GET_OSC_STATUS     1
#define     TEST_SET_PWM                2


#define     CURRENT_TEST        TEST_SET_GET_OSC_STATUS
//------------------------------------------------------------------------------


/**
 * @brief Global init function/task 
 * 
 * @param	None
 * 
 * @return  Nothing 
 *
 */
void Initialiser(void);

/**
 * @brief  
 * 
 * @param	
 * 
 * @return   
 *
 */
void    mainTask(void);


 /**
 * @brief  
 * 
 * @param	
 * 
 * @return   
 *
 */
 void    error_handler(void);
 
 
 /**
 * @brief  
 * 
 * @param	
 * 
 * @return   
 *
 */
 
 


#endif



