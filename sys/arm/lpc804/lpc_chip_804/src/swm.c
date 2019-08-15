#include "swm.h"
#include "LPC8xx.h"

/*****************************************************************************
** Function name:  ConfigSWM
** Description:	   Attaches a pin (designated by its GPIO port and bit numbers)
**                 to a function.
** parameters:	   func: Any function name that is movable.
**                 port_pin: Any pin which has a GPIO port number and bit number.
** Returned value: None
*****************************************************************************/
void ConfigSWM(uint32_t func, uint32_t port_pin) {
  uint32_t temp, i;
  uint32_t pinassign = 0;
  uint32_t shifter = 0;

  for (i=0; i<num_funcs; i++) {
    if (func != i)
      continue;
    else {
      pinassign = i/4;
      shifter = (i%4)*8;
      break;
    }
  }

  temp = LPC_SWM->PINASSIGN[pinassign];
  temp &= ~(0xFF<<(shifter));
  temp |= (port_pin << shifter);
  LPC_SWM->PINASSIGN[pinassign] = temp;

  return;
}



/*****************************************************************************
** Function name:  EnableFixedPinFunc
** Description:	   Enables a fixed-pin function in PINENABLE0 or PINENABLE1.
** parameters:	   func: Any function name that is a fixed-pin function.
** Returned value: None
*****************************************************************************/
void EnableFixedPinFunc(uint32_t func) {
  if (func <= 0x80000000) {
    LPC_SWM->PINENABLE0 &= ~(func);
  }
  else {
    LPC_SWM->PINENABLE1 &= ~(func);
  }
}



/*****************************************************************************
** Function name:  DisableFixedPinFunc
** Description:	   Disables a fixed-pin function in PINENABLE0 or PINENABLE1.
** parameters:	   func: Any function name that is a fixed-pin function.
** Returned value: None
*****************************************************************************/
void DisableFixedPinFunc(uint32_t func) {
  if (func <= 0x80000000) {
    LPC_SWM->PINENABLE0 |= (func);
  }
  else {
    LPC_SWM->PINENABLE1 |= (func);
  }
}

