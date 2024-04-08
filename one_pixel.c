#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <peekpoke.h>

// NES PPU Addresses
#define PPUCTRL   *((unsigned char*)0x2000) // PPU Control Register
#define PPUMASK   *((unsigned char*)0x2001) // PPU Mask Register
#define PPUSTATUS *((unsigned char*)0x2002) // PPU Status Register
#define OAMADDR   *((unsigned char*)0x2003) // OAM Address Register
#define OAMDATA   *((unsigned char*)0x2004) // OAM Data Register
#define PPUSCROLL *((unsigned char*)0x2005) // PPU Scroll Register
#define PPUADDR   *((unsigned char*)0x2006) // PPU Address Register
#define PPUDATA   *((unsigned char*)0x2007) // PPU Data Register
#define OAMDMA    *((unsigned char*)0x4014) // OAM DMA Register


// Function to draw a pixel at (x, y) with specified color
void drawPixel(uint8_t x, uint8_t y, uint8_t color) {
    // Calculate the address in VRAM
    uint16_t vramAddress = y * 32 + x; // Each row occupies 32 bytes

    // Set the VRAM address
    PPUADDR = vramAddress >> 8; // High byte
    PPUADDR = vramAddress & 0xFF; // Low byte

    // Write the color to VRAM
    PPUDATA = color;
}



// Function to draw a line from (x0, y0) to (x1, y1) with specified color
void drawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color) {
    // Calculate dx and dy
    int16_t dx = abs(x1 - x0);
    int16_t dy = abs(y1 - y0);

    // Calculate signs of slopes
    int16_t sx = x0 < x1 ? 1 : -1;
    int16_t sy = y0 < y1 ? 1 : -1;

    // Initial error
    int16_t err = dx - dy;

    // Current position
    int16_t x = x0;
    int16_t y = y0;
    int16_t e2;
    // Loop until the end of the line
    while (x != x1 || y != y1) {
        // Draw the pixel
        drawPixel(x, y, color);

        // Calculate the next position
        e2 = (2 * err);
        if (e2 > -dy) {
            err -= dy;
            x += sx;
        }
        if (e2 < dx) {
            err += dx;
            y += sy;
        }
    }

    // Draw the last pixel
    drawPixel(x1, y1, color);
}


void main() {
    // Set up PPU
    PPUCTRL = 0; // Disable NMI
    PPUMASK = 0xFF; // Show sprites, background

    // Draw a line from (10, 10) to (20, 20) with color 0
    drawLine(10, 10, 20, 20, 0);

    // Wait for input
    while (1){

    }
}