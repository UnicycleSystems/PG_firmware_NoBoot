/**
  Generated main.c file from MPLAB Code Configurator

  @Company
    Microchip Technology Inc.

  @File Name
    main.c

  @Summary
    This is the generated main.c using PIC24 / dsPIC33 / PIC32MM MCUs.

  @Description
    This source file provides main entry point for system initialization and application code development.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.171.5
        Device            :  PIC24FJ64GA004
    The generated drivers are tested against the following:
        Compiler          :  XC16 v2.10
        MPLAB 	          :  MPLAB X v6.05
*/

/*
    (c) 2020 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

/**
  Section: Included Files
*/
//#pragma config FWDTEN = OFF      // Watchdog Timer Enable (WDT disabled in hardware)
//#pragma config WINDIS = ON       // Windowed WDT disabled (optional)

//#include "mcc_generated_files/system.h"
//#include "mcc_generated_files/mcc.h"
#include "Events.h"
#include "pin_CustomISR.h"
//#include "MemoryMap.h"
#include "RGS_MCC_Copies/FrontAlignLaser.h"
#include "RGS_MCC_Copies/system.h"
#include "RGS_MCC_Copies/clock.h"
#include "RGS_MCC_Copies/interrupt_manager.h"
#include "RGS_MCC_Copies/i2c2.h"
#include "RGS_MCC_Copies/i2c1.h"
#include "RGS_MCC_Copies/tmr4.h"
#include "RGS_MCC_Copies/tmr2.h"
#include "RGS_MCC_Copies/pin_manager.h"
//#include "lis2dw12.h"

#include <stdbool.h>
#include <stdint.h>


#include <xc.h>


#define FCY 16000000UL  // or whatever your instruction clock is
#include <libpic30.h>


//accelerometer specific stuff....
// to be removed if we ever implement a proper .c/.h set up

//commented out here, copied to top, but preserved for moving to header file
/* ---- LIS2DW12 / LIS2DW1TR basics ---- */
#define LIS_ADDR_0          0x18
#define LIS_ADDR_1          0x19
#define REG_WHO_AM_I        0x0F    /* expect 0x44 */
#define REG_CTRL1           0x20
#define REG_CTRL2           0x21
#define REG_OUT_X_L         0x28    /* then X_H, Y_L, Y_H, Z_L, Z_H */

#define CTRL2_BDU           (1u << 3)
#define CTRL2_IF_ADD_INC    (1u << 2)

#define LIS_ADDR_0 0x18
#define LIS_ADDR_1 0x19


#define FrontLaserOn FRONT_LASER_PWM_SetLow()
#define FrontLaserOff FRONT_LASER_PWM_SetHigh()

#define RearLaserOn REAR_LASER_PWM_SetLow()
#define RearLaserOff REAR_LASER_PWM_SetHigh()

#define BeamOn BEAM_SetLow()
#define BeamOff BEAM_SetHigh()

#define ButtonDelay 5    // The number of cycles of the button hold to return 'true)
                         // NOT debounce, this is the long (ish)user press and hold

#define On 1
#define Off 0




///Test functions switches




// round robin tasks
#define NUM_Tasks 2
// functions as needed, increment NUM_Tasks 
void GetBattVolts(void);
void GetAccel(void);

void (*Task[NUM_Tasks])(void)={GetBattVolts,GetAccel};




uint8_t TaskIndex;



static uint8_t s_addr = LIS_ADDR_0;
void LIS2DW12_SetAddress_I2C2(uint8_t addr) { s_addr = addr; }
static bool i2c2_wait_done(volatile I2C2_MESSAGE_STATUS *st, uint16_t timeout_ms);
static bool i2c2_write_u8(uint8_t dev7, uint8_t reg, uint8_t val);
static bool i2c2_read_regs(uint8_t dev7, uint8_t start_reg, uint8_t *dst, uint8_t n);
//end accelerometer specific defines

// accelerometer prototypes
//void LIS2DW12_SetAddress_I2C2(uint8_t addr);
void QuickAcellerometerGrabber(void);
bool LIS2DW12_Init_I2C2(void);                 // returns true on success
bool LIS2DW12_ReadXYZ_I2C2(int16_t *x, int16_t *y, int16_t *z);
static bool lis_probe_addr(uint8_t addr);

//end accelerometer prototypes

void CallJetsonBall(void);
void CallJetsonJob(void);
void ChangeLighting(void);
void ReadOneByteExample(void);
bool lis2dw12_read_register(uint8_t reg, uint8_t *value);
//bool LIS2DW12_Configure(void);
void ShutdownProcessTemp(void);
void PowerDown(void);

void POST_Routine(void);
void LaunchTest(void);
uint8_t PowerButton (bool OnOff);

 int16_t x;
 int16_t y;
 int16_t z;
 int16_t a;
 int16_t xx;
 int16_t yy;
 int16_t zz;
 
 uint8_t testval;
 
// power management
uint32_t SwitchState;
uint32_t ButtonCount;
bool PowerOff;


int main(void)
{
    LedOn=0;
    FrontSense=0;
    RearSense=0;
    TransitTime=0;
    GateTimeout=0;
    SwitchState=0; 
    ButtonCount=0;
    PowerOff=false;
    TaskIndex=0;
     
    //This is a 
    EMULATE_EEPROM_Memory[128] = 0;  //This should cause both lasers off
    
    
    
    SYSTEM_Initialize();
    
    
    // Explicit changes for new versions
    POWER_BUTTON_SetDigitalInput();
    
    //
    HOLD_PWR_SetDigitalOutput();
    HOLD_PWR_SetLow();
    //ODCCbits.ODCC1 = 1;
    JETSON_5V_ON_SetDigitalOutput();
    JETSON_5V_ON_SetLow();
    
    
    // create a branch for VBUS power on state
   // while VBUS on........ 
  //button test
    
    bool charged;
    charged =0;
    //put on red front led
    // this is the charger loop, will later contain a 'sleep' with wdg wake up
    while(POWER_BUTTON_GetValue())

    {
        ClrWdt();
        if(!charged)
        {    
         RED_LED_ON_SetHigh();
        __delay_ms(5);
        RED_LED_ON_SetLow();
        __delay_ms(1000);
        }
        
        if(VER_0_GetValue())
        {
            charged=1;
            PWM_BLUE_SetHigh();//Turn on Green LED
            PWM_GREEN_SetLow();//Turn off Red LED 
        }
        else
        {
            charged=0;
           PWM_GREEN_SetHigh();//Turn off Red LED 
            PWM_BLUE_SetLow();//Turn on Green LED
        }
        // wait for some other eventn
        //or sleep and try again on wake..... 
    }
    
    RED_LED_ON_SetLow();
 // 
     while(!PowerButton(On))//wait for a 'power on  press and hold to complete
         ClrWdt();
     PWM_BLUE_SetHigh();//Turn on Green LED
     PWM_GREEN_SetLow();//Turn off Red LED

  // #define RunPOST
#ifdef RunPOST
     POST_Routine();
#endif
   
    
    FrontSensorOff;
    RearSensorOff;
    IFS1bits.T5IF = false;
    IFS1bits.T5IF = false;
    IEC1bits.T5IE = false;
    IFS1bits.CNIF = 0;

    
    ClrWdt();

    IFS1bits.T5IF = false;
    IFS1bits.T5IF = false;
    IEC1bits.T5IE = false;
    IFS1bits.CNIF = 0;
    INTERRUPT_GlobalEnable();   // this is an inline, in the interrupt manager header, so not a function call as such!
 
    ChangeLighting();
    LightingUpdate=0;
    uint8_t bugout;
  
    char LastOnOff;
    LedOn=0;
    LastOnOff=0;
    uint32_t DebugTime;
  
  
    EMULATE_EEPROM_Memory[4] = (uint8_t)(0x00);  // Most significant byte
    EMULATE_EEPROM_Memory[5] = (uint8_t)(0xF4);
    EMULATE_EEPROM_Memory[6] = (uint8_t)(0x24);
    EMULATE_EEPROM_Memory[7] = (uint8_t)(0x00);
 
   // INTERRUPT_TO_JETSON_SetLow();
    RestoreDetect();
    //LOCAL_STATUS_LED_SetHigh();
     //TODO : A loop waiting for teh jetson to handshake
     // this will flash a red light or something
     // 
 
   
  //  I2C2_Initialize();
    /* Probe WHO_AM_I on a given 7-bit addr; record findings at EEPROM offsets */

  //  LIS2DW12_Init_I2C2();    // TODO: should ensure it inits, or returns an error
  //  uint8_t id = 0x00;

  //PWM_GREEN_SetHigh();PWM_GREEN is RED!!!!!
  PWM_BLUE_SetHigh(); //PWM_BLUE is green!!!!
   for(bugout=0;bugout<10;bugout++)
            {
                //changes this to flashing red light
                PWM_BLUE_Toggle();  
                PWM_GREEN_Toggle();
                __delay_ms(50); 
                PWM_BLUE_Toggle();  
                PWM_GREEN_Toggle();
                __delay_ms(150); 
                ClrWdt();
                // restore green light
                PWM_BLUE_SetHigh();//Turn on Green LED
                PWM_GREEN_SetLow();//Turn off Red LED
            }
  
//#define PWR_BUTT_LAUNCH_TEST
    while(1)
    {    
       if(!POWER_BUTTON_GetValue())
#ifdef  PWR_BUTT_LAUNCH_TEST
            LaunchTest();
#else
         
             PowerDown();
#endif
          if(DoTask)
             {   
                Task[TaskIndex](); 
                TaskIndex++;
                if(TaskIndex>=NUM_Tasks)
                    TaskIndex=0;
                 DoTask=0;
             }
         if(LightingUpdate)
         {
             ChangeLighting();
             LightingUpdate=0;
         }
         
         //add power bit...
        if (LedOn)
        {
            if(FrontSense)
            { 
            
            
             PWM_GREEN_SetHigh();   //Red LED on        
           //  LOCAL_STATUS_LED_SetHigh();
             PWM_BLUE_SetLow();//green LED off
            //  CallJetsonJob(); //just a little thing to knock on the jetson's door
            //Switch on RED front light, off green light
            FrontSense=0;
            }
            
            if(RearSense)
            {
                
               // LOCAL_STATUS_LED_SetLow();
                EMULATE_EEPROM_Memory[0] = (uint8_t)(TransitTime >> 24);  // Most significant byte
                EMULATE_EEPROM_Memory[1] = (uint8_t)(TransitTime >> 16);
                EMULATE_EEPROM_Memory[2] = (uint8_t)(TransitTime>> 8);
                EMULATE_EEPROM_Memory[3] = (uint8_t)(TransitTime);  
                RearSense=0;
                CallJetsonBall();
                // wait a bit....
                // turn off red light, turn on green light
                RestoreDetect();
                TMR2_Start();
                PWM_GREEN_SetLow();   //Red LED off       
           //  LOCAL_STATUS_LED_SetHigh();
             PWM_BLUE_SetHigh();//green LED on    
            }
            LedOn=0;
        }
        
        if(GateTimeout)
        {   
            PWM_GREEN_SetLow(); 
            PWM_BLUE_SetHigh();//green LED off      

            DebugTime=TransitTime;
            EMULATE_EEPROM_Memory[0] = 255;  // Most significant byte
            EMULATE_EEPROM_Memory[1] = 254;
            EMULATE_EEPROM_Memory[2] = 253;
            EMULATE_EEPROM_Memory[3] = 252; 
            CallJetsonBall();
            GateTimeout=0;
            //Just flashes on board LED, make this external indicator
            for(bugout=0;bugout<10;bugout++)
            {
                //changes this to flashing red light
                PWM_BLUE_Toggle();  
                PWM_GREEN_Toggle();
                __delay_ms(50); 
                PWM_BLUE_Toggle();  
                PWM_GREEN_Toggle();
                __delay_ms(150); 
                ClrWdt();
                // restore green light
                PWM_BLUE_SetHigh();//Turn on Green LED
                PWM_GREEN_SetLow();//Turn off Red LED
            }
            
            GateTimeout=0;
            FrontSense=0;
            RearSense=0;
            TMR4_Initialize ();
            RestoreDetect();
            TMR2_Start();
                      
        }
 
    }
} 


///End of int main(void)    


///functions, TBD other headers etc

void CallJetsonBall(void)
{
    
     BALL_DETECT_INT_SetHigh();
    __delay_ms(10);
    BALL_DETECT_INT_SetLow();
  //  __delay_ms(10);
   // BALL_DETECT_INT_SetHigh();
  //  __delay_ms(10);
 //   BALL_DETECT_INT_SetLow();
  //  __delay_ms(10);  
}
#define powerdowntest
#ifdef powerdowntest
void CallJetsonJob(void)
{
    FrontSensorOff;
    RearSensorOff;
    IFS1bits.T5IF = false;
    IFS1bits.T5IF = false;
    IEC1bits.T5IE = false;
    IFS1bits.CNIF = 0;

    LOCAL_STATUS_LED_SetLow(); //turn the LED off 
    INTERRUPT_TO_JETSON_SetHigh();
    __delay_ms(10);
    INTERRUPT_TO_JETSON_SetLow();
    //ensure Jetson heartbeat is high
    while(!JETSON_HEARTBEAT_GetValue())
    {
        __delay_ms(10);
        ClrWdt();
    }
    LOCAL_STATUS_LED_SetHigh();  //high again when heartbeat seen
    while(JETSON_HEARTBEAT_GetValue())
    {
        __delay_ms(100);
        ClrWdt();
        PWM_BLUE_Toggle();
    };
    PWM_BLUE_SetLow();
    //we should have had the jetson acknowledge by now, so turn off
  //  JETSON_5V_ON_SetLow();
   
    while(!JETSON_HEARTBEAT_GetValue())
    {
        __delay_ms(100);
        ClrWdt();
        PWM_BLUE_Toggle();
    };
       
       RestoreDetect();
}
#endif


void ChangeLighting(void)
{
  uint8_t LightingStateNew;
  LightingStateNew=EMULATE_EEPROM_Memory[128];
  if(LightingStateNew==4)
  {
      ShutdownProcessTemp();
      return;
  }
  if (LightingStateNew&0x01)
      FRONT_LASER_PWM_SetLow();
  else
      FRONT_LASER_PWM_SetHigh();
  
  if (LightingStateNew&0x02)
      REAR_LASER_PWM_SetLow();
  else
      REAR_LASER_PWM_SetHigh();
    
}

void ShutdownProcessTemp()

{
    uint8_t delay;
     PWM_GREEN_SetLow();   
    for(delay=0;delay<200;delay++)
    {
         ClrWdt();
        __delay_ms(50);
        RED_LED_ON_Toggle();  
    }
        RED_LED_ON_SetLow();         

   JETSON_5V_ON_SetLow();
        
    ButtonCount=0;
    while(ButtonCount<10)
    {
       
        if(!POWER_BUTTON_GetValue())
            ButtonCount++;
        else
            ButtonCount=0;
        __delay_ms(50);
        ClrWdt();
       
    }
    while(!POWER_BUTTON_GetValue())
    {
        ClrWdt();
        __delay_ms(50);
         PWM_BLUE_Toggle();   
    }
       PWM_BLUE_SetLow();       
   
     JETSON_5V_ON_SetHigh();
    
}





/*snippets and reminders/scraps .... 
 //Power button and led test
     while(1)
     {
         ClrWdt();
      // temppower= (POWER_BUTTON_GetValue());
       
    // 
       if(!POWER_BUTTON_GetValue())
           PWM_BLUE_SetHigh();
       else
           PWM_BLUE_SetLow();
         
           
     }
 * 
 * 
 *  // if(!POWER_BUTTON_GetValue())
             ButtonCount++;
         else 
             ButtonCount=0;
         
        if(ButtonCount>1000)
           PWM_BLUE_SetHigh(); 
         if(ButtonCount>1500)
         {
           PWM_BLUE_SetLow();
           SwitchState=1;
         }
           
         if(SwitchState)
             CallJetsonJob();
         
           //PWM_BLUE_SetHigh();
  
 * 
 * 
 * 
 * 
 *  
#ifdef nothello
    // initialize the device
    //set a dummy pin change ISR 
    CN_SetInterruptHandler(ISR_CN_Dummy);
    
    SYSTEM_Initialize();
    //pwr_on_self_test();
    //standBy(); 
    //PowerUp();

    while (1)
    {
    //PollPowerButton-> go to blocking code if pressed
        if (POWER_BUTTON_GetValue())
            pwrButt();
        //Poll Accelerometer
        if(ACCEL_INT_GetValue()) 
         EventFlags.AccelEvent=1;
        
        if (EventFlags)
            SetEventActions();
        
            
            
              //-> Read and react, alert jetson if needs be
        //Poll HeartBeat 
            // if time since last heartbeat exceeds 'x' - go to alert
            // and reset jetson (managed power down and power up )
        // Add your application code
        
        
       //Attempt stand by..... 
        
    }
#endif 
    

    return 1;
}
     

   
        //Handle Power control
         if(!POWER_BUTTON_GetValue())
         {
             if(ButtonCount=0)
             {
                //switch on button LED 
                LOCAL_STATUS_LED_SetHigh(); 
                SwitchState=1;
             }
                ButtonCount++;
                 
             
             if(ButtonCount>10000)
             {
                 //switch off button LED
                 LOCAL_STATUS_LED_SetLow();
                 PowerOff=true; 
                 ButtonCount=0;
                 SwitchState=0;
                 
             }
         }
         else
         { 
             ButtonCount=0;
             SwitchState=0;
             LOCAL_STATUS_LED_SetLow();
         }
     
     ClrWdt();
 
 
  uint8_t config[] = {0x20, 0x00};  // Write to CTRL_REG6
I2C2_MESSAGE_STATUS status;

I2C2_MasterWrite(config, 2, 0x18, &status);  // Try 0x19 if this fails
while (status == I2C2_MESSAGE_PENDING)
    ClrWdt();

if (status == I2C2_MESSAGE_COMPLETE)
    accelinit=1; // Success
else
    accelinit=0; // Failed
 
 */


// Accelerometer code move to seperate .h and .c when good

//#include <xc.h>
//#include <stdint.h>
//#include <stdbool.h>
//#include "mcc_generated_files/i2c2.h"
//#include "lis2dw12_i2c2.h"


//commented out here, copied to top, but preserved for moving to header file
// ---- LIS2DW12 / LIS2DW1TR basics ---- 
//#define LIS_ADDR_0          0x18
//#define LIS_ADDR_1          0x19
//#define REG_WHO_AM_I        0x0F    /* expect 0x44 */
//#define REG_CTRL1           0x20
//#define REG_CTRL2           0x21
//#define REG_OUT_X_L         0x28    /* then X_H, Y_L, Y_H, Z_L, Z_H */

//#define CTRL2_BDU           (1u << 3)
//#define CTRL2_IF_ADD_INC    (1u << 2)

/* ---- Config: address and small delay helper ---- */
//static uint8_t s_addr = LIS_ADDR_0;   //  default SA0=0

//void LIS2DW12_SetAddress_I2C2(uint8_t addr); 

/* Provide a tiny delay; replace with your system delay if you have one */
static void small_delay(void)
{
    /* ~1?2 ms software delay; adjust to taste or replace with __delay_ms(1) */
    //for (volatile uint32_t i = 0; i < 8000UL; i++) { __asm__ volatile ("nop"); }
    __delay_ms(1);
}
//void LIS2DW12_SetAddress_I2C2(uint8_t addr)
//{
    
//}

/* ---- Low-level helpers using ONLY your TRB API ---- */

static bool i2c2_wait_done(volatile I2C2_MESSAGE_STATUS *st, uint16_t timeout_ms)
{
    while (*st == I2C2_MESSAGE_PENDING) {
        __delay_ms(1);
        ClrWdt();
        if (timeout_ms-- == 0) return false;   // timeout
    }
    return true;
}


static bool i2c2_write_u8(uint8_t dev7, uint8_t reg, uint8_t val)
{
    volatile I2C2_MESSAGE_STATUS st = I2C2_MESSAGE_PENDING;
    uint8_t w[2] = { reg, val };
    I2C2_MasterWrite(w, 2, dev7, (I2C2_MESSAGE_STATUS*)&st);
    return i2c2_wait_done(&st, 50) && (st == I2C2_MESSAGE_COMPLETE);
}

/* Proper repeated-start via TRB pair: write(reg) then read(n) */
/* Preferred: repeated-start via TRB pair. Falls back to STOP (write then read). */
static bool i2c2_read_regs(uint8_t dev7, uint8_t start_reg, uint8_t *dst, uint8_t n)
{
    volatile I2C2_MESSAGE_STATUS st = I2C2_MESSAGE_PENDING;
    I2C2_TRANSACTION_REQUEST_BLOCK trb[2];

    I2C2_MasterWriteTRBBuild(&trb[0], &start_reg, 1, dev7);
    I2C2_MasterReadTRBBuild (&trb[1], dst,        n, dev7);
    I2C2_MasterTRBInsert(2, trb, (I2C2_MESSAGE_STATUS*)&st);

    if (i2c2_wait_done(&st, 50) && st == I2C2_MESSAGE_COMPLETE)
        return true;

    // Fallback: STOP between write(reg) and read(n)
    st = I2C2_MESSAGE_PENDING;
    I2C2_MasterWrite(&start_reg, 1, dev7, (I2C2_MESSAGE_STATUS*)&st);
    if (!i2c2_wait_done(&st, 50) || st != I2C2_MESSAGE_COMPLETE) return false;

    st = I2C2_MESSAGE_PENDING;
    I2C2_MasterRead(dst, n, dev7, (I2C2_MESSAGE_STATUS*)&st);
    return i2c2_wait_done(&st, 50) && (st == I2C2_MESSAGE_COMPLETE);
}

static bool i2c2_read_u8(uint8_t dev7, uint8_t reg, uint8_t *val)
{
    return i2c2_read_regs(dev7, reg, val, 1);
}

/* ---- High-level sensor ops ---- */

static bool lis_probe(void)
{
    uint8_t id = 0;
    if (!i2c2_read_u8(s_addr, REG_WHO_AM_I, &id)) return false;
    return (id == 0x44);
}

bool LIS2DW12_Init_I2C2(void)
{
    EMULATE_EEPROM_Memory[30] = 0xA1;     // entered init

    if (lis_probe_addr(0x18)) { s_addr = 0x18; }
    else if (lis_probe_addr(0x19)) { s_addr = 0x19; }
    else { EMULATE_EEPROM_Memory[31] = 0xEE; return false; } // probe failed

    EMULATE_EEPROM_Memory[31] = s_addr;

    if (!i2c2_write_u8(s_addr, REG_CTRL2, (uint8_t)(CTRL2_BDU | CTRL2_IF_ADD_INC)))
        { EMULATE_EEPROM_Memory[32] = 0xC2; return false; }

    if (!i2c2_write_u8(s_addr, REG_CTRL1, 0x50))   // ~100 Hz, FS �2g
        { EMULATE_EEPROM_Memory[33] = 0xC1; return false; }

    EMULATE_EEPROM_Memory[34] = 0x00;     // success
    return true;
}

bool LIS2DW12_ReadXYZ_I2C2(int16_t *x, int16_t *y, int16_t *z)
{
    uint8_t raw[6];
    if (!i2c2_read_regs(s_addr, REG_OUT_X_L, raw, 6)) return false;

    *x = (int16_t)((uint16_t)raw[1] << 8 | raw[0]);
    *y = (int16_t)((uint16_t)raw[3] << 8 | raw[2]);
    *z = (int16_t)((uint16_t)raw[5] << 8 | raw[4]);
    return true;
}

static bool lis_probe_addr(uint8_t addr)
{
    uint8_t id = 0;
    return i2c2_read_regs(addr, REG_WHO_AM_I, &id, 1) && (id == 0x44);
}

void QuickAcellerometerGrabber(void)
{

  
        ClrWdt();
        int16_t x, y, z;
        if (LIS2DW12_ReadXYZ_I2C2(&x, &y, &z)) 
        {
        uint16_t ux = (uint16_t)x;
        uint16_t uy = (uint16_t)y;
        uint16_t uz = (uint16_t)z;

        // Pack as [X_H, X_L, Y_H, Y_L, Z_H, Z_L]
        EMULATE_EEPROM_Memory[8]  = (uint8_t)(ux >> 8);
         EMULATE_EEPROM_Memory[9] = (uint8_t)(ux);
         EMULATE_EEPROM_Memory[10] = (uint8_t)(uy >> 8);
         EMULATE_EEPROM_Memory[11] = (uint8_t)(uy);
         EMULATE_EEPROM_Memory[12] = (uint8_t)(uz >> 8);
         EMULATE_EEPROM_Memory[13] = (uint8_t)(uz);
            /* use raw counts; scale later if needed */
        }
        else // if it fubars, then just fill with 0xFF;
        {
         EMULATE_EEPROM_Memory[8]  = 0xFF;
         EMULATE_EEPROM_Memory[9] = 0xFF;
         EMULATE_EEPROM_Memory[10] = 0xFF;
         EMULATE_EEPROM_Memory[11] =0xFF;;
         EMULATE_EEPROM_Memory[12] =0xFF;
         EMULATE_EEPROM_Memory[13] =0xFF; 
        }
}

//refactor into a general purpose button thing...
void PowerDown(void)
{ 

    if (PowerButton(Off))
    {
        JETSON_5V_ON_SetLow();
        HOLD_PWR_SetLow();
        asm("reset");
    }
    return;
    
}


//returns 1 if Power button held long enough
// or 0 if not....
uint8_t PowerButton (bool OnOff)
{
    uint8_t ButtonPressHold;
    ButtonPressHold=0;
    bool ButtonPassed;
    ButtonPassed=0;
    
    while(!POWER_BUTTON_GetValue())
    {
        ButtonPressHold++;
        
     
       PWM_BLUE_SetHigh();//Turn on Green LED
       PWM_GREEN_SetLow();//Turn off Red LED
      __delay_ms(100); 
      PWM_GREEN_SetHigh();//Turn on Red LED
      PWM_BLUE_SetLow();//Turn off Green LED
      __delay_ms(50);
      if (ButtonPressHold>5)  
      {
          ButtonPassed=1;
        
          break;
      }
          
    }
    if (OnOff)
    {
         HOLD_PWR_SetHigh();
         JETSON_5V_ON_SetHigh();
         PWM_BLUE_SetHigh();//Turn/JETSON_5V_ON_SetHigh(); on Green LED
         PWM_GREEN_SetLow();//Turn off Red LED 
    }
    else
    {
      PWM_GREEN_SetHigh();//Turn on Red LED
      PWM_BLUE_SetLow();//Turn off Green LED  
    }
        

    __delay_ms(50);
    while(!POWER_BUTTON_GetValue())
    {
        ClrWdt();
       __delay_ms(50);    
    }
    
    __delay_ms(50);
    
     while(!POWER_BUTTON_GetValue())
    {
         ClrWdt();
         __delay_ms(50);    
    }
    
    return(ButtonPassed);
         
}



//Self Test routine ... flashes lasers, checks sensors
void POST_Routine(void)
{
    uint8_t count;
   
    BeamOn;
    
    //Flash the lasers a few times
        for(count=0;count<5;count++)
        {
            FrontLaserOn;
            RearLaserOn;
            __delay_ms(200);
            ClrWdt();
            FrontLaserOff;
            RearLaserOff;
            __delay_ms(200);
            FrontLaserOn;
            __delay_ms(200)
            FrontLaserOff;
            __delay_ms(200);
            ClrWdt();
            __delay_ms(200);
            PWM_BLUE_Toggle();
            PWM_GREEN_Toggle();
        }
        

    //check both sensors
          PWM_BLUE_SetLow();//Turn off Green LED
          PWM_GREEN_SetHigh();//Turn on Red LED
        
        __delay_ms(200);
        ClrWdt();
      
       
          
            ClrWdt();
           FrontLaserOn; 
          while(FRONT_BALL_SENSE_GetValue())
          {
            ClrWdt();
             if(!POWER_BUTTON_GetValue())
              PowerDown();
          }
          PWM_BLUE_SetHigh();//Turn onGreen LED
          PWM_GREEN_SetLow();//Turn off Red LED
          FrontLaserOff;
          __delay_ms(500);
          ClrWdt();
          RearLaserOn;
           PWM_BLUE_SetLow();//Turn off Green LED
          PWM_GREEN_SetHigh();//Turn on Red LED
         while(REAR_BALL_SENSE_GetValue())
         {
            ClrWdt();
            if(!POWER_BUTTON_GetValue())
             PowerDown();
         }
            PWM_BLUE_SetHigh();//Turn onGreen LED
          PWM_GREEN_SetLow();//Turn off Red LED
          RearLaserOff;
          
}      
    
 void LaunchTest(void)
 {
     float voltage;
     voltage=0;
     float conversion;
     conversion = 0.0003125;
    
     uint16_t RegInt;
     ClrWdt();
     RegInt=EMULATE_EEPROM_Memory[8];
     RegInt<<=8;
     RegInt+=EMULATE_EEPROM_Memory[9];
     voltage=(float)RegInt;
     
     
     voltage=voltage*conversion;
     
     conversion=voltage/12;
      
 }


 
 void GetBattVolts(void)
 {
   
    uint8_t dst[2];
     PWM_BLUE_SetLow();//Turn off Green LED
     PWM_GREEN_SetHigh();//Turn on Red LED
     ClrWdt();
     i2c2_read_regs(0x36, 0x09, dst, 2);
      ClrWdt(); 
      
     EMULATE_EEPROM_Memory[8]=dst[1];
     EMULATE_EEPROM_Memory[9]=dst[0];
     PWM_BLUE_SetHigh();//Turn off Green LED
     PWM_GREEN_SetLow();//Turn on Red LED
 }

void GetAccel(void)
{
    PWM_BLUE_SetLow();//Turn off Green LED
    PWM_GREEN_SetHigh();//Turn on Red LED
    
    PWM_BLUE_SetHigh();//Turn off Green LED
    PWM_GREEN_SetLow();//Turn on Red LED
}
// accellerometer header will look something like this

/*
 #pragma once
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Set to 0x18 (SA0=0) or 0x19 (SA0=1); default 0x18 */
//void LIS2DW12_SetAddress_I2C2(uint8_t addr);

//bool LIS2DW12_Init_I2C2(void);                 // returns true on success
//bool LIS2DW12_ReadXYZ_I2C2(int16_t *x, int16_t *y, int16_t *z);

//#ifdef __cplusplus
//}
//#endif

 