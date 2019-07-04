/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   measure.cpp
 * Author: alex
 *
 * Created on July 4, 2019, 10:33 PM
 */

#include <sys/time.h>
#include <string>
#include <stdio.h> //printf

namespace measure {

    using namespace std;

    struct timeval tval_before, tval_after, tval_result;

    void measure_start() {

        gettimeofday(&tval_before, NULL);

    }

    void measure_stop() {

        gettimeofday(&tval_after, NULL);
    }

    void measure_print_result() {
        
        timersub(&tval_after, &tval_before, &tval_result);

        long usec_diff = (tval_after.tv_sec - tval_before.tv_sec)*1000000 + (tval_after.tv_usec - tval_before.tv_usec);

        printf("Time elapsed: %ld.%06ld\n", (long int) tval_result.tv_sec, (long int) tval_result.tv_usec);
        printf("Time elapsed microseconds: %ld\n", (long int) usec_diff);
        double res = static_cast<double>(usec_diff) / 1000000.0;
        printf("Time elapsed seconds: %f \n",  res);
    }

}