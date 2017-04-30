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
	
    

*/

