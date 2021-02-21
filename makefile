# COMPILER AND FLAGS
CC := gcc
CCFLAGS := -Iinclude/ -g -MMD -Wall
LDFLAGS := -lSDL2

# LINK OBJECTS INTO THE EXECUTABLE
CHIP8: build/main.o build/CHIP8.o build/display.o
	@ $(CC) $^ -o $@ $(LDFLAGS)
	@ echo "linking into $@ ..."
	@ echo "build complete!"

# COMPILE SOURCE FILES AND CREATE DEPENDENCY FILES
build/%.o: source/%.c | build/
	@ echo "compiling $< ..."
	@ $(CC) $(CCFLAGS) -c $< -o $@

# CREATE THE BUILD DIRECTORY IF IT DOES NOT ALREADY EXIST
build/:
	@ mkdir $@

# INCLUDE DEPENDENCY RULES FOR SUBSEQUENT BUILDS
include $(wildcard build/*.d)

# CLEAN UP BUILD FILES
clean:
	@ echo "removing build files ..."
	@ rm -rf build/

# REMOVE THE EXECUTABLE
remove:
	@ echo "removing executable ..."
	@ rm CHIP8
