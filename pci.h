/**
 * INSERT HEADER COMMENT HERE
 */

#ifndef _PCI_H_
#define _PCI_H_

#include "common.h"

#include "queues.h"

queue_t _pci_devices;

struct pci_device {
    // Add fields here
    long pci_class;
};

void _pci_init( void );

#endif
