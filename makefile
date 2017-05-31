CC = gcc

# INCLUDE_PATHS
INCLUDE_PATHS = -IC:\SDL2\include\SDL2 -IC:\VulkanSDK\1.0.49.0\Include

# LIBRARY_PATHS
LIBRARY_PATHS = -LC:\SDL2\lib -LC:\VulkanSDK\1.0.49.0\Lib 
# -LC:\mingw_dev\windowslibs

# COMPILER FLAGS
# -w; suppresses all warnings
# -Wl, -subsystem,windows; gets rid of the console window

# LINKER_FLAGS
LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lvulkan-1 
# -lkernel32 -ladvapi32 -lcomdlg32 -lgdi32 -lodbc32 -lodbccp32 -lole32 -loleaut32 -lshell32 -luser32 -luuid -lwinspool


OBJ_NAME = diskretspace

objects = main.o vk.o sdl.o planets.o stars.o maths.o gen.o camera.o fps.o

all : $(objects)
	gcc $(objects) $(OBJ_NAME).res $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS) -o winbuild\$(OBJ_NAME)

main.o : vk.h sdl.h planets.h gen.h fps.h
	gcc -c main.c $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS) 

vk.o : vk.h
	gcc -c vk.c $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS)

sdl.o: sdl.h
	gcc -c sdl.c $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS)

planets.o : planets.h
	gcc -c planets.c $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS)

stars.o : stars.h
	gcc -c stars.c $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS)

maths.o : maths.h
	gcc -c maths.c $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS)

gen.o : gen.h maths.h
	gcc -c gen.c $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS)

camera.o : camera.h maths.h
	gcc -c camera.c $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS)

fps.o : fps.h
	gcc -c fps.c $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS)

clean :
	rm $(objects)
