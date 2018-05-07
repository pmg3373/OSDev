/* HEADER COMMENT */
#include "common.h"

#include "c_io.h"

#include "usb.h"

void _xhci_setup(int bus, int dev, int func, int id) {
    if (_xhci_initialized)
        return;

    // check class and subclass
    int data = _pci_get_data(bus, dev, func, 8);
    // Class:    0x0C (24-31)
    // Subclass: 0x03 (16-23)
    // Prog IF:  0x30 (08-15)
    if ((data & 0xFFFFFF00) != 0x0C033000)
        return;
    c_printf("\nXHCI Device found\n");

    data = _pci_get_data(bus, dev, func, 0xC) & 0xFF;
    c_printf("Header type: %x\n", data);

    struct _xhci_cap_regs* capability_registers;
    struct _xhci_com_regs* command_registers;

    // Steps can be found in section 4.2
    // Initialize the system I/O memory maps (I think this is what it means)
    data = _pci_get_data(bus, dev, func, 0x10); // bar0
    capability_registers = data & 0xFFFFFF00;
    command_registers = (data & 0xFFFFFF00) + capability_registers->caplength;

    /*
    c_printf("Caplength: %x\n", capability_registers->caplength);
    c_printf("hciversion: %x\n", capability_registers->hciversion);
    c_printf("current sts: %x\n", command_registers->usbsts);
    */
    uint8_t ports = ((capability_registers->hcsparams1) >> 24) & 0xF;
    c_printf("Max Ports: %x\n", ports);

    // After chip hardware reset wait until CNR flag in USBSTS is 0
    while ((command_registers->usbsts) & USBSTS_CNR) c_printf(".");

    // Program the Max Device Slots Enabled in the CONFIG register (5.4.7)
    command_registers->config = ((command_registers->config) & 0xFFFFFF00)
                                                             & USBDEV_ENABLED;

    // Program the Device Context Base Address Array Pointer
    if ( (int)dcbaa & 0x3F ) c_printf("dcbaa not aligned!\n");
    command_registers->dcbaaplo = command_registers->dcbaaplo & (int)dcbaa;
    command_registers->dcbaaphi = 0;

    // Define the Command Ring Dequeue Pointer by programming the
    // Command Ring Control Register (5.4.5) with a 64-bit address pointing
    // to the starting address of the first TRB of the command ring
    uint64_t cr_addr = (uint64_t)command_ring;
    command_registers->crcrlo = (uint32_t)((cr_addr & 0xFFFFFFFF) | 0);
    command_registers->crcrhi = (uint32_t)((cr_addr >> 32) & 0xFFFFFFFF);

    // Set up the TRB so that the last one is a Link TRB
    command_ring[15].fields[0] = (uint32_t)(cr_addr & 0xFFFFFFFF);
    command_ring[15].fields[1] = (uint32_t)((cr_addr >> 32) & 0xFFFFFFFF);
    command_ring[15].fields[2] = 0;
    command_ring[15].fields[3] = 0x1802; //Type = Link; Toggle = 1

    // Initialize Interrupts by: OR DON'T THEY'RE OPTIONAL
    // Actually though I have to enable some crap here
    
    // Allocate and initialize Event Ring Segments
    uint64_t er_addr = (uint64_t)event_ring;

    // Allocate Event Ring Segment Table
    // Initialize ERST table entires to point to Event Ring Segments
    erst_table.fields[2] = 16; // size 16 table
    erst_table.fields[0] = (uint32_t)(er_addr & 0xFFFFFFFF);
    erst_table.fields[1] = (uint32_t)((er_addr >> 32) & 0xFFFFFFFF);

    // Set interrupter ERSTSZ register (5.5.2.3.1)
    volatile struct _xhci_interrupter_regs * intregs = (void*)capability_registers +
                                              capability_registers->rtsoff + 0x20;

    uint64_t ertb_addr = (uint64_t)(&erst_table);

    
    // Program ERDP (5.5.2.3.3)
    intregs->fields[6] = (uint32_t)(er_addr & 0xFFFFFFFF);
    intregs->fields[7] = (uint32_t)((er_addr >> 32) & 0xFFFFFFFF);

    intregs->fields[2] = (short)1;
    intregs->fields[4] = (uint32_t)((ertb_addr) & 0xFFFFFFFF);
    intregs->fields[5] = (uint32_t)(((ertb_addr) >> 32) & 0xFFFFFFFF);


    // Write the USBCMD (5.4.1) to turn the host controller ON via setting the
    // R/S bit to '1', so it starts accepting doorbell references
    command_registers->usbcmd = command_registers->usbcmd & 0x1;

    //sleep
    for (int i = 0; i < 100000000; i++);
    command_registers->usbsts = 0;
    intregs->fields[0] = 0;


    // record the controller we just initialized
    _initialized_controller.capabilities = capability_registers;
    _initialized_controller.commands = command_registers;
    _initialized_controller.maxports = ports;
    _initialized_controller.ports = (volatile struct _xhci_ports *)
        (((void *)command_registers) + 0x400);
    _xhci_initialized = 1;


    for(int i = 0; i < _initialized_controller.maxports; i++) {
        c_printf("Port %x: %x\n", i, _initialized_controller.ports[i].portsc);
    }

    _initialized_controller.ports[4].portsc |= 0x10;

    c_printf("Port %x: %x\n", 4, _initialized_controller.ports[4].portsc);

    // Try to send "enable slot" command"
    command_ring[0].fields[3] = 0x2401;

    _xhci_doorbells =
        (void*) capability_registers + capability_registers->dboff;
    c_printf("doorbell offset: %x\n", capability_registers->dboff);
    c_printf("%x + %x = %x\n",
        capability_registers, capability_registers->dboff, _xhci_doorbells);

    _xhci_doorbells[0] = 1;
    c_printf("Doorbell: %x\n", _xhci_doorbells[0]);


    //sleep
    for (int i = 0; i < 100000000; i++);

    for(int i = 0; i < 4; i++) c_printf("CReg %d: %x\n",i,command_ring[0].fields[i]);
    if (event_ring[0].fields[3] & 1) {
        c_printf("Something on event ring\n");
        for(int i = 0; i < 4; i++) c_printf("EReg %d: %x\n",i,event_ring[0].fields[i]);
    }


    for(;;) {
        for(int i = 0; i < _initialized_controller.maxports; i++) {
            c_printf("Port %x: %x\n", i, _initialized_controller.ports[i].portsc);
        }
        c_printf("\n");
        for(int i = 0; i < 4; i++)
            c_printf("EReg %d: %x\n",i,command_ring[0].fields[i]);

        for (int i = 0; i < 100000000; i++);

        if (event_ring[0].fields[3] & 1) {
            c_printf("Something on event ring\n");
            for(int i = 0; i < 4; i++)
                c_printf("EReg %d: %x\n",i,event_ring[0].fields[i]);
        } else {
            c_printf(".\n.\n.\n.\n.\n");
        }

    }
}

void _poll_usb() {
    // This should work
}
