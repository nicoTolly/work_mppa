all: opal_mono opal_ppcg

opal_mono:
	$(MAKE) -C src_mono

opal_ppcg:
	$(MAKE) -C src_ppcg

.PHONY: tidy arch

tidy:
	$(MAKE) -C src_mono clean
	$(MAKE) -C src_ppcg clean
	$(RM) *.html *.xz 
#	$(RM) *.html *.xz test/*

