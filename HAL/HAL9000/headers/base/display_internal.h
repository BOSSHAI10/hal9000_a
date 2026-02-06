#pragma once

// We will use RGB colors
// Layout 0x00RRGGBB
typedef DWORD COLOR_INTERNAL;

// Information about color format
#define COLOR_MASK_SIZE               8
#define COLOR_RED_FIELD_POS           16
#define COLOR_GREEN_FIELD_POS         8
#define COLOR_BLUE_FIELD_POS          0

// Transform BIOS colors to RGB
static DWORD BIOS_COLOR_MAP[16] =
{
    0x00000000, // BLACK
    0x000000AA, // BLUE
    0x0000AA00, // GREEN
    0x0000AAAA, // CYAN
    0x00AA0000, // RED
    0x00AA00AA, // MAGENTA
    0x00AA5500, // BROWN
    0x00AAAAAA, // WHITE
    0x00555555, // GRAY
    0x005555FF, // BRIGHT_BLUE
    0x0055FF55, // BRIGHT_GREEN
    0x0055FFFF, // BRIGHT_CYAN
    0x00FF5555, // BRIGHT_RED
    0x00FF55FF, // BRIGHT_MAGENTA
    0x00FFFF55, // YELLOW
    0x00FFFFFF, // BRIGHT_WHITE
};

// Glyphs for the font
#include "display_glyphs.h"