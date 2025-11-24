#!/bin/bash

# ============================================================================
# LIBRARIES.SH - Install all required libraries for the project
# ============================================================================
# This script installs all necessary libraries and dependencies including:
# - GCC (GNU Compiler Collection) for C++ compilation
# - SFML (Simple and Fast Multimedia Library) for graphics and windowing
# - Build tools (make, cmake)
# - Development libraries
# ============================================================================

echo "=========================================="
echo "Installing Required Libraries"
echo "=========================================="
echo ""

# Update package list
echo "Updating package list..."
sudo apt-get update

# Install GCC and G++ compiler
echo ""
echo "Installing GCC and G++ compilers..."
sudo apt-get install -y gcc g++

# Install build tools
echo ""
echo "Installing build tools (make, cmake)..."
sudo apt-get install -y make cmake

# Install SFML development libraries
echo ""
echo "Installing SFML libraries..."
sudo apt-get install -y libsfml-dev

# Install additional development libraries that might be needed
echo ""
echo "Installing additional development libraries..."
sudo apt-get install -y build-essential

# Install font libraries (for SFML text rendering)
echo ""
echo "Installing font libraries..."
sudo apt-get install -y libfreetype6-dev

# Install X11 development libraries (for windowing)
echo ""
echo "Installing X11 development libraries..."
sudo apt-get install -y libx11-dev libxrandr-dev libxcb1-dev libxcb-image0-dev

# Install OpenGL libraries (SFML uses OpenGL)
echo ""
echo "Installing OpenGL libraries..."
sudo apt-get install -y libgl1-mesa-dev libglu1-mesa-dev

# Install audio libraries (SFML audio support)
echo ""
echo "Installing audio libraries..."
sudo apt-get install -y libopenal-dev libflac-dev libvorbis-dev libogg-dev

# Verify installations
echo ""
echo "=========================================="
echo "Verifying installations..."
echo "=========================================="

# Check GCC version
if command -v gcc &> /dev/null; then
    echo "✓ GCC installed: $(gcc --version | head -n 1)"
else
    echo "✗ GCC not found"
fi

# Check G++ version
if command -v g++ &> /dev/null; then
    echo "✓ G++ installed: $(g++ --version | head -n 1)"
else
    echo "✗ G++ not found"
fi

# Check Make version
if command -v make &> /dev/null; then
    echo "✓ Make installed: $(make --version | head -n 1)"
else
    echo "✗ Make not found"
fi

# Check if SFML libraries are installed
if pkg-config --exists sfml-all; then
    echo "✓ SFML libraries installed"
    pkg-config --modversion sfml-all
else
    echo "✗ SFML libraries not found"
fi

echo ""
echo "=========================================="
echo "Installation complete!"
echo "=========================================="
echo ""
echo "You can now build the project using:"
echo "  make"
echo ""
echo "Or run the project with:"
echo "  make run"
echo ""

