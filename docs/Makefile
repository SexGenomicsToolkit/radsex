SPHINXOPTS        = -b html -c .
SPHINXBUILD       = sphinx-build
SOURCEDIR         = src
BUILDDIR          = html
DOXYGEN_BUILDDIR  = html/doxygen
DOXYGEN_HTMLDIR   = html/doxygen
DOXYGEN_SOURCEDIR = ../src

.PHONY: all clean

all: $(BUILDDIR) $(DOXYGEN_BUILDDIR)

clean:
	rm -rf $(BUILDDIR)
	rm -rf $(DOXYGEN_BUILDDIR)

rebuild: clean $(BUILDDIR) $(DOXYGEN_BUILDDIR)

$(BUILDDIR): $(SOURCEDIR)/*.rst
	@$(SPHINXBUILD) "$(SOURCEDIR)" "$(BUILDDIR)" $(SPHINXOPTS)

$(DOXYGEN_BUILDDIR): Doxyfile $(DOXYGEN_SOURCEDIR)/*.h $(DOXYGEN_SOURCEDIR)/*.cpp
	doxygen $^
	mv $(DOXYGEN_BUILDDIR)/html/* $(DOXYGEN_HTMLDIR)
	rm -rf $(DOXYGEN_BUILDDIR)/html
