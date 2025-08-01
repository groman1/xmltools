example:
	gcc src/example.c src/xmltools.c -o example
example-debug:
	gcc src/example.c src/xmltools.c -g -o example-g
shared:
	gcc -c src/xmltools.c -Wall -Werror -fPIC
	gcc -shared -o xmltools.so xmltools.o
	rm xmltools.o
shared-nochecks:
	gcc -c src/xmltools.c -D NOCHECKS -Wall -Werror -fPIC
	gcc -shared -o xmltools.so xmltools.o
	rm xmltools.o
install:
	sudo mv xmltools.so /usr/lib/
