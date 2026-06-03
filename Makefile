encode = $(subst $() ,*,$(1))
decode = $(subst *,$() ,$(1))
quotes = $(addprefix ",$(addsuffix ",$(1)))
includes = $(addprefix -I,$(1))

RM = cmd.exe /C del

SRC_DIR = src
OBJ_DIR = obj
INC_DIR = includes
OUT_DIR = out
INCS = $(INC_DIR) ..\..\Wav\includes # $(call encode,C:\Program Files (x86)\OpenAL 1.1 SDK\include)
INCS := $(call decode,$(call includes,$(call quotes,$(INCS))))
LIB_DIRS = ..\..\Wav\lib ..\..\OctBinary\lib # $(call encode,C:\Program Files (x86)\OpenAL 1.1 SDK\libs\Win64)
LIB_DIRS_GXX = $(call decode,$(addprefix -L,$(call quotes,$(LIB_DIRS))))
LIB_DIRS_CL = $(call decode,$(addprefix -LIBPATH:,$(call quotes,$(LIB_DIRS))))
LIBS = oct_binary.lib oct_wav.lib Winmm.lib
LIBS := $(call decode, $(addprefix -l,$(call quotes,$(LIBS))))

IGNORE = $(SRC_DIR)\main.cpp
LIB_SRCS = $(filter-out $(IGNORE), $(wildcard $(SRC_DIR)/*.cpp))
LIB_OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.obj,$(LIB_SRCS))
LIB_TARGET = simple_drawing

OUT = main

CFLAGS = -c

all: $(OUT).exe

$(OUT).exe:
#	g++ -Os $(INCS) $(LIB_DIRS_GXX) $(LIBS) $(wildcard $(SRC_DIR)/*.cpp) -o $(OUT_DIR)/$@ -mwindows
	cl -O1 -GL -GR- -MD -EHsc $(INCS) User32.lib Gdi32.lib $(wildcard $(SRC_DIR)/*.cpp) -Fe$(OUT_DIR)/$(OUT).exe -link $(LIB_DIRS_CL) -OPT:REF -OPT:ICF -LTCG

lib: $(LIB_OBJS)
	lib $(LIB_OBJS) /OUT:$(LIB_TARGET).lib

$(OBJ_DIR)/%.obj: $(SRC_DIR)/%.cpp
	cl $(CFLAGS) -I $(INC_DIR) $< /Fo$@

run:
	$(OUT_DIR)/$(OUT).exe

define RMFUNC
	$(RM) $(1)
endef

clean:
	$(foreach x,$(wildcard $(OUT_DIR)/*.obj) $(wildcard $(OUT_DIR)/*.exe),$(call RMFUNC, $(x)))
