# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g

# Include paths
INCLUDES = -Iinclude

# Source and object directories
SRCDIR = src
OBJDIR = obj
BINDIR = bin

# Source files
SOURCES = $(SRCDIR)/Action.cpp \
          $(SRCDIR)/Auxiliary.cpp \
          $(SRCDIR)/Facility.cpp \
          $(SRCDIR)/main.cpp \
          $(SRCDIR)/Plan.cpp \
          $(SRCDIR)/SelectionPolicy.cpp \
          $(SRCDIR)/Settlement.cpp \
          $(SRCDIR)/Simulation.cpp

# Object files
OBJECTS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SOURCES))

# Target executable
TARGET = $(BINDIR)/simulation

# Default rule
all: $(TARGET)

# Linking the executable
$(TARGET): $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(TARGET)

# Compiling object files
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean up build files
clean:
	rm -rf $(OBJDIR) $(BINDIR)

# Rebuild the project
rebuild: clean all
