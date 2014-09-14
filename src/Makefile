CC = g++
CFLAGS = -c -Wall -g -O0
LFLAGS = -lSDL -lSDL_image -lSDL_mixer
OBJDIR = objs
DEPSDIR = deps
SOURCES = $(wildcard *.cpp)
SOURCES += $(wildcard iml/*.cpp)
DEPS = $(addprefix $(DEPSDIR)/, $(SOURCES:.cpp=.d))
OBJECTS = $(addprefix $(OBJDIR)/, $(SOURCES:.cpp=.o))
BINDIR = ../bin
EXECUTABLE = $(BINDIR)/ITD++

.PHONY: all clean

all: $(EXECUTABLE)

-include $(DEPS)

$(EXECUTABLE): $(BINDIR) $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LFLAGS)

$(BINDIR):
	mkdir $(BINDIR)

$(OBJDIR)/%.o: %.cpp
	$(CC) $(CFLAGS) $< -o $@

$(OBJECTS): | $(OBJDIR)

$(OBJDIR):
	mkdir $(OBJDIR)
	mkdir $(OBJDIR)/iml

$(DEPSDIR):
	mkdir $(DEPSDIR)
	mkdir $(DEPSDIR)/iml

clean:
	-rm $(OBJDIR)/iml/*.o $(DEPSDIR)/iml/*.d
	-rmdir $(OBJDIR)/iml
	-rmdir $(DEPSDIR)/iml
	-rm $(OBJDIR)/*.o $(EXECUTABLE) $(DEPSDIR)/*.d
	-rmdir $(OBJDIR)
	-rmdir $(DEPSDIR)

$(DEPSDIR)/%.d: %.cpp
	@set -e; rm -f $@; \
    $(CC) -MM $(CFLAGS) $< > $@.$$$$; \
    sed -r 's/^(.*\.o:)/$(DEPSDIR)\/$(notdir $@) $(OBJDIR)\/\1/g' \
            < $@.$$$$ > $@; \
    rm -f $@.$$$$

$(DEPS): | $(DEPSDIR)

