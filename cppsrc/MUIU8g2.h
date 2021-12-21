/*
  
  MUIU8g2.h
  
  C++ Arduino wrapper for mui.h (monochome minimal user interface)

  Universal 8bit Graphics Library (https://github.com/olikraus/u8g2/)

  Copyright (c) 2016, olikraus@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification, 
  are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this list 
    of conditions and the following disclaimer.
    
  * Redistributions in binary form must reproduce the above copyright notice, this 
    list of conditions and the following disclaimer in the documentation and/or other 
    materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  


*/


#ifndef MUIU8G2_HH
#define MUIU8G2_HH

#include "mui.h"
#include "mui_u8g2.h"

class MUIU8G2
{
  protected:
    mui_t mui;
  public:
    MUIU8G2(void) { }
  
    MUIU8G2(U8G2 &u8g2, fds_t *fds, muif_t *muif_list, size_t muif_cnt) {
      mui_Init(&mui, (void *)u8g2.getU8g2(), fds, muif_list, muif_cnt);
    }
    void begin(U8G2 &u8g2, fds_t *fds, muif_t *muif_list, size_t muif_cnt) {
      mui_Init(&mui, (void *)u8g2.getU8g2(), fds, muif_list, muif_cnt);
    }
    mui_t *getMUI(void) { return &mui; }

    uint8_t getCurrentCursorFocusPosition(void) { return mui_GetCurrentCursorFocusPosition(&mui); }
    void draw(void) { mui_Draw(&mui); }
    void getSelectableFieldTextOption(uint8_t form_id, uint8_t cursor_position, uint8_t nth_token)
      { mui_GetSelectableFieldTextOption(&mui, form_id, cursor_position, nth_token); }      
    void enterForm(fds_t *fds, uint8_t initial_cursor_position) { mui_EnterForm(&mui, fds, initial_cursor_position); }
    void leaveForm(void) { mui_LeaveForm(&mui); }
    uint8_t gotoForm(uint8_t form_id, uint8_t initial_cursor_position) { return mui_GotoForm(&mui, form_id, initial_cursor_position); }
    
    void saveForm(void) { mui_SaveForm(&mui); }
    void restoreForm(void) { mui_RestoreForm(&mui); }
    void nextField(void) { mui_NextField(&mui); }
    void prevField(void) { mui_PrevField(&mui); }
    void sendSelect(void) { mui_SendSelect(&mui); }
    int isFormActive(void) { return mui_IsFormActive(&mui); }    
};


#endif /* MUIU8G2_HH */
