/* INSERT HEADER COMMENT HERE */
#ifndef _USB_H_
#define _USB_H_

#include "pci.h"

void _xhci_setup(int, int, int, int);

struct _xhci_cap_regs {
    uint8_t caplength;
    uint8_t rsvd;
    uint16_t hciversion;
    uint32_t hcsparams1;
    uint32_t hcsparams2;
    uint32_t hcsparams3;
    uint32_t hccparams1;
    uint32_t dboff;
    uint32_t rtsoff;
    uint32_t hccparams2;
} __attribute__((packed));

struct _xhci_com_regs {
    uint32_t usbcmd;
    uint32_t usbsts;
    uint32_t pagesize;
    uint32_t rsvd[3];
    uint32_t dncntrl;
    uint32_t crcr;
    uint32_t rsvd2[4];
    uint32_t dcbaaplo;
    uint32_t dcbaaphi;
    uint32_t config;
} __attribute__((packed));

struct _xhci_slot_context {
    uint32_t fields[8];
};

struct _xhci_ep_context {
    uint32_t fields[8];
};

struct _xhci_dev_context {
    struct _xhci_slot_context slot;
    struct _xhci_ep_context eps[31];
};

// default should be zero because this is global
uint64_t dcbaa[10] __attribute__((aligned(64)));

//Bit 11
#define USBSTS_CNR 0x800

#define USBDEV_ENABLED 9

struct _xhci_trb {
    uint32_t reg[4];
};

#endif
