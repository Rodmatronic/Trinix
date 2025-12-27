/*
 * Do special text-mode specific shenanigans within
 * VGA
 */

#include <types.h>
#include <x86.h>
#include <defs.h>
#include <config.h>

uint8_t gnome_palette[16 * 3] = {
	1, 0, 2,
	4, 18, 34,
	9, 40, 26,
	10, 41, 44,
	48, 7, 10,
	41, 17, 46,
	42, 35, 12,
	52, 51, 50,

	23, 22, 25,
	10, 30, 55,
	13, 52, 31,
	13, 49, 55,
	61, 24, 20,
	48, 24, 50,
	58, 43, 3,
	63, 63, 63
};

void _setpalette(const uint8_t *palette, uint8_t count){
	outb(0x3C8, 0);
	for(uint16_t i = 0; i < count * 3; i++)
	{
		outb(0x3C9, palette[i]);
	}
}

void vgatextpalette(void){
#ifdef CONFIG_VGA_PALETTE
	_setpalette(gnome_palette, 16);

	// fix the weird colours
	inb(0x3DA);
	outb(0x3C0, 0x06);
	outb(0x3C0, 0x06);
	outb(0x3C0, 0x20);
#endif
}
