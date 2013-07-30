CCACHE=
CXX=clang++
SRC=main.cpp
LIBS=-L/usr/local/include -lopencv_highgui -lopencv_core -lopencv_features2d -lopencv_nonfree -D_GNU_SOURCE
OUTPUT=cv
CLEAN=${OUTPUT} *.jpg

all: cv

cv:
	${CCACHE} ${CXX} -O3 ${LIBS} ${SRC} -o ${OUTPUT}

debug:
	${CCACHE} ${CXX} -g -ggdb -O0 ${LIBS} ${SRC} -o ${OUTPUT}

clean:
	rm -rf ${CLEAN}

.PHONY: cv debug clean

