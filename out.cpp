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

//	printf_line(0," ...  %x ...", odi.caddr);

	//Add data references
	if(odi.drefmode==DMODE_LOAD_HIGH) 
	{
		qstring name;

		
		size_t len = get_max_ascii_length(odi.daddr, ASCSTR_C, true);

		
		
		if(len>2)
		{
			char string[30]={0};

			//fill the registers with the right values for reference...
			if(get_max_ascii_length(odi.daddr-4, ASCSTR_C, true)==4)
				get_ascii_contents(odi.daddr-4, len, ASCSTR_C, string, sizeof(string));

			get_ascii_contents(odi.daddr, len, ASCSTR_C, string, sizeof(string));

			if(get_colored_long_name(&name, odi.daddr))
				printf_line(0,"                      -> %s => " COLSTR("\"%s\"",SCOLOR_RPTCMT), name.c_str(), string);
			else
				printf_line(0,"                      -> " COLSTR("0x%lx",SCOLOR_NUMBER) " => " COLSTR("\"%s\"",SCOLOR_RPTCMT) , (unsigned long) odi.daddr,string);
		}
		else
		{
			if(get_colored_long_name(&name, odi.daddr))
				printf_line(0,"                      -> %s", name.c_str());
			else
				printf_line(0,"                      -> " COLSTR("0x%lx",SCOLOR_NUMBER), (unsigned long) odi.daddr);
		}
	}

}

//--------------------------------------------------------------------------
// function to produce start of disassembled text
//set as member of LPH struct
void idaapi header(void) 
{
	printf_line(0,"");
	printf_line(0,"");
	printf_line(0,"# Blackfin Processor plugin by Andreas Schuler <andreas@schulerdev.de>");
	printf_line(0,"# ");
	printf_line(0,"# Based on Simple Python Byte Code Module (Chris Eagle <cseagle@gmail.com>)");
	printf_line(0,"# Released under GPL v2");
	printf_line(0,"# ");
	printf_line(0,"# More infos at http://www.codenaschen.de");
}

void idaapi footer() 
{
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
