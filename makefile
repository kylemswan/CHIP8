# compiler and flags
CC := gcc
CCFLAGS := -Iinclude/ -g -MMD -Wall
LDFLAGS := -lSDL2

# link objects into the executable
CHIP8: build/main.o build/CHIP8.o build/display.o
	@ echo "linking into $@ ..."
	@ $(CC) $^ -o $@ $(LDFLAGS)
	@ echo "build complete!"

# compile source files and create dependecy (.d) files
build/%.o: source/%.c | build/
	@ echo "compiling $< ..."
	@ $(CC) $(CCFLAGS) -c $< -o $@

# create the build directory if it does not already exist
build/:
	@ mkdir $@

# include dependecy rules for subeseqeuent builds
include $(wildcard build/*.d)

# remove build files
clean:
	@ echo "removing build files ..."
	@ rm -rf build/

# remove the executable
remove:
	@ echo "removing executable ..."
	@ rm CHIP8
