CC := g++
RM := rm -rf
CD := cd
OPTIONS := -std=c++17 -Wall -Wextra# -O3
LDFLAGS := -lncursesw
INCLUDES := -I.
OBJS := $(addprefix bin/,main.o ui.o stackElement.o stringUtils.o lineEditor.o)
SEOBJS := $(addprefix bin/,booleanElement.o commandElement.o numberElement.o stringElement.o substackElement.o typeElement.o)
EXENAME := stacklang
.PHONY: all clean run remake rerun

all: ${EXENAME}

${EXENAME}: ${OBJS} ${SEOBJS}
	${CC} -o ${EXENAME} ${OPTIONS} ${OBJS} ${SEOBJS} ${LDFLAGS}

${OBJS}: $(notdir *.cpp *.h)
	${CC} ${OPTIONS} ${INCLUDES} -c $(notdir $*.cpp) -o $*.o

${SEOBJS}: $(addprefix stackElements/, $(notdir *.cpp *.h))
	${CC} ${OPTIONS} ${INCLUDES} -c stackElements/$(notdir $*.cpp) -o $*.o

run: | all
	./${EXENAME}

clean:
	${RM} bin/*.o ${EXENAME}

remake: | clean all

rerun: | remake run