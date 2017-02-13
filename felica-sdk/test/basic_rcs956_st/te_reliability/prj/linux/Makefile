
MAKE = make
CD = cd
LOGFILE = make.log
CP = cp

ifeq ($(DRV), UART)
	MAKE_LIST =	uart
else
ifeq ($(DRV), USB)
	MAKE_LIST =	usb
else
	MAKE_LIST =	uart \
			usb
endif
endif

all:
	for mk_target in $(MAKE_LIST) ; \
	do \
		$(CD) $$mk_target && $(MAKE) -f Makefile.tss_linux_reliability_$$mk_target 2>&1 | tee $(LOGFILE) ; \
		$(CP) tss_linux_reliability_$$mk_target .. ; \
		$(CD) .. ; \
	done

clean:
	for mk_target in $(MAKE_LIST) ; \
	do \
		$(CD) $$mk_target && $(MAKE) -f Makefile.tss_linux_reliability_$$mk_target clean ; \
		$(CD) .. ; \
		$(RM) tss_linux_reliability_$$mk_target ; \
	done
