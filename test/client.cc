#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <errno.h> 
#include <stdlib.h> 
#include <string.h> 
#include <stdio.h> 
#include <time.h> 
#include <iostream> 

#include "NetworkAnalyzer/NetworkAnalyzer.h"

using namespace std;

// Define packet structure to be sent over the network
struct packet_struct {
    long int packet_seq;       // Unique ID for each packet
    long int length;         // Length of data in the packet
    char data[BUFFER_SIZE];  // Data contained in the packet
    long int file_size;     
};

int main(int argc, char const *argv[]){
    time_t start, end, now;
    
    if (argc != 3) {
        cout << "Usage" << argv[0] << " [IP Address] [Port Number]" << endl; 
        exit (-1);
    }

    // Initialization 
    struct packet_struct packet;
    struct sockaddr_in server_addr, client_addr;

    // set network
    FillAddress(server_addr, argv[1], argv[2]);


    int clientSocket; 
    if ((clientSocket = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
        cout <<  "Client Error: socket" << endl;

    // Measure the RTT
    // double rtt = RequestRttMeasure(clientSocket, client_addr, server_addr);
    // cout << "RTT is : " << rtt << "s" << endl;


    // read user input filename
    std::string filename;
    std::cout << "Enter the filename you want to download: " << std::endl;
    std::getline(std::cin, filename);

    if(filename.empty()){
        cout << "no input string" << endl;
        exit(-1);
    }

    // Send request to server
    const char *nameBuffer = filename.c_str();

    if (sendto(clientSocket, nameBuffer, strlen (nameBuffer), 0, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) 
        cout << "Client Error: send" << endl;

    // TODO:  Provide reliable tranfer to ensure filesize & packet_count has been received
    // Receive file info
    ssize_t length;
    long int filesize = 0;
    long int packet_count = 0;

    recvfrom(clientSocket, &(packet), sizeof(packet), 0, (struct sockaddr *) &server_addr, (socklen_t *) &length);
    filesize = packet.file_size;
    packet_count = filesize/BUFFER_SIZE + (filesize%BUFFER_SIZE == 0 ? 0 : 1);
    cout << "filesize: " << filesize << endl; 
    cout << "packet_count: " << packet_count << endl; 

    // calculate stat of the file
    long int lastPackSize;
    int allSameSize = 0;
    if((filesize % BUFFER_SIZE) == 0){
        allSameSize = 1;
    }else{
        lastPackSize = filesize - ((packet_count - 1) * BUFFER_SIZE);
    }

    char *cache;
    cache = new char[filesize + 1];
    // cout << "filesize: " << filesize << endl; 
    time (&start);
    FILE *fptr; 
    // fptr = fopen(filename, "wb");
    fptr = fopen("copy.txt", "wb");
    double diff;
    // Ack array, check whihc packet is received 
    int ack[packet_count + 1];
    ack[0] = 1;
    for(int i = 1; i < packet_count + 1; i++){
        ack[i]=0;
    }

    int receivedCount = 0; 
    int allowFailed = 0;
    
    usleep (1000);  
    // Retransmission  
    int numOfLoss = 0;
    int initialLoss = 0; 
    // Check for lost packets
    // for (int i = 1; i <= packet_count; i++) {
    //     if (ack[i] == 0) {
    //         // cout << "packet " << i << " lost" << endl;
    //         numOfLoss++;
    //     }
    // }
    initialLoss = numOfLoss;
    // Setting the timeout for recvfrom
    struct timeval tv;
    tv.tv_sec = 0;  // 5 seconds timeout
    tv.tv_usec = RTT; // 0 microseconds
    if(setsockopt(clientSocket, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
        std::cerr << "Error setting timeout!" << std::endl;
        close(clientSocket);
        exit(-1);
    }

    for (int i = 1; i <= packet_count; i+=1) {
        while(ack[i] == 0) { 
            memset (&packet, 0, sizeof (packet));
            cout << "requesting " << i << endl; 
            sendto(clientSocket, &i, sizeof(i), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));

            while(true){
                if(recvfrom(clientSocket, &packet, sizeof(packet), 0, (struct sockaddr *)&server_addr, (socklen_t *)&length) == -1){
                    cout << "timeout retrans" << endl; 
                    break; 
                }else{
                    cout << "success retrans" << endl; 
                }
                cout << "actuall:  " << packet.packet_seq << " received" << endl;
                // If the packet sequence is correct, decrease the numOfLoss counter and break out of the while loop
                if (ack[packet.packet_seq] == 0) {
                    cout << "Retransmission packet " << i << " received" << endl;
                    memcpy(&cache[(packet.packet_seq - 1) * BUFFER_SIZE], packet.data, packet.length);
                    numOfLoss--;
                    ack[packet.packet_seq] = 1;
                    usleep (10);  
                }
                
                time(&now);
    
                diff = difftime(now, start);
                cout << "===========" << endl;
                cout << "Time consumed: " << diff << endl; 
            }
        }
    }

    
    // all packets received, send close signal to server
    int closeSingal = 0;
    cout << "No data loss, retransmission not needed" << endl;
    sendto(clientSocket, &closeSingal, sizeof(closeSingal), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    

    // write to local
    fwrite(cache, filesize, 1, fptr);

    // Calculate time stat
    time(&end);
    
    diff = difftime(end, start);
    cout << "Time consumed: " << diff << endl; 
    cout << "loss rate" << (static_cast<double>(initialLoss) / packet_count )* 100 << endl; 
    cout << "recv loss: " << allowFailed << endl; 
    fclose(fptr);
    close(clientSocket);
    return 0; 
}
