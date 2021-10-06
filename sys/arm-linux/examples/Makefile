PI=pi@raspberrypi.local

SUBDIRS			= $(wildcard *examples)
TARGET			= subdirs
PHONY			+= $(TARGET) $(SUBDIRS) %.clean
OUTDIR          = bin
all: $(TARGET)

subdirs: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

%.clean:
	@(cd $(patsubst %.clean, %, $@) && $(MAKE) clean)

clean distclean: $(patsubst %, %.clean, $(SUBDIRS))

.PHONY: $(PHONY) install clean distclean

upload:
	scp $(OUTDIR)/* $(PI):~/
