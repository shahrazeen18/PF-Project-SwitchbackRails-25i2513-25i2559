# ============================================================================
# Makefile for Switchback Rails
# ============================================================================

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -g
SFML_FLAGS = -lsfml-graphics -lsfml-window -lsfml-system

# Source files
CORE_SRCS = core/simulation_state.cpp core/grid.cpp core/trains.cpp \
            core/switches.cpp core/simulation.cpp core/io.cpp
SFML_SRCS = sfml/app.cpp sfml/main.cpp

# Object files
CORE_OBJS = $(CORE_SRCS:.cpp=.o)
SFML_OBJS = $(SFML_SRCS:.cpp=.o)
ALL_OBJS = $(CORE_OBJS) $(SFML_OBJS)

# Output executable
TARGET = switchback_rails

# Default target
all: $(TARGET)

# Link executable
$(TARGET): $(ALL_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(SFML_FLAGS)
	@echo "Build complete! Run with: ./$(TARGET)"

# Compile source files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(ALL_OBJS) $(TARGET)
	rm -f out/*.csv out/*.txt
	@echo "Clean complete!"

# Run the complex network level (default)
run: $(TARGET)
	./$(TARGET) data/levels/complex_network.lvl

# Show help
help:
	@echo "Switchback Rails - Makefile"
	@echo ""
	@echo "Targets:"
	@echo "  make          - Build the project"
	@echo "  make run      - Build and run Complex Railway Network"
	@echo "  make clean    - Remove build artifacts"
	@echo "  make help     - Show this help message"
	@echo ""
	@echo "🚂 Complex Network Level Features:"
	@echo "  - 10 trains with interconnected paths"
	@echo "  - 20 switches (A-T) for dynamic routing"
	@echo "  - 16 crossing points for track exchanges"
	@echo "  - Multiple route options per train"
	@echo "  - Signal lights (GREEN/YELLOW/RED)"
	@echo "  - NORMAL weather for consistent behavior"
	@echo ""
	@echo "Read README.md for complete documentation!"

.PHONY: all clean run help

