SPECSDIR=specification
SPECIFICATION_SOURCES = $(wildcard $(SPECSDIR)/*.tex)

.PHONY: spec
#spec: SPECIFICATION_SOURCES
spec: $(SPECSDIR)/SpecTemplate.pdf

$(SPECSDIR)/SpecTemplate.pdf: $(SPECIFICATION_SOURCES)
	cd $(SPECSDIR) ; pdflatex --shell-escape SpecTemplate.tex

.PHONY: clean
clean:
	rm $(SPECSDIR)/SpecTemplate.pdf

