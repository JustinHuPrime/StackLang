#command options
CC := g++
RM := rm -rf
MKDIR := mkdir -p


#File options
#list of folders to include code from.
SRCDIR := src
#grabs source code files
SRCS := $(shell find -O3 $(SRCDIR)/ -type f -name '*.cc')

#name of directory to put .o files in
OBJDIR := bin
#path names of .o files - preserves folder structure of source files.
OBJS := $(patsubst $(SRCDIR)/%.cc,$(OBJDIR)/%.o,$(SRCS))

#location of dependencies
DEPDIR := dependencies
#dependency list
DEPS := $(patsubst $(SRCDIR)/%.cc,$(DEPDIR)/%.dep,$(SRCS))


#Test file options
#where to find test sources
TSRCDIR := tests
#grabs test source files
TSRCS := $(shell find -O3 $(TSRCDIR)/ -type f -name '*.cc')

#where to put .o files for tests
TOBJDIR := tests/bin
#path names of test .o files - shouldn't have any subdirectories.
TOBJS := $(patsubst $(TSRCDIR)/%.cc,$(TOBJDIR)/%.o,$(TSRCS))

#location of dependencies
TDEPDIR := tests/dependencies
#dependency list
TDEPS := $(patsubst $(TSRCDIR)/%.cc,$(TDEPDIR)/%.dep,$(TSRCS))


#standard warning request list
WARNINGS := -pedantic -pedantic-errors -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef -Wzero-as-null-pointer-constant -Wuseless-cast -Wno-unused
#disabled warnings when compiling tetss
TNOWARN := -Wno-missing-declarations -Wno-write-strings
#always-included compiler options
OPTIONS := -std=c++17 $(WARNINGS)

#build-specific compiler options
DEBUGOPTIONS := -Og -ggdb
RELEASEOPTIONS := -Os -Wunused

#libraries and included files
LIBS := $(shell pkg-config --libs ncurses) -lgmp -lgmpxx
INCLUDES := -I$(SRCDIR)
TINCLUDES := -I$(TSRCDIR)

#final executable name
EXENAME := stacklang
TEXENAME := stacklangTest


.PHONY: debug release clean diagnose
.SECONDEXPANSION:


debug: OPTIONS := $(OPTIONS) $(DEBUGOPTIONS)
debug: $(EXENAME) $(TEXENAME)
	@echo "Done compiling debug."
	@echo ""
	@./$(TEXENAME)
	@echo "Done testing debug."
	@echo "Debug build finished."

release: OPTIONS := $(OPTIONS) $(RELEASEOPTIONS)
release: $(EXENAME) | clean
	@echo "Done with release."
	@echo ""
	@./$(TEXENAME)
	@echo "Done testing release."
	@echo "Release build finished."


clean:
	@echo "Removing $(DEPDIR)/, $(OBJDIR)/, and $(EXENAME)"
	@$(RM) $(OBJDIR) $(DEPDIR) $(EXENAME)
	@echo "Removing $(TDEPDIR)/, $(TOBJDIR)/, and $(TEXENAME)"
	@$(RM) $(TOBJDIR) $(TDEPDIR) $(TEXENAME)


$(EXENAME): $(OBJS)
	@echo "Linking..."
	@$(CC) -o $(EXENAME) $(OPTIONS) $(OBJS) $(LIBS)

$(OBJS): $$(patsubst $(OBJDIR)/%.o,$(SRCDIR)/%.cc,$$@) $$(patsubst $(OBJDIR)/%.o,$(DEPDIR)/%.dep,$$@) | $$(dir $$@)
	@echo "Compiling $@..."
	@clang-format -i $(filter-out %.dep,$^)
	@$(CC) $(OPTIONS) $(INCLUDES) -c $< -o $@

$(DEPS): $$(patsubst $(DEPDIR)/%.dep,$(SRCDIR)/%.cc,$$@) | $$(dir $$@)
	@set -e; $(RM) $@; \
	 $(CC) $(OPTIONS) $(INCLUDES) -MM -MT $(patsubst $(DEPDIR)/%.dep,$(OBJDIR)/%.o,$@) $< > $@.$$$$; \
	 sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	 rm -f $@.$$$$


$(TEXENAME): $(TOBJS) $(OBJS)
	@echo "Linking test..."
	@$(CC) -o $(TEXENAME) $(OPTIONS) $(TNOWARN) $(filter-out %main.o,$(OBJS)) $(TOBJS) $(LIBS)

$(TOBJS): $$(patsubst $(TOBJDIR)/%.o,$(TSRCDIR)/%.cc,$$@) $$(patsubst $(TOBJDIR)/%.o,$(TDEPDIR)/%.dep,$$@) | $$(dir $$@)
	@echo "Compiling $@..."
	@clang-format -i $(filter-out %.dep,$^)
	@$(CC) $(OPTIONS) $(TNOWARN) $(INCLUDES) $(TINCLUDES) -c $< -o $@

$(TDEPS): $$(patsubst $(TDEPDIR)/%.dep,$(TSRCDIR)/%.cc,$$@) | $$(dir $$@)
	@set -e; $(RM) $@; \
	 $(CC) $(OPTIONS) $(INCLUDES) $(TINCLUDES) -MM -MT $(patsubst $(TDEPDIR)/%.dep,$(TOBJDIR)/%.o,$@) $< > $@.$$$$; \
	 sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	 rm -f $@.$$$$


%/:
	@$(MKDIR) $@


-include $(DEPS) $(TDEPS)