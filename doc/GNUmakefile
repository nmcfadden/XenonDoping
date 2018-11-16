# $Id: GNUmakefile,v 1.1.1.1 2004-11-08 17:53:42 gast Exp $
# --------------------------------------------------------------
# GNUmakefile for this module.      Akbar Mokhtarani, March-4-2004 
# --------------------------------------------------------------
#


# 
.PHONY : all clean test
#
SGMDOCS := $(wildcard MCUsersGuide/*.sgm)
HTMLDOCS := $(SGMDOCS:%.sgm=%.html)
#
NWALSH := ../MJTools/docbook/docbook-xsl-1.65.1/html/docbook.xsl
#PARSER := xsltproc $(NWALSH)
PARSER := db2html
#
###all : $(HTMLDOCS)
all : 
	$(PARSER) MCUsersGuide/udg.sgm
%.html : %.sgm
	$(PARSER) $< 
#	$(PARSER) $< > $@
# 
clean : 
	-\rm -rf *.html
	-\rm -rf *.fot
#
test :
	@echo $(SGMDOCS)
	@echo $(HTMLDOCS)
#
