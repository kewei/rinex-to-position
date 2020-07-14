#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex mtx;
std::condition_variable header;

class Pro_rinex 
{
	
	// read observation rinex file
	void read_rinex_obs(string osb_file_name) 
	{
		string line;
		ifstream obs_file(obs_file_name);
		if (obs_file.is_open()) 
		{
			while (getline(obs_f, line)) 
			{
				
			}
		}
				    
    	return 0;
	}

	
	void read_rinex_nav() {
    
    	return 0;
	}
	
	// get nav file from ftp server
	void get_nav_ftp() {
    
    	return 0;
	}

}
