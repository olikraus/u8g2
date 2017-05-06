/*

  guistate.c
  
  
  
  
  Events:
    POR (power on reset)
    Select Button Pressed
    Next Button Pressed
    Current Time Equals Alarm Time
    Signal Timer Expired
    Menu Timer Expired
    Goto Stop Timer Expired
    
  Output
    Signal Timer Start
    Alarm Signal On/Off
  
  
  STOP STATE
    --> SIGNAL ALARM STATE, if "Current Time Equals Alarm Time"
    --> DISPLAY TIME STATE, if "Select Button Pressed" or "Next Button Pressed"
    --> DISPLAY TIME STATE, if POR
    
  SIGNAL ALARM STATE
    --> DISPLAY TIME STATE, if "Signal Timer Expired" or "Select Button Pressed" or "Next Button Pressed"
  
  DISPLAY TIME STATE
    --> STOP STATE, if "Goto Stop Timer Expired"
    --> SIGNAL ALARM STATE, if "Current Time Equals Alarm Time"
    --> MENU STATE, if "Select Button Pressed" or "Next Button Pressed"
  
  MENU STATE
    --> DISPLAY TIME STATE, if "Menu Timer Expired"
    --> STOP STATE, if "Goto Stop Timer Expired"
    ( -->SIGNAL ALARM STATE ???)


  STOP STATE
    Description: 
      When entering this state, the uC goes into stop mode.
      Leaving stop mode always looks like a reset. After this, the state changes to SIGNAL ALARM or DISPLAY TIME STATE
      
      
  SIGNAL ALARM STATE
    Description:
      This state is usually entered if the "Current Time Equals Alarm Time".
      
    Transition to SIGNAL ALARM STATE:
      Start "Signal Timer"
      "Alarm Signal On"
    
    Transition from SIGNAL ALARM STATE:
      1) "Select Button Pressed"
	"Alarm Signal Off"
	Goto DISPLAY TIME STATE
      2) "Next Button Pressed"
	"Alarm Signal Off"
	Snooze function
	Goto DISPLAY TIME STATE
      3) "Signal Timer Expired"
	"Alarm Signal Off"
	Goto DISPLAY TIME STATE
	
  DISPLAY TIME STATE
    Description:
      The default screen visible to the user. It may include time, date and alarm.
      Menu is started from here.
      
  
  
*/

#ifdef OBSOLETE

#include "gui.h"


int is_current_time_equal_to_alarm_time(void)
{
  return gui_data.is_alarm;
}



/*============================================*/
/* next state function */

void gui_state_next(void)
{
  switch(gui_data.gui_state)
  {
    case GUI_STATE_STOP:		/* this is also the default value after each reset */
      if ( is_por_reset() )
      {
	/* no alarm is possible, just display the time */
	gui_Init(&u8g2, 1);
      }
      else
      {
	/* user has pressed a button or the 1 minute wakeup happend */
	gui_Init(&u8g2, 0);
	
	if ( is_current_time_equal_to_alarm_time() )
	{
	  enable_alarm();	
	  gui_data.gui_state = GUI_STATE_SIGNAL_ALARM;
	  menu_SetMEList(&gui_menu, melist_active_alarm_menu, 0);
	}
	else
	{
	  /* display time: this is default of gui_Init() */
	}
      }
      break;
    case GUI_STATE_SIGNAL_ALARM:
      break;
    case GUI_STATE_DISPLAY_TIME:
      break;
    case GUI_STATE_MENU:
      break;
  }
}


#endif