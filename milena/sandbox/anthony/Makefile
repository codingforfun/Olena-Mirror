CCACHE=ccache
CC=g++
CFLAGS=-Wall -Werror
CLIBS=-I../../
CLEAN=*.o output/* log

SRC=src/main.cc src/matrix.cc src/keypoint.cc
OUTPUT=a.out

all: scale

scale: clean
	$(CCACHE) $(CC) $(CFLAGS) -O3 -DNDEBUG $(CLIBS) $(SRC) -o $(OUTPUT)

debug: mrproper
	$(CCACHE) $(CC) $(CFLAGS) -g -ggdb $(CLIBS) $(SRC) -o $(OUTPUT)

clean:
	rm -rf $(CLEAN)

mrproper: clean
	rm -f $(OUTPUT)

.PHONY: scale clean mrproper
