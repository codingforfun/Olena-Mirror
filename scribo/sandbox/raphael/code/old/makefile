INC_QT4 = -I /usr/include/qt4 -I /usr/include/qt4/Qt -I /usr/include/qt4/QtCore -I /usr/include/qt4/QtGui -I shared -I viewer
speed:
	ccache g++ test.cc -o ~/Bureau/test/bin/test.elf -I . -I ./../../../../milena -O3 -fwhole-program
	~/Bureau/test/bin/test.elf -D ~/Bureau/test/bin -P ~/Bureau/test/bin/script ima.pbm
std:
	ccache g++ test.cc -o ~/Bureau/test/bin/test.elf -I . -I ./../../../../milena
	~/Bureau/test/bin/test.elf -D ~/Bureau/test/bin  -P ~/Bureau/test/bin/script ima.pbm
release: shared/src/image_viewer.o
	ccache g++ test.cc -o ~/Bureau/test/bin/test.elf -I . -I ./../../../../milena $(INC_QT4) -DNDEBUG -O2
	~/Bureau/test/bin/test.elf -D ~/Bureau/test/bin -P ~/Bureau/test/bin/script ima.pbm
debug:
	ccache g++ test.cc -o ~/Bureau/test/bin/test.elf -I . -I ./../../../../milena -DNDEBUG -ggdb
	~/Bureau/test/bin/test.elf -D ~/Bureau/test/bin -P ~/Bureau/test/bin/script ima.pbm
test:
	~/Bureau/test/bin/test.elf -D ~/Bureau/test/bin -P ~/Bureau/test/bin/script ima.pbm
clean:
	rm -r -f ../bin/*
.cc.o:
	ccache g++ $< -o $@ -I . -I ./../../../../milena $(INC_QT4) -DNDEBUG -O2

