CC=c99
CFLAGS=-Wall -Wfatal-errors -g -O2
LIBS += -lm
PROG=test-pn
all: $(PROG)
$(PROG): polynomes.o test-pn.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)
test-pn.o:  test-pn.c 
	$(CC) -c $(CFLAGS) $^
polynomes.o: polynomes.c
	$(CC) -c $(CFLAGS) $^
.PHONY: clean distclean
clean:
	rm -f *.o
distclean: clean
	rm -f $(PROG)
