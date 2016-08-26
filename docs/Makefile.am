if HAVE_DOXYGEN
#directory = $(top_srcdir)/docs/man/man3/

#dist_man_MANS = $(directory)/man_page_1.3 $(directory)/man_page_2.3
#$(directory)/man_page_1.3: doxyfile.stamp
#$(directory)/man_page_2.3: doxyfile.stamp

.PHONY: doxyfile.stamp

doxyfile.stamp:
	$(DOXYGEN) Doxyfile
	echo $$(date +%Y%m%d%H%M%S) > doxyfile.stamp

CLEANFILES = doxyfile.stamp

all-local: doxyfile.stamp
clean-local:
	rm -rf $(top_builddir)/docs/output
endif
