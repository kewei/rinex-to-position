#include "read_nav_online.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <curl/curl.h>
#include <string>

static size_t ReadNavOnline::write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
    size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
    return written;
}

FILE* ReadNavOnline::get_ephemeris_online(int year, int DOY, std::string dir_name)
{
    CURL *curl_handle;
    FILE *page_file;
    std::string page_url_base = "ftp://cddis.nasa.gov/gnss/data/hourly/";
    
    if (DOY > 99)
    {
        static const char *pagefilename = "hour" + std::to_string(DOY) + "0" + std::to_string(year)[2:3] + "n.Z";
        static const std::string data_url = page_url_base + std::to_string(year) + "/0" + std::to_string(DOY) + "/";
    }
    else
    {
        static const char *pagefilename = "hour" + "0" + std::to_string(DOY) + "0" + std::to_string(year)[2:3] + "n.Z";
        static const std::string data_url = page_url_base + std::to_string(year) + std::to_string(DOY) + "/";
    }
    
    
    curl_global_init(CURL_GLOBAL_ALL);

    // init the curl session
    curl_handle = curl_easy_init();

    // set URL to get
    curl_easy_setopt(curl_handle, CURLOPT_URL, data_url);

    // switch on full protocol/debug output while testing
    curl_wasy_setopt(curl_handle, CURLOPT_VERBOSE, 1L);

    // disable progree meter, set to 0L to enable it
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);

    // open the file
    page_file = fopen(pagefilename, "wr");
    if(page_file)
    {
        // write the page body to this file handle
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, page_file);

        curl_easy_perform(curl_handle);
        // close the handle
        fclose(page_file);
    }

    // cleanup curl stuff
    curl_easy_cleanup(curl_handle);
    curl_global_cleanup();

    return page_file;
}