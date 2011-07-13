VIEWER = -DVIEWER
QT4 = -I/usr/share/qt4/mkspecs/linux-g++ -I. -I/usr/include/qt4/QtUiTools -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtXml -I/usr/include/qt4
QT4_LIB = -L/usr/lib -lQtXml -lQtGui -lQtCore -lQtUiTools -lpthread 

speed:
	ccache g++ test.cc -o ~/Bureau/test/bin/test.elf -I . -I ./../../../../milena -DNDEBUG -DNDEMO -O3 -fwhole-program -march=native 
	~/Bureau/test/bin/test.elf -D ~/Bureau/test/bin -P ~/Bureau/test/bin/script ima.pbm
std:
	ccache g++ test.cc -o ~/Bureau/test/bin/test.elf -I . -I ./../../../../milena
	~/Bureau/test/bin/test.elf -D ~/Bureau/test/bin  -P ~/Bureau/test/bin/script ima.pbm
release:
	ccache g++ test.cc -msse3 -o ~/Bureau/test/bin/test.elf -I . -I ./../../../../milena $(QT4) $(QT4_LIB) -DNDEBUG -DNDEMO -O2 -march=native 
	~/Bureau/test/bin/test.elf -D ~/Bureau/test/bin -P ~/Bureau/test/bin/script ima.pbm
release_ndebug:
	ccache g++ test.cc -msse3 -o ~/Bureau/test/bin/test.elf -I . -I ./../../../../milena $(QT4) $(QT4_LIB) -DNDEBUG -DNDEMO -DNDEBUGGRAPH -O2 -march=native 
	~/Bureau/test/bin/test.elf -D ~/Bureau/test/bin -P ~/Bureau/test/bin/script ima.pbm
dem:
	ccache g++ test.cc -msse3 -o ~/Bureau/test/bin/test.elf -I . -I ./../../../../milena $(QT4) $(QT4_LIB) -DNDEBUG -O2 -march=native 
	~/Bureau/test/bin/test.elf -D ~/Bureau/test/bin -P ~/Bureau/test/bin/script ima.pbm
debug:
	ccache g++ test.cc  -o ~/Bureau/test/bin/test.elf -I . -I ./../../../../milena -DNDEBUG -ggdb
	~/Bureau/test/bin/test.elf -D ~/Bureau/test/bin -P ~/Bureau/test/bin/script ima.pbm
test:
	~/Bureau/test/bin/test.elf -D ~/Bureau/test/bin -P ~/Bureau/test/bin/script ima.pbm
clean:
	rm -r -f ../bin/*
