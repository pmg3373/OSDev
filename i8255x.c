#include "i8255x.h"

uint32_t scb_base_addr;
uint32_t pci_base_addr;

struct mac_address src_mac_addr;
struct ip_addr src_ip_addr;
struct mac_address dst_mac_addr;
struct ip_addr dst_ip_addr;

struct rfd_header* current_rfd_header;


/**
 * Build the RFA and set the current header
 */
void setup_rfa(void){
    struct rfd_header* header1 
        = _kmalloc(sizeof(rfd_header) + IPV6_MAX_PACKET_SIZE);
    struct rfd_header* header2 
        = _kmalloc(sizeof(rfd_header) + IPV6_MAX_PACKET_SIZE);
    struct rfd_header* header3 
        = _kmalloc(sizeof(rfd_header) + IPV6_MAX_PACKET_SIZE);
    struct rfd_header* header4 
        = _kmalloc(sizeof(rfd_header) + IPV6_MAX_PACKET_SIZE);
    
    create_rfd_block(header1, 0, 0, IPV6_MAX_PACKET_SIZE, (uint32_t)(&header2));
    create_rfd_block(header2, 0, 0, IPV6_MAX_PACKET_SIZE, (uint32_t)(&header3));
    create_rfd_block(header3, 0, 0, IPV6_MAX_PACKET_SIZE, (uint32_t)(&header4));
    create_rfd_block(header4, 0, 0, IPV6_MAX_PACKET_SIZE, (uint32_t)(&header1));
    
    current_rfd_header = header1;
}


/**
 * Poll the status of the RU
 */
int poll_ru_status(void){
   return (inw_wrap(scb_base_addr + I8255X_SCBSTATUS_WORD_OFFSET) & I8255X_SCBSTATUS_RUS_MASK) 
           >> I8255X_SCBSTATUS_RUS_SHIFT;
}

/**
 * Poll the status of the CU
 */
int poll_cu_status(void){
   return (inw_wrap(scb_base_addr + I8255X_SCBSTATUS_WORD_OFFSET) & I8255X_SCBSTATUS_CUS_MASK) 
           >> I8255X_SCBSTATUS_CUS_SHIFT;
}

/**
 * Enter a loop of polling the CU and waiting until it is the desired status
 */
int wait_until_cu_x(int desired_cu_status){
    if( (desired_cu_status != I8255X_CU_IDLE) 
            && (desired_cu_status != I8255X_CU_SUSPENDED)
            && (desired_cu_status != I8255X_CU_LPQ_ACTIVE) 
            && (desired_cu_status != I8255X_CU_HPQ_ACTIVE)){
        return -1;
    }
    while(!( poll_cu_status() == desired_cu_status )){
	//c_printf("WAITING FOR CU TO GO %d CURRENT %d \n",
		 desired_cu_status, poll_cu_status());
	__delay(100);
    }
    return poll_cu_status();
}

/**
 * Enter a loop of polling the RU and waiting until it is the desired status
 */
int wait_until_ru_x(int desired_ru_status){
    if( (desired_ru_status != I8255X_RU_IDLE) 
            && (desired_ru_status != I8255X_RU_SUSPENDED)
            && (desired_ru_status != I8255X_RU_NO_RESOURCES) 
            && (desired_ru_status != I8255X_RU_READY)){
        return -1;
    }
    while(!( poll_ru_status() == desired_ru_status )){
	__delay(100);
    }
    return poll_ru_status();
}

/**
 * Wait until the RU has finished reading then print the results
 */
void polled_read(void){

    while(!rfd_finished(current_rfd_header)){
	__delay(100);
    }

    char* data[rfd_actual_count(current_rfd_header)];
    read_rfd_area(current_rfd_header, (char**)(&data));
    current_rfd_header = (struct rfd_header*)(current_rfd_header->link_address);
	//_add_window(x,y,h,w, fg_col, bg_col, title, buffer);
	//in kwindow.h
}

//*******************************CU COMMANDS************************************

/**
 * Issue a start command to the CU
 * cbl_start_pointer address of the cbl to execute
 */
void cu_start(int cbl_start_pointer){
	//outl_wrap( int port, int value );
	//c_printf("Starting CU with %d at location %d \n",
(int)(scb_base_addr + I8255X_SCBGNRLPTR_OFFSET), cbl_start_pointer);
__delay(1000);
	outl_wrap((int)(scb_base_addr + I8255X_SCBGNRLPTR_OFFSET), cbl_start_pointer);
	outw_wrap((int)(scb_base_addr + I8255X_SCBCOMMAND_OFFSET),
		I8255X_SCBCOMMAND_CU_START);

/*
    *(scb_base_addr + I8255X_SCBGNRLPTR_OFFSET) = cbl_start_pointer;
    *(scb_base_addr + I8255X_SCBCOMMAND_CUC_SHIFT) 
            = I8255X_SCBCOMMAND_CU_START;
*/
}

/**
 * Issue a resume command to the CU
 * cbl_start_pointer address of the cbl to execute
 */
void cu_resume(int cbl_start_pointer){
	outl_wrap((int)(scb_base_addr + I8255X_SCBGNRLPTR_OFFSET), cbl_start_pointer);
	outw_wrap((int)(scb_base_addr + I8255X_SCBCOMMAND_OFFSET),
		I8255X_SCBCOMMAND_CU_RESUME);

/*
    *(scb_base_addr + I8255X_SCBGNRLPTR_OFFSET) = cbl_start_pointer;
    *(scb_base_addr + I8255X_SCBCOMMAND_CUC_SHIFT)
            = I8255X_SCBCOMMAND_CU_RESUME;
*/
}

/**
 * Issue a load dump command to the CU
 */
void cu_load_dump(void){
	outw_wrap((int)(scb_base_addr + I8255X_SCBCOMMAND_OFFSET),
		I8255X_SCBCOMMAND_CU_LOADDUMP);

/*
    *(scb_base_addr + I8255X_SCBCOMMAND_CUC_SHIFT) 
            = I8255X_SCBCOMMAND_CU_LOADDUMP;
*/
}

/**
 * Issuer a dump stat command to the CU
 */
void cu_dump_stat(void){
	outw_wrap((int)(scb_base_addr + I8255X_SCBCOMMAND_OFFSET),
		I8255X_SCBCOMMAND_CU_DUMPSTAT);
/*
    *(scb_base_addr + I8255X_SCBCOMMAND_CUC_SHIFT) = I8255X_SCBCOMMAND_CU_STAT;
*/
}

/**
 * Issue a load CU base command to the CU
 * cu_base_addr Address to load as the base CU address
 */
void cu_load_cu_base(int cu_base_addr){
	outl_wrap((int)(scb_base_addr + I8255X_SCBGNRLPTR_OFFSET), cu_base_addr);
	outw_wrap((int)(scb_base_addr + I8255X_SCBCOMMAND_OFFSET),
		I8255X_SCBCOMMAND_CU_LOADCUBASE);

/*
    *(scb_base_addr + I8255X_SCBGNRLPTR_OFFSET) = cu_base_addr;
    *(scb_base_addr + I8255X_SCBCOMMAND_CUC_SHIFT) 
            = I8255X_SCBCOMMAND_CU_LOADCUBASE;
*/
}

/**
 * Issue a dump reset command to the CU
 */
void cu_dump_reset(void){
	outw_wrap((int)(scb_base_addr + I8255X_SCBCOMMAND_OFFSET),
		I8255X_SCBCOMMAND_CU_DUMPRESET);

/*
    *(scb_base_addr + I8255X_SCBCOMMAND_CUC_SHIFT) = I8255X_SCBCOMMAND_CU_RESET;
*/
}

/**
 * Issue a static resume command to the CU
 */
void cu_static_resume(void){
	outw_wrap((int)(scb_base_addr + I8255X_SCBCOMMAND_OFFSET),
		I8255X_SCBCOMMAND_CU_RESUME);

/*
    *(scb_base_addr + I8255X_SCBCOMMAND_CUC_SHIFT) 
            = I8255X_SCBCOMMAND_CU_RESUME;
*/
}

/**
 * Issue a nop command to the CU
 */
void cu_nop(void){
	outw_wrap((int)(scb_base_addr + I8255X_SCBCOMMAND_OFFSET),
		I8255X_SCBCOMMAND_CU_NOP);

/*
    *(scb_base_addr + I8255X_SCBCOMMAND_CUC_SHIFT) = I8255X_SCBCOMMAND_CU_NOP;
*/
}
//*******************************CU COMMANDS************************************



//*******************************RU COMMANDS************************************

/**
 * Issue a nop command to the RU
 */
void ru_nop(void){
	outw_wrap((int)(scb_base_addr + I8255X_SCBCOMMAND_OFFSET),
		I8255X_SCBCOMMAND_RU_NOP);

/*
    *(scb_base_addr + I8255X_SCBCOMMAND_RUC_SHIFT) = I8255X_SCBCOMMAND_RU_NOP;
*/
}

/**
 * Issue a start command to the RU
 * rfa_start_pointer pointer to the first RFD header
 */
void ru_start(int rfa_start_pointer){

	outl_wrap((int)(scb_base_addr + I8255X_SCBGNRLPTR_OFFSET), rfa_start_pointer);
	outw_wrap((int)(scb_base_addr + I8255X_SCBCOMMAND_OFFSET),
		I8255X_SCBCOMMAND_RU_START);

/*
    *(scb_base_addr + I8255X_SCBGNRLPTR_OFFSET) = rfa_start_pointer;
    *(scb_base_addr + I8255X_SCBCOMMAND_RUC_SHIFT) = I8255X_SCBCOMMAND_RU_START;
*/
}

/**
 * Issue a resume command to the RU
 * rfa_start_pointer pointer to the first RFD header
 */
void ru_resume(int rfa_start_pointer){

	outl_wrap((int)(scb_base_addr + I8255X_SCBGNRLPTR_OFFSET), rfa_start_pointer);
	outw_wrap((int)(scb_base_addr + I8255X_SCBCOMMAND_OFFSET),
		I8255X_SCBCOMMAND_RU_RESUME);

/*
    *(scb_base_addr + I8255X_SCBGNRLPTR_OFFSET) = cbl_start_pointer;
    *(scb_base_addr + I8255X_SCBCOMMAND_RUC_SHIFT) 
            = I8255X_SCBCOMMAND_RU_RESUME;
*/
}

/**
 * Issue a receive MA Redirect command to the RU
 */
void ru_recieve_ma_redirect(){

	outw_wrap((int)(scb_base_addr + I8255X_SCBCOMMAND_OFFSET),
		I8255X_SCBCOMMAND_RU_RECEIVEDMAREDIRECT);
	
/*
    *(scb_base_addr + I8255X_SCBCOMMAND_RUC_SHIFT)
            = I8255X_SCBCOMMAND_RU_RECIEVEMAREDIRECT;
*/
}

/**
 * Issue a abort command to the RU
 */
void ru_abort(void){

	outw_wrap((int)(scb_base_addr + I8255X_SCBCOMMAND_OFFSET),
		I8255X_SCBCOMMAND_RU_ABORT);


/*
    *(scb_base_addr + I8255X_SCBCOMMAND_RUC_SHIFT) 
            = I8255X_SCBCOMMAND_RU_ABORT;
*/
}

/**
 * Issue a load hds command to the RU
 */
void ru_load_hds(void){

	outw_wrap((int)(scb_base_addr + I8255X_SCBCOMMAND_OFFSET),
		I8255X_SCBCOMMAND_RU_LOADHDS);

/*
    *(scb_base_addr + I8255X_SCBCOMMAND_RUC_SHIFT) 
            = I8255X_SCBCOMMAND_RU_LOADHDS;
*/
}

/**
 * Issue a load ru base command to the RU
 * ru_base base address for the RU
 */
void ru_load_ru_base(int ru_base){
	//c_printf("Loading RU base \n");
	int status = poll_ru_status();
	//c_printf("Current RU Status: %d", status);

	outl_wrap((int)(scb_base_addr + I8255X_SCBGNRLPTR_OFFSET), ru_base);
	outw_wrap((int)(scb_base_addr + I8255X_SCBCOMMAND_OFFSET),
		I8255X_SCBCOMMAND_RU_LOADRUBASE);

/*
    *(scb_base_addr + I8255X_SCBGNRLPTR_OFFSET) = ru_base_addr;
    *(scb_base_addr + I8255X_SCBCOMMAND_RUC_SHIFT) 
            = I8255X_SCBCOMMAND_RU_LOADRUBASE;
*/
}
//*******************************RU COMMANDS************************************




/**
 * Transmit a single block of data to a given address
 * Goes through the entire process of creating the CBL and issuing a start cmd
 * data_address location of an array of char to send
 * data_length length of the char array in bytes
 * destination_mac mac address of the destination
 * dest_ip IP address of the destination
 */
void transmit_single_block(uint8_t* data_address[], uint32_t data_length, 
        struct mac_address* destination_mac, struct ip_addr* dest_ip){
    
    void* com_block = _kmalloc(sizeof(commandblock_transmit) + data_length);
	
//    struct commandblock_transmit com_block;
//	com_block.e_frame.payload.data = _kmalloc(data_length);

    build_ip_packet(6, 0, 0,
	data_length, 0, 0b11111111,
 	src_ip_addr.addr1, src_ip_addr.addr2,
	src_ip_addr.addr3, src_ip_addr.addr4,
 	dest_ip_addr.addr1, dest_ip_addr.addr2,
 	dest_ip_addr.addr3, dest_ip_addr.addr4, 
	data_address, &((commandblock_transmit*)com_block)->e_frame.payload);
    
    
    build_ethernet_frame_wmacs( &((commandblock_transmit*)com_block)->e_frame, destination_mac,
            &src_mac_addr);
    
    
    create_transmit_command(((commandblock_transmit*)com_block), 1, 0, 0, 0, 0, 0, 
            0, 0, 0, 1, 1, data_length);
    
    wait_until_cu_x(I8255X_CU_IDLE);
    ru_start((int)&com_block);
    wait_until_cu_x(I8255X_CU_IDLE);
    _kfree(&com_block);
}

/**
* This function is called to setup the device on boot or reset
* First the RFA is created, then the cu base and ru base are loaded to 0
 * then the RU is issued a start command
 * then an Load Individual Address command is created, then a CU start is issued
 * with that load_ind_addr command
*/
void i8255X_setup(void){
	//c_printf("START OF I8255X SETUP \n\n");
	//c_printf("Setting RFA START \n");
    setup_rfa();
    	//c_printf("Setting RFA END \n");

    //Absolute addressing, since we are not MMIO
	//c_printf("Setting RU Base START \n");
    ru_load_ru_base(0);
	//c_printf("Setting RU Base END \n");
	//c_printf("Setting CU Base START \n");
    cu_load_cu_base(0);
	//c_printf("Setting CU Base END \n");
    
	//c_printf("Telling RU to Start START \n");
    ru_start((int)current_rfd_header);
	//c_printf("Telling RU to Start END \n");

    	//c_printf("Creating IND ADDR CMD BLOCK Area START \n");
    struct commandblock_load_ind_addr* ind_addr_command 
        = _kmalloc(sizeof(commandblock_load_ind_addr));
	//c_printf("Creating IND ADDR CMD BLOCK Area END \n");

    	//c_printf("Creating IND ADDR CMD BLOCK START \n");
    create_load_ind_addr_command(ind_addr_command, 1, 0, 0, 0, src_mac_addr);
	__delay(1000);
	//c_printf("Creating IND ADDR CMD BLOCK END \n");

	//c_printf("Telling CU to Start START \n");
    cu_start((int)ind_addr_command);
    	//c_printf("Telling CU to Start END \n");

	//c_printf("Waiting for CU to go idle START \n");
    wait_until_cu_x(I8255X_CU_IDLE);
    	//c_printf("Waiting for CU to go idle END \n");

    _kfree(ind_addr_command);
	//c_printf("END OF I8255X SETUP \n\n");
}


/**
 * Test send function, sends a single block of 20 'A' to the hard coded dest
 */
void _i8255X_test_send(void){

	//c_printf("Beginning Test Send \n");
	
	uint8_t data[20];

	data[0] = 0x41;
	data[1] = 0x41;
	data[2] = 0x41;
	data[3] = 0x41;

	data[4] = 0x41;
	data[5] = 0x41;
	data[6] = 0x41;
	data[7] = 0x41;

	data[8] = 0x41;
	data[9] = 0x41;
	data[10] = 0x41;
	data[11] = 0x41;

	data[12] = 0x41;
	data[13] = 0x41;
	data[14] = 0x41;
	data[15] = 0x41;

	data[16] = 0x41;
	data[17] = 0x41;
	data[18] = 0x41;
	data[19] = 0x41;
	
	//c_printf("Test Hardcode Data Set \n");
	transmit_single_block((uint8_t**)(&data), 20, &dst_mac_addr);
}


/**
 * Initialization code for the I8255x called on boot only
 */
void i8255x_init(int pci_base_addr){
    //this.pci_base_addr = pci_base_addr;
//    scb_base_addr = (*(pcb_base_addr + PCI_IOMAPBAR_01H));
	//c_printf("Setting SCB base addr START \n");

	//c_printf("PCI Addr: %u \n", pci_base_addr);

	//int vendor_id = inw_wrap(pci_base_addr);
	//c_printf("Vend Id: %u \n", vendor_id);


	//int entire_line = inl_wrap(pci_base_addr);
	//c_printf("TEST: %u \n", entire_line);

        /*
	int testo, test_l1, test_l2, temp;
	for(int i = 0; i < 6; i++){
		testo = inw_wrap(pci_base_addr + i*4);
		c_printf("Word Read (%u, %u) \n", i*4, testo);
		temp = inl_wrap(pci_base_addr + i*4);
		test_l1 = temp & 0b1111111111111111;
		test_l2 = (temp & (0b1111111111111111 << 16)) >> 16;
		c_printf("Long Read (%u, %u) \n", i*4, temp);
		c_printf("uLong Read (%u, %u) \n", i*4, test_l2);
		c_printf("lLong Read (%u, %u) \n\n", i*4, test_l1);
		__delay(700);
	}


	scb_base_addr = inl_wrap(pci_base_addr + 20) >> 4;
    	c_printf("SCB base addr Before: %u \n", scb_base_addr);

	outw_wrap(pci_base_addr + 4, 1);

	scb_base_addr = inl_wrap(pci_base_addr + 20) >> 4;
    	c_printf("SCB base addr After: %u \n", scb_base_addr);

	

	__delay(1000);
        */

	//DEVICE HARDCODING

	//c_printf("Setting Hardcoded IP and Macs START  \n");
	//Sholto
	src_ip_addr.addr1 = 10;
	src_ip_addr.addr2 = 10;
	src_ip_addr.addr3 = 10;
	src_ip_addr.addr4 = 18;

	src_mac_addr.l1 = 0x70;
	src_mac_addr.l2 = 0x4d;
	src_mac_addr.l3 = 0x7b;
	src_mac_addr.l4 = 0x84;
	src_mac_addr.l5 = 0x39;
	src_mac_addr.l6 = 0x83;

/*

	//Wiggins
        dst_mac_addr.l1 = 0x00;
	dst_mac_addr.l2 = 0x90;
	dst_mac_addr.l3 = 0x27;
	dst_mac_addr.l4 = 0x90;
	dst_mac_addr.l5 = 0x29;
	dst_mac_addr.l6 = 0x9d;

	dst_ip_addr.addr1 = 10;
	dst_ip_addr.addr1 = 10;
	dst_ip_addr.addr1 = 10;
	dst_ip_addr.addr1 = 19;

*/

	//My Laptop
	dst_mac_addr.l1 = 0x28;
	dst_mac_addr.l2 = 0xd2;
	dst_mac_addr.l3 = 0x44;
	dst_mac_addr.l4 = 0x24;
	dst_mac_addr.l5 = 0xdb;
	dst_mac_addr.l6 = 0x49;

	dst_ip_addr.addr1 = 0xfe08;
	dst_ip_addr.addr2 = 0x2d8b;
	dst_ip_addr.addr3 = 0x8db7;
	dst_ip_addr.addr4 = 0xa56f;

	//c_printf("Setting Hardcoded IP and Macs START  \n");
	
	i8255X_setup();
}



