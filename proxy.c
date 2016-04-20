/*
 * proxy.c - CS:APP Web proxy
 *
 * TEAM MEMBERS:  Austin Arnold
                  Sasha Peters
                  Lonnie Scott
 * 
 * IMPORTANT: Give a high level description of your code here. You
 * must also provide a header comment at the beginning of each
 * function that describes what that function does.
 */ 

#include "csapp.h"

/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400

/* You won't lose style points for including this long line in your code */
static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";

/*
 * Function prototypes
 */
void doit(int fd);
void read_requesthdrs(rio_t *rp);

int main(int argc, char **argv)
{
    int listenfd, connfd, port;
    char hostname[MAXLINE], port[MAXLINE];
    socklen_t clientlen; 
    struct sockaddr_storage clientaddr;

    /* Check arguments */
    if (argc != 2) {
	fprintf(stderr, "Usage: %s <port number>\n", argv[0]);
	exit(0);
    }
    port = atoi(argv[1]);

    listenfd = Open_listenfd(argv[1]);
    while(1) 
    {
	clientlen = sizeof(clientaddr);
	connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen); //line:netp:tiny:accept
        Getnameinfo((SA *) &clientaddr, clientlen, hostname, MAXLINE, 
                    port, MAXLINE, 0);
        printf("Accepted connection from (%s, %s)\n", hostname, port);
	doit(connfd);                                             //line:netp:tiny:doit
	Close(connfd);                                            //line:netp:tiny:close
    }
    printf("%s", user_agent_hdr);
    return 0;
}

void doit(int fd) 
{
    int is_static;
    struct stat sbuf;
    char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    char filename[MAXLINE], cgiargs[MAXLINE];
    rio_t rio;

    /* Read request line and headers */
    Rio_readinitb(&rio, fd);
    if (!Rio_readlineb(&rio, buf, MAXLINE))  //line:netp:doit:readrequest
        return;
    printf("%s", buf);  //GET request
    read_requesthdrs(&rio);
}

void read_requesthdrs(rio_t *rp) 
{
    char host[MAXLINE];

    Rio_readlineb(rp, host, MAXLINE);
    printf("%s\n", host); //HOSTNAME

    return;
}
