//AUTOGENERATED FILE FROM png2asset

#include <stdint.h>
#include <gbdk/platform.h>
#include <gbdk/metasprites.h>

BANKREF(diamond)

const palette_color_t diamond_palettes[2] = {

};

const uint8_t diamond_tiles[64] = {
	0x10,0x00,0x38,0x00,0x7c,0x00,0xfe,0x00,0xff,0x00,0x7e,0x00,0x3c,0x00,0x18,0x00,
	0x18,0x00,0x3c,0x00,0x7e,0x00,0xff,0x00,0xfe,0x00,0x7c,0x00,0x38,0x00,0x10,0x00,
	0x00,0x00,0x7c,0x28,0x3c,0x68,0x7c,0x28,0x7c,0x28,0x00,0x00,0x18,0x00,0x7e,0x00,
	0x68,0x5c,0x3c,0x68,0x00,0x00,0x7c,0x28,0x00,0x00,0x08,0x9c,0x3c,0x28,0xff,0x00
};

const metasprite_t diamond_metasprite0[] = {
	METASPR_ITEM(0, 0, 0, 0), METASPR_ITEM(0, 8, 1, 0), METASPR_ITEM(8, -8, 2, 0), METASPR_ITEM(0, 8, 3, 6), METASPR_TERM
};

const metasprite_t* const diamond_metasprites[1] = {
	diamond_metasprite0
};
