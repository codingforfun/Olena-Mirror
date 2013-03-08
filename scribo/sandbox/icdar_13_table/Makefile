CCACHE=
CC=g++
CFLAGS=-Wall -Werror -O3
CLIBS=-I../../../milena/ -I../../
CLEAN=*.o $(OUTPUT) output/* log final.xml

SRC=src/main.cc
OUTPUT=table

all: table

table:
	$(CCACHE) $(CC) $(CFLAGS) -DHAVE_TESSERACT_3 $(CLIBS) $(SRC) -ltesseract -o $(OUTPUT)

clean:
	rm -rf $(CLEAN)

.PHONY: table
