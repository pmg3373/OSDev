/* HEADER COMMENT */
#include "common.h"

#include "c_io.h"

#include "usb.h"

void _xhci_setup(int bus, int dev, int func, int id) {
    if (id != 0x1033)
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

    // After chip hardware reset wait until CNR flag in USBSTS is 0
    while ((command_registers->usbsts) & USBSTS_CNR) c_printf(".");

    // Program the Max Device Slots Enabled in the CONFIG register (5.4.7)
    command_registers->config = ((command_registers->config) & 0xFFFFFF00)
                                                             & USBDEV_ENABLED;

    // Program the Device Context Base Address Array Pointer
    if ( (int)dcbaa & 0x3F ) c_printf("dcbaa not aligned!\n");
    command_registers->dcbaaplo = command_registers->dcbaaplo & (int)dcbaa;

    // Define the Command Ring Dequeue Pointer by programming the
    // Command Ring Control Register (5.4.5) with a 64-bit address pointing
    // to the starting address of the first TRB of the command ring


    // Initialize Interrupts by: OR DON'T THEY'RE OPTIONAL
    

    // Write the USBCMD (5.4.1) to turn the host controller ON via setting the
    // R/S bit to '1', so it starts accepting doorbell references
}
