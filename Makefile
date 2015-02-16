ASM       := as
LINK      := ld
CC        := gcc
CFLAGS    += -Wall

SRC       := ./src
OBJ       := ./obj
BUILD     := ./build
TARGET    := ./build/target

all:
	# Target test files
	@mkdir -p $(OBJ) $(BUILD) $(TARGET)
	
	$(CC) $(CFLAGS) -c $(SRC)/util.c -o $(OBJ)/util.o
	$(CC) $(CFLAGS) -c $(SRC)/memory_and_registers.c -o $(OBJ)/memory_and_registers.o
	
	$(CC) $(CFLAGS) $(SRC)/target/target1.c -o $(TARGET)/target1
	$(CC) $(CFLAGS) $(SRC)/target/target2.c -o $(TARGET)/target2
	
	$(ASM) $(SRC)/target/target3.s -o $(OBJ)/target3.o
	$(LINK) $(OBJ)/target3.o -o $(TARGET)/target3
	
	$(ASM) $(SRC)/target/target4.s -o $(OBJ)/target4.o
	$(LINK) $(OBJ)/target4.o -o $(TARGET)/target4
	
	$(ASM) $(SRC)/target/target5.s -o $(OBJ)/target5.o
	$(LINK) $(OBJ)/target5.o -o $(TARGET)/target5
	
	$(CC) $(CFLAGS) $(SRC)/target/target6.c -o $(TARGET)/target6
	
	$(CC) $(CFLAGS) -shared $(SRC)/target/target7.c -o $(TARGET)/target7.so
	
	$(CC) $(CFLAGS) -ldl src/target/target8.c -o $(TARGET)/target8
	
	# Debugger blocks
	$(CC) $(CFLAGS) $(OBJ)/util.o $(SRC)/sample1_loading.c -o $(BUILD)/sample1_loading
	$(CC) $(CFLAGS) $(OBJ)/util.o $(SRC)/sample2_attach.c -o $(BUILD)/sample2_attach
	$(CC) $(CFLAGS) $(OBJ)/util.o $(SRC)/sample3_events.c -o $(BUILD)/sample3_events
	$(CC) $(CFLAGS) $(OBJ)/util.o $(SRC)/sample4_reading_registers.c -o $(BUILD)/sample4_reading_registers
	$(CC) $(CFLAGS) $(OBJ)/util.o $(SRC)/sample5_reading_fpu.c -o $(BUILD)/sample5_reading_fpu
	$(CC) $(CFLAGS) $(OBJ)/util.o $(SRC)/sample6_mmx_and_3dnow.c -o $(BUILD)/sample6_mmx_and_3dnow
	$(CC) $(CFLAGS) $(OBJ)/util.o $(SRC)/sample7_memory.c -o $(BUILD)/sample7_memory
	$(CC) $(CFLAGS) $(OBJ)/util.o $(SRC)/sample8_libraries.c -o $(BUILD)/sample8_libraries
	$(CC) $(CFLAGS) $(OBJ)/util.o $(SRC)/sample9_symbols.c -o $(BUILD)/sample9_symbols
	$(CC) $(CFLAGS) $(OBJ)/util.o $(OBJ)/memory_and_registers.o $(SRC)/sample10_breakpoint.c -o $(BUILD)/sample10_breakpoint
	

rm:
	@rm -rf $(OBJ) $(BUILD) $(TARGET)
	
