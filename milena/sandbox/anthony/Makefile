CCACHE=ccache
CC=g++
CFLAGS=-Wall -Werror -O3 -DNDEBUG
CLIBS=-I../../../scribo/ -I../../
CLEAN=*.o output/* log final.xml *.pgm

SRC=src/scale_space.cc
OUTPUT=a.out

all: scale

scale:
	$(CCACHE) $(CC) $(CFLAGS) $(CLIBS) $(SRC) -o $(OUTPUT)

clean:
	rm -rf $(CLEAN)

mrproper: clean
	rm -f $(OUTPUT)

.PHONY: scale clean mrproper
