all: ping

include ../MCONFIG
include ../MRULES

ping:	ping.o
	$(CC) $(LDFLAGS) $^ $(LIBS) -o $@

ping.o: ../version.h pingpack.h

install: ping
	install -o root -m$(SUIDMODE) ping $(INSTALLROOT)$(ROOTBINDIR)
	install -m$(MANMODE) ping.8 $(INSTALLROOT)$(MANDIR)/man8

clean:
	rm -f *.o ping

