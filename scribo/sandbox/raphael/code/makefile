speed:
	g++ test.cc -o ~/Bureau/test/bin/test.elf -I . -I ./../../../../milena -O3 -fwhole-program
	~/Bureau/test/bin/test.elf -D ~/Bureau/test/bin ima.pbm
std:
	
	g++ test.cc -o ~/Bureau/test/bin/test.elf -I . -I ./../../../../milena
	~/Bureau/test/bin/test.elf -D ~/Bureau/test/bin ima.pbm
release:
	g++ test.cc -o ~/Bureau/test/bin/test.elf -I . -I ./../../../../milena -DNDEBUG -O2
	~/Bureau/test/bin/test.elf -D ~/Bureau/test/bin ima.pbm
clean:
	rm -r -f ../bin/*

