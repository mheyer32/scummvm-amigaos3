MODULE := backends/platform/amigaos3

# change amigaos3-graphics-cgx.o to amigaos3-graphics-aga.o

MODULE_OBJS = \
	amigaos3-main.o \
	amigaos3-aga.o \
	amigaos3-graphics-cgx.o

# We don't use rules.mk but rather manually update OBJS and MODULE_DIRS.
MODULE_OBJS := $(addprefix $(MODULE)/, $(MODULE_OBJS))
OBJS := $(MODULE_OBJS) $(OBJS)
MODULE_DIRS += $(sort $(dir $(MODULE_OBJS)))

# A rule to ouput .asm files for source files, allows us to inspect
# what the compiler produced
%.asm: %.c
	$(QUIET)$(MKDIR) $(*D)/$(DEPDIR)
	$(QUIET_CC)$(CC) $(CXX_UPDATE_DEP_FLAG) $(CFLAGS) $(CPPFLAGS) -Wa,-adhln -g -c $(<) > $*.asm
%.asm: %.cpp
	$(QUIET)$(MKDIR) $(*D)/$(DEPDIR)
	$(QUIET_CXX)$(CXX) $(CXX_UPDATE_DEP_FLAG) $(CXXFLAGS) $(CPPFLAGS) -Wa,-adhln -g -c $(<) > $*.asm

