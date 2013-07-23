.PHONY: cv

CCACHE=
CXX=g++
SRC=main.cpp
LIBS=-L/opt/local/include -lopencv_highgui -lopencv_core -lopencv_features2d -lopencv_nonfree -D_GNU_SOURCE
OUTPUT=cv
CLEAN=${OUTPUT}

all: cv

cv:
	${CCACHE} ${CXX} -O2 ${LIBS} ${SRC} -o ${OUTPUT}

debug:
	${CCACHE} ${CXX} -g -ggdb -O0 ${LIBS} ${SRC} -o ${OUTPUT}

clean:
	rm -rf ${CLEAN}
