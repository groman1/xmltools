example:
	c99 src/example.c src/xmltools.c -o example -Wall -Wextra
example-debug:
	gcc src/example.c src/xmltools.c -fsanitize=address -g -o example-g
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

perftest:
	cc tests/perftest.c src/xmltools.c -o perftest -Wall -Wextra -O2
selftest:
	cc tests/selftest.c src/xmltools.c -o selftest -Wall -Wextra -O2
