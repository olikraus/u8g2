/*
 * @brief LPC80X Analog comparator driver
 *
 * Copyright(C) NXP Semiconductors, 2012
 * All rights reserved.
 *
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */


#include "LPC8xx.h"
#include "core_cm0plus.h"
#include "syscon.h"
#include "plu.h"
#include <stdarg.h>

typedef struct {
	uint16_t id;
	uint16_t inputs;
	uint32_t tt;
} _stdl_id_ins_tt;

uint32_t generate_plu_tt(uint32_t (* pnt_tt_function)(uint32_t, uint32_t, uint32_t, uint32_t, uint32_t), uint32_t * pnt_tt)
{
	uint32_t result;
	
	uint32_t fin, fin4, fin3, fin2, fin1, fin0;
	uint32_t tt_loc;
	
	result = PLU_ROUTINE_FAILED;
	
	tt_loc = 0;
	for (fin = 0; fin != 32; fin++)
	{
		fin0 = (fin >> 0) & 1;
		fin1 = (fin >> 1) & 1;
		fin2 = (fin >> 2) & 1;
		fin3 = (fin >> 3) & 1;
		fin4 = (fin >> 4) & 1;
		
		tt_loc |= (*pnt_tt_function)(fin0, fin1, fin2, fin3, fin4)<<fin;
	}
	*pnt_tt = tt_loc;
	
	result = PLU_OK;
	
	return result;
}
