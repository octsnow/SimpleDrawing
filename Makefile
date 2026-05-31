RM = cmd.exe /C del

SRC_DIR = src
INC_DIR = includes
OUT_DIR = out

WINDOWS_LIB = C:/"Program Files (x86)"/"Windows Kits"/10/Lib/10.0.22621.0/um/x64

OUT = main

all: $(OUT).exe

$(OUT).exe:
	gcc -I$(INC_DIR) $(wildcard $(SRC_DIR)/*.c) -o $(OUT_DIR)/$@ -mwindows

cl:
	cl -I $(INC_DIR) User32.lib Gdi32.lib $(wildcard $(SRC_DIR)/*.c) -Fe$(OUT_DIR)/$(OUT).exe /link /SUBSYSTEM:CONSOLE

run:
	$(OUT_DIR)/$(OUT).exe

define RMFUNC
	$(RM) $(1)
endef

clean:
	$(foreach x,$(wildcard $(OUT_DIR)/*.obj) $(wildcard $(OUT_DIR)/*.exe),$(call RMFUNC, $(x)))
