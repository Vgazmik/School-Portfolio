#include "CS450Header7.h"
#include "450UtilsUDP.h"

#define TICK 5
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
    	char *map;
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
    	map = (char*)mmap(0, fsz, PROT_READ, MAP_SHARED, fd, 0);
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
			if(j < fsz){
				pack->data[j] = map[j + place];
				if(j == (BLOCKSIZE - 1))
					place+=j;
			}
		}
		
		printf("Data Buffer Filled!\n");
		
		data = (void*)&pack->data;
		
		if(fsz > BLOCKSIZE)
			packHeader( header, 7, 654218587, p, p+1, 
				    999, from_IP, to_IP, from_IP, to_IP, 
			  	    BLOCKSIZE, fsz, fname, laddr.sin_port, port, 
			   	    0, 3, 1, 0, 1, "slopez23", 0, 0, 0, 0, w_sz, 31);
		else
			packHeader( header, 7, 654218587, p, p+1, 
				    999, from_IP, to_IP, from_IP, to_IP, 
			  	    fsz, fsz, fname, laddr.sin_port, port, 
			   	    0, 3, 1, 0, 1, "slopez23", 0, 0, 0, 0, w_sz, 31);
		
		printf("Header Packed!\n");
			
		if(numPackets > 1 && p == (numPackets-1)) 
			header.nbytes = htonl((fsz%BLOCKSIZE));
		
		printf("Calculating checksum...\n");
		
		header.checksum = htons(calcChecksum(&data, ntohl(header.nbytes)));	
		printf("Checksum calculated as: %d!\n",ntohs(header.checksum));	
		pack->header = header;
		
		//packets[p] = pack;
		
	}
		
	////////////////MAKE PACKETS AHEAD OF TIME///////////////////
	sent = 0;
	W.start = W.end = 1;
	W.sz = 0;
	W.timers = new int[numPackets];
	int ACK;
	
	//printf("#Packets: %d\n",numPackets);
	
	for(int t = 0; t < numPackets; t++)
		W.timers[t] = TICK;
	
	trans = 1;
	
	for(; sent < numPackets; ) {
		
		for(int s = W.start; s <= W.end; s++){
			
			if(W.timers[s] == TICK) {
				printf("Sending packet %d\n", s);
				
				packets[s-1].header.windowSize = W.sz;
				packets[s-1].header.transactionNumber = trans;
				
				if(sendto(sock, (const void*)&packets[s-1], 
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
			W.timers[s-1]--;
		}
		
		//Decrement each timer in window
		/*for(int q = W.start; q < W.end; q++) {
			W.timers[q-1]--;
			if(W.timers[q-1] == 0)
				W.timers[q-1] = 7;
		}*/
		
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
		
		if(ACK != W.start) {
			
			if(ACK == 0) {
				//nothing received
			}
			else if(ACK <= W.end && ACK >= W.start) {
				//within the window
				W.timers[ACK-1] = -1; //finished
				sent++;
				
				if(sent == numPackets)
					break;
			}
			else if(ACK < W.start-1) {
				W.start = ACK + 1;
				W.sz = 1;
				W.end = W.start + W.sz; 
				continue;
			}
			else {
				if(W.end != 1)
					W.end--;
				else
					printf("Sanity\n");
			}
		}	
		else {
			W.timers[W.start] = -1;
			W.start++;
			W.sz++;
			W.end = W.sz + W.start - 1;
			sent++;	
			if(sent == numPackets)
				break;
		}
		
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
