# Compilation macros
 CC = gcc
 CFLAGS = -ansi -Wall -pedantic -g # Flags
 GLOBAL_DEPS = globals.h # Dependencies for everything
 EXE_DEPS = assembler.o  util.o hashmap.o preAssembler.o firstPassScript.o secondPassScript.o dynamicArray.o machinecodeline.o errors.o # Deps for exe

 ## Executable
%.o: ./SourceFiles/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

assembler: $(EXE_DEPS)
	$(CC) -g $(EXE_DEPS) $(CFLAGS) -o $@


all: assembler

clean:
	rm -rf *.o *.am *.ob *.ent *.ext
