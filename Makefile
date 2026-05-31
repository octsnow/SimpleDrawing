RM = cmd.exe /C del

SRC_DIR = src
OBJ_DIR = obj
INC_DIR = includes
OUT_DIR = out
INCS = $(INC_DIR) ../OctBinary/includes

IGNORE = $(SRC_DIR)/main.c
LIB_SRCS = $(filter-out $(IGNORE), $(wildcard $(SRC_DIR)/*.c))
LIB_OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.obj,$(LIB_SRCS))
LIB_TARGET = simple_drawing

OUT = main

CFLAGS = -c

all: $(OUT).exe

$(OUT).exe:
	gcc -I$(INCS) $(wildcard $(SRC_DIR)/*.c) -o $(OUT_DIR)/$@ -mwindows

cl:
	cl -I $(INCS) User32.lib Gdi32.lib $(wildcard $(SRC_DIR)/*.c) -Fe$(OUT_DIR)/$(OUT).exe /link /SUBSYSTEM:CONSOLE

lib: $(LIB_OBJS)
	lib $(LIB_OBJS) /OUT:$(LIB_TARGET).lib

$(OBJ_DIR)/%.obj: $(SRC_DIR)/%.c
	cl $(CFLAGS) -I $(INC_DIR) $< /Fo$@

run:
	$(OUT_DIR)/$(OUT).exe

define RMFUNC
	$(RM) $(1)
endef

clean:
	$(foreach x,$(wildcard $(OUT_DIR)/*.obj) $(wildcard $(OUT_DIR)/*.exe),$(call RMFUNC, $(x)))
