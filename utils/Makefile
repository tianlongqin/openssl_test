sinclude $(TOPDIR)/rules.mk

TODO		+= cipher digest cert bio

IDIR		+= ./ ../include 

DEFS		+= CONFIG_DEBUG
CFLAGS		+= $(addprefix -I, $(IDIR))
CFLAGS		+= $(addprefix -D, $(DEFS))
CFLAGS		+= -g3

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
digest: digest.o $(TOOLS_OBJ)
	$(call cmd_link_exec_target)
cert: cert.o $(TOOLS_OBJ)
	$(call cmd_link_exec_target)
bio: bio_test.o $(TOOLS_OBJ)
	$(call cmd_link_exec_target)
clean:
	@$(RM) $(TODO) *.o $(TOOLS_DIR)/*.o
