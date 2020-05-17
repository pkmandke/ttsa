CC=g++
CPPFLAGS=-std=c++11 -O3

ODIR=../obj
PROG=ttsa
# OBJS=$(ODIR)/main.o $(ODIR)/core.o $(ODIR)/ttsa_utils.o $(ODIR)/nbd_utils.o $(ODIR)/utils.o

OBJFILES=main.o core.o ttsa_utils.o nbd_utils.o utils.o
OBJS=$(patsubst %,$(ODIR)/%,$(OBJFILES))

INCL=ttsa.h

$(ODIR)/%.o: %.cpp $(INCL)
	$(CC) $(CPPFLAGS) -c -o $@ $<

$(ODIR)/$(PROG): $(OBJS)
	$(CC) $(CPPFLAGS) -o $@ $^

.PHONY: clean

clean:
	rm -f $(ODIR)/**
