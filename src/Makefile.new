# $Id $

# Makefile for Rom24. Works fine on my Debian system.
# You may need to use 'gmake' on BSD systems.

CC = gcc
RM = rm
EXE = rom
PROF = -g

C_FLAGS = -Wall $(PROF) -DQMFIXES
L_FLAGS = $(PROF) -lcrypt

#IMC2 - Comment out to disable IMC2 support
IMC = 1

ifdef IMC
   C_FLAGS := $(C_FLAGS) -DIMC -DIMCROM
endif

# Source Files
SRC_FILES := $(wildcard *.c)

# Object Files
OBJ_DIR = obj
OBJ_FILES := $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRC_FILES))

rom: $(OBJ_FILES)
	$(RM) -f $(EXE)
	$(CC) $(L_FLAGS) -o $(EXE) $(OBJ_FILES) -lcrypt -lm -L/usr/lib/mysql -lmysqlclient

$(OBJ_DIR)/%.o: %.c
	$(CC) $(C_FLAGS) -c -o $@ $<

clean:
	$(RM) -f $(OBJ_FILES) $(EXE) *~ *.bak *.orig *.rej *.~c *.~h *.c~ *.h~

distro:
	make clean
	cd ../..
	$(RM) -f log/*.log
	tar zvcf quickmud-`date -I`.tar.gz QuickMUD
