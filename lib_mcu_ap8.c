/**
 * @file 
 * @author 	
 * @date	
 * @brief 	
 */

#include "lib_mcu_ap8.h"

/* Directives de compilation - Macros		*/

/* Déclarations des variables globales 	*/
const   pca9633_pwm_t tab_colors[5] =  {{0x00,0x00,0x00,0x00},   // BLACK (all leds OFF)
                                        {0x00,0x00,0xFF,0x00},   // RED
                                        {0x00,0xFF,0x00,0x00},   // GREEN
                                        {0xFF,0x00,0x00,0x00},   // BLUE
                                        {0xFF,0xFF,0xFF,0x00}};  // WHITE
                     
uint8_t ColorIndex = 0;
pca9633_err_t   Res;

uint8_t lastCN15value;
uint8_t ColorChangeRequest = 0;

uint16_t    Vpot;
uint8_t     AcqOK = 0;

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
            
    // CN15 (RD6)
    TRISDbits.TRISD6 = 1;   // RD6 as input
    lastCN15value = ReadCN15();
    
    CNEN1bits.CN15IE = 1;   // Enable CN for CN15
    IFS1bits.CNIF = 0;      // Security
    IEC1bits.CNIE = 1;      // Enable CN interrupt
    IPC4bits.CNIP = 2;      // Priority = 2 (why not ?))
        
    // Timer 1 for ISR each 100ms
    T1CON = 0x0010; // Prediv by 2
    PR1 = VAL_100MS;
    TMR1 = 0;       
    IFS0bits.T1IF = 0;  // Security
    IEC0bits.T1IE = 1;  // Enable ISR for Timer 1
    IPC0bits.T1IP = 4;  // Priority = 4
    
    // PCA9633
    
    pcaCfg.pi2c = &I2CModule;
    pcaCfg.initType = INIT_WITH_I2C1;
    pcaCfg.i2c_Address = PCA9633_DEFAULT_I2C_ADDR;
    
    Res = pca9633_init(&MyPca9633,&pcaCfg);
    if (Res != PCA9633_OK) error_handler();
    
    Res = pca9633_set_group_control_mode(&MyPca9633, PCA9633_DIMMING_GROUP_CONTROL);
    if (Res != PCA9633_OK) error_handler();
    
    // Config ADC (step by step)
    
    AD1CON2bits.VCFG = 0b000;     //  Vr+ = AVDD, Vr- = AVSS
    AD1CON3bits.ADRC = 1;       //  Tad is issued from internal dedicated RC oscillator (Tad = 250ns)
    AD1PCFGbits.PCFG5 = 0;      //  RB5/AN5 is an anlog input
    AD1CHSbits.CH0SA = 5;       //  MUX A input+ is AN5
    
    AD1CHSbits.CH0NA = 0;       //  MUX A input- is Vref-
    AD1CON2bits.ALTS = 0;       //  MUX A is the only used MUX
    AD1CON2bits.CSCNA = 0;      //  No scan
    AD1CON1bits.ASAM = 1;       //  Auto Sample (at the end of the previous conversion)
    AD1CON3bits.SAMC = 1;       //  Auto Sample time = 1 Tad            
    AD1CON1bits.SSRC = 0b000;   //  Clearing SAMP ends sampling & starts conversion
    AD1CON1bits.FORM = 0b00;    //  Data format = unsigned integer
    AD1CON2bits.SMPI = 0;       //  IRQ à chaque conversion       
    
    IFS0bits.AD1IF = 0; //  Clear IF - Security
    IEC0bits.AD1IE = 1; //  Enable Interrupt
    IPC3bits.AD1IP = 5; //  Interrupt priority = 5 
        
    AD1CON1bits.ADON = 1;   // Power on ADC
        
    T1CONbits.TON = 1;  // Start Timer 1
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//  Test : Color generation
//------------------------------------------------------------------------------
#if FINAL_EXE == TEST_SETCOLOR
void    mainTask(void){
    
    __delay_ms(500);

    if (ColorIndex < 4) ColorIndex++;
    else ColorIndex = 0;
    
    Res = pca9633_setPWM(&MyPca9633,&tab_colors[ColorIndex]);
    if (Res != PCA9633_OK) error_handler();
}
#endif      /*  #if FINAL_EXE == TEST_SETCOLOR  */
//------------------------------------------------------------------------------
        
//------------------------------------------------------------------------------
//  Test : CN ISR
//------------------------------------------------------------------------------
#if FINAL_EXE == TEST_CN_ISR
void    mainTask(void){
    if (ColorChangeRequest == 1){
        
        if (ColorIndex < 4) ColorIndex++;
        else ColorIndex = 0;
    
        Res = pca9633_setPWM(&MyPca9633,&tab_colors[ColorIndex]);
        if (Res != PCA9633_OK) error_handler();
        
        IEC1bits.CNIE = 1;
        ColorChangeRequest = 0;
    }
    
}
#endif  /*  #if     FINAL_EXE == TEST_CN_ISR    */
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  Test : ISR Timer - Needs a "local" ISR and TicTimer variable
//------------------------------------------------------------------------------
#if FINAL_EXE == TEST_TIMER_ISR
uint8_t TicTimer = 0;
void    mainTask(void){
    if (TicTimer == 1){
        LATA++;
        TicTimer = 0;
    }
}

void _ISR __attribute__((no_auto_psv)) _T1Interrupt (void)
{
    TicTimer = 1;
    IFS0bits.T1IF = 0;
}   //_T1Interrupt 

#endif  /*  if FINAL_EXE == TEST_TIMER_ISR  */

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//  Test : ADC + ADC ISR
#if FINAL_EXE == TEST_ADC_ISR
void    mainTask(void){
    if (AcqOK){
        LATA = Vpot >> 2;
        clearAcqOK();
    }
}

#endif  /*  #if FINAL_EXE == TEST_ADC_ISR   */
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#if FINAL_EXE ==  APPLICATION
void    mainTask(void){
    if (AcqOK){
        Res = pca9633_set_group_duty_cycle(&MyPca9633, (Vpot >> 2));
        if (Res != PCA9633_OK) error_handler();
        clearAcqOK();
    }
    
    if (ColorChangeRequest == 1){
        
        if (ColorIndex < 4) ColorIndex++;
        else ColorIndex = 0;
    
        Res = pca9633_setPWM(&MyPca9633,&tab_colors[ColorIndex]);
        if (Res != PCA9633_OK) error_handler();
        
        IEC1bits.CNIE = 1;
        ColorChangeRequest = 0;
    }
    
}

#endif  /*  #if FINAL_EXE ==  APPLICATION   */
//------------------------------------------------------------------------------
//  Common (real) functions & ISR (test independant)
//------------------------------------------------------------------------------
void _ISR __attribute__((no_auto_psv)) _CNInterrupt (void)
{
    uint8_t actualCN15value = ReadCN15();
    if ((lastCN15value == 1) && (actualCN15value == 0)){
        ColorChangeRequest = 1;
        IEC1bits.CNIE = 0;      // Disable CNIE (to avoid bouncing errors)
    }
    lastCN15value = actualCN15value;
    IFS1bits.CNIF = 0;
}   // _CNInterrupt
//------------------------------------------------------------------------------
#if FINAL_EXE != TEST_TIMER_ISR
void _ISR __attribute__((no_auto_psv)) _T1Interrupt (void)
{
    StartConversion();  // Obvious...
    IFS0bits.T1IF = 0;
}   //_T1Interrupt 
#endif
//------------------------------------------------------------------------------
void _ISR __attribute__((no_auto_psv)) _ADC1Interrupt (void){
    Vpot = (ADC1BUF0);          // get the result
    setAcqOK();                 // Set software Flag
    IFS0bits.AD1IF = 0;         //  Clear IF 
}   /* _ADC1Interrupt    */
//------------------------------------------------------------------------------
void    error_handler(void){
    LATAbits.LATA0 = 0;
    while(1){
        LATAbits.LATA0 = ~LATAbits.LATA0;
        __delay_ms(50);
    }
}
//------------------------------------------------------------------------------

