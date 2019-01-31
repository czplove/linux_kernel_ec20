SUBDIRS := $(filter-out ./, $(shell find ./ -maxdepth 1 -type d))
PWD := $(shell pwd)

.PHONY: subdirs clean

sudbdirs:
	@for dir in $(SUBDIRS);do\
		make -C $$dir;\
	done

clean:
	@for dir in $(SUBDIRS);do\
		make -C $$dir clean;\
	done