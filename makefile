CC := g++
RM := rm -rf
MKDIR := mkdir
OPTIONS := -std=c++17 -Wall -Wextra -g# -O3
LDFLAGS := -lncursesw
INCLUDES := -I.
MODULES := utils language ui
SRCS := $(wildcard $(addsuffix /*.cpp, $(MODULES)) $(addsuffix /*/*.cpp, $(MODULES))) main.cpp
OBJS := $(addprefix bin/, $(notdir $(SRCS:.cpp=.o)))
EXENAME := stacklang
.PHONY: all clean run remake rerun bin

all: $(EXENAME)

$(EXENAME): $(OBJS)
	$(CC) -o $(EXENAME) $(OPTIONS) $(OBJS) $(LDFLAGS)

bin/main.o: main.cpp | bin
	$(CC) $(OPTIONS) $(INCLUDES) -c $< -o $@

bin/%.o: */%.cpp */%.h | bin
	$(CC) $(OPTIONS) $(INCLUDES) -c $< -o $@

bin/%.o: */*/%.cpp */*/%.h | bin
	$(CC) $(OPTIONS) $(INCLUDES) -c $< -o $@

run: | all
	./$(EXENAME)

clean:
	$(RM) bin $(EXENAME)

remake: | clean all

rerun: | remake run

bin:
	$(MKDIR) bin