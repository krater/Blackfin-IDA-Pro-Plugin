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


#ifndef DIS_ASM_H
#define DIS_ASM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "bfin.h"

typedef int (*fprintf_ftype) (void *, const char*, ...) /*ATTRIBUTE_FPTR_PRINTF_2*/;

typedef unsigned int bu32;

struct saved_state
{
  bu32 dpregs[16], iregs[4], mregs[4], bregs[4], lregs[4];
  bu32 ax[2], aw[2];
  bu32 lt[2], lc[2], lb[2];
  bu32 rets;
};

typedef struct disassemble_info
{
	char str[MAXSTR];
	char comment[MAXSTR];

	ulong feature;

	//code reference
	ea_t	caddr;
	cref_t	ctype;

	//data reference
	ea_t	daddr;
	dref_t	dtype;
	long	drefmode;

#define DMODE_LOAD_LOW	0x01
#define DMODE_LOAD_HIGH	0x02
#define DMODE_READPTR	0x04
#define DMODE_WRITEPTR	0x08

	struct saved_state saved_state;

	blackfin_instructions itype;

} disassemble_info;

#define SET_FEATURE(outf,x)					(((outf)->feature)|=(x))
#define SET_CODEREF10(outf,pc,addr,type)	((outf)->caddr=(pc)+((SIGNEXTEND((addr),10))*2));((outf)->ctype=(type))
#define SET_CODEREF12(outf,pc,addr,type)	((outf)->caddr=(pc)+((SIGNEXTEND((addr),12))*2));((outf)->ctype=(type))
#define SET_CODEREF24(outf,pc,addr,type)	((outf)->caddr=(pc)+((SIGNEXTEND((addr),24))*2));((outf)->ctype=(type))
#define SET_DATAREF(outf,addr,type,mode)	((outf)->daddr=(addr));((outf)->dtype=(type));((outf)->drefmode=(mode))

extern int disasm_insn_bfin					(bfd_vma, disassemble_info *);


#ifdef __cplusplus
}
#endif

#endif /* ! defined (DIS_ASM_H) */
