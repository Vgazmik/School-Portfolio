#include "CS450Header7.h"
#include "450UtilsUDP.h"

#define TICK 3
#define PORT 54323
#define N_SZ 100

struct window{
	int start;
	int end;
	int sz;
	int *timers;
};

int main(int argc, char **argv) {
	struct sockaddr_in serv;
	int sock, trans, fd, fsz, numPackets, slen = sizeof(serv);
	int port = atoi(argv[2]);
	char *fname = argv[3];
	struct stat file;
    	unsigned char *map;
	socklen_t addl = sizeof(struct sockaddr);
	struct hostent *hent;
	char name[N_SZ];
	struct sockaddr_in laddr;
	Packet resp;
	Packet *packets;
	int sent = 0;
	time_t bt, at;	
	uint8_t w_sz = 0;
	struct window W;
	
	// Use FSTAT and MMAP to map the file to a memory buffer.
    	
	if(gethostname(name, N_SZ) < 0) {
		perror("gethostname()");
		return -1;
	}
	
	hent = gethostbyname(name);
	
    	if((fd = open(fname,O_RDONLY)) < 0) {
    		perror("open()");
		return -1;
    	}
    
    	if(fstat(fd, &file) < 0) {
    		perror("fstat()");
		return -1;
    	}
    	cout << "\nFile: " 
             << fname 
             << " opened successfully\n" 
             << endl;
    	fsz = file.st_size;
	cout << "File Size: "<< fsz  << "Bytes\n" << endl;
    	map = (unsigned char*)mmap(0, fsz, PROT_READ, MAP_SHARED, fd, 0);
    	void *addr = (void*)map;
	
	numPackets = fsz / BLOCKSIZE;
	if(fsz % BLOCKSIZE > 0)
		numPackets++;
	
	if((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
		perror("socket()");
		munmap(addr,fsz);
		return -1;
	}
	
	if(getsockname(sock,(struct sockaddr  *)&laddr, &addl) < 0) {
		perror("getsockname()");
		munmap(addr,fsz);
		return -1;
	}
		
	serv.sin_family = AF_INET;
	serv.sin_port = htons(PORT);
	if(inet_aton(argv[1], &serv.sin_addr) == 0) {
		perror("Invalid Address");
		close(sock);
		munmap(addr,fsz);
		return -1;
	}
	
	uint32_t from_IP = inet_addr(hent->h_addr);
        uint32_t to_IP = inet_addr(argv[1]);
	int place = 0;
	
	bt = time(0);
	
	////////////////MAKE PACKETS AHEAD OF TIME///////////////////
	
	packets = new Packet[numPackets];
	
	for(int p = 0; p < numPackets; p++) {
		
		printf("Making a packet!\n");
		Packet *pack;
		CS450Header header;
		void *data;
		
		pack = &packets[p];
		
		for(int j = 0; j < BLOCKSIZE; j++) {
			if((j + place) < fsz){
				pack->data[j] = map[j + place];
			}
			//place += BLOCKSIZE;
		}
		place += BLOCKSIZE;
		
		printf("Data Buffer Filled!\n");
		
		data = (void*)pack->data;
		
		if(fsz > BLOCKSIZE)
			packHeader( header, 7, 654218587, p, p+1, 
				    999, from_IP, to_IP, from_IP, to_IP, 
			  	    BLOCKSIZE, fsz, fname, laddr.sin_port, port, 
			   	    0, 4, 1, 0, 1, "slopez23", 0, 0, 0, 0, w_sz, 34);
		else
			packHeader( header, 7, 654218587, p, p+1, 
				    999, from_IP, to_IP, from_IP, to_IP, 
			  	    fsz, fsz, fname, laddr.sin_port, port, 
			   	    0, 4, 1, 0, 1, "slopez23", 0, 0, 0, 0, w_sz, 34);
		
		printf("Header Packed!\n");
			
		if(numPackets > 1 && p == (numPackets-1)) 
			header.nbytes = htonl((fsz%BLOCKSIZE));
		
		printf("Calculating checksum...\n");
		
		header.checksum = htons(calcChecksum(data, ntohl(header.nbytes)));	
		printf("Checksum calculated as: %d!\n",ntohs(header.checksum));	
		pack->header = header;
	}
		
	////////////////MAKE PACKETS AHEAD OF TIME///////////////////
	sent = 0; // sent and ACK'd
	W.start = 0;
	W.end = 0;
	W.sz = 1;
	W.timers = new int[numPackets];
	int ACK;
	
	//printf("#Packets: %d\n",numPackets);
	
	for(int t = 0; t < numPackets; t++)
		W.timers[t] = 0;
	
	trans = 1;
	
	while(sent < numPackets){
		
		if(W.sz > 1) {
			for(int s = W.start; s < W.end; s++) {
			
				if(W.timers[s] > -1) {
					printf("Sending packet %d\n", s);
				
					packets[s].header.windowSize = W.sz;
					packets[s].header.transactionNumber = trans;
				
					if(sendto(sock, (const void*)&packets[s], 
				         	 sizeof(Packet),0,
					  	(const sockaddr*) &serv, 
					  	(socklen_t)slen) < 0) {
	
						perror("sendto()");
						munmap(addr,fsz);
						close(sock);
						return -1;
					}
					trans++;
				}
			}
		}
		else {
			if(sendto(sock, (const void*)&packets[W.start], 
				         sizeof(Packet),0,
					(const sockaddr*) &serv, 
					(socklen_t)slen) < 0) {
	
				perror("sendto()");
				munmap(addr,fsz);
				close(sock);
				return -1;
			}
		}
		
		if(recvfrom(sock, (void*)&resp, sizeof(Packet), 0, NULL, 
		            (socklen_t*)&slen) < 0) {
			perror("recvfrom()");
			munmap(addr,fsz);
			close(sock);
			return -1;
		}
		
		printf("\nReceived Ack. Header!\n");
		
		printInfo(resp.header);
		
		ACK = ntohl(resp.header.ackNumber);
		
		printf("ACK# %d\n",ACK);
		
		if(ACK-1 == W.start) {
			
			W.timers[W.start] = -1;
			W.start++;
			if(W.start >= (fsz-1)) {
				W.start = fsz-1;
				W.end = fsz-1;
				W.sz = 1;
			}
			else
				W.sz++;
			W.end = W.sz + W.start - 1;
			if(W.end >= fsz-1) {
				W.end = fsz;
				W.sz = (W.end - W.start) + 1;
			}	
			sent++;	
			if(sent == numPackets)
				break;
			continue;
		}
		else if(ACK-1 > W.start && ACK-1 < W.end)
			W.timers[ACK-1] = -1;
		
	}
	
	at = time(NULL);
	
	cout << "\n\nTime before transmission: " << ctime(&bt) << "\n" << endl;
	cout << "Time after transmission: " << ctime(&at) << "\n" << endl;
	cout << "Time elapsed: " << difftime(at,bt) << " seconds\n" << endl;
	
	cout << "\nBandwidth: " 
	     << (fsz / (difftime(at,bt))) 
	     << " Bytes/sec\n" 
	     << endl;
	
	close(fd);
	munmap(addr,fsz);
	close(sock);

	return 0;	
}
