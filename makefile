CC := g++
RM := rm -rf
CD := cd
OPTIONS := -std=c++17 -Wall -Wextra# -O3
LDFLAGS := -lncursesw -L/usr/lib/x86_64-linux-gnu
INCLUDES := -I.
OBJS := main.o ui.o stackElement.o stringUtils.o lineEditor.o
SEOBJS := booleanElement.o commandElement.o numberElement.o stringElement.o substackElement.o typeElement.o
EXENAME := stacklang
.PHONY: all clean run remake rerun

all: ${OBJS} ${SEOBJS}
	${CC} -o ${EXENAME} ${OPTIONS} ${OBJS} ${SEOBJS} ${LDFLAGS}

${OBJS}: *.cpp
	${CC} ${OPTIONS} ${INCLUDES} -c $*.cpp

${SEOBJS}: stackElements/*.cpp
	${CC} ${OPTIONS} ${INCLUDES} -c stackElements/$*.cpp

run: all
	./${EXENAME}

clean:
	${RM} *.o ${EXENAME}

remake: clean all

rerun: remake run