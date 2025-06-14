# Variáveis
CC = gcc
# Se for mingw 32bits usar:
CFLAGS = -I SDL2\i686-w64-mingw32\include
# Se for mingw 64bits usar:
# CFLAGS = -I SDL2\x86_64-w64-mingw32\include
# Se for mingw 32bits usar:
#LDFLAGS = -L SDL2\i686-w64-mingw32\lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
# Se for mingw 64bits usar:
LDFLAGS = -L SDL2\x86_64-w64-mingw32\lib -lmingw32 -lSDL2main -lSDL2_image -lSDL2
SRC = main.c
OBJ = $(SRC:.c=.o)
TARGET = programa
# Regra padrão para compilar o programa
all: $(TARGET)
$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)
%.o: %.c
	$(CC) -c $< $(CFLAGS)
# Limpeza
clean:
	del $(OBJ) $(TARGET)
.PHONY: all clean