.PHONY: all wii gc wii-clean gc-clean wii-run gc-run wii-pgo-generate wii-pgo-optimize gc-pgo-generate gc-pgo-optimize pgo-clean

all: wii gc

run: wii-run

clean: wii-clean gc-clean

wii:
	$(MAKE) -f Makefile.wii

wii-clean:
	$(MAKE) -f Makefile.wii clean

wii-run: wii
	$(MAKE) -f Makefile.wii run

gc:
	$(MAKE) -f Makefile.gc

gc-clean:
	$(MAKE) -f Makefile.gc clean

gc-run: gc
	$(MAKE) -f Makefile.gc run

# PGO targets
wii-pgo-generate:
	$(MAKE) -f Makefile.wii pgo-generate

wii-pgo-optimize:
	$(MAKE) -f Makefile.wii pgo-optimize

gc-pgo-generate:
	$(MAKE) -f Makefile.gc pgo-generate

gc-pgo-optimize:
	$(MAKE) -f Makefile.gc pgo-optimize

pgo-clean:
	$(MAKE) -f Makefile.wii pgo-clean
	$(MAKE) -f Makefile.gc pgo-clean
