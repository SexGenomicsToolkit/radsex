DIRS = radseq_bootstrap
BUILDDIRS = $(DIRS:%=build-%)
CLEANDIRS = $(DIRS:%=clean-%)
INITDIRS = $(DIRS:%=init-%)

all: update $(BUILDDIRS)
$(BUILDDIRS):
	$(MAKE) -C ./include/$(@:build-%=%)


clean: $(CLEANDIRS)
$(CLEANDIRS):
	$(MAKE) -C ./include/$(@:clean-%=%) clean


init: makedirs $(INITDIRS)
$(INITDIRS):
	$(MAKE) -C ./include/$(@:init-%=%) init


update:
	git submodule foreach git pull origin master
