include $(top_srcdir)/Makefile.tool.am

EXTRA_DIST =

#----------------------------------------------------------------------------
# Headers
#----------------------------------------------------------------------------

pkginclude_HEADERS = \
	flowcheck.h

noinst_HEADERS = \
	fc_include.h      \
	fc_translate.h    \
	trace_translate.h \
	trace_runtime.h \
	popcount.h \
	fold.h

#----------------------------------------------------------------------------
# exp-flowcheck-<platform>
#----------------------------------------------------------------------------

noinst_PROGRAMS  = exp-flowcheck-@VGCONF_ARCH_PRI@-@VGCONF_OS@
if VGCONF_HAVE_PLATFORM_SEC
noinst_PROGRAMS += exp-flowcheck-@VGCONF_ARCH_SEC@-@VGCONF_OS@
endif

EXP_FLOWCHECK_SOURCES_COMMON = \
	fc_leakcheck.c \
	fc_malloc_wrappers.c \
	fc_main.c \
	fc_translate.c \
	fc_machine.c \
	fc_errors.c \
	trace_translate.c \
	trace_runtime.c \
	popcount.c \
	fold.c

exp_flowcheck_@VGCONF_ARCH_PRI@_@VGCONF_OS@_SOURCES      = \
	$(EXP_FLOWCHECK_SOURCES_COMMON)
exp_flowcheck_@VGCONF_ARCH_PRI@_@VGCONF_OS@_CPPFLAGS     = \
	$(AM_CPPFLAGS_@VGCONF_PLATFORM_PRI_CAPS@)
exp_flowcheck_@VGCONF_ARCH_PRI@_@VGCONF_OS@_CFLAGS       = \
	$(AM_CFLAGS_@VGCONF_PLATFORM_PRI_CAPS@) -O2
exp_flowcheck_@VGCONF_ARCH_PRI@_@VGCONF_OS@_DEPENDENCIES = \
	$(TOOL_DEPENDENCIES_@VGCONF_PLATFORM_PRI_CAPS@)
exp_flowcheck_@VGCONF_ARCH_PRI@_@VGCONF_OS@_LDADD        = \
	$(TOOL_LDADD_@VGCONF_PLATFORM_PRI_CAPS@)
exp_flowcheck_@VGCONF_ARCH_PRI@_@VGCONF_OS@_LDFLAGS      = \
	$(TOOL_LDFLAGS_@VGCONF_PLATFORM_PRI_CAPS@)
exp_flowcheck_@VGCONF_ARCH_PRI@_@VGCONF_OS@_LINK = \
	$(top_builddir)/coregrind/link_tool_exe_@VGCONF_OS@ \
	@VALT_LOAD_ADDRESS_PRI@ \
	$(LINK) \
	$(exp_flowcheck_@VGCONF_ARCH_PRI@_@VGCONF_OS@_CFLAGS) \
	$(exp_flowcheck_@VGCONF_ARCH_PRI@_@VGCONF_OS@_LDFLAGS)

if VGCONF_HAVE_PLATFORM_SEC
exp_flowcheck_@VGCONF_ARCH_SEC@_@VGCONF_OS@_SOURCES      = \
	$(EXP_FLOWCHECK_SOURCES_COMMON)
exp_flowcheck_@VGCONF_ARCH_SEC@_@VGCONF_OS@_CPPFLAGS     = \
	$(AM_CPPFLAGS_@VGCONF_PLATFORM_SEC_CAPS@)
exp_flowcheck_@VGCONF_ARCH_SEC@_@VGCONF_OS@_CFLAGS       = \
	$(AM_CFLAGS_@VGCONF_PLATFORM_SEC_CAPS@) -O2
exp_flowcheck_@VGCONF_ARCH_SEC@_@VGCONF_OS@_DEPENDENCIES = \
	$(TOOL_DEPENDENCIES_@VGCONF_PLATFORM_SEC_CAPS@)
exp_flowcheck_@VGCONF_ARCH_SEC@_@VGCONF_OS@_LDADD        = \
	$(TOOL_LDADD_@VGCONF_PLATFORM_SEC_CAPS@)
exp_flowcheck_@VGCONF_ARCH_SEC@_@VGCONF_OS@_LDFLAGS      = \
	$(TOOL_LDFLAGS_@VGCONF_PLATFORM_SEC_CAPS@)
exp_flowcheck_@VGCONF_ARCH_SEC@_@VGCONF_OS@_LINK = \
	$(top_builddir)/coregrind/link_tool_exe_@VGCONF_OS@ \
	@VALT_LOAD_ADDRESS_SEC@ \
	$(LINK) \
	$(exp_flowcheck_@VGCONF_ARCH_SEC@_@VGCONF_OS@_CFLAGS) \
	$(exp_flowcheck_@VGCONF_ARCH_SEC@_@VGCONF_OS@_LDFLAGS)
endif

# fc_main.c contains the helper function for flowcheck that get called
# all the time. To maximise performance compile with -fomit-frame-pointer
# Primary beneficiary is x86.
fc_main.o: CFLAGS += -fomit-frame-pointer

#----------------------------------------------------------------------------
# vgpreload_exp-flowcheck-<platform>.so
#----------------------------------------------------------------------------

noinst_PROGRAMS += vgpreload_exp-flowcheck-@VGCONF_ARCH_PRI@-@VGCONF_OS@.so
if VGCONF_HAVE_PLATFORM_SEC
noinst_PROGRAMS += vgpreload_exp-flowcheck-@VGCONF_ARCH_SEC@-@VGCONF_OS@.so
endif

if VGCONF_OS_IS_DARWIN
noinst_DSYMS =- $(noinst_PROGRAMS)
endif

VGPRELOAD_EXP_FLOWCHECK_SOURCES_COMMON = fc_replace_strmem.c

vgpreload_exp_flowcheck_@VGCONF_ARCH_PRI@_@VGCONF_OS@_so_SOURCES      = \
	$(VGPRELOAD_EXP_FLOWCHECK_SOURCES_COMMON)
vgpreload_exp_flowcheck_@VGCONF_ARCH_PRI@_@VGCONF_OS@_so_CPPFLAGS     = \
	$(AM_CPPFLAGS_@VGCONF_PLATFORM_PRI_CAPS@)
vgpreload_exp_flowcheck_@VGCONF_ARCH_PRI@_@VGCONF_OS@_so_CFLAGS       = \
	$(AM_CFLAGS_PSO_@VGCONF_PLATFORM_PRI_CAPS@)
vgpreload_exp_flowcheck_@VGCONF_ARCH_PRI@_@VGCONF_OS@_so_DEPENDENCIES = \
	$(LIBREPLACEMALLOC_@VGCONF_PLATFORM_PRI_CAPS@)
vgpreload_exp_flowcheck_@VGCONF_ARCH_PRI@_@VGCONF_OS@_so_LDFLAGS      = \
	$(PRELOAD_LDFLAGS_@VGCONF_PLATFORM_PRI_CAPS@) \
	$(LIBREPLACEMALLOC_LDFLAGS_@VGCONF_PLATFORM_PRI_CAPS@)

if VGCONF_HAVE_PLATFORM_SEC
vgpreload_exp_flowcheck_@VGCONF_ARCH_SEC@_@VGCONF_OS@_so_SOURCES      = \
	$(VGPRELOAD_EXP_FLOWCHECK_SOURCES_COMMON)
vgpreload_exp_flowcheck_@VGCONF_ARCH_SEC@_@VGCONF_OS@_so_CPPFLAGS     = \
	$(AM_CPPFLAGS_@VGCONF_PLATFORM_SEC_CAPS@)
vgpreload_exp_flowcheck_@VGCONF_ARCH_SEC@_@VGCONF_OS@_so_CFLAGS       = \
	$(AM_CFLAGS_PSO_@VGCONF_PLATFORM_SEC_CAPS@)
vgpreload_exp_flowcheck_@VGCONF_ARCH_SEC@_@VGCONF_OS@_so_DEPENDENCIES = \
	$(LIBREPLACEMALLOC_@VGCONF_PLATFORM_SEC_CAPS@)
vgpreload_exp_flowcheck_@VGCONF_ARCH_SEC@_@VGCONF_OS@_so_LDFLAGS      = \
	$(PRELOAD_LDFLAGS_@VGCONF_PLATFORM_SEC_CAPS@) \
	$(LIBREPLACEMALLOC_LDFLAGS_@VGCONF_PLATFORM_SEC_CAPS@)
endif

# fc_replace_strmem.c runs on the simulated CPU, and it often appears
# in stack traces shown to the user.  It is built with
# -fno-omit-frame-pointer so as to guarantee robust backtraces on x86,
# on which CFI based unwinding is not the "normal" case and so is
# sometimes fragile.
fc_replace_strmem.o: CFLAGS += -fno-omit-frame-pointer

