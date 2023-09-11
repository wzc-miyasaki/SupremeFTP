#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h> 
#include <sys/time.h>
#include <netinet/in.h> 
#include <unistd.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <stdbool.h>

#include <iostream> 
#include "ReadFile/ReadFile.h"
using namespace std;

#define BUFFER_SIZE 9000 // maximum number of bytes that can be sent without receiving an ACK

struct packet_def {
    long int packet_seq;
    long int length;
    char data[BUFFER_SIZE];
};

// 0: evenly partitioned, every packet same size 1: not same size
int allSameSize = 0; 

int main(int argc, char **argv){
    if (argc != 2) {
        cout << "Usage: " << argv[0] <<  " [Port Number]" << endl;
        exit (-1);
    }

    // For UDP connection
    struct packet_def packet;
    struct sockaddr_in server_addr, client_addr;
    ssize_t length;
    int serverSocket;
    memset (&server_addr, 0, sizeof (server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons (atoi (argv[1]));
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if ((serverSocket = socket (AF_INET, SOCK_DGRAM, 0)) == -1){
        cout << "Server Error: socket" << endl;
    }
        

    if (::bind(serverSocket, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
        cout << "Server Error: bind" << endl;
    }

    char recvBuffer [BUFFER_SIZE];
    // Requested file info 
    char filename [64];
    long int fileSize;
    long int packet_count = 0;

    for (;;) {
        cout << "Waiting for initial request" << endl;
        memset (recvBuffer, 0, sizeof(recvBuffer));
        memset (filename, 0, sizeof (filename)) ;
        
        // read requested file name
        ssize_t recvStatus; 
        length = sizeof(client_addr);
        if((recvStatus = recvfrom(serverSocket, recvBuffer, BUFFER_SIZE, 0, (struct sockaddr*) &client_addr, (socklen_t *) &length)) == -1){
            cout << ("Server error: receive") << endl;
        } else {
            cout << "recv : " << recvStatus << endl;
        }
        
        // calculate fileSize
        strcpy(filename, recvBuffer);
        cout << "filename is : " << filename << endl;
        fileSize = getFileSize(filename);

        // check for amount of packet
        if ((fileSize % BUFFER_SIZE) != 0){
            packet_count = (fileSize / BUFFER_SIZE) + 1;
        }else{
            packet_count = fileSize / BUFFER_SIZE; 
            allSameSize=1;
        }
        
        int lastPackSize;
        if (fileSize % BUFFER_SIZE != 0){
            lastPackSize = fileSize - ((packet_count-1) *BUFFER_SIZE);
        }

        cout << "Total number of packet: " << packet_count << endl;

        // send header packet to client 
        sendto(serverSocket, &(packet_count), sizeof(packet_count), 0, (struct sockaddr *) &client_addr, sizeof (client_addr));
        usleep (1000);
        sendto(serverSocket, &(fileSize), sizeof(fileSize), 0, (struct sockaddr *) &client_addr, sizeof (client_addr));
        cout << "file size: " << fileSize << endl; 
        usleep (1000);
        
        // transmit the file to client
        long int curCount = 1;
        while(curCount <= packet_count){
            memset(&packet, 0, sizeof(packet));
            packet.packet_seq = curCount; 
            
            // calculate data offset
            size_t data_offset;
            size_t data_length;
            if(curCount == packet_count && allSameSize != 1) {
                // Handle the last packet, which might not be BUFFER_SIZE.
                data_offset = fileSize - lastPackSize;
                data_length = lastPackSize;
            } else {
                // Handle all other packets.
                data_offset = (curCount - 1) * BUFFER_SIZE;
                data_length = BUFFER_SIZE;
            }

            // read data from a specific range and fill packet 
            ReadFileWithRange(filename, data_offset, data_length, packet.data);
            packet.length = data_length;

            // Send packet
            sendto(serverSocket, &(packet), sizeof(packet), 0, (struct sockaddr *) &client_addr, sizeof(client_addr));
            cout << "Packet " << curCount << " sent with length " << packet.length << endl;

            if(packet_count == curCount){
                cout << "Transmission complete" << endl; 
                break;
            }
            curCount++; 
        }
        
        // check for retransmission 
        cout << "Start retransmission Seesion" << endl; 
        long int ackSeq = 0L; 
        
        while(true){

            memset(&packet, 0, sizeof(packet));
            memset(&ackSeq, 0, sizeof(ackSeq));
            recvfrom(serverSocket, &(ackSeq), sizeof(ackSeq), 0, (struct sockaddr *) &client_addr, (socklen_t *) &length);
            
            if(ackSeq == 0){
                cout<<"Retransmission Done, All File sent" << endl;
                exit(0);
                break;
            }

            // Determine the size and location of the requested data.
            size_t data_offset;
            size_t data_length;

            if(ackSeq == packet_count && allSameSize != 1) {
                // It's the last packet and it's of different size.
                data_offset = fileSize - lastPackSize;
                data_length = lastPackSize;
            } else {
                // Any other packet.
                data_offset = (ackSeq - 1) * BUFFER_SIZE;
                data_length = BUFFER_SIZE;
            }

            // Read requested data and send to client.
            ReadFileWithRange(filename, data_offset, data_length, packet.data);
            packet.packet_seq = ackSeq;
            packet.length = data_length;

            sendto (serverSocket, &(packet), sizeof (packet), 0, (struct sockaddr *) &client_addr, sizeof (client_addr));
            cout << "resent packet " << packet.packet_seq << endl; 
        }
    }
    close(serverSocket);
}