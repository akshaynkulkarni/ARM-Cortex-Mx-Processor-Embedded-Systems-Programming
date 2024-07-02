CC:= arm-none-eabi-gcc

COMMON_FLAGS := -mcpu=cortex-m4 -mthumb -g -O0 -std=gnu11 -Wall
CFLAGS_NOSTD := -nostdlib $(COMMON_FLAGS)
CFLAGS_NANO  := --specs=nano.specs $(COMMON_FLAGS)
CFLAGS_SEMI  := --specs=rdimon.specs -lc -lrdimon $(COMMON_FLAGS)


LFLAGS_COMMON := -T linker.ld -Wl,-Map=output.map
LFLAGS_NOSTD  := $(LFLAGS_COMMON) $(CFLAGS_NOSTD)
LFLAGS_NANOLIBC := $(LFLAGS_COMMON) $(CFLAGS_NANO)
LFLAGS_SEMIHOSTING := $(LFLAGS_COMMON) $(CFLAGS_SEMI)

OUTPUT_FILE := binary_semi #default -> semihosting

ifeq ($(OPT),1) #Nanolib
	CFLAGS := -c $(CFLAGS_NANO) -D ENABLE_C_LIB=1
	LFLAGS := $(LFLAGS_NANOLIBC) -D ENABLE_C_LIB=1
	INPUT_FILES := main.o startup.o faults.o syscalls.o
	OUTPUT_FILE := binary_nanolib
else ifeq ($(OPT),2) #No std libc
	CFLAGS := -c $(CFLAGS_NOSTD)
	LFLAGS := $(LFLAGS_NOSTD)
	INPUT_FILES := main.o startup.o
	OUTPUT_FILE := binary
else #Semihosting
	CFLAGS := -c $(CFLAGS_SEMI) -D ENABLE_C_LIB=1
	LFLAGS := $(LFLAGS_SEMIHOSTING) -D ENABLE_C_LIB=1
	INPUT_FILES := main.o startup.o faults.o
	OUTPUT_FILE := binary_semi
endif

all: clean $(OUTPUT_FILE).elf 


$(OUTPUT_FILE).elf: $(INPUT_FILES)
	$(CC) $(LFLAGS)  $^ -o $@

main.o: main.c
	$(CC) $(CFLAGS) $^ -o $@

faults.o: faults.c
	$(CC) $(CFLAGS) $^ -o $@

startup.o: startup.c
	$(CC) $(CFLAGS) $^ -o $@

syscalls.o: syscalls.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -rf *.o *.elf *.map

help:
	@echo "make OPT 	- compile with semihosting"
	@echo "make OPT=1	- compile with c std nano lib"
	@echo "make OPT=2	- compile with **NO** std lib"
