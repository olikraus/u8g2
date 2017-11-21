TOPTARGETS := all clean

NOWANTS=common/. text_kerning/.
SUBDIRS := $(filter-out $(NOWANTS), $(wildcard */.))

$(TOPTARGETS): $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

.PHONY: $(TOPTARGETS) $(SUBDIRS)
