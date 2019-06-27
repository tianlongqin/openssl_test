sinclude $(TOPDIR)/rules.mk

TODO		+= cipher

IDIR		+= ./ ../include

CFLAGS		+= $(addprefix -I, $(IDIR))

LIBS		:= crypto
LDIR		:= $(TOPDIR)/libs

LDFLAGS		:= $(addprefix -L, $(LDIR))
LDFLAGS		+= $(addprefix -l, $(LIBS))
LDFLAGS		+= -Wl,-rpath=$(LDIR)

TOOLS_SOUREC	+= $(wildcard $(TOOLS_DIR)/*.c)
TOOLS_OBJ	+= $(patsubst %.c, %.o, $(TOOLS_SOUREC))
.PHONY: all

all: $(TODO)

cipher: cipher.o $(TOOLS_OBJ)
	$(call cmd_link_exec_target)
clean:
	@$(RM) $(TODO) *.o $(TOOLS_DIR)/*.o
