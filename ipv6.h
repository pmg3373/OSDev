#ifndef IPV6_DEF
#define IPV6_DEF

#include "common.h"

#define IPV6_MAX_PACKET_SIZE  65534

#define IPV6_FLOWLABEL_BITMASK  (0b11111111111111111111)
#define IPV6_TRAFFICCLASS_BITMASK  (0b11111111)
#define IPV6_VERSION_BITMASK  (0b1111)
#define IPV6_HOPLIMIT_BITMASK  (0b11111111)
#define IPV6_NEXTHEADER_BITMASK  (0b11111111)
#define IPV6_PAYLOADLEN_BITMASK  (0b1111111111111111)

#define IPV6_FLOWLABEL_POSMASK  (0)
#define IPV6_TRAFFICCLASS_POSMASK  (20)
#define IPV6_VERSION_POSMASK  (28)
#define IPV6_HOPLIMIT_POSMASK  (0)
#define IPV6_NEXTHEADER_POSMASK  (8)
#define IPV6_PAYLOADLEN_POSMASK  (16)


enum traffic_classes{
    default_class = 0,
    expedited = 46
};


/**
 * Structure of an IP address as 4 longs
 */
typedef struct ip_addr{
    uint32_t addr1;
    uint32_t addr2;
    uint32_t addr3;
    uint32_t addr4;
} ip_addr;

/**
 * Structure of an IPv^ header
 */
typedef struct ipv6_header {
    uint32_t l1;

    uint32_t l2;

    uint32_t sourceAddr1;
    uint32_t sourceAddr2;
    uint32_t sourceAddr3;
    uint32_t sourceAddr4;

    uint32_t destAddr1;
    uint32_t destAddr2;
    uint32_t destAddr3;
    uint32_t destAddr4;
} ipv6_header;

/**
 * Structure of an ip packet with a header
 */
typedef struct ipv6_packet {
    struct ipv6_header header;
    uint8_t* data[];
} ipv6_packet;


uint32_t reverse_endian_uint32(uint32_t to_reverse);


void set_ip_header(struct ipv6_header* header, 
	uint32_t version, uint32_t trafficClass, uint32_t flowLabel, 
	uint32_t payloadLength, uint32_t nextHeader, uint32_t hopLimit, 
	uint32_t sourceAddr1, uint32_t sourceAddr2,
	uint32_t sourceAddr3, uint32_t sourceAddr4,
	uint32_t destAddr1, uint32_t destAddr2, 
	uint32_t destAddr3, uint32_t destAddr4);

void build_ip_packet(uint32_t version, uint32_t trafficClass, uint32_t flowLabel, 
	uint32_t payloadLength, uint32_t nextHeader, uint32_t hopLimit, 
	uint32_t sourceAddr1, uint32_t sourceAddr2, 
	uint32_t sourceAddr3, uint32_t sourceAddr4,
	uint32_t destAddr1, uint32_t destAddr2, 
	uint32_t destAddr3, uint32_t destAddr4,
        uint8_t* payloadAddress[], struct ipv6_packet* packet);

#endif
