CCACHE=ccache
CC=g++
CFLAGS=-Wall -Wextra
CLIBS=-I../../
CLEAN=*.o output/* log

SRC=src/main.cc src/matrix.cc
OUTPUT=a.out

all: sift

sift: clean
	$(CCACHE) $(CC) $(CFLAGS) -O3 -DNDEBUG $(CLIBS) $(SRC) -o $(OUTPUT)

debug: mrproper
	$(CCACHE) $(CC) $(CFLAGS) -g -ggdb $(CLIBS) $(SRC) -o $(OUTPUT)

clean:
	rm -rf $(CLEAN)

mrproper: clean
	rm -f $(OUTPUT)

.PHONY: sift clean mrproper
