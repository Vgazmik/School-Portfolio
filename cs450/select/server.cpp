#include <stdio.h>

#include "CS450Header7.h"
#include "450UtilsUDP.h"

#define BUF_SZ 1024
#define PORT 54323

int main(int argc, char** argv)
{
	struct sockaddr_in serv, cli;
	int sock, fd, slen = sizeof(cli);
	uint32_t total, rec, written = 0;
	Packet pack;
	int expected = 1;
	int save = 0;
	
	if((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
		perror("socket()");
	
	serv.sin_family = AF_INET;
	serv.sin_port = htons(PORT);
	serv.sin_addr.s_addr = htonl(INADDR_ANY);
	
	if(bind(sock,(const sockaddr*)&serv,sizeof(serv)) < 0) {
		perror("bind()");
		close(sock);
	}
	
	printf("Server Runnning, Master!\n");
	
	if(recvfrom(sock, (void*)&pack, sizeof(Packet), 0, (sockaddr*)&cli, 
		            (socklen_t*)&slen) < 0) { // first packet received
		perror("revfrom()");
		close(sock);
		return -1;
	}
	
	printf("Received packet from %s : %d\n\n",
		       inet_ntoa(cli.sin_addr), 
		       ntohs(cli.sin_port));
	
	printInfo(pack.header);
	
	total = ntohl(pack.header.nTotalBytes);
	rec = 0;
	rec += ntohl(pack.header.nbytes);

	char dat[total];
	int place = 0;
	
	if(ntohs(pack.header.saveFile) != 0) {
			save = 1;
			expected = ntohl(pack.header.sequenceNumber) + 1;
			printf("Saving file...\n");
			fd = open(pack.header.filename, O_CREAT | O_TRUNC | O_RDWR, 0600);
			//write(fd,&pack.data,ntohl(pack.header.nbytes));
			written += ntohl(pack.header.nbytes);
			
			for(int r = 0; r < ntohl(pack.header.nbytes); r++) {
				dat[place + r] = pack.data[r];
			}

			place += BLOCKSIZE;
			
			if(written == total) 
				rec = total;
	}
	
	pack.header.ackNumber = htonl(ntohl(pack.header.sequenceNumber));
	pack.header.packetType = 2;

	printf("\nSending Ack Header!\n");
	if(sendto(sock, (const void*)&pack, sizeof(Packet),0, 
	          (const sockaddr*) &cli, (socklen_t)slen) < 0) {
		perror("Ack sendto()");
		close(sock);
		return -1;
	}

	while(rec < total) {
		if(recvfrom(sock, (void*)&pack, sizeof(Packet), 0, (sockaddr*)&cli, 
		            (socklen_t*)&slen) < 0)
			perror("revfrom()");
			
		if(ntohl(pack.header.sequenceNumber) == expected)
		{
			expected++;
			printf("Received packet from %s : %d\n\n",
		       		inet_ntoa(cli.sin_addr), 
		       		ntohs(cli.sin_port));
			
			printInfo(pack.header);	
			
			if(save == 1) {
				//write(fd,&pack.data,ntohl(pack.header.nbytes));
				written += ntohl(pack.header.nbytes);
				
				for(int r = 0; r < ntohl(pack.header.nbytes); r++) {
					dat[place + r] = pack.data[r];
				}

				place += BLOCKSIZE;

				//if(written == ntohl(pack.header.nTotalBytes)) 
					//break;
			}
			
			rec += ntohl(pack.header.nbytes);
			
			pack.header.ackNumber = htonl(ntohl(pack.header.sequenceNumber));
			pack.header.packetType = 2;
			
			printf("\nSending Ack Header!\n");
			if(sendto(sock, (const void*)&pack, sizeof(Packet),0, 
	          		  (const sockaddr*) &cli, (socklen_t)slen) < 0) {
				perror("Ack sendto()");
				close(sock);
				return -1;
			}
		}	
	}
	if(save == 1)
		for(int f = 0; f < (int)total; f++)
			write(fd,&dat[f],1);
	
	close(fd);
	close(sock);
	return 0;
}

