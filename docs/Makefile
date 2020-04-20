BUILDDIR  = html
SOURCEDIR = ../src

.PHONY: all clean

all: $(BUILDDIR)

clean:
	rm -rf $(BUILDDIR)

rebuild: clean $(BUILDDIR)

$(BUILDDIR): Doxyfile $(SOURCEDIR)/*.h $(SOURCEDIR)/*.cpp
	doxygen $^
