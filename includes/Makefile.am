# includes/Makefile.am
# This directory contains symbolic links to 7-Zip source code

EXTRA_DIST = 

# Clean up symbolic links when doing distclean
DISTCLEANFILES = CPP C

# Make sure the symbolic links exist
all-local:
	@if [ ! -L CPP ]; then \
		echo "Warning: CPP symbolic link not found. Run configure with SEVENZIP_SOURCE_DIR set."; \
	fi
	@if [ ! -L C ]; then \
		echo "Warning: C symbolic link not found. Run configure with SEVENZIP_SOURCE_DIR set."; \
	fi
