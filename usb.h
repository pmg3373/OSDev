/* INSERT HEADER_xhci_ COMMENT HERE */
#ifndef _USB_H_
#define _USB_H_

#include "pci.h"

void _xhci_setup(int, int, int, int);

void _poll_usb( void );

uint8_t _xhci_initialized;

struct _xhci_controller {
    struct _xhci_cap_regs* capabilities;
    struct _xhci_com_regs* commands;
    uint8_t maxports;
    struct _xhci_ports* ports;
};

struct _xhci_controller _initialized_controller;

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
    uint32_t crcrlo;
    uint32_t crcrhi;
    uint32_t rsvd2[4];
    uint32_t dcbaaplo;
    uint32_t dcbaaphi;
    uint32_t config;
} __attribute__((packed));

struct _xhci_ports {
    uint32_t portsc;
    uint32_t portpmsc;
    uint32_t portli;
    uint32_t porthlpmc;
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

struct _xhci_interrupter_regs {
    uint32_t fields[8];
};

struct _xhci_erst_entry {
    uint32_t fields[4];
};

// default should be zero because this is global
volatile uint64_t dcbaa[10] __attribute__((aligned(64)));

//Bit 11
#define USBSTS_CNR 0x800

#define USBDEV_ENABLED 9

struct _xhci_trb {
    uint32_t fields[4];
};

volatile struct _xhci_trb command_ring[16] __attribute__((aligned(16)));

volatile struct _xhci_trb event_ring[16] __attribute__((aligned(16)));

volatile struct _xhci_erst_entry erst_table __attribute__((aligned(16)));

volatile uint32_t * _xhci_doorbells;

#endif
