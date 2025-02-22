#OBJS specifies which files to compile as part of the project
OBJS = main.cpp RenderWindow.cpp Entity.cpp Player.cpp npc.cpp DialogueSystem.cpp TextureManager.cpp EventManager.cpp utils.cpp SpatialPartitioning.cpp Enemy.cpp
 
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

#This is the target that compiles our executable and runs it
all : $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
	./$(OBJ_NAME).exe   # Automatically run the executable after build

