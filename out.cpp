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

#include "bfin.h"
#include "blackfin.h"
#include "dis-asm.h"


disassemble_info odi;


inline void out_instruction(const char *str)
{
	out_line(str, COLOR_INSN);
}

//----------------------------------------------------------------------
void idaapi blackfin_out(void) 
{
	char str[MAXSTR];  //MAXSTR is an IDA define from pro.h
	init_output_buffer(str, sizeof(str));

	disasm_insn_bfin(cmd.ea,&odi);

	out_instruction(odi.str);

	term_output_buffer();
	gl_comm = 1;      //we want comments!
	MakeLine(str);    //output the line with default indentation


	//Add data references
	if(odi.drefmode==DMODE_LOAD_HIGH) 
	{
		char name[60];
		if(get_colored_long_name(0, odi.daddr, name,60))
			printf_line(0,"                      -> %s", name);
		else
			printf_line(0,"                      -> " COLSTR("0x%lx",SCOLOR_NUMBER), (unsigned long) odi.daddr);
	}

}

//--------------------------------------------------------------------------
// function to produce start of disassembled text
//set as member of LPH struct
void idaapi header(void) 
{
   MakeLine("My header line");
}

void idaapi footer() 
{
   MakeLine("My footer line");
}

//----------------------------------------------------------------------
// function to produce start of segment
void idaapi segstart(ea_t ea) 
{  
}

//--------------------------------------------------------------------------
// function to produce end of segment
void idaapi segend(ea_t ea) 
{
}