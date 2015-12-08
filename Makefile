PREFIX?=$(PWD)/build
DIRS=frdm-k64f

.PHONY: all  $(DIRS)

all: $(patsubst %, _all_%, $(DIRS))
		@mkdir -p $(PREFIX)/lib
		@cp -r ./*.o  $(PREFIX)/lib
		@rm -rf ./*.o
clean: $(patsubst %, _clean_%, $(DIRS))


$(patsubst %, _all_%, $(DIRS)):
	make -C $(patsubst _all_%, %, $@)

$(patsubst %, _clean_%, $(DIRS)):
	make -C $(patsubst _clean_%, %, $@) clean
