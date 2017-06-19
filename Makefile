build:
	gcc -std=c9x cozi_generale.c -o cozi_generale
clean:
	rm cozi_generale test_*
run:	
	./cozi_generale
runValgrind:
	valgrind ./cozi_generale
