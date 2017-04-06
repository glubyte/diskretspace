CC = gcc

# INCLUDE_PATHS
INCLUDE_PATHS = -IC:\mingw_dev\SDL2\include\SDL2 -IC:\VulkanSDK\1.0.42.1\Include

# LIBRARY_PATHS
LIBRARY_PATHS = -LC:\mingw_dev\SDL2\lib -LC:\VulkanSDK\1.0.42.1\Lib -LC:\mingw_dev\windowslibs

# COMPILER FLAGS
# -w; suppresses all warnings
# -Wl, -subsystem,windows; gets rid of the console window

# LINKER_FLAGS
LINKER_FLAGS = -m64 -lmingw32 -lSDL2main -lSDL2 -lvulkan-1 -lkernel32 -ladvapi32 -lcomdlg32 -lgdi32 -lodbc32 -lodbccp32 -lole32 -loleaut32 -lshell32 -luser32 -luuid -lwinspool


OBJ_NAME = diskretspace

objects = main.o vksdl.o physik.o shaderpipes.o r3.o planets.o

all : $(objects)
	gcc $(objects) $(OBJ_NAME).res $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS) -o winbuild\$(OBJ_NAME)

main.o : vksdl.h physik.h shaderpipes.h r3.h
	gcc -c main.c $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS) 
vksdl.o : vksdl.h shaderpipes.h
	gcc -c vksdl.c $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS) 
physik.o : physik.h
	gcc -c physik.c $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS)
shaderpipes.o : shaderpipes.h vksdl.h
	gcc -c shaderpipes.c $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS)
r3.o : r3.h vksdl.h
	gcc -c r3.c $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS)
planets.o : planets.h
	gcc -c planets.c $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS)
clean :
	rm $(objects)
