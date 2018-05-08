#ifndef _I8255XCOMMANDBLOCK_H_
#define _I8255XCOMMANDBLOCK_H_

#include "ethernet.h"
#include "common.h"
#include "c_io.h"

//page 57 describes the CB commands
#define I8255XCOMMANDBLOCK_NOP  (0b000)
#define I8255XCOMMANDBLOCK_INDADDRSETUP  (0b001)
#define I8255XCOMMANDBLOCK_CONFIG  (0b010)
#define I8255XCOMMANDBLOCK_MULTIADDRSETUP  (0b011)
#define I8255XCOMMANDBLOCK_TRANSMIT  (0b100)
#define I8255XCOMMANDBLOCK_LOADMICROCODE  (0b101)
#define I8255XCOMMANDBLOCK_DUMP  (0b110)
#define I8255XCOMMANDBLOCK_DIAGNOSE  (0b111)



//page 100 describes the format of the RFD

#define I8255XRFD_FORMAT_OK_MASK  (1 << 13)
#define I8255XRFD_FORMAT_C_MASK  (1 << 15)
#define I8255XRFD_FORMAT_SF_MASK  (0b1 << 19)
#define I8255XRFD_FORMAT_H_MASK  (0b1 << 20)
#define I8255XRFD_FORMAT_S_MASK  (1 << 30)
#define I8255XRFD_FORMAT_EL_MASK  (1 << 31)
#define I8255XRFD_FORMAT_SIZE_MASK  (0b0011111111111111 << 16)
#define I8255XRFD_FORMAT_EOF_MASK  (0b1 << 15)
#define I8255XRFD_FORMAT_F_MASK  (0b1 << 14)
#define I8255XRFD_FORMAT_ACTUAL_COUNT_MASK  (0b0011111111111111)

#define I8255XRFD_FORMAT_OK_POS  (13)
#define I8255XRFD_FORMAT_C_POS  (15)
#define I8255XRFD_FORMAT_SF_POS  (19)
#define I8255XRFD_FORMAT_H_POS  (20)
#define I8255XRFD_FORMAT_S_POS  (30)
#define I8255XRFD_FORMAT_EL_POS  (31)
#define I8255XRFD_FORMAT_SIZE_POS  (16)
#define I8255XRFD_FORMAT_EOF_POS  (15)
#define I8255XRFD_FORMAT_F_POS  (14)
#define I8255XRFD_FORMAT_ACTUAL_COUNT_POS  (0)

//page 58 describes the format of the command blocks

#define I8255XCOMMANDBLOCK_FORMAT_U_MASK  (0b1 << 12)
#define I8255XCOMMANDBLOCK_FORMAT_OK_MASK  (1 << 13)
#define I8255XCOMMANDBLOCK_FORMAT_C_MASK  (1 << 15)
#define I8255XCOMMANDBLOCK_FORMAT_CMD_MASK  (0b111 << 16)
#define I8255XCOMMANDBLOCK_FORMAT_SF_MASK  (0b1 << 19)
#define I8255XCOMMANDBLOCK_FORMAT_NC_MASK  (0b1 << 20)
#define I8255XCOMMANDBLOCK_FORMAT_CID_MASK  (0b1111 << 24)
#define I8255XCOMMANDBLOCK_FORMAT_I_MASK  (1 << 29)
#define I8255XCOMMANDBLOCK_FORMAT_S_MASK  (1 << 30)
#define I8255XCOMMANDBLOCK_FORMAT_EL_MASK  (1 << 31)

#define I8255XCOMMANDBLOCK_FORMAT_U_POSITION  (12)
#define I8255XCOMMANDBLOCK_FORMAT_OK_POSITION  (13)
#define I8255XCOMMANDBLOCK_FORMAT_C_POSITION  (15)
#define I8255XCOMMANDBLOCK_FORMAT_CMD_POSITION  (16)
#define I8255XCOMMANDBLOCK_FORMAT_SF_POSITION  (19)
#define I8255XCOMMANDBLOCK_FORMAT_NC_POSITION  (20)
#define I8255XCOMMANDBLOCK_FORMAT_CID_POSITION  (24)
#define I8255XCOMMANDBLOCK_FORMAT_I_POSITION  (29)
#define I8255XCOMMANDBLOCK_FORMAT_S_POSITION  (30)
#define I8255XCOMMANDBLOCK_FORMAT_EL_POSITION  (31)

#define I8255XCOMMANDBLOCK_FORMAT_LINKADDR_OFFSET  (0h4)
#define I8255XCOMMANDBLOCK_FORMAT_ADDRDATAFIELD_OFFSET  (0h8)

#define I8255XCOMMANDBLOCK_TRANSMIT_FORMAT_LINE_OFFSET  (0hc)

typedef struct commandblock_general {
	uint32_t command_status_line;
	uint32_t link_address;
	uint32_t addr_data_field1;
} commandblock_general;

typedef struct rfd_header {
    uint32_t status_line;
    uint32_t link_address;
    uint32_t RESERVED;
    uint32_t data_field;
    char* data_area[];
} rfd_header;

//after each command block there is 4 dwords of space reserve for the extended TCB
//even if it is not in use, this is included in the struct for malloc calculation
typedef struct commandblock_transmit {
    struct commandblock_general cb_gen;
    uint32_t transmit_line;
    uint32_t blankSpace1;
    uint32_t blankSpace2;
    uint32_t blankSpace3;
    uint32_t blankSpace4;
    struct ethernet_frame e_frame;
} commandblock_transmit;

typedef struct commandblock_load_ind_addr {
    struct commandblock_general cb_gen;
    uint32_t addr_data_field2;
} commandblock_load_ind_addr;



void set_dest_addr( uint8_t l1, uint8_t l2, uint8_t l3, uint8_t l4,
uint8_t l5,uint8_t l6);

void create_rfd_block(rfd_header* header, uint32_t SF, uint32_t EL, uint32_t SIZE,
        uint32_t link_addr);

int rfd_finished(rfd_header* header);

int rfd_actual_count(rfd_header* header);

void read_rfd_area(rfd_header* header, char* data[]);

/**
 * 
 * @param block TCB with memory already allocated to it
 * @param EL This bit indicates that this is the last block in the CBL
 * @param S This will cause the device to suspend after the command is completed if 1
 * @param I This indicates whether completion of command generates an interrupt
 * @param CID Length of time CNA interrupts are delayed by this device
 * @param NC THIS SHOULD ALWAYS BE 0, it will compute crc and insert source addr for you if this is 0
 * @param SF This should be 0, 0 indicates working in simplified mode
 * @param C This bit is set by the controller, 0 = command not done, 1 = done
 * @param OK This bit is set by the controller, 0 = no error, 1 = error
 * @param U This bit is set to 1 by the device
 * @param linkAddress Address of next command
 * @param bufferDescAddr This should be 0FFFFFFFFh, we are in simp mode
 * @param TBDNumber This should be 0 in simp mode
 * @param transThresh Unsure?
 * @param EOF This should be 0, simp mode requires
 * @param blockByteCount Total number of bytes to be transmitted
 * @param data
 */
void create_transmit_command(commandblock_transmit* block, uint8_t EL, uint8_t S,
        uint8_t I, uint8_t CID, uint8_t NC, uint8_t SF, uint32_t linkAddress, 
        uint32_t bufferDescAddr, uint8_t TBDNumber,
        uint8_t transThresh, uint8_t EOF, uint16_t blockByteCount);

/**
 * 
 * @param block TCB with memory already allocated to it
 * @param EL This bit indicates that this is the last block in the CBL
 * @param S This will cause the device to suspend after the command is completed if 1
 * @param I This indicates whether completion of command generates an interrupt
 * @param linkAddress Address of next command
 * @param source_addr Mac address of this device
 */
void create_load_ind_addr_command(commandblock_load_ind_addr* block, uint8_t EL, uint8_t S,
        uint8_t I, uint32_t linkAddress, mac_address source_addr);

int command_finished(commandblock_general* block);

int command_error(commandblock_general* block);

#endif
