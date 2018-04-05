/**
 * ADD FILE HEADER COMMENT HERE
 */

#include "common.h"

#include "c_io.h"

#include "pci.h"

void _pci_init() {
    c_puts( " PCI" );

    for (int bus = 0; bus < 1; bus++) {
        for(int dev = 0; dev < 10; dev++) {

			uint32_t baseId = ((bus) << 16) | ((dev) << 11) | ((0) << 8);
            //bus,dev,fun

			uint32_t address = 0x80000000 | baseId | (0x0 & 0xfc);
            //PCI_CONFIG_HEADER_TYPE

			// IOWRITE32
			__asm__ volatile("outl %0, %w1" : : "a" (address), "Nd" (0xcf8));	
			// IOREAD8
			uint16_t data;
			__asm__ volatile("inw %w1, %w0" : "=a" (data) : "Nd" (0xcfC +
                                                                (0x0 & 0x03)));

            c_printf("Bus %d Device %d Value %x\n",bus,dev,data);
        }
    }
}
