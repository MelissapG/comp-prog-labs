# Nome do projeto
PROJ_NAME=MPshell
 
# .c arquivos
C_SOURCE=$(wildcard *.c)
 
# .h arquivos
H_SOURCE=$(wildcard *.h)
 
# .o arquivos
OBJ=$(C_SOURCE:.c=.o)
 
# Compilador
CC=gcc
 
# Flags pro compilador
CC_FLAGS=-c         \
#
# Compilador e linkador
#
all: $(PROJ_NAME)
 
$(PROJ_NAME): $(OBJ)
	$(CC) -o  $@ $^ 
 
%.o: %.c %.h
	$(CC) -o $@ $< $(CC_FLAGS) 
 
shell.o: shell.c $(H_SOURCE)
	$(CC) -o $@ $< $(CC_FLAGS) 
 
clean:
	rm -rf *.o $(PROJ_NAME) *~