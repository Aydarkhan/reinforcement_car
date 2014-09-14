#
# 
#

CC := g++ -std=c++11 -g # This is the main compiler
# CC := clang --analyze # and comment out the linker last line for sanity
SRCDIR := src
BUILDDIR := build
TARGET := bin/race

SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name "*.$(SRCEXT)")
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
#GTKFLAGS := -g # -Wall
GTKFLAGS=`pkg-config gtkmm-3.0 --cflags --libs`
#LIB := -pthread -lmongoclient -L lib -lboost_thread-mt -lboost_filesystem-mt -lboost_system-mt
LIB := -larmadillo #-llapack -lblas
INC := -Iinclude

$(TARGET): $(OBJECTS)
	@echo "Linking..."
	$(CC) $(GTKFLAGS) $(LIB) $^ -o $(TARGET) -O1 

$(BUILDDIR)/%_gtk.o: $(SRCDIR)/%_gtk.$(SRCEXT)
	mkdir -p $(BUILDDIR)
	$(CC) $(GTKFLAGS) $(INC) -c -o $@ $<

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	mkdir -p $(BUILDDIR)
	$(CC) $(INC) $(LIB) -c -o $@ $< -O1

clean:
	@echo "Cleaning..."; 
	$(RM) -r $(BUILDDIR) $(TARGET)

# Tests
#tester:
#$(CC) $(GTKFLAGS) test/tester.cpp $(INC) $(LIB) -o bin/tester

## Spikes
#ticket:
#$(CC) $(GTKFLAGS) spikes/ticket.cpp $(INC) $(LIB) -o bin/ticket

.PHONY: clean
