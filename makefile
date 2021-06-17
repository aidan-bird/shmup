SRC_PATH=src
OBJ_PATH=.obj
SRC=$(wildcard $(SRC_PATH)/*.c)
OBJ=$(patsubst $(SRC_PATH)/%,$(OBJ_PATH)/%,$(SRC:.c=.o))
INC= -I/usr/include/SDL2 -lm

LDFLAGS = -D_REENTRANT -L/usr/lib -lSDL2 -lSDL2_image -lm 
CFLAGS = -ggdb3 -O0 -Wall -fstrict-aliasing -std=c99

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c
	$(CC) -c $(INC) -o $@ $< $(CFLAGS) 

debug: $(OBJ)
	$(CC) -o shmup_debug $^ $(LDFLAGS)

release: $(OBJ)
	$(CC) -o shmup_release $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -rf $(OBJ)
	rm -f shmup_debug shmup_release
