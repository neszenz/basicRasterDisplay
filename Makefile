# -*- Makefile -*-

# compiler setup
CC := g++
CFLAGS := -Wall -std=c++17 -O3
LDFLAGS := -lSDL2

# settings
SRCEXT := cpp
SRCDIR := src
INCDIR := include
BUILDDIR := build
TARGET := $(BUILDDIR)/bRD

# automatically generated
SOURCES := $(shell find $(SRCDIR) -type f -name "*.$(SRCEXT)")
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
INC := -I $(INCDIR)

# makefile targets
$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $(TARGET)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

clean:
	rm -rf $(BUILDDIR)
.PHONY: clean
