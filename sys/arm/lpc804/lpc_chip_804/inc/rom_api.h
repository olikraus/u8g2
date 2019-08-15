/*
 * rom_api_.h
 *
 *  Created on: 
 *   By Arthur:
 */

#ifndef __ROM_API_H_
#define __ROM_API_H_



// FRO API 
typedef struct _PWRD {
  const unsigned int  reserved[2];
  void (*set_fro_frequency)(unsigned frequency);
} PWRD_API_T;



// Integer divide API routines
typedef struct {
  int quot;      // Quotient
  int rem;       // Remainder
} IDIV_RETURN_T;

typedef struct {
  unsigned quot; // Quotient
  unsigned rem;  // Reminder
} UIDIV_RETURN_T;

typedef struct {
  int (*sidiv)(int numerator, int denominator);                         // Signed integer division 
  unsigned (*uidiv)(unsigned numerator, unsigned denominator);          // Unsigned integer division 
  IDIV_RETURN_T (*sidivmod)(int numerator, int denominator);            // Signed integer division with remainder 
  UIDIV_RETURN_T (*uidivmod)(unsigned numerator, unsigned denominator); // Unsigned integer division with remainder 
} ROM_DIV_API_T;





// The master structure that defines the table of all ROM APIs on the device (a.k.a ROM Driver table)
typedef struct _ROM_API {
  const unsigned int  reserved3[3];   // Offsets 0, 4, 8
  const PWRD_API_T    *pPWRD;         // Offset 0xC. Power APIs function table base address
  const ROM_DIV_API_T *divApiBase;    // Offset 0x10. Integer division routines function table base address
  const unsigned int  reserved7[7];   // Offsets 0x14 - 0x2C
} LPC_ROM_API_T;





#define ROM_DRIVER_BASE (0x0F001FF8UL)

// Define a pointer to the master table
#define LPC_ROM_API     (*(LPC_ROM_API_T * *)ROM_DRIVER_BASE)

// Use like this:
// ROM_DIV_API_T const *pROMDiv = LPC_ROM_API->divApiBase;             // Create and initialize a pointer to the DIVIDE functions table
// int32_t result;                                                     // Declare an int variable
// result = pROMDiv->sidiv(-99, 6);                                    // Call the sidiv routine, result now contains -99/6 = -16
// ROM_DIV_API_T const *pPwr = LPC_ROM_API->pPWRD;                     // Create and initialize a pointer to the power API functions table
// pPwr->set_power((uint32_t *)&cmd_table, (uint32_t *)&result_table); // Call the set_power routine




// Alternate form
#define LPC_PWRD_API    ((PWRD_API_T *   ) ((*(LPC_ROM_API_T * *) (ROM_DRIVER_BASE))->pPWRD))
#define LPC_DIVD_API    ((ROM_DIV_API_T *) ((*(LPC_ROM_API_T * *) (ROM_DRIVER_BASE))->divApiBase))

// Use like this:
// LPC_PWRD_API->set_power((uint32_t *)&cmd_table, (uint32_t *)&result_table); // Call the set_power routine


#endif // rom_api.h
