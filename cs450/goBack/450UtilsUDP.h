/*  450UtilsUDP.h

    This file contains a number of useful functions for use in CS 450 HW
        
    Written by John T. Bell January 2014 for CS 450
*/

#ifndef CS450UTILSUDP_H
#define CS450UTILSUDP_H

#include <cstdlib>
#include <cstdio>

#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>

#include <netinet/in.h>

#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>

#include "CS450Header7.h"

using namespace std;

void printIP( ostream & out, uint32_t IP );

ostream & operator << ( ostream & out, const CS450Header & h );
ostream & operator << ( ostream & out, const struct sockaddr_in & addr );

uint16_t calcChecksum( void *data, int nbytes ) {
	printf("In checksum()!\n");
	uint16_t *data16 = (uint16_t*)data;
	uint8_t *data8 = (uint8_t*)data;
	uint32_t sum = 0;
	uint32_t max16 = 0xffff;
/* Algorithm:
    1. Typecast data into a uint16_t * data16, and also into a uint8_t * data8
    2. Set uint32_t sum = 0, max16 = 0xffff;
    3. Loop for i = 0 to nbytes / 2
        A. add data16[ i ] to sum
        B. if sum > max16, subtract max16 from sum
    4. if nbytes is odd
        A. add data8[ nbytes - 1 ] to sum
        B. if sum > max16, subtract max16 from sum
    5. Return the one's complement of sum.
    
    To use:
        1. Set checksum field of header to 0
        2. Calculate checksum of Packet 
        3. Store result in checksum field of header.
        4. Now if you calculate the checksum of the packet,
            the result should be zero.
*/
	printf("looping from 0 to %d!\n",nbytes);
	for(int i = 0; i < nbytes; i++) {
		sum+=data16[i];
		if(sum > max16)
			sum-=max16;
		if(nbytes%2 > 0) {
			sum+=data8[nbytes-1];
			if(sum > max16)
				sum-=max16;
		}
		
	}
	
	return ~sum;
}

void packHeader( CS450Header & h, int32_t version, int32_t UIN, 
    int32_t transactionNumber, int32_t sequenceNumber, 
    int32_t ackNumber, uint32_t from_IP, uint32_t to_IP, 
    uint32_t trueFromIP, uint32_t trueToIP, uint32_t nbytes,
    uint32_t nTotalBytes, const char * filename, uint16_t from_Port,
    uint16_t to_Port, uint16_t checksum, int8_t HW_number, int8_t packetType, 
    int8_t relayCommand, int8_t saveFile, const char * ACCC, int8_t dropChance, 
    int8_t dupeChance, int8_t garbleChance, int8_t delayChance, 
    uint8_t windowSize, int8_t protocol ) {

	h.version = htonl(version);
	h.UIN = htonl(UIN);
	h.transactionNumber = htonl(transactionNumber);
	h.sequenceNumber = htonl(sequenceNumber);
	h.ackNumber = htonl(ackNumber);
	h.from_IP = htonl(from_IP);
	h.to_IP = htonl(to_IP);
	h.trueFromIP = htonl(trueFromIP);
	h.trueToIP = htonl(trueToIP);
	h.nbytes = htonl(nbytes);
	h.nTotalBytes = htonl(nTotalBytes);
	//memset(h.filename,"\0",sizeof(h.filename));
	strncpy(h.filename, "\0", sizeof(h.filename));
	strncpy(h.filename, filename, sizeof(filename));
	h.from_Port = htons(from_Port);
	h.to_Port = htons(to_Port);
	h.checksum = htons(checksum);
	h.HW_number = (int8_t) HW_number;
	h.packetType = (int8_t) packetType;
	h.relayCommand = (int8_t) relayCommand;
	h.saveFile = (int8_t) saveFile;
	//memset(h.ACCC,"\0",sizeof(h.ACCC));
	strncpy(h.ACCC, "\0", sizeof(h.ACCC));
	strncpy(h.ACCC, ACCC, sizeof(ACCC));
	h.dropChance = (int8_t) dropChance;
	h.dupeChance = (int8_t) dupeChance;
	h.garbleChance = (int8_t) garbleChance;
	h.delayChance = (int8_t) delayChance;
	h.windowSize = (uint8_t) windowSize;
	h.protocol = (int8_t) protocol;

}
    
void unpackHeader( const CS450Header & h, int32_t & version, int32_t & UIN, 
    int32_t & transactionNumber, int32_t & sequenceNumber, 
    int32_t & ackNumber, uint32_t & from_IP, uint32_t & to_IP, 
    uint32_t & trueFromIP, uint32_t & trueToIP, uint32_t & nbytes,
    uint32_t & nTotalBytes, char * filename, uint16_t & from_Port,
    uint16_t & to_Port, uint16_t & checksum, int8_t & HW_number, 
    int8_t & packetType, int8_t & relayCommand, int8_t & saveFile, char * ACCC,
    int8_t & dropChance, int8_t & dupeChance, int8_t & garbleChance,
    int8_t & delayChance, uint8_t windowSize, int8_t & protocol );

void printInfo(CS450Header & h) {
	
	printf("Header Version: %d\n",ntohl(h.version));
	printf("UIN: %d\n",ntohl(h.UIN));
	printf("Trans. # %d\n",ntohl(h.transactionNumber));
	printf("Seq. # %d\n",ntohl(h.sequenceNumber));
	printf("Ack. # %d\n",ntohl(h.ackNumber));
	printf("# Bytes: %d\n",ntohl(h.nbytes));
	printf("Total Bytes: %d\n",ntohl(h.nTotalBytes));
	printf("File Name: %s\n",h.filename);
	printf("Checksum: %d\n",ntohs(h.checksum));
	printf("HW # %d\n",h.HW_number);
	printf("Packet Type: %d\n",h.packetType);
	printf("Relay Cmd: %d\n",h.relayCommand);
	printf("Save File: %d\n",h.saveFile);
	printf("ACCC: %s\n",h.ACCC);
	printf("Drop Chance: %d\n",h.dropChance);
	printf("Dupe Chance: %d\n",h.dupeChance);
	printf("Garble Chance: %d\n",h.garbleChance);
	printf("Delay Chance: %d\n",h.delayChance);
	printf("Window Size: %d\n",h.windowSize);
	printf("Protocol: %d\n\n",h.protocol);
}

#endif
