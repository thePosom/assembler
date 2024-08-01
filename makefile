# Compilation macros
 CC = gcc
 CFLAGS = -ansi -Wall -pedantic -g # Flags
 GLOBAL_DEPS = globals.h # Dependencies for everything
 EXE_DEPS = assembler.o  util.o hashmap.o preAssembler.o firstPassScript.o dynamicArray.o machinecodeline.o # Deps for exe

 ## Executable
%.o: ./SourceFiles/%.c $(DEPS)
#	$(CC) -c assembler.c $(CFLAGS) -o $@ $<
	$(CC) -c -o $@ $< $(CFLAGS)

assembler: $(EXE_DEPS)
	$(CC) -g $(EXE_DEPS) $(CFLAGS) -o $@


all: assembler
# assembler.o:  ../SourceFiles/* $(GLOBAL_DEPS)
# 	$(CC) -c assembler.c $(CFLAGS) -o $@

# preAssembler.o: ../SourceFiles/* ./HeaderFiles $(GLOBAL_DEPS)
# 	$(CC) -c preAssembler.c $(CFLAGS) -o $@

# hashmap.o: ../SourceFiles/* ./HeaderFiles $(GLOBAL_DEPS)
# 	$(CC) -c hashmap.c $(CFLAGS) -o $@

# util.o: ../SourceFiles/* ./HeaderFiles $(GLOBAL_DEPS)
# 	$(CC) -c util.c $(CFLAGS) -o $@

clean:
	rm -rf *.o *.am *.ob *.ent *.ext
