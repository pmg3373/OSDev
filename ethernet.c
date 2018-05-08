/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "ethernet.h"

void build_MAC(struct mac_address* mac, uint8_t l1, uint8_t l2, uint8_t l3, uint8_t l4,
uint8_t l5,uint8_t l6){
    mac->l1 = l1;
    mac->l2 = l2;
    mac->l3 = l3;
    mac->l4 = l4;
    mac->l5 = l5;
    mac->l6 = l6;
}

void build_ethernet_frame_wmacs(struct ethernet_frame* frame,
 struct mac_address* dst_mac, struct mac_address* src_mac){
    
    frame->destinationaddress = *dst_mac;
    frame->sourceaddress = *src_mac;
}

void build_ethernet_frame(struct ethernet_frame* frame,
        uint8_t dest_l1, uint8_t dest_l2, uint8_t dest_l3, uint8_t dest_l4,
        uint8_t dest_l5, uint8_t dest_l6,
        uint8_t src_l1, uint8_t src_l2, uint8_t src_l3, uint8_t src_l4,
        uint8_t src_l5, uint8_t src_l6,
	struct ipv6_packet* payload){
    
    build_MAC(&frame->destinationaddress, dest_l1, dest_l2, dest_l3, dest_l4,
         dest_l5, dest_l6);
    
    build_MAC(&frame->sourceaddress, src_l1, src_l2, src_l3, src_l4, src_l5, src_l6);
    
	frame->payload = *payload;
}
