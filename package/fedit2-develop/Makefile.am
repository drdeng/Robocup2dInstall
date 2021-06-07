ACLOCAL_AMFLAGS=-I m4

SUBDIRS = src tool

LIBTOOL_DEPS = @LIBTOOL_DEPS@
libtool: $(LIBTOOL_DEPS)
	$(SHELL) ./config.status --recheck

EXTRA_DIST = \
	bootstrap \
	fedit2.pro \
	xpm/chase.xpm \
	xpm/delete.xpm \
	xpm/hand.xpm \
	xpm/insert.xpm \
	xpm/new.xpm \
	xpm/open.xpm \
	xpm/record.xpm \
	xpm/replace.xpm \
	xpm/reverse.xpm \
	xpm/save.xpm \
	xpm/fedit2.xpm \
	xpm/symmetry.xpm \
	xpm/train.xpm

CLEANFILES = *~
