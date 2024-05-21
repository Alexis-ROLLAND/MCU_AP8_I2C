/**
 * @file  
 * @author 	
 * @date	
 * @brief 	
 *  
 *
 */
#ifndef	__LIB_MACU_AP8_H__
#define	__LIB_MACU_AP8_H__

#ifndef FCY
#define FCY 4000000UL
#endif

#include <libpic30.h>
#include <xc.h>

#include "lib_pca9633.h"

#define I2C_MODULE  _I2C1

#define TEST_SETCOLOR   1
#define TEST_CN_ISR     2
#define TEST_TIMER_ISR  3

#define FINAL_EXE   TEST_TIMER_ISR   


#define ReadCN15()  PORTDbits.RD6
#define VAL_100MS   50000   

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
 
 


#endif  /*  __LIB_MACU_AP8_H__  */



