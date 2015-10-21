#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <stdbool.h>

char* strdup (const char* str);

#include "http.h"

#define ERR_NODATA "Nothing received after an HTTP request. "
 
void usage(const char* program_name)
{
  printf("usage: %s hostname page [num]\n", program_name);
  printf("example: %s www.example.com \"/index.php?id=42&action=foo\"\n", program_name);
  puts("note the abscence of http:// and the usage of quote if your terminal proceed the special characters in the page");
  puts("num is an optional parameter to (re)start the dump from an ets index");
}

//////////////////////////////////////

int main(int argc, char **argv)
{
  //check arguments 
  if(argc < 3){
    usage((*argv));
    exit (EXIT_FAILURE);
  }

  const char *hostname = strdup(*(argv+1));
  const char *page = strdup(*(argv+2));
 
  int i;
  if(argc == 4){
    i = atoi(*(argv+3));
  }
  else{
    i = 0;
  }

  //some networking initialisations
#if defined (WIN32)
  WSADATA WSAData;
  int erreur = WSAStartup(MAKEWORD(2,2), &WSAData);
  if(erreur){
    puts("Cannot start WSA"); 
    exit (EXIT_FAILURE);
  }
#endif
 
  const char *ip = host_to_ip(hostname);
  if(!ip){
    puts("Cannot resolve hostname's ip");
    puts("Check your network connection and that you entered the hostname correcly");
    usage(*(argv));    

    exit(EXIT_FAILURE);
  }
 
  SOCKET sock;
  SOCKADDR_IN sin;
 
  if(!http_create_socket(&sock, &sin, ip)){
    puts("Cannot create http socket");
    exit (EXIT_FAILURE);
  }
 
  printf("Connected to %s/%s (%s:%d)\n", hostname, page, inet_ntoa(sin.sin_addr), htons(sin.sin_port));
  char *content = NULL;
  
  content = http_request(sock, hostname, page);
  
  if(content == NULL){
    puts(ERR_NODATA);
    exit(EXIT_FAILURE);
  }

  puts(content);
  
  //char *content_core = http_header_strip(content);
 
  free(content);
  closesocket(sock);
  
#if defined (WIN32)
  WSACleanup();
#endif
 
  return EXIT_SUCCESS;
}
