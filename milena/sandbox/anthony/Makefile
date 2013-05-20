CCACHE=ccache
CC=g++
CFLAGS=-Wall -Werror
CLIBS=-I../../
CLEAN=*.o output/* log

SRC=src/scale_space.cc
OUTPUT=a.out

all: scale

scale:
	$(CCACHE) $(CC) $(CFLAGS) -O2 -DNDEBUG $(CLIBS) $(SRC) -o $(OUTPUT)

debug:
	$(CCACHE) $(CC) $(CFLAGS) -g -ggdb $(CLIBS) $(SRC) -o $(OUTPUT)

clean:
	rm -rf $(CLEAN)

mrproper: clean
	rm -f $(OUTPUT)

.PHONY: scale clean mrproper
