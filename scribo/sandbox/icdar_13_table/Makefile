CCACHE=
CC=g++
CFLAGS=-Wall -Werror -O3 -DHAVE_TESSERACT_3 -DNDEBUG
CLIBS=-I../../../milena/ -I../../ -I/usr/include/poppler
CLEAN=*.o output/* log final.xml

SRC=src/main.cc
OUTPUT=table

all: table

table:
	$(CCACHE) $(CC) $(CFLAGS) $(CLIBS) $(SRC) -ltesseract -lpoppler-cpp -o $(OUTPUT)

clean:
	rm -rf $(CLEAN)

mrproper: clean
	rm -f $(OUTPUT)

.PHONY: table clean mrproper
