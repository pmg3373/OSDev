#ifndef I8255X_DEF
#define I8255X_DEF

#include "common.h"

#ifndef _PCI_H_
    #include "pci.h"
#endif

#ifndef _I8255XCOMMANDBLOCK_H_
	#include "i8255xcommandblock.h"
#endif

#include "kmalloc.h"
#include "startup.h"
#include "support.h"
#include "c_io.h"


#define I8255X_SCBSTATUS_WORD_OFFSET  (0)
//table 13 page 35 pf intel 8255x manual describes SCB status word bits
#define I8255X_SCBSTATUS_CXTNO_MASK  (1 << 15)
#define I8255X_SCBSTATUS_FR_MASK  (1 << 14)
#define I8255X_SCBSTATUS_CNA_MASK  (1 << 13)
#define I8255X_SCBSTATUS_RNR_MASK  (1 << 12)
#define I8255X_SCBSTATUS_MDI_MASK  (1 << 11)
#define I8255X_SCBSTATUS_SWI_MASK  (1 << 10)
#define I8255X_SCBSTATUS_FCP_MASK  (1 << 8)
#define I8255X_SCBSTATUS_CUS_MASK  (0b11 << 6)
#define I8255X_SCBSTATUS_RUS_MASK  (0b1111 << 2)

#define I8255X_SCBSTATUS_CXTNO_SHIFT  (15)
#define I8255X_SCBSTATUS_FR_SHIFT  (14)
#define I8255X_SCBSTATUS_CNA_SHIFT  (13)
#define I8255X_SCBSTATUS_RNR_SHIFT  (12)
#define I8255X_SCBSTATUS_MDI_SHIFT  (11)
#define I8255X_SCBSTATUS_SWI_SHIFT  (10)
#define I8255X_SCBSTATUS_FCP_SHIFT  (8)
#define I8255X_SCBSTATUS_CUS_SHIFT  (6)
#define I8255X_SCBSTATUS_RUS_SHIFT  (2)

#define I8255X_CU_IDLE  (0b00)
#define I8255X_CU_SUSPENDED  (0b01)
#define I8255X_CU_LPQ_ACTIVE  (0b10)
#define I8255X_CU_HPQ_ACTIVE  (0b11)

#define I8255X_RU_IDLE  (0b0000)
#define I8255X_RU_SUSPENDED  (0b0001)
#define I8255X_RU_NO_RESOURCES  (0b0010)
#define I8255X_RU_READY  (0b0100)

//page 37 describes the interrupts bits of the SCB command word in further detail
#define I8255X_SCBCOMMAND_CX_MASK  (1 << 31)
#define I8255X_SCBCOMMAND_FR_MASK  (1 << 30)
#define I8255X_SCBCOMMAND_CNA_MASK  (1 << 29)
#define I8255X_SCBCOMMAND_RNR_MASK  (1 << 28)
#define I8255X_SCBCOMMAND_ER_MASK  (1 << 27)
#define I8255X_SCBCOMMAND_FCP_MASK  (1 << 26)
#define I8255X_SCBCOMMAND_SI_MASK  (1 << 25)
#define I8255X_SCBCOMMAND_M_MASK  (1 << 24)

//page 38 describes the CU commands
#define I8255X_SCBCOMMAND_OFFSET  (0x2)
#define I8255X_SCBCOMMAND_CUC_MASK  (0b11110000 << 16)
#define I8255X_SCBCOMMAND_CUC_SHIFT  (16)
#define I8255X_SCBCOMMAND_CU_START  (0b00010000)
#define I8255X_SCBCOMMAND_CU_RESUME  (0b00100000)
#define I8255X_SCBCOMMAND_CU_LOADDUMP  (0b01000000)
#define I8255X_SCBCOMMAND_CU_DUMPSTAT  (0b01010000)
#define I8255X_SCBCOMMAND_CU_LOADCUBASE  (0b01100000)
#define I8255X_SCBCOMMAND_CU_DUMPRESET  (0b01110000)
#define I8255X_SCBCOMMAND_CU_STATICRESUME  (0b10100000)
#define I8255X_SCBCOMMAND_CU_NOP  (0b00000000)

//page 39 describes the RU commands
#define I8255X_SCBCOMMAND_RUC_MASK  (0b00000111 << 16)
#define I8255X_SCBCOMMAND_RUC_SHIFT  (16)
#define I8255X_SCBCOMMAND_RU_NOP  (0b00000000)
#define I8255X_SCBCOMMAND_RU_START (0b00000001)
#define I8255X_SCBCOMMAND_RU_RESUME  (0b00000010)
#define I8255X_SCBCOMMAND_RU_RECEIVEDMAREDIRECT  (0b00000011)
#define I8255X_SCBCOMMAND_RU_ABORT  (0b00000100)
#define I8255X_SCBCOMMAND_RU_LOADHDS  (0b00000101)
#define I8255X_SCBCOMMAND_RU_LOADRUBASE  (0b00000110)

#define I8255X_SCBGNRLPTR_OFFSET  (0x4)

#define I8255X_PORT_OFFSET  (0x8)
#define I8255X_PORT_SOFTRESET  (0b0000)
#define I8255X_PORT_SELFTEST  (0b0001)
#define I8255X_PORT_SELECTRESET  (0b0010)
#define I8255X_PORT_DUMP  (0b0011)
#define I8255X_PORT_DUMPWAKEUP  (0b0111)

#define I8255X_EEPROMCTRLREG_OFFSET  (0xE)
#define I8255X_MDICTRLREG_OFFSET  (0x10)
#define I8255X_RXDMABTCNT_OFFSET  (0x14)
#define I8255X_FLOWCTRLREG_OFFSET  (0x19)
#define I8255X_GCTRL_OFFSET (0x1C)
#define I8255X_GSTAT_OFFSET  (0x1D)
#define I8255X_FUNCEVENTREG_OFFSET  (0x30)
#define I8255X_FUNCEVENTMASKREG_OFFSET  (0x34)
#define I8255X_FUNCPRESSTATEREG_OFFSET  (0x38)
#define I8255X_FORCEEVENTREG_OFFSET  (0x3C)




//*******************************CU COMMANDS************************************

void cu_start(int cbl_start_pointer);

void cu_resume(int cbl_start_pointer);

void cu_load_dump(void);

void cu_dump_stat(void);

void cu_load_cu_base(int cu_base_addr);

void cu_dump_reset(void);

void cu_static_resume(void);

void cu_nop(void);

//*******************************CU COMMANDS************************************



//*******************************RU COMMANDS************************************

void ru_nop(void);

void ru_start(int rfa_start_pointer);

void ru_resume(int rfa_start_pointer);

void ru_recieve_ma_redirect(void);

void ru_abort(void);

void ru_load_hds(void);

void ru_load_ru_base(int ru_base);

//*******************************RU COMMANDS************************************



int poll_ru_status(void);

int poll_cu_status(void);

int wait_until_cu_x(int desired_cu_status);

int wait_until_ru_x(int desired_ru_status);

void polled_read(void);

void transmit_single_block(uint8_t* data_address[], uint32_t data_length, 
        struct mac_address* destination_mac);

void i8255X_setup(void);

void _i8255X_test_send(void);

void i8255x_init(int pci_base_addr);



#endif
