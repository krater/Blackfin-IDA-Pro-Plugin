/*
   Rigol firmware loader v0.2 for IDA Pro
   
   Copyright (c) 2011 by TomasVlad.

   Based on parts of libopcodes (Free Software Foundation, Inc.) and
   Simple Python Byte Code Module (Chris Eagle <cseagle@gmail.com>)

   Some minor changes from Andreas Schuler <andreas at schulerdev.de>
   
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

/*
        L O A D E R  for firmware files for Rigol oscilloscope.
*/

#include "../../ldr/idaldr.h"
#include "rigol_ldr.h"

static rgl_hdr _hdr;
static ldr_hdr blk_hdr;
static int flg_fin;
unsigned int addr,size;
unsigned short flags;
static char buf[0x10000];

//--------------------------------------------------------------------------
//
//      check input file format. if recognized, then return 1
//      and fill 'fileformatname'.
//      otherwise return 0
//
static int idaapi accept_rgl_file(linput_t *li,char fileformatname[MAX_FILE_FORMAT_NAME], int n) 
{
  if(n) return(0);
  qlseek(li, 0, SEEK_SET);
  if(qlread(li, &_hdr, sizeof(_hdr)) != sizeof(_hdr)) return(0);
  qstrncpy(fileformatname, "Rigol GEN/BIN ROM file", MAX_FILE_FORMAT_NAME);
  return(1);
}

int get_hdr(linput_t *li)
{
	unsigned char tmp[10];

	if(qlread(li, tmp, 10) != 10) return(0);
	addr = tmp[0] | tmp[1]<<8 | tmp[2]<<16 | tmp[3]<< 24;
	size = (tmp[4] | tmp[5]<<8) & 0xFFFF;
	flags = tmp[8] | tmp[9]<<8;
	return(1);
}
//--------------------------------------------------------------------------
//
//      load file into the database.
//
static void idaapi load_rgl_file(linput_t *li, ushort neflags,const char * /*fileformatname*/)
{
	flg_fin = 0;
	qlseek(li, 21, SEEK_SET);
	set_processor_type("Blackfin", SETPROC_ALL|SETPROC_FATAL);

	if(!add_segm(0, 0x0, 0x100000, "SDRAM0", CLASS_CODE)) loader_failure();
	if(!add_segm(0, 0x19e000, 0x19e400, "SDRAM1", CLASS_CODE)) loader_failure();
	if(!add_segm(0, 0x608800, 0x700000, "SDRAM2", CLASS_CODE)) loader_failure();
	if(!add_segm(0, 0xff806000, 0xff808000, "SDRAM3", CLASS_CODE)) loader_failure();
	if(!add_segm(0, 0xffa08000, 0xffa10000, "SDRAM4", CLASS_CODE)) loader_failure();

	msg("Start loading\n");

	while(1)
	{
		if(!get_hdr(li)) loader_failure();
		msg("hdr:: addr=0x%0X, size=0x%0X, flags=0x%0X\n", addr, size, flags);
		if(flags & _FINAL) flg_fin = 1;
		if((flags & _IGNORE) || (flags & _INIT)) { qlread(li, buf, size); continue; }
		if(flags & _ZEROFILL) {memset(buf,0,size); mem2base(buf, addr, addr + size, -1); continue; }
		
		file2base(li, qltell(li), addr, addr + size, FILEREG_PATCHABLE);
		
		if(flg_fin) break;
	}

	inf.af = 
	AF_FIXUP        | //   0x0001          // Create offsets and segments using fixup info
	AF_MARKCODE     | //   0x0002          // Mark typical code sequences as code
	AF_UNK          | //   0x0004          // Delete instructions with no xrefs
	AF_CODE         | //   0x0008          // Trace execution flow
	AF_PROC         | //   0x0010          // Create functions if call is present
	AF_USED         | //   0x0020          // Analyze and create all xrefs
	AF_FLIRT        | //   0x0040          // Use flirt signatures
	AF_PROCPTR      | //   0x0080          // Create function if data xref data->code32 exists
	AF_NULLSUB      | //   0x0200          // Rename empty functions as nullsub_...
	AF_IMMOFF       | //   0x2000          // Convert 32bit instruction operand to offset
	AF_DREFOFF    ; //   0x4000          // Create offset if data xref to seg32 exists
	inf.af2 = 0;
}

//----------------------------------------------------------------------
static bool idaapi init_loader_options(linput_t*)
{
  set_processor_type("Blackfin", SETPROC_ALL|SETPROC_FATAL);
  return true;
}

//----------------------------------------------------------------------
//
//      LOADER DESCRIPTION BLOCK
//
//----------------------------------------------------------------------
loader_t LDSC =
{
  IDP_INTERFACE_VERSION,
  0,                            // loader flags
  accept_rgl_file,
  load_rgl_file,
  NULL,
  NULL,
  init_loader_options,
};
