#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>

#if defined(_WIN32) || defined(_WIN64)
    #include <winsock2.h>
    #pragma comment(lib, "ws2_32.lib") // MSVC  
    #pragma comment(lib, "ws2_32.a") // MinGW 
#else 
    #include <sys/socket.h>
    #include <sys/types.h>
    #include <netinet/in.h>
    #include <unistd.h>
 
    
    typedef int SOCKET;
    typedef struct sockaddr_in SOCKADDR_IN;

#endif


int main(int argc, char** argv)
{
    SOCKET sockfd;
    SOCKADDR_IN sockaddr;
    unsigned int startPort = 0;
    unsigned int stopPort = 0;
    unsigned int port = 0;
    unsigned int i = 0;
    char* split = NULL;
    char* ip = NULL;
   
    #if defined(_WIN32) || defined(_WIN64)
    
    WSADATA  WSAData;
        
    if(WSAStartup(MAKEWORD(2, 2), &WSAData) != 0)
    {
        fprintf(stderr, "Error : code (%d)\n", WSAGetLastError()); 
        return EXIT_FAILURE;
    }
    
    #endif 

    if(argc != 4)
    {
        fprintf(stderr, "Usage : ./portscan <ip> <start port> <stop port>\n");
        return EXIT_FAILURE;
    }

    split = malloc(sizeof(char)* strlen(argv[1]));

    if(split == NULL)
    {
        fprintf(stderr, "Error : %s\n", strerror(errno));
        return EXIT_FAILURE;
    }

    ip = malloc(sizeof(char)* strlen(argv[1]));

    if(ip == NULL)
    {
        free(split);
        fprintf(stderr, "Error : %s\n", strerror(errno));
        return EXIT_FAILURE; 
    }

    strcpy(ip, argv[1]);
    split = strtok(ip, ".");
    while(split != NULL)
    {
        split = strtok(NULL, ".");

        if(atoi(split) < 0 || atoi(split) > 255)
        {
            free(split);
            free(ip);
            fprintf(stderr, "Usage : ./portscan <ip> <start port> <stop port>\n");
            return EXIT_FAILURE;
        }
        i++;
    } 
    free(split);
    free(ip);
 

    if(i != 4)
    {
        fprintf(stderr, "Usage : ./portscan <ip> <start port> <stop port>\n");
        return EXIT_FAILURE; 
    }
 
    if(isdigit(*argv[2]) && isdigit(*argv[3]))
    {
        if(startPort < 0 || stopPort < 0)
        {
            fprintf(stderr, "Usage : ./portscan <ip> <start port> <stop port>\n");
            return EXIT_FAILURE;
        }
        startPort = atoi(argv[2]);
        stopPort = atoi(argv[3]);    
    }
    else 
    {
        fprintf(stderr, "Usage : ./portscan <ip> <start port> <stop port>\n");
        return EXIT_FAILURE;
    }
 
    for(port = startPort; port <= stopPort; port++)
    {
        sockfd = socket(AF_INET, SOCK_STREAM, 0);

        if(sockfd < 0)
        {
            fprintf(stderr, "Error connot create the socket for %s:%d\n", argv[1], port);
        }
        else 
        {
            sockaddr.sin_family = AF_INET;
            sockaddr.sin_port = htons(port);
            sockaddr.sin_addr.s_addr = htonl(inet_addr(argv[1]));


            if(connect(sockfd, (SOCKADDR*)&sockaddr, sizeof(sockaddr)) == SOCKET_ERROR)
            {
                fprintf(stdout, "[+] %s:%d [OFF]\n", argv[1], port);
            }
            else 
            {
                fprintf(stdout, "[+] %s:%d [ON]\n", argv[1], port);
            }
        #if defined(_WIN32) || defined(_WIN64)
            closesocket(sockfd);
        #else 
            close(sockfd);
        #endif 
        }
       
    }
    return EXIT_SUCCESS;
}