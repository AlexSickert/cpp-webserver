/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   request.cpp
 * Author: alex
 *
 * Created on July 4, 2019, 9:09 PM
 */

#include <cstdlib>
#include <stdio.h> //printf
#include <string.h> //strlen
#include <sys/socket.h> //socket
#include <arpa/inet.h>
#include<iostream>
#include <sstream>
#include <sys/time.h>
#include "util.h"



namespace request {

    using namespace std;

    void time_stamp() {

        struct timeval tval_before, tval_after, tval_result;

        gettimeofday(&tval_before, NULL);
        
        string tst = "c";
        for(int i = 1; i < 100000; i++){
            tst.append("x");
        }

        //sleep(1);

        gettimeofday(&tval_after, NULL);

        timersub(&tval_after, &tval_before, &tval_result);
        
        //long usec_diff = (e.tv_sec - s.tv_sec)*1000000 + (e.tv_usec - s.tv_usec);

        printf("Time elapsed: %ld.%06ld\n", (long int) tval_result.tv_sec, (long int) tval_result.tv_usec);

    }

    string int_to_str(int i) {

        std::stringstream out;
        out << i;
        return out.str();

    }

    void get(string url) {

        int sock;
        struct sockaddr_in server;
        string message;
        char server_reply[2000];
        char server_reply_1[1];

        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == -1) {
            printf("Could not create socket");
        }

        puts("socket created");

        server.sin_addr.s_addr = inet_addr("216.58.215.110");
        //server.sin_addr.s_addr = inet_addr("http://google.com");
        server.sin_family = AF_INET;
        server.sin_port = htons(80);

        //Connect to remote server
        if (connect(sock, (struct sockaddr *) &server, sizeof (server)) < 0) {
            perror("connect failed. Error");
            //return 1;
        }

        puts("Connected\n");

        //        message = util::str_to_char_arr("");
        message = "GET / HTTP/1.1\r\nHost: " + url + "\r\nConnection: close\r\n\r\n";

        //
        //        //Send some data
        if (send(sock, message.c_str(), strlen(message.c_str()), 0) < 0) {
            puts("Send failed");
            //return 1;
        }

        //Receive a reply from the server

        int c;
        int counter = 0;
        bool continue_loop = true;

        string res = "";

        while (recv(sock, server_reply_1, 1, 0) > 0) {

            c = (int) server_reply_1[0];
            if (c < 1) {
                break;
            }
            counter += 1;

            if (counter > 50000) {
                break;
            }

            //std::cout << "counter is " << counter << " c is " << c << "reply is " << server_reply_1 << endl;
            //puts(server_reply_1);

            res.append(server_reply_1, 1);
        }

        std::cout << res << endl;






        //socket::close(sock);


    }


}