CC ?= cc
CFLAGS ?= -O2 -g -pipe
PKG_CONFIG ?= pkg-config

DIR_PERM = 0755
FILE_PERM = 0644

LIBPURPLE_CFLAGS += $(shell $(PKG_CONFIG) --cflags glib-2.0 json-glib-1.0 purple pidgin zlib)
LIBPURPLE_LIBS += $(shell $(PKG_CONFIG) --libs glib-2.0 json-glib-1.0 purple pidgin zlib)
PLUGIN_DIR_PURPLE =  $(shell $(PKG_CONFIG) --variable=plugindir purple)

PRPL_NAME = libwebpidginz.so
PRPL_LIBNAME = ${PRPL_NAME}

SKYPEWEB_SOURCES = \
	webpidginz.c

.PHONY: all clean install
all: $(PRPL_NAME)
install:
	mkdir -m $(DIR_PERM) -p $(DESTDIR)$(PLUGIN_DIR_PURPLE)
	install -m $(FILE_PERM) $(PRPL_LIBNAME) $(DESTDIR)$(PLUGIN_DIR_PURPLE)/$(PRPL_NAME)
clean:
	rm -f libwebpidginz.so

$(PRPL_NAME): $(SKYPEWEB_SOURCES)
	$(CC) -Wall -I. -fPIC $(CFLAGS) $(SKYPEWEB_SOURCES) -o $@ $(LIBPURPLE_CFLAGS) $(LIBPURPLE_LIBS) $(LDFLAGS) -shared