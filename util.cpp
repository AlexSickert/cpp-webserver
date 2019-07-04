/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include<iostream>
#include<string> 

#include<stdio.h>
#include<string.h> //strlen

#include <sstream>
#include <ctime>
#include <fstream>

#include <ctime>



namespace util {
    
    using namespace std;

    char str_to_char_arr(string s) {

//        int n = s.length();
//        char char_array[n + 1];
//        strcpy(char_array, s.c_str());
//        return char_array;
    }

    void log(string s) {

        FILE * pFileTXT;
        pFileTXT = fopen("log.txt", "a");


        int n = s.length();
        char char_array[n + 1];
        strcpy(char_array, s.c_str());

        fprintf(pFileTXT, char_array);
        fprintf(pFileTXT, "\n");
        fclose(pFileTXT);

    }

}

