/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "i8255xcommandblock.h"
#include "ipv6.h"
#include "ethernet.h"

mac_address sourceAddress;
mac_address destinationAddress;

void set_dest_addr( uint8_t l1, uint8_t l2, uint8_t l3, uint8_t l4,
uint8_t l5,uint8_t l6){
    destinationAddress.l1 = l1;
    destinationAddress.l2 = l2;
    destinationAddress.l3 = l3;
    destinationAddress.l4 = l4;
    destinationAddress.l5 = l5;
    destinationAddress.l6 = l6;
}


void create_rfd_block(rfd_header* header, uint32_t SF, uint32_t EL, uint32_t SIZE,
        uint32_t link_addr){

    header->status_line = 0;
    header->status_line = header->status_line & 
            ( (SF << I8255XRFD_FORMAT_SF_POS) & I8255XRFD_FORMAT_SF_MASK );
    header->status_line = header->status_line & 
            ( (EL << I8255XRFD_FORMAT_EL_POS) & I8255XRFD_FORMAT_EL_MASK );
    
    header->data_field = 0;
    header->data_field = header->data_field & 
            ( (SIZE << I8255XRFD_FORMAT_SIZE_POS) & I8255XRFD_FORMAT_SIZE_MASK );
    header->link_address = link_addr;
}

int rfd_finished(rfd_header* header){
    return (header->status_line & I8255XRFD_FORMAT_C_MASK) 
            >> I8255XRFD_FORMAT_C_POS;
}

int rfd_actual_count(rfd_header* header){
    return (header->data_field & I8255XRFD_FORMAT_ACTUAL_COUNT_MASK) 
            >> I8255XRFD_FORMAT_ACTUAL_COUNT_POS;
}

void read_rfd_area(rfd_header* header, char* data[]){
    *data = *header->data_area;
}

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
        uint8_t transThresh, uint8_t EOF, uint16_t blockByteCount){
    
    block->cb_gen.command_status_line = 0;
    block->cb_gen.command_status_line = block->cb_gen.command_status_line | 
            ( (EL << I8255XCOMMANDBLOCK_FORMAT_EL_POSITION) & I8255XCOMMANDBLOCK_FORMAT_EL_MASK );
    block->cb_gen.command_status_line = block->cb_gen.command_status_line | 
            ( (S << I8255XCOMMANDBLOCK_FORMAT_S_POSITION) & I8255XCOMMANDBLOCK_FORMAT_S_MASK );
    block->cb_gen.command_status_line = block->cb_gen.command_status_line | 
            ( (I << I8255XCOMMANDBLOCK_FORMAT_I_POSITION) & I8255XCOMMANDBLOCK_FORMAT_I_MASK );
    block->cb_gen.command_status_line = block->cb_gen.command_status_line | 
            ( (CID << I8255XCOMMANDBLOCK_FORMAT_CID_POSITION) & I8255XCOMMANDBLOCK_FORMAT_CID_MASK );
    block->cb_gen.command_status_line = block->cb_gen.command_status_line | 
            ( (CID << I8255XCOMMANDBLOCK_FORMAT_CID_POSITION) & I8255XCOMMANDBLOCK_FORMAT_CID_MASK );
    block->cb_gen.command_status_line = block->cb_gen.command_status_line | 
            ( (NC << I8255XCOMMANDBLOCK_FORMAT_NC_POSITION) & I8255XCOMMANDBLOCK_FORMAT_NC_MASK );
    block->cb_gen.command_status_line = block->cb_gen.command_status_line | 
            ( (SF << I8255XCOMMANDBLOCK_FORMAT_SF_POSITION) & I8255XCOMMANDBLOCK_FORMAT_SF_MASK );
    
    block->cb_gen.link_address = linkAddress;
    block->cb_gen.addr_data_field1 = bufferDescAddr;
}

/**
 * 
 * @param block TCB with memory already allocated to it
 * @param EL This bit indicates that this is the last block in the CBL
 * @param S This will cause the device to suspend after the command is completed if 1
 * @param I This indicates whether completion of command generates an interrupt
 * @param linkAddress Address of next command
 * @param source_addr Mac address of this device
 */
void create_load_ind_addr_command(commandblock_load_ind_addr* block, uint8_t EL,
uint8_t S, uint8_t I, uint32_t linkAddress, mac_address source_addr){

    block->cb_gen.command_status_line = 0;

	c_printf("Setting Command Status Line START \n");
    block->cb_gen.command_status_line = block->cb_gen.command_status_line | 
            ( (EL << I8255XCOMMANDBLOCK_FORMAT_EL_POSITION) & I8255XCOMMANDBLOCK_FORMAT_EL_MASK );
	c_printf("Set EL, CSL now is %d \n", block->cb_gen.command_status_line);

    block->cb_gen.command_status_line = block->cb_gen.command_status_line | 
            ( (S << I8255XCOMMANDBLOCK_FORMAT_S_POSITION) & I8255XCOMMANDBLOCK_FORMAT_S_MASK );
	c_printf("Set S, CSL now is %d \n", block->cb_gen.command_status_line);

    block->cb_gen.command_status_line = block->cb_gen.command_status_line | 
            ( (I << I8255XCOMMANDBLOCK_FORMAT_I_POSITION) & I8255XCOMMANDBLOCK_FORMAT_I_MASK );
	c_printf("Set I, CSL now is %d \n", block->cb_gen.command_status_line);

	block->cb_gen.command_status_line = block->cb_gen.command_status_line | 
        	( (I8255XCOMMANDBLOCK_INDADDRSETUP <<
		I8255XCOMMANDBLOCK_FORMAT_CMD_POSITION)
 		& I8255XCOMMANDBLOCK_FORMAT_CMD_MASK );
	c_printf("Set CMD, CSL now is %d \n", block->cb_gen.command_status_line);
    
     block->cb_gen.link_address = linkAddress;
     
     block->cb_gen.addr_data_field1 = 0;
     block->cb_gen.addr_data_field1 = block->cb_gen.addr_data_field1 & (source_addr.l1);
     block->cb_gen.addr_data_field1 = block->cb_gen.addr_data_field1 & (source_addr.l2 << 8);
     block->cb_gen.addr_data_field1 = block->cb_gen.addr_data_field1 & (source_addr.l3 << 16);
     block->cb_gen.addr_data_field1 = block->cb_gen.addr_data_field1 & (source_addr.l4 << 24);
     
     block->addr_data_field2 = 0;
     block->addr_data_field2 = block->addr_data_field2 & source_addr.l5;
     block->addr_data_field2 = block->addr_data_field2 & (source_addr.l6 << 8);
}

int command_finished(commandblock_general* block){
    return block->command_status_line & I8255XCOMMANDBLOCK_FORMAT_C_MASK;
}

int command_error(commandblock_general* block){
    return block->command_status_line & I8255XCOMMANDBLOCK_FORMAT_OK_MASK;
}
