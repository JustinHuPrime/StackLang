#command options
CC := g++
RM := rm -rf
MKDIR := mkdir -p

#standard warning request list
WARNINGS := -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef -Wzero-as-null-pointer-constant -Wuseless-cast -Wno-unused
#always-included compiler options
OPTIONS := -std=c++17 -pedantic -pedantic-errors $(WARNINGS)
#build-specific compiler options
DEBUGOPTIONS := -Og -ggdb
RELEASEOPTIONS := -Os -Wunused
#libraries and included files
LIBS := -lncurses
INCLUDES := -Isrcs

#list of folders to include code from. Code from base dir is automatically included
SRCDIR := srcs
#grabs source code files
SRCS := $(shell find -O3 $(SRCDIR)/ -type f -name '*.cpp')
#name of directory to put .o files in
OBJDIR := bin
#path names of .o files - preserves folder structure of source files.
OBJS := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))
#location of dependencies
DEPDIR := dependencies
#dependency list
DEPS := $(patsubst $(SRCDIR)/%.cpp,$(DEPDIR)/%.dep,$(SRCS))
#final executable name
EXENAME := stacklang

.PHONY: debug release clean
.SECONDEXPANSION:

debug: OPTIONS := $(OPTIONS) $(DEBUGOPTIONS)
debug: $(EXENAME)

release: OPTIONS := $(OPTIONS) $(RELEASEOPTIONS)
release: $(EXENAME) | clean

clean:
	@$(RM) $(OBJDIR) $(DEPDIR) $(EXENAME)

$(EXENAME): $(OBJS)
	@$(CC) -o $(EXENAME) $(OPTIONS) $(OBJS) $(LIBS)

$(OBJS): $$(patsubst $(OBJDIR)/%.o,$(SRCDIR)/%.cpp,$$@) | $$(dir $$@)
	@$(CC) $(OPTIONS) $(INCLUDES) -c $< -o $@

%/:
	@$(MKDIR) $@

$(DEPS): $$(patsubst $(DEPDIR)/%.dep,$(SRCDIR)/%.cpp,$$@) | $$(dir $$@)
	@set -e; $(RM) $@; \
	 $(CC) $(OPTIONS) $(INCLUDES) -MM -MT $(patsubst $(DEPDIR)/%.dep,$(OBJDIR)/%.o,$@) $< > $@.$$$$; \
	 sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	 rm -f $@.$$$$

-include $(DEPS)