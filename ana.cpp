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

#include "blackfin.h"
#include "dis-asm.h"

disassemble_info adi;

//----------------------------------------------------------------------
int idaapi blackfin_ana(void) 
{
	if(cmd.ea&0x01)			//only process even addresses
		return 0;

	cmd.size = disasm_insn_bfin(cmd.ea,&adi);
	cmd.itype = adi.itype;

	return cmd.size;
}
