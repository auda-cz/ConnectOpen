#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

#define BUFFER_SIZE 409600 // Buffer for reading data

// Function to extract host and path from URL
void parse_url(const char *url, char *host, char *path) {
    const char *host_start = strstr(url, "://");
    if (host_start) {
        host_start += 3; // Skip "://"
    } else {
        host_start = url; // No "http://" prefix
    }

    const char *path_start = strchr(host_start, '/');
    if (path_start) {
        strncpy(host, host_start, path_start - host_start);
        host[path_start - host_start] = '\0';
        strcpy(path, path_start);
    } else {
        strcpy(host, host_start);
        strcpy(path, "/");
    }
}

// Function to send HTTP request and discard response
void send_http_request(const char *host, const char *path) {
    int sockfd;
    struct sockaddr_in server_addr;
    struct hostent *server;
    char request[BUFFER_SIZE], response[BUFFER_SIZE];
    ssize_t bytes_read;

    // Create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Resolve the hostname to an IP address
    server = gethostbyname(host);
    if (server == NULL) {
        fprintf(stderr, "Error: No such host\n");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Configure the server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    memcpy(&server_addr.sin_addr.s_addr, server->h_addr, server->h_length);
    server_addr.sin_port = htons(80); // HTTP port

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Prepare HTTP GET request
    snprintf(request, sizeof(request),
             "GET %s HTTP/1.1\r\n"
             "Host: %s\r\n"
             "Connection: close\r\n\r\n",
             path, host);

    // Send the request
    if (write(sockfd, request, strlen(request)) < 0) {
        perror("Write to socket failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Read and discard the response
    while ((bytes_read = read(sockfd, response, sizeof(response))) > 0) {
        // Do nothing with the data to avoid writing
    }

    if (bytes_read < 0) {
        perror("Read from socket failed");
    }

    // Close the socket
    close(sockfd);
}

int main(int argc, char *argv[]) {
    // Default URL for the random text file generator
    const char *default_url = "http://onlinefiletools.com/generate-random-text-file?length=4096kb";

    const char *url = default_url;
    char host[256], path[1024];

    // Allow overriding default URL with a command-line argument
    if (argc == 2) {
        url = argv[1];
    } else if (argc != 1) {
        fprintf(stderr, "Usage: %s [url]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Parse the URL into host and path
    parse_url(url, host, path);

    // Send the HTTP request
    send_http_request(host, path);

    return 0;
}