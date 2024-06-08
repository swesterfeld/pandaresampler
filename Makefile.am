SUBDIRS = filter-design lib tests

rebuild-api-docs:
	doxygen Doxyfile
	rsync -av html/ stefan@space.twc.de:public_html/pandaresampler/api-$(VERSION)
