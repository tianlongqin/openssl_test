MAKEFLAGS += --no-print-directory

TOPDIR		:= $(shell pwd)
CC		?= $(CROSS_COMPILE)gcc
STRIP		:= $(CROSS_COMPILE)strip
AR		:= $(CROSS_COMPILE)ar
LD		:= $(CROSS_COMPILE)ld

TOOLS_DIR	:= $(TOPDIR)/tools

export TOPDIR CC STRIP AR LD TOOLS_DIR
obj-y		= utils/

PHONY   := all
all:
	$(foreach DIR, $(obj-y), $(MAKE) -C $(DIR) $@ ;)

PHONY   += clean
clean:
	$(foreach DIR, $(obj-y), $(MAKE) -C $(DIR) $@ ;)
