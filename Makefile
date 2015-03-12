#
# 
#

CC := g++ -std=c++11 -g -O2 # This is the main compiler
SRCDIR := src
BUILDDIR := build
TARGET := bin/race

SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name "*.$(SRCEXT)")
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
#GTKFLAGS := -g # -Wall
GTKFLAGS=`pkg-config gtkmm-3.0 --cflags --libs`
LIB := -larmadillo -lboost_iostreams -lboost_system -lboost_filesystem -lutil 
INC := -Iinclude

$(TARGET): $(OBJECTS)
	@echo "Linking..."
	$(CC) $(GTKFLAGS) $(LIB) $^ -o $(TARGET) 

$(BUILDDIR)/%_gtk.o: $(SRCDIR)/%_gtk.$(SRCEXT)
	mkdir -p $(BUILDDIR)
	$(CC) $(GTKFLAGS) $(INC) -c -o $@ $<

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	mkdir -p $(BUILDDIR)
	$(CC) $(INC) $(LIB) -c -o $@ $< 

clean:
	@echo "Cleaning..."; 
	$(RM) -r $(BUILDDIR) $(TARGET)

.PHONY: clean
