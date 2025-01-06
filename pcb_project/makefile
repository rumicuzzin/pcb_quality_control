CC=gcc
CFLAGS=-g -Wall -Wextra -Werror -Iinclude -g 
LIBS=-lm
TARGET=pcb_check 

SRC_DIR = src
OBJ_DIR = build
INC_DIR = include
# you may need to put your extra files here
_DEPS = bitmap.h component_display.h component_list.h list_and_connections.h sorting.h dfs.h free_functions.h
_OBJS = main.o bitmap.o component_display.o component_list.o list_and_connections.o sorting.o dfs.o free_functions.o
DEPS = $(patsubst %,$(INC_DIR)/%,$(_DEPS))
OBJS = $(patsubst %,$(OBJ_DIR)/%,$(_OBJS))


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LIBS) 

.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJS)
	
