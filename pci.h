/**
 * INSERT HEADER COMMENT HERE
 */

#ifndef _PCI_H_
#define _PCI_H_

#include "common.h"

#include "queues.h"
#include "usb.h"
#include "i8255x.h"


#define PCI_IOMAPBAR_01H_OFFSET  (0x14)

#define PCI_IOMAPBAR_MASK  (0b11111111111111111111111111110000)

queue_t _pci_devices;

/**
* Stucure of a PCI header
*/
typedef struct pci_device_common {
    // Add fields here
    	long pci_class;
	
	uint16_t device_id;
	uint16_t vendor_id;

	uint16_t command_reg;
	uint16_t status_reg;

	uint8_t rev_id; 
	uint8_t prog_if; 
	uint8_t sub_class_code; 
	uint8_t class_code;

	uint8_t cache_line_size; 
	uint8_t lat_timer;
	uint8_t head_type;
	uint8_t bist;
} pci_device_common;

typedef struct pci_device_00h{
    struct pci_device_common common;
    
    uint32_t bar0;
    uint32_t bar1;
    uint32_t bar2;
    uint32_t bar3;
    uint32_t bar4;
    uint32_t bar5;
    
    uint32_t carbus_cis_pointer;
    
    uint16_t subsystem_vend_id;
    uint16_t subsystem_id;
    
    uint32_t exp_rom_bar;
    
    
    uint8_t capab_pointer;
    
    uint8_t max_lat;
    uint8_t min_grant;
    uint8_t inter_pin;
    uint8_t inter_line;
};

/**
 * Structure of a PCI type 01 header
 */
typedef struct pci_device_01h{
    //First 4 lines are common to all PCI header types
    struct pci_device_common common;	

    uint32_t bar0;

    uint32_t bar1;

    uint8_t p_bus_num;
    uint8_t s_bus_num;
    uint8_t sub_bus_num;
    uint8_t sec_lat_timer;

    uint8_t io_base;
    uint8_t io_lim;
    uint16_t sec_stat;

    uint16_t mem_base;
    uint16_t mem_lim;

    uint16_t pre_mem_base;
    uint16_t pre_mem_lim;

    uint32_t pre_base_u_32_bits;

    uint32_t pre_lim_u_32_bits;

    uint16_t io_u_16_bits;
    uint16_t io_lim_16_bits;

    uint8_t capab_point;
    //the other 28 bits of this line are reserved

    uint32_t exp_rom_bar;

    uint8_t inter_line;
    uint8_t inter_pin;
    uint16_t bridge_ctrl;
} pci_device_01h;


/**
 * Wraps the process of writing to ioaddr and iodata into one call
 * @param addr address to write to
 * @param data data to write
 */
void outl_wrap(uint32_t addr, uint32_t data);

/**
 * Wraps the process of writing to ioaddr and iodata into one call
 * @param addr address to write to
 * @param data data to write
 */
void outw_wrap(uint32_t addr, uint16_t data);

/**
 * Wraps the process of writing to ioaddr and iodata into one call
 * @param addr address to write to
 * @param data data to write
 */
void outb_wrap(uint32_t addr, uint8_t data);

/**
 * Wraps the process of writing to ioaddr and reading iodata into one call
 * @param addr address to write to
 * @param data data to write
 */
uint32_t inl_wrap(uint32_t addr);

/**
 * Wraps the process of writing to ioaddr and reading iodata into one call
 * @param addr address to write to
 * @param data data to write
 */
uint16_t inw_wrap(uint32_t addr);

/**
 * Wraps the process of writing to ioaddr and reading iodata into one call
 * @param addr address to write to
 * @param data data to write
 */
uint8_t inb_wrap(uint32_t addr);



void _pci_init( void );

int _pci_get_data(int, int, int, int);


#endif
