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
#include <diskio.hpp>
#include <ieee.h>

static netnode helper;
char device[MAXSTR] = "";
static size_t numports;
static ioport_t *ports;

#define CHECK_IORESP 1			//activate config file reading, thanks to tommie

#include "../iocommon.cpp"		//it doesn't look nice, but i think its designated to include it


// A well behaving processor module should call invoke_callbacks()
// in his notify() function. If invoke_callbacks function returns 0, 
// then the processor module should process the notification itself
// Otherwise the code should be returned to the caller:

//--------------------------------------------------------------------------
static int idaapi notify(processor_t::idp_notify msgid, ...) 
{
	va_list va;
	va_start(va, msgid);   //setup args list

	int result = invoke_callbacks(HT_IDP, msgid, va);
	if (result == 0) 
	{
		result = 1;             //default success
		switch(msgid) 
		{
			case processor_t::init:
				inf.mf = 0;       //ensure little endian!
				break;  
			case processor_t::newprc:
			{
				/*char cfgfile[QMAXFILE];
				get_cfg_filename(cfgfile, sizeof(cfgfile));
				if(choose_ioport_device(cfgfile, device, sizeof(device), parse_area_line0))
					set_device_name(device, IORESP_ALL);				
				break;*/
			}
			case processor_t::make_data: 
			{
				ea_t ea = va_arg(va, ea_t);
				flags_t flags = va_arg(va, flags_t);
				tid_t tid = va_arg(va, tid_t);
				asize_t len = va_arg(va, asize_t);
				break;
			}
			case processor_t::newfile:
			{
				char cfgfile[QMAXFILE];
				get_cfg_filename(cfgfile, sizeof(cfgfile));
				if(choose_ioport_device(cfgfile, device, sizeof(device), parse_area_line0))
					set_device_name(device, IORESP_ALL);
			}
			break;
		}
	}

	va_end(va);

	return result;
}

//----------------------------------------------------------------------
// Set IDP options. Either from the configuration file either allow the user
// to specify them in a dialog box.
static const char* idaapi set_idp_options(const char *keyword, int value_type,const void *value)
{
  return(IDPOPT_OK);
}

//----------------------------------------------------------------------
static void idaapi func_header(func_t *f) 
{
   char buf[256];
   char name[64];
   
   printf_line(0, "# =============== S U B R O U T I N E =======================================");
   printf_line(0, "");
   
   get_func_name(f->startEA, name, sizeof(name));
   init_output_buffer(buf, sizeof(buf));
   out_snprintf("def %s(", name);
   out_snprintf("...):");
   term_output_buffer();
   printf_line(0, "%s", buf);
   printf_line(0, "");
   
}

static void idaapi func_footer(func_t *) 
{
   printf_line(0, "# end function");
}


//----------------------------------------------------------------------
static asm_t blackfin_asm=
{
	AS_COLON | AS_UDATA | ASH_HEXF3 | ASO_OCTF1 | ASD_DECF0 | ASB_BINF0,
	0,
	"Blackfin assembler",
	0,        // no help screen
	NULL,     // header
	NULL,     // bad instructions
	NULL,     // origin
	NULL,     // end of file

	"#",      // comment string
	'"',      // string delimiter
	'\'',     // char delimiter
	"\\\"'",  // special symbols in char and string constants

	"ascii",         // ascii string directive
	"db",         // byte directive
	"dw",       // word directive
	"dd",       // double words
	NULL,       // qwords
	NULL,       // oword  (16 bytes)
	NULL,       // float
	NULL,       // double
	NULL,       // no tbytes
	NULL,       // no packreal
	"#d dup(#v)",     // arrays:
			// #h - header(.byte,.word)
			// #d - size of array
			// #v - value of array elements
	"db ?",         //".reserv  %s",  // uninited data (reserve space)
	" = ",        // equ
	NULL,         // seg prefix
	NULL,         // preline for checkarg
	NULL,         // checkarg_atomprefix
	NULL,         // checkarg operations
	NULL,         // XlatAsciiOutput
	NULL,         // a_curip
	func_header,  // func header
	func_footer,  // func footer
	"",     // public (disable ouput)
	NULL,		// weak
	NULL,		// extrn
	NULL,		// comm
	NULL,		// get_type_name
	"align",		// align
	'(', ')',	// lbrace, rbrace
	"%",     // mod
	"&",     // and
	"|",     // or
	"^",     // xor
	"!",     // not
	"<<",    // shl
	">>",    // shr
	NULL,    // sizeof
	0,		 // flag2
	NULL,	 // comment close
	NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL
};

//-----------------------------------------------------------------------
char author[]="Copyright (C) 2011 Andreas Schuler <andreas at schulerdev.de>, License GPL v2";

static asm_t *asms[]={&blackfin_asm,NULL};
static char *shnames[]={"Blackfin",NULL};
static char *lnames[]={"Blackfin",NULL};

//--------------------------------------------------------------------------
static uchar result_0[]={1};
static bytes_t results[]={{sizeof(result_0),result_0},{0,NULL}};


static char *RegNames[] =
{
  "R0.L", "R1.L", "R2.L", "R3.L", "R4.L", "R5.L", "R6.L", "R7.L",
  "R0.H", "R1.H", "R2.H", "R3.H", "R4.H", "R5.H", "R6.H", "R7.H",
  "R0", "R1", "R2", "R3", "R4", "R5", "R6", "R7",
  "R1:0", "R3:2", "R5:4", "R7:6", "P0", "P1", "P2", "P3",
  "P4", "P5", "SP", "FP", "A0.X", "A1.X", "A0.W", "A1.W",
  "A0", "A1", "I0", "I1", "I2", "I3", "M0", "M1",
  "M2", "M3", "B0", "B1", "B2", "B3", "L0", "L1",
  "L2", "L3",
  "AZ", "AN", "AC0", "AC1", "AV0", "AV1", "AV0S", "AV1S",
  "AQ", "V", "VS",
  "sftreset", "omode", "excause", "emucause", "idle_req", "hwerrcause", "CC", "LC0",
  "LC1", "ASTAT", "RETS", "LT0", "LB0", "LT1", "LB1",
  "CYCLES", "CYCLES2", "USP", "SEQSTAT", "SYSCFG", "RETI", "RETX", "RETN",
  "RETE", "EMUDAT",
  "R0.B", "R1.B", "R2.B", "R3.B", "R4.B", "R5.B", "R6.B", "R7.B",
  "P0.L", "P1.L", "P2.L", "P3.L", "P4.L", "P5.L", "SP.L", "FP.L",
  "P0.H", "P1.H", "P2.H", "P3.H", "P4.H", "P5.H", "SP.H", "FP.H",
  "I0.L", "I1.L", "I2.L", "I3.L", "M0.L", "M1.L", "M2.L", "M3.L",
  "B0.L", "B1.L", "B2.L", "B3.L", "L0.L", "L1.L", "L2.L", "L3.L",
  "I0.H", "I1.H", "I2.H", "I3.H", "M0.H", "M1.H", "M2.H", "M3.H",
  "B0.H", "B1.H", "B2.H", "B3.H", "L0.H", "L1.H", "L2.H", "L3.H",
  "AC0_COPY", "V_COPY", "RND_MOD",
  "LASTREG"
};

instruc_t Instructions[] = {
{"",            0                               },
{"STOP",        CF_STOP                         },
{"CALL",        CF_CALL                         },
{"JUMP",        CF_JUMP|CF_STOP                 },
{"CONDJUMP",	CF_JUMP							},
{"OTHER",       0                               },
};



//----------------------------------------
ea_t idaapi get_ref_addr(ea_t ea, const char *name, int pos)
{
   return BADADDR-1;
}

//-----------------------------------------------------------------------
//      Processor Definition
//-----------------------------------------------------------------------
processor_t LPH =
{
	IDP_INTERFACE_VERSION,     //kernel version
	PLFM_BLACKFIN,           //id
	PR_RNAMESOK | PR_NO_SEGMOVE,
	8,                  // 8 bits in a byte for code segments
	8,                  // 8 bits in a byte for other segments

	shnames,
	lnames,

	asms,        // assemblers

	notify,

	header,      // function to produce start of disassembled text
	footer,      // function to produce end of disassembled text

	segstart,    // function to produce start of segment
	segend,      // function to produce end of segment

	NULL,        // function to produce assume directives

	// Analyze one instruction and fill 'cmd' structure.
	// cmd.ea contains address of instruction to analyze.
	// Return length of the instruction in bytes, 0 if instruction can't be decoded.
	// This function shouldn't change the database, flags or anything else.
	// All these actions should be performed only by u_emu() function.
	blackfin_ana,

	// Emulate instruction, create cross-references, plan to analyze
	// subsequent instructions, modify flags etc. Upon entrance to this function
	// all information about the instruction is in 'cmd' structure.
	// Return length of the instruction in bytes.
	blackfin_emu,

	// Generate text representation of an instruction in 'cmd' structure.
	// This function shouldn't change the database, flags or anything else.
	// All these actions should be performed only by u_emu() function.
	blackfin_out,

	// Generate text representation of an instructon operand.
	// This function shouldn't change the database, flags or anything else.
	// All these actions should be performed only by u_emu() function.
	// The output text is placed in the output buffer initialized with init_output_buffer()
	// This function uses out_...() functions from ua.hpp to generate the operand text
	// Returns: 1-ok, 0-operand is hidden.
	0,		// We doesn't use this function because there are up to 4 parallel instructions that 
			// can have more operators as exists in the operator-structs



	// Generate text represenation of data items
	// This function MAY change the database and create cross-references, etc.
	intel_data,	   //sdk offers intel_data

	NULL,          //  cmp_opnd,  // 0 if not cmp 1 if eq
	NULL, //(&op)  int : 1 -yes 0-no

	qnumber(RegNames),  // Number of registers
	RegNames,           // Regsiter names
	NULL,               // get abstract register

	0,                  // Number of register files
	NULL,               // Register file names
	NULL,               // Register descriptions
	NULL,               // Pointer to CPU registers

	r_cs,r_ds,
	0,                  // size of a segment register
	r_cs,r_ds,

	NULL,               // No known code start sequences
	results,

	i_null,i_last,      // first and last number of instruction
	Instructions,       // Ptr to Instructions-array
	NULL,               // isFarJump or Call
	NULL,               //  Offset Generation Function. Usually NULL.
	0,                  // size of tbyte
	ieee_realcvt,
	{0,7,15,0},         // real width
	NULL,               // is this instruction switch
	NULL,               // generate map-file
	get_ref_addr,       // extract address from a string
	NULL,               // is_sp_based
	create_func_frame,  // create_func_frame
	NULL,               // get_func_retsize
	NULL,               // gen_stkvar_def
	NULL, //python_specseg,       // out special segments
	1,//RETURN_VALUE,              // icode_return
	NULL, //set_idp_options,    // Set IDP options
	NULL,               // Is alignment instruction?
	NULL,               // Micro virtual machine description
	0,                  // high_fixup_bits
};

//-----------------------------------------------------------------------
