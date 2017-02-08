BINDIR=./bin
OBJDIR=./obj
SRCDIR=./src
INCDIR=./include

CC=clang
CFLAGS=-std=c99 -O0 -g -Wall -Werror -I$(INCDIR)
LDFLAGS=-lm -lSDL -g

SRC_FILES=$(wildcard $(SRCDIR)/*.c)
OBJ_FILES=$(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC_FILES))

EXEC=$(BINDIR)/simips

all: $(BINDIR)/simips

# Règle générique

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(INCDIR)/%.h
	$(CC) -c $(CFLAGS) $< -o $@

# Coeur du fil rouge

$(EXEC): $(OBJ_FILES)
	$(CC) $(LDFLAGS) $^ ./obj/elf_reader.o ./obj/relocation.o ./obj/framebuffer.o -o $@

$(OBJ_FILES) : | $(OBJDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)
	mkdir -p $(BINDIR)

$(OBJDIR)/simips.o: $(INCDIR)/MIPS_systeme.h $(INCDIR)/MIPS_memoire.h $(INCDIR)/MIPS_registre.h $(INCDIR)/MIPS_decode.h $(INCDIR)/MIPS_pc.h $(INCDIR)/MIPS_execution.h $(INCDIR)/SIMU_commande.h $(INCDIR)/elf_reader.h $(INCDIR)/relocation.h $(INCDIR)/framebuffer.h

.PHONY: clean

clean:
	rm -f $(EXEC)
	find $(OBJDIR) -name "*.o" -and -not -name elf_reader.o -and -not -name relocation.o -and -not -name framebuffer.o | xargs rm -f
