/**
 * ADD FILE HEADER COMMENT HERE
 */

#include "common.h"

#include "c_io.h"

#include "pci.h"

#include "support.h"

int _pci_get_data(int bus, int dev, int function, int offset) {
    int address = ((bus << 16) | (dev << 11) | (function << 8) | (offset & 0xfc) |
               ((int)0x80000000));

    // IOWRITE32
    __asm__ volatile("outl %0, %w1" : : "a" (address), "Nd" (0xcf8));	
    // IOREAD8
    uint32_t data;
    __asm__ volatile("inl %w1, %0" : "=a" (data) : "Nd" (0xcfC +
                                                                (0x0 & 0x03)));

    data = data >> ((offset & 2) * 8);

    return data;
}


void _pci_init() {
    c_puts( " PCI" );

    for (int bus = 0; bus < 5; bus++) {
        for(int dev = 0; dev < 32; dev++) {
            int data = _pci_get_data(bus, dev, 0, 0);

            if ((data & 0xFFFF) != 0xFFFF) {
                int class = _pci_get_data(bus, dev, 0, 8);
                c_printf("Bus %d Device %d Value %x: Class: %x\n",bus,dev,data, class);
            }

            _xhci_setup(bus, dev, 0, data);
        }
    }
}
