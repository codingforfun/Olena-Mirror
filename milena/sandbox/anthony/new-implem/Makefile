CCACHE=
CC=g++
CFLAGS=-Wall -Wextra
CLIBS=-I../../../
CLEAN=*.o output/* log debug_*

SRC=src/main.cc src/matrix.cc
OUTPUT=new-implem

all: new-implem

new-implem: clean
	$(CCACHE) $(CC) $(CFLAGS) -O3 -DNDEBUG $(CLIBS) $(SRC) -o $(OUTPUT)

debug: mrproper
	$(CCACHE) $(CC) $(CFLAGS) -g -ggdb $(CLIBS) $(SRC) -o $(OUTPUT)

clean:
	rm -rf $(CLEAN)

mrproper: clean
	rm -f $(OUTPUT)

.PHONY: new-implem clean mrproper
