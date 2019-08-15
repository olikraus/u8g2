#ifndef __IAP_H__
#define __IAP_H__

#include <stdint.h>       // standard types definitions

enum eIAP_COMMANDS
{	
  IAP_PREPARE = 50,       // Prepare sector(s) for write operation
  IAP_COPY_RAM2FLASH,     // Copy RAM to Flash
  IAP_ERASE,              // Erase sector(s)
  IAP_BLANK_CHECK,        // Blank check sector(s)
  IAP_READ_PART_ID,       // Read chip part ID
  IAP_READ_BOOT_VER,      // Read chip boot code version
  IAP_COMPARE,            // Compare memory areas
  IAP_REINVOKE_ISP,       // Reinvoke ISP
  IAP_READ_UID,           // Read unique ID
  IAP_ERASE_PAGE,          // Erase page(s)
  IAP_READ_MISR=70,
  IAP_READ_MISR_EX=73,
};

struct sIAP
{
  uint32_t cmd;           // Command
  uint32_t par[4];        // Parameters
  uint32_t stat;          // Status
  uint32_t res[4];        // Result
};

#define REINVOKE_AUTO			0
#define REINVOKE_UART			1

// Pointer to ROM IAP entry functions
#define IAP_ENTRY_LOCATION              0x0F001FF1

// IAP Call 
typedef void (*IAP_Entry) (uint32_t *cmd, uint32_t *stat);
#define IAP_Call ((IAP_Entry) IAP_ENTRY_LOCATION)

#endif // __IAP_H__
