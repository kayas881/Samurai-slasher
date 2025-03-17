#OBJS specifies which files to compile as part of the project
OBJS = main.o src/renderWindow.o src/Entity.o src/Player.o src/npc.o src/DialogueSystem.o src/TextureManager.o src/EventManager.o src/utils.o src/SpatialPartitioning.o src/Enemy.o

#CC specifies which compiler we're using
CC = g++

#INCLUDE_PATHS specifies the additional include paths we'll need
INCLUDE_PATHS = -IC:\mingw_dev_lib\include\SDL2

#LIBRARY_PATHS specifies the additional library paths we'll need
LIBRARY_PATHS = -LC:\mingw_dev_lib\lib

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
# -Wl,-subsystem,windows gets rid of the console window
#COMPILER_FLAGS = -w -Wl,-subsystem,windows
COMPILER_FLAGS = -w

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image  -lSDL2_ttf

#OBJ_NAME specifies the name of our executable
OBJ_NAME = main

#Pattern rule to compile .cpp files to .o files
%.o: %.cpp
	$(CC) -c $< $(INCLUDE_PATHS) $(COMPILER_FLAGS) -o $@

#This is the target that compiles our executable
all: $(OBJS)
	$(CC) $(OBJS) $(LIBRARY_PATHS) $(LINKER_FLAGS) -o $(OBJ_NAME)
	./$(OBJ_NAME).exe

#Clean up build files
clean:
	rm -f $(OBJS) $(OBJ_NAME).exe