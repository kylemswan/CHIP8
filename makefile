# EXECUTABLE NAME
EXE := CHIP8

# DIRECTORY NAMES
SRCDIR := source
INCDIR := include
BLDDIR := build

# COMPILER AND FLAGS
CC := gcc
CCFLAGS := -I$(INCDIR) -g

# LIBRARIES TO LINK WITH
LIBS := -lSDL2

# SOURCE FILES AND (INFERRED) OBJECT FILES
SRCS := $(wildcard $(SRCDIR)/*.c)
OBJS := $(addprefix $(BLDDIR)/, $(addsuffix .o, $(basename $(notdir $(SRCS)))))

# LINK OBJECTS INTO THE EXECUTABLE
$(EXE): $(OBJS)
	@ $(CC) $^ -o $@ $(LIBS)
	@ echo "linking into $@ ..."
	@ echo "build complete!"

# COMPILE SOURCE FILES AND CREATE DEPENDENCY FILES
$(BLDDIR)/%.o: $(SRCDIR)/%.c | $(BLDDIR)
	@ echo "compiling $< ..."
	@ $(CC) $(CCFLAGS) -MMD -c $< -o $@

# CREATE THE BUILD DIRECTORY IF IT DOES NOT ALREADY EXIST
$(BLDDIR):
	@ mkdir $@

# INCLUDE DEPENDENCY RULES FOR SUBSEQUENT BUILDS
include $(wildcard $(BLDDIR)/*.d)

# CLEAN UP BUILD FILES
clean:
	@ echo "removing build files ..."
	@ rm -r $(BLDDIR)

# REMOVE THE EXECUTABLE
remove:
	@ echo "removing executable ..."
	@ rm $(EXE)
