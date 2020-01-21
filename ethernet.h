/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ethernet.h
 * Author: patrick
 *
 * Created on April 26, 2018, 2:15 PM
 */

#ifndef ETHERNET_H
#define ETHERNET_H

#include "ipv6.h"
#include "common.h"

typedef struct preamble{
    uint8_t l1;
    uint8_t l2;
    uint8_t l3;
    uint8_t l4;
    uint8_t l5;
    uint8_t l6;
    uint8_t l7;
} preamble;

/**
 * Structure of a Mac address split into 8 bit fields
 */
typedef struct mac_address{
    uint8_t l1;
    uint8_t l2;
    uint8_t l3;
    uint8_t l4;
    uint8_t l5;
    uint8_t l6;
} mac_address;

/**
 * Structure of and ethernet frame, frame check has space reserved but is
 * calculated but hardware it will be ignored
 */
typedef struct ethernet_frame{
    struct mac_address destinationaddress;
    struct mac_address sourceaddress;
    struct ipv6_packet payload;
    uint32_t framecheckseq;
} ethernet_frame;



void build_MAC(struct mac_address* mac, uint8_t l1, uint8_t l2, uint8_t l3, uint8_t l4,
uint8_t l5,uint8_t l6);

void build_ethernet_frame_wmacs(struct ethernet_frame* frame,
	struct mac_address* dst_mac, struct mac_address* src_mac);

void build_ethernet_frame(struct ethernet_frame* frame,
        uint8_t dest_l1, uint8_t dest_l2, uint8_t dest_l3, uint8_t dest_l4,
        uint8_t dest_l5,uint8_t dest_l6,
        uint8_t src_l1, uint8_t src_l2, uint8_t src_l3, uint8_t src_l4,
        uint8_t src_l5,uint8_t src_l6, 
        struct ipv6_packet* payload);

#endif /* ETHERNET_H */

