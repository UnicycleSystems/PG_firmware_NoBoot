/* 
 * File:   Events.h
 * Author: Peter
 *
 * Created on August 31, 2018, 12:06 PM
 */

#ifndef EVENTS_H
#define	EVENTS_H

#ifdef	__cplusplus
extern "C" {
#endif
  #include <stdint.h>  
 extern char LedOn;
 extern char FrontSense;
 extern char RearSense;
 extern uint32_t TransitTime;
 extern uint8_t GateTimeout;
 extern uint8_t WakeUp;
 extern uint8_t LightingUpdate;
 
 
/*
 typedef struct EventFlags
    {
        unsigned int AddNewJetsonJob :   1;   // A command from the jetson has been loaded on the queue
        unsigned int JobsActive : 1;  
        unsigned int BallDetected : 1;  // A complete ball detcted evnt has occured, 
        unsigned int PowerButtonPushed : 1;  // A command has been assembled by the console handler
        unsigned int SelfTestComplete : 1 ; // 
        unsigned int NextSelfTest : 1 ; // 
        unsigned int AccelEvent : 1; // 
        unsigned int Event4 : 1;   //
        
        //etc, so up to 16 flags in total
        
    }Events;
    
    extern Events EventJob;
   
  typedef struct ActionFlags
    {
        unsigned int GoToStandby :   1;
        unsigned int ProcessChar  :1; 
        unsigned int ProcessCommand : 1; 
        unsigned int ProcessADC : 1; 
        unsigned int ProcessUpstreamByte : 1;  
        unsigned int SendDownStreamPacket : 1; 
       
        unsigned int Send4Cells :1; // Rename is assigned
        unsigned int Spare2 :1; // don't forget to update in Events.c if
        unsigned int Spare3 :1; // used
        
        
        //etc, so up to 16 flags in total
        
    }Actions;
    
    extern Actions RtosActions;   
    
  void ClearAllEvents(void);
  
 */


#ifdef	__cplusplus
}
#endif

#endif	/* EVENTS_H */
