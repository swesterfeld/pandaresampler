SUBDIRS = filter-design include lib tests

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = pandaresampler.pc
$(pkgconfig_DATA): $(top_builddir)/config.status
CLEANFILES = pandaresampler.pc
EXTRA_DIST = pandaresampler.pc.in MPL-2.0.txt README.md Doxyfile

rebuild-api-docs:
	doxygen Doxyfile
	rsync -av html/ stefan@space.twc.de:public_html/pandaresampler/api-$(VERSION)
