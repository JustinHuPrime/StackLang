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
INCLUDES := -I.

#list of folders to include code from. Code from base dir is automatically included
MODULES := utils language ui
#grabs source code files
SRCS := $(foreach MODULE,$(MODULES),$(shell find -O3 $(MODULE)/ -type f -name '*.cpp')) $(wildcard *.cpp)
#name of directory to put .o files in
OBJDIR := bin
#path names of .o files - preserves folder structure of source files.
OBJS := $(addprefix $(OBJDIR)/,$(SRCS:.cpp=.o))
#location of dependencies
DEPDIR := dependencies
#dependency list
DEPS := $(addprefix $(DEPDIR)/,$(SRCS:.cpp=.dep))
#final executable name
EXENAME := stacklang

.PHONY: debug release clean format
.SECONDEXPANSION:

debug: OPTIONS := $(OPTIONS) $(DEBUGOPTIONS)
debug: $(EXENAME)

release: OPTIONS := $(OPTIONS) $(RELEASEOPTIONS)
release: $(EXENAME) | clean

$(EXENAME): $(OBJS)
	@$(CC) -o $(EXENAME) $(OPTIONS) $(OBJS) $(LIBS)

$(OBJS): $$(patsubst $(OBJDIR)/%.o,%.cpp,$$@) | $$(dir $$@)
	@clang-format -i $<
	@$(CC) $(OPTIONS) $(INCLUDES) -c $< -o $@

%.h:
	@clang-format -i $@

%/:
	@$(MKDIR) $@

clean:
	@$(RM) $(OBJDIR) $(DEPDIR) $(EXENAME)

$(DEPS): $$(patsubst $(DEPDIR)/%.dep,%.cpp,$$@) | $$(dir $$@)
	@set -e; $(RM) $@; \
	 $(CC) $(OPTIONS) $(INCLUDES) -MM $< > $@.$$$$; \
	 sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	 rm -f $@.$$$$

-include $(DEPS)