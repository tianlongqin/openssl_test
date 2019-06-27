#
# Building rules
#

%.o : %.c
	@$(CC) $(CFLAGS) -c $< -o $@ && echo "   CC     $@"

#
# Rule to compile a set of .o files into one .a file
#
quiet_cmd_link_l_target = "   AR     $@"
define cmd_link_l_target
   @rm -f $@
   @$(AR) $(ARFLAGS) $@ $^
   @echo "   AR     $@"
endef

%.a :
	$(call cmd_link_l_target)

#
# Rule to combine a set of .a files into one .a file
#
ar_script	= ar.mri
define cmd_combine_static_lib
	$(if $(wildcard $@),@$(RM) $@)
	$(if $(wildcard $(ar_script)),@$(RM) $(ar_script))
	@echo "CREATE $@" > $(ar_script)
	@$(foreach lib, $(filter %.a, $^), echo "ADDLIB $(lib)" >> $(ar_script);)
	@echo "SAVE" >> $(ar_script)
	@echo "END" >> $(ar_script)
	@$(AR) -M < $(ar_script) && echo "   AR     $@"
	@$(RM) $(ar_script)
endef

#
# Rule to compile a set of .o files into one .o file
#
quiet_cmd_link_o_target = "   LD     $@"
# If the list of objects to link is empty, just create an empty built-in.o
define cmd_link_o_target
   @$(if $(strip $(obj-y)), $(LD) -r -o $@ $(filter $(obj-y), $^),
		   @rm -f $@; $(AR) rcs $@)
   @echo $(quiet_cmd_link_o_target)
endef

#
# Rule to compile a set of object files into one exec file
#
quiet_cmd_link_exec_target = "   LD     $@"
define cmd_link_exec_target
   @$(CC) -o $@ $^ $(LDFLAGS)
   @$(STRIP) $@
   @echo $(quiet_cmd_link_exec_target)
endef
