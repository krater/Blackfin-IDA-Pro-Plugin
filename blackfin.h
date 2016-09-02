/*
   IDA Pro Processor Module for Blackfin Instruction Set
   
   Copyright (C) 2011 
   Andreas Schuler <andreas@schulerdev.de>

   Based on parts of libopcodes (Free Software Foundation, Inc.) and
   Simple Python Byte Code Module (Chris Eagle <cseagle@gmail.com>)
   
   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the Free
   Software Foundation; either version 2 of the License, or (at your option) 
   any later version.
   
   This program is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
   FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for 
   more details.
   
   You should have received a copy of the GNU General Public License along with 
   this program; if not, write to the Free Software Foundation, Inc., 59 Temple 
   Place, Suite 330, Boston, MA 02111-1307 USA
*/


#ifndef __BLACKFIN_HPP
#define __BLACKFIN_HPP

#include <pro.h>
#include <kernwin.hpp>
#include "../module/idaidp.hpp"
#include <fpro.h>

#pragma pack(1)

#define PLFM_BLACKFIN 0x42235

enum blackfin_registers { r_cs, r_ds };

enum blackfin_instructions {
i_null = 0,           // Unknown Operation
i_stop,
i_call,
i_jump,
i_condjump,
i_other,
i_last
};

//------------------------------------------------------------------------
void  idaapi header(void);
void  idaapi footer(void);

void  idaapi segstart(ea_t ea);
void  idaapi segend(ea_t ea);

int   idaapi blackfin_ana(void);
int   idaapi blackfin_emu(void);
void  idaapi blackfin_out(void);

bool idaapi create_func_frame(func_t *pfn);

ea_t  idaapi get_ref_addr(ea_t ea, const char *str, int pos);

#pragma pack()
#endif

