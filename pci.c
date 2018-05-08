/**
 * ADD FILE HEADER COMMENT HERE
 */

#include "common.h"

#include "c_io.h"

#include "pci.h"

#include "support.h"
#include "i8255x.h"



void outl_wrap(uint32_t addr, uint32_t data){
	__outl(0xcf8, addr);
	__outl(0xcfc, data);
}

void outw_wrap(uint32_t addr, uint16_t data){
	__outl(0xcf8, addr);
	__outw(0xcfc, data);
}

void outb_wrap(uint32_t addr, uint8_t data){
	__outl(0xcf8, addr);
	__outb(0xcfc, data);
}

uint32_t inl_wrap(uint32_t addr){
	__outl(0xcf8, addr);
	return __inl(0xcfc);
}

uint16_t inw_wrap(uint32_t addr){
	__outl(0xcf8, addr);
	return __inw(0xcfc);
}

uint8_t inb_wrap(uint32_t addr){
	__outl(0xcf8, addr);
	return __inb(0xcfc);
}


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
            if(data == 0x8086){
				//c_printf("Attempting to setup i8255x \n");
                //TODO: Bookmark
                //i8255x_init( (int)(((bus << 16) | (dev << 11) |
						//((int)0x80000000))) );

				//_i8255X_test_send();
				//c_printf("Sent single test packet");
            }
        }
    }
}
