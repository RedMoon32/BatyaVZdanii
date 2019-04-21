EXE = netpbm-sobel
SRC_DIR = src
OBJ_DIR = build
CC = gcc

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

CLIBS = -lm -lpthread

.PHONY: all clean

all: $(OBJ_DIR)/$(EXE)

$(OBJ_DIR)/$(EXE): $(OBJ)
	$(CC) $^ $(CLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CLIBS) -c $< -o $@

clean:
	$(RM) $(OBJ
