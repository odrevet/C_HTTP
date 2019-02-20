#include <stdlib.h>
#include <stdio.h>

char* strdup (const char* str);

#include "http.h"

#define ERR_NODATA "Nothing received after an HTTP request. "

void usage(const char* program_name){
  printf("usage: %s <hostname> [<page>]\n", program_name);
  puts("`hostname` is mandatory, `page` is optional (defaults to '/')");
  puts("`is present,`page must start with `/`. Otherwise the response will likely be `400 - bad request`");
  printf("example: %s www.example.com /\n", program_name);
}

int main(int argc, char **argv){
  //check arguments
  if(argc <= 1){
    puts("Missing arguments. ");
    usage((*argv));
    exit (EXIT_FAILURE);
  }

  const char *hostname = *(argv+1);
  const char *page = (argc == 2) ? "/" : *(argv+2);

  if(page[0] != '/')puts("Warning: page argument do not starts with `/`\n");

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
    puts("Check your network connection and that you entered the valid hostname");
    usage(*(argv));

    exit(EXIT_FAILURE);
  }

  SOCKET sock;
  SOCKADDR_IN sin;

  if(!http_create_socket(&sock, &sin, ip)){
    puts("Cannot create http socket");
    exit (EXIT_FAILURE);
  }

  printf("Connected to %s%s (%s:%d)\n\n", hostname, page, inet_ntoa(sin.sin_addr), htons(sin.sin_port));

  char *content = NULL;
  content = http_request(sock, hostname, page);

  if(content == NULL){
    puts(ERR_NODATA);
    exit(EXIT_FAILURE);
  }

  puts(content);
  free(content);
  closesocket(sock);

#if defined (WIN32)
  WSACleanup();
#endif

  return EXIT_SUCCESS;
}
