shared:
	gcc -c src/xmltools.c -Wall -Werror -fPIC src/xmltools.c
	gcc -shared -o xmltools.so xmltools.o
	rm xmltools.o
install:
	sudo mv xmltools.so /usr/lib/
