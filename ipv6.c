#include "ipv6.h"

/*
 * Reverse the byte order of a unsigned 32 bit integer
 */
uint32_t reverse_endian_uint32(uint32_t to_reverse){
    uint32_t reversed = 0;
    uint32_t byte_mask = 0b00000000000000000000000011111111;
    for(int i = 0; i < 4; i++){
        reversed = reversed & ((to_reverse & byte_mask << (i*8)) << ((3 - i)*8) );
    }
    return reversed;
}


/**
 * Build an IP packet
 * version: IP version, should be 6
 * trafficClass:
 * flowLabel:
 * payloadLength: Length of payload in bytes
 * nextHeader: location of next ip header
 * hopLimit: Number of Hops the packet can make
 * sourceAddr1: the first word of the source ip address
 * sourceAddr2: the second word of the source ip address
 * sourceAddr3: the third word of the source ip address
 * sourceAddr4: the four word of the source ip address
 * destAddr1: the first word of the destination ip address
 * destAddr2: the second word of the destination ip address
 * destAddr3: the third word of the destination ip address
 * destAddr4: the four word of the destination ip address
 * payloadAddress: Address of the message to put in the ip packet
 * packet: pointer to a malloced structure to place the data in
 */
void build_ip_packet(uint32_t version, uint32_t trafficClass, uint32_t flowLabel, 
	uint32_t payloadLength, uint32_t nextHeader, uint32_t hopLimit, 
	uint32_t sourceAddr1, uint32_t sourceAddr2, 
	uint32_t sourceAddr3, uint32_t sourceAddr4,
	uint32_t destAddr1, uint32_t destAddr2,
 	uint32_t destAddr3, uint32_t destAddr4,
        uint8_t* payloadAddress[], struct ipv6_packet* packet){
    
    set_ip_header(&packet->header, version, trafficClass, flowLabel,
 	payloadLength, nextHeader, hopLimit,
        sourceAddr1, sourceAddr2, sourceAddr3, sourceAddr4,
        destAddr1, destAddr2, destAddr3, destAddr4);
    
	
    *(packet->data) = *(payloadAddress);
}


/** 
 * Build the header portion of the ip packet
 * header: pointer to the header section of the packet
 * version: IP version, should be 6
 * trafficClass:
 * flowLabel:
 * payloadLength: Length of payload in bytes
 * nextHeader: location of next ip header
 * hopLimit: Number of Hops the packet can make
 * sourceAddr1: the first word of the source ip address
 * sourceAddr2: the second word of the source ip address
 * sourceAddr3: the third word of the source ip address
 * sourceAddr4: the four word of the source ip address
 * destAddr1: the first word of the destination ip address
 * destAddr2: the second word of the destination ip address
 * destAddr3: the third word of the destination ip address
 * destAddr4: the four word of the destination ip address
 */
void set_ip_header(struct ipv6_header* header, 
	uint32_t version, uint32_t trafficClass, uint32_t flowLabel, 
	uint32_t payloadLength, uint32_t nextHeader, uint32_t hopLimit, 
	uint32_t sourceAddr1, uint32_t sourceAddr2,
	uint32_t sourceAddr3, uint32_t sourceAddr4,
	uint32_t destAddr1, uint32_t destAddr2, 
	uint32_t destAddr3, uint32_t destAddr4){
	
	
	header->l1 = 0;
	int versionBits = version & IPV6_VERSION_BITMASK;
	header->l1 = header->l1 | (versionBits << IPV6_VERSION_POSMASK);
	
	int trafficClassBits = trafficClass & IPV6_TRAFFICCLASS_BITMASK;
	header->l1 = header->l1 | (trafficClassBits << IPV6_TRAFFICCLASS_POSMASK);
	
	int flowLabelBits = flowLabel & IPV6_FLOWLABEL_BITMASK;
	header->l1 = header->l1 | (flowLabelBits << IPV6_FLOWLABEL_POSMASK);
	
	
	header->l2 = 0;
	int hopLimitBits = hopLimit & IPV6_HOPLIMIT_BITMASK;
	header->l2 = header->l2 | (hopLimitBits << IPV6_HOPLIMIT_POSMASK);
	
	int nextHeaderBits = nextHeader & IPV6_NEXTHEADER_BITMASK;
	header->l2 = header->l2 | (nextHeaderBits << IPV6_NEXTHEADER_POSMASK);
	
	int payloadLengthBits = payloadLength & IPV6_PAYLOADLEN_BITMASK;
	header->l2 = header->l2 | (payloadLengthBits << IPV6_PAYLOADLEN_POSMASK);
	
	
	header->sourceAddr1 = sourceAddr1;
	header->sourceAddr2 = sourceAddr2;
	header->sourceAddr3 = sourceAddr3;
	header->sourceAddr4 = sourceAddr4;
	
	
	header->destAddr1 = destAddr1;
	header->destAddr2 = destAddr2;
	header->destAddr3 = destAddr3;
	header->destAddr4 = destAddr4;
	
}
