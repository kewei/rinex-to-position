#ifndef READ_NAV_ONLINE_H
#define READ_NAV_ONLINE_H

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>

class ReadNavOnline 
{
    
    public:
        static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream);
        FILE* get_ephemeris_online(int year, int DOY, std::string dir_name);
};

#endif