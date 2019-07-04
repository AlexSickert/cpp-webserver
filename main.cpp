#include<iostream>
#include<string> 
#include<stdio.h>
#include<string.h> //strlen
#include<stdlib.h> //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h> //write
#include<pthread.h> //for threading , link with lpthread
#include <sstream>
#include <ctime>
#include <fstream>

#include "util.h"
#include "request.h"
#include "measure.h"

/*
 
 For -pthread error - it needs to be added in commandline arg of compiler
 * 
 * see this
 * 
 * https://stackoverflow.com/questions/1662909/undefined-reference-to-pthread-create-in-linux
 * 
 * originally based on this source
 * 
 * https://www.binarytides.com/server-client-example-c-sockets-linux/
 * 
 * but this source assumes an ongoing connection with each clinet. it does not close the connection
 
 
 */

using namespace std;

//the thread function
void *connection_handler(void *);

int main(int argc, char *argv[]) {

    int socket_desc, client_sock, c, *new_sock;
    struct sockaddr_in server, client;

    util::log("start");

    puts("now in request");


    measure::measure_start();

    for (int i = 0; i < 1; i++) {
        request::get("");
    }

    measure::measure_stop();
    measure::measure_print_result();


    //Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1) {
        printf("Could not create socket");
    }
    puts("Socket created");

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8888);

    //Bind
    if (bind(socket_desc, (struct sockaddr *) &server, sizeof (server)) < 0) {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");

    //Listen
    listen(socket_desc, 3);

    //Accept and incoming connection
    //puts("Waiting for incoming connections...");
    //c = sizeof (struct sockaddr_in);


    //Accept an incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof (struct sockaddr_in);
    while ((client_sock = accept(socket_desc, (struct sockaddr *) &client, (socklen_t*) & c))) {
        puts("Connection accepted");

        pthread_t sniffer_thread;
        new_sock = (int *) malloc(1);
        *new_sock = client_sock;

        if (pthread_create(&sniffer_thread, NULL, connection_handler, (void*) new_sock) < 0) {
            perror("could not create thread");
            return 1;
        }

        //Now join the thread , so that we dont terminate before the thread
        //pthread_join( sniffer_thread , NULL);
        puts("Handler assigned");
    }

    if (client_sock < 0) {
        perror("accept failed");
        return 1;
    }

    util::log("end");

    return 0;
}

string get_date_time() {

    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, sizeof (buffer), "%d-%m-%Y %H:%M:%S", timeinfo);
    std::string time_str(buffer);

    return time_str;

}

string int_to_str(int i) {

    std::stringstream out;
    out << i;
    return out.str();

}

/*
 * This will handle connection for each client
 * */
void *connection_handler(void *socket_desc) {
    //Get the socket descriptor
    int sock = *(int*) socket_desc;
    int read_size;
    char *message, client_message[2000];

    read_size = recv(sock, client_message, 2000, 0);

    std::cout << "read size is " << read_size;
    std::cout << "message is " << client_message;

    util::log("in connection_handler");

    string s = "HTTP/1.1 200 OK\n\n";
    
    s.append("<!DOCTYPE html>\n");
    s.append("<html>\n");
    s.append("<script>\n");
    s.append(" location.reload(true); \n");
    s.append("</script>\n");
    s.append("</html>\n");
    
    
    // location.reload(true);
    
    

    //s.append(int_to_str(1223454));

    //s.append(get_date_time());

    int n = s.length();
    char char_array[n + 1];
    strcpy(char_array, s.c_str());

    write(sock, char_array, strlen(char_array));
    fflush(stdout);
    close(sock);

    puts("socket now closed");

    //Free the socket pointer
    free(socket_desc);

    return 0;
}