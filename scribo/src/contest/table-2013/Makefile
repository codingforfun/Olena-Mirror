CCACHE=ccache
CC=g++
CFLAGS=-Wall -Werror -O3 -DHAVE_TESSERACT_3 -DNDEBUG -DMLN_WO_GLOBAL_VARS
CFLAGS_OLD=-Wall -Werror -O3 -DHAVE_TESSERACT_3 -DNDEBUG
CLIBS=-I../../../milena/ -I../../ -I/usr/include/poppler
CLEAN=*.o output/* log final.xml

SRC=src/xml.cc src/new.cc
SRC_OLD=src/main.cc
OUTPUT=table
OUTPUT_OLD=old

all: table

table:
	$(CCACHE) $(CC) $(CFLAGS) $(CLIBS) $(SRC) -ltesseract -lpoppler-cpp -o $(OUTPUT)

old:
	$(CCACHE) $(CC) $(CFLAGS_OLD) $(CLIBS) $(SRC_OLD) -ltesseract -lpoppler-cpp -o $(OUTPUT_OLD)

clean:
	rm -rf $(CLEAN)

mrproper: clean
	rm -f $(OUTPUT) $(OUTPUT_OLD)

.PHONY: table clean mrproper
