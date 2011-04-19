#Set this variable to point to your SDK directory
IDA=../../

#Set this variable to the desired name of your compiled processor
PROC=blackfin

#Specify a description string for your processor, this is required
#The syntax is <long name>:<short name>
DESCRIPTION=blackfin:blackfin

ifndef __LINUX__
PLATFORM_CFLAGS=-D__NT__ -D__IDP__ -mno-cygwin
PLATFORM_LDFLAGS=-mno-cygwin
IDALIB=$(IDA)libgcc.w32/ida.a
PROC_EXT=.w32
else
PLATFORM_CFLAGS=-D__LINUX__
IDALIB=$(IDA)libgcc32.lnx/pro.a
PROC_EXT=.ilx
endif

#Platform specific compiler flags
CFLAGS=-Wextra $(PLATFORM_CFLAGS)

#Platform specific ld flags
LDFLAGS=-Wl -shared -s $(PLATFORM_LDFLAGS) 

#specify any additional libraries that you may need
EXTRALIBS=

# Destination directory for compiled plugins
OUTDIR=$(IDA)bin/procs/

# Postprocessing tool to add processor comment
MKIDP=$(IDA)bin/fix_proc
#MKIDP=$(IDA)bin/mkidp

#list out the object files in your project here
OBJS=	ana.o emu.o ins.o out.o reg.o
      
BINARY=$(OUTDIR)$(PROC)$(PROC_EXT)

all: $(OUTDIR) $(BINARY)

clean:
	-@rm *.o
	-@rm $(BINARY)

$(OUTDIR):
	-@mkdir -p $(OUTDIR)

CC=g++
INC=-I$(IDA)include/

%.o: %.cpp
	$(CC) -c $(CFLAGS) $(INC) $< -o $@

LD=g++

ifndef __LINUX__
#Windows processor's require post processing
$(BINARY): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS) $(IDALIB) $(EXTRALIBS)
	$(MKIDP) $(BINARY) "$(DESCRIPTION)"
else
$(BINARY): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS) $(IDALIB) $(EXTRALIBS)
endif

blackfin.o: blackfin.cpp
ana.o: ana.cpp
emu.o: emu.cpp
ins.o: bfin-dis.cpp
out.o: out.cpp
reg.o: reg.cpp
