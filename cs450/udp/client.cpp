#include "CS450Header6.h"
#include "450UtilsUDP.h"

//#define BUF_SZ 1024
#define PORT 54323
#define N_SZ 100

int main(int argc, char **argv) {
	struct sockaddr_in serv;
	int sock, count, fd, fsz, numPackets, slen = sizeof(serv);
	//char buf[BUF_SZ];
	int port = atoi(argv[2]);
	char *fname = argv[3];
	struct stat file;
    	char *map;
	socklen_t addl = sizeof(struct sockaddr);
	struct hostent *hent;
	char name[N_SZ];
	struct sockaddr_in laddr;
	Packet resp;
	uint32_t sent = 0;
	time_t bt, at;	
	
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
	count = 0;
	
	bt = time(0);
	
	for(; sent < (uint32_t)fsz;) {
		printf("Making a packet!\n");
		Packet pack;
		CS450Header header;
		void *data;
		
		for(int j = 0; j < BLOCKSIZE; j++) {
			if(j < fsz){
				pack.data[j] = map[j + place];
				if(j == (BLOCKSIZE - 1))
					place+=j;
			}
		}
		
		printf("Data Buffer Filled!\n");
		
		data = (void*)&pack.data;
		
		if(fsz > BLOCKSIZE)
			packHeader( header, 6, 654218587, count, (count%2), 
				    999, from_IP, to_IP, from_IP, to_IP, 
			  	    BLOCKSIZE, fsz, fname, laddr.sin_port, port, 
			   	    0, 2, 1, 0, 1, "slopez23", 0, 0, 0, 0);
		else
			packHeader( header, 6, 654218587, count, (count%2), 
				    999, from_IP, to_IP, from_IP, to_IP, 
			  	    fsz, fsz, fname, laddr.sin_port, port, 
			   	    0, 2, 1, 0, 1, "slopez23", 0, 0, 0, 0);
		
		printf("Header Packed!\n");
			
		if(numPackets > 1 && count == (numPackets-1)) 
			header.nbytes = htonl((fsz%BLOCKSIZE));
		
		printf("Calculating checksum...\n");
		
		header.checksum = htons(calcChecksum(&data, ntohl(header.nbytes)));	
		printf("Checksum calculated as: %d!\n",ntohs(header.checksum));	
		pack.header = header;
		
		printf("Sending packet %d\n", count);
		//sprintf(buf, "This is packet number %d\n", count);
		if(sendto(sock, (const void*)&pack, sizeof(Packet),0, 
			        (const sockaddr*) &serv, (socklen_t)slen) < 0) {
			perror("sendto()");
			munmap(addr,fsz);
			close(sock);
			return -1;
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
		
		if(ntohl(resp.header.ackNumber) != (count%2))
			while(ntohl(resp.header.ackNumber) != (count%2)) {
			
				printf("incorrect ack!\n");
				if(sendto(sock, (const void*)&pack, sizeof(Packet),0, 
			          	  (const sockaddr*) &serv, (socklen_t)slen) < 0) {
					perror("sendto()");
					munmap(addr,fsz);
					close(sock);
					return -1;
				}
			}
		else {
			sent+=header.nbytes;
			count++;
			continue;
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
