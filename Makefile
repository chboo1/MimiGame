OBJS = main.cpp
CC = g++
OBJ_NAME = PourMimi
COMPILER_FLAGS = -w
LINNKER_FLAGS = -framework SDL2 -framework SDL2_Mixer
all: $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINNKER_FLAGS) -o $(OBJ_NAME)
