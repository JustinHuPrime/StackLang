CC := g++
RM := rm -rf
CD := cd
OPTIONS := -std=c++17 -Wall -Wextra# -O3
LDFLAGS := -lncursesw
INCLUDES := -I.
SRCS := $(wildcard *.cpp) $(wildcard stackElements/*.cpp)
OBJS := $(addprefix bin/, $(notdir $(SRCS:.cpp=.o)))
EXENAME := stacklang
.PHONY: all clean run remake rerun

all: $(EXENAME)

$(EXENAME): $(OBJS)
	$(CC) -o $(EXENAME) $(OPTIONS) $(OBJS) $(LDFLAGS)

bin/main.o: main.cpp
	$(CC) $(OPTIONS) $(INCLUDES) -c $< -o $@

bin/%.o: %.cpp %.h
	$(CC) $(OPTIONS) $(INCLUDES) -c $< -o $@

bin/%.o: stackElements/%.cpp stackElements/%.h
	$(CC) $(OPTIONS) $(INCLUDES) -c $< -o $@

run: | all
	./$(EXENAME)

clean:
	$(RM) bin/*.o $(EXENAME)

remake: | clean all

rerun: | remake run