## Makefile.am for DirectFB/src
SUBDIRS = \
	core display windows	\
	gfx input media misc

INCLUDES = \
	-I$(top_builddir)/include	\
	-I$(top_builddir)/lib		\
	-I$(top_builddir)/src		\
	-I$(top_srcdir)/include		\
	-I$(top_srcdir)/lib		\
	-I$(top_srcdir)/src

AM_CPPFLAGS = \
	-DDATADIR=\"${RUNTIME_SYSROOT}@DATADIR@\"	\
	-DMODULEDIR=\"${RUNTIME_SYSROOT}@MODULEDIR@\"


internalincludedir = $(INTERNALINCLUDEDIR)

internalinclude_HEADERS = \
	idirectfb.h		\
	init.h


lib_LTLIBRARIES = libdirectfb.la

libdirectfb_la_SOURCES = 		\
	idirectfb.c	\
	directfb.c			\
	directfb_result.c		\
	dummy.cpp			\
	init.c

libdirectfb_la_LDFLAGS = \
	-version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE)	\
	-release $(LT_RELEASE)					\
	$(DFB_LDFLAGS)

libdirectfb_la_LIBADD = \
	core/libdirectfb_core.la	\
	display/libdirectfb_display.la	\
	windows/libdirectfb_windows.la  \
	media/libdirectfb_media.la	\
	input/libdirectfb_input.la	\
	misc/libdirectfb_misc.la	\
	gfx/libdirectfb_gfx.la		\
	../lib/direct/libdirect.la	\
	../lib/fusion/libfusion.la


MKRESULT = $(top_srcdir)/tools/mkresult.sh

directfb_result.c: $(top_srcdir)/include/directfb.h $(MKRESULT)
	@echo '#include <config.h>'    		> $@
	@echo                                   >> $@
	@echo '#include <directfb.h>'    	>> $@
	@echo                                   >> $@
	@echo                                   >> $@

	@$(MKRESULT) DFBResult DFB $< 	>> $@

	@echo                                   >> $@


#
## and now rebuild the static version with the *correct* object files
#
if BUILD_STATIC

clean-local:
	rm -f libdirectfb_fixed.a

all-local: libdirectfb_fixed.a

libdirectfb_fixed.a: .libs/libdirectfb.a
	rm -f libdirectfb_fixed.a
	${AR} cru libdirectfb_fixed.a `find . -name "*.o" | grep -v '.libs'`
	${RANLIB} libdirectfb_fixed.a
	cp -pf libdirectfb_fixed.a .libs/libdirectfb.a

.libs/libdirectfb.a: libdirectfb.la

else

clean-local:

all-local:

endif


include $(top_srcdir)/rules/nmfile.make
