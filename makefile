CC := g++
RM := rm -rf
MKDIR := mkdir
OPTIONS := -std=c++17 -Wall -Wextra -g# -O3
LDFLAGS := -lncursesw
INCLUDES := -I.
MODULES := utils language ui
SRCS := $(wildcard $(addsuffix /*.cpp, $(MODULES)) $(addsuffix /*/*.cpp, $(MODULES))) main.cpp
OBJDIR := bin
OBJS := $(addprefix $(OBJDIR)/, $(notdir $(SRCS:.cpp=.o)))
EXENAME := stacklang
.PHONY: all clean run remake rerun

all: $(EXENAME)

$(EXENAME): $(OBJS)
	$(CC) -o $(EXENAME) $(OPTIONS) $(OBJS) $(LDFLAGS)

$(OBJDIR)/main.o: main.cpp | bin
	$(CC) $(OPTIONS) $(INCLUDES) -c $< -o $@

$(OBJDIR)/%.o: */%.cpp */%.h | bin
	$(CC) $(OPTIONS) $(INCLUDES) -c $< -o $@

$(OBJDIR)/%.o: */*/%.cpp */*/%.h | bin
	$(CC) $(OPTIONS) $(INCLUDES) -c $< -o $@

run: | all
	./$(EXENAME)

clean:
	$(RM) bin $(EXENAME)

remake: | clean all

rerun: | remake run

$(OBJDIR):
	$(MKDIR) $(OBJDIR)