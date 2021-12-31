SRCDIR = sources
OBJDIR = obj
INCLUDEDIR = includes
CC = gcc
TARGETS = time_sort
FLAGS = $(INCLUDEDIR:%=-I %) -Wall -pthread
SOURCES = $(filter-out $(TARETS:%=$(SRCDIR)/%.c), $(wildcard $(SRCDIR)/*.c))
OBJECTS = $(SOURCES:$(SRCDIR)%.c=$(OBJDIR)%.o)
INCLUDES = $(SOURCES:$(SRCDIR)%.c=$(INCLUDEDIR)%.h)


.PHONY: all 
all: $(TARGETS)

.SECONDEXPANSION:
$(TARGETS): $(OBJECTS) $(OBJDIR)/$$@.o
	$(CC) $^ $(FLAGS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c 
	@mkdir -p $(OBJDIR)
	$(CC) -c $< $(FLAGS) -o $@

$(INCLUDES):

.PHONY: clean
clean: 
	rm -rf $(OBJDIR) $(TARGETS)

.PHONY: debug
debug: FLAGS += -g -D DEBUG
debug: clean
debug: $(TARGETS)
