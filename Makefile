SPECSDIR=specification
SPECIFICATION_SOURCES = $(wildcard $(SPECSDIR)/*.tex)

.PHONY: spec
#spec: SPECIFICATION_SOURCES
spec: $(SPECSDIR)/SpecTemplate.pdf

#This target quits with an error, but it still works.
$(SPECSDIR)/SpecTemplate.pdf: $(SPECIFICATION_SOURCES) 
	cd $(SPECSDIR) ; pdflatex --shell-escape SpecTemplate.tex
	clean

.PHONY: clean
clean:
	rm $(wildcard *.aux)
	rm SpecTemplate.log texput.log

