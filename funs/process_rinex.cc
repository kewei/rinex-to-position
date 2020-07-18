#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "rinex_obs.h"

std::mutex mtx;
std::condition_variable header;

class Process_rinex 
{
	
	// read observation rinex file
	void read_rinex_obs(string osb_file_name) 
	{	
		RinexObs rinex_obs;
		std::string line;
		ifstream obs_file(obs_file_name);
		if (obs_file.is_open()) 
		{
			while (getline(obs_file, line)) 
			{
				if (line.find("RINEX VERSION") != -1)
				{
					int start_pos;
					start_pos = line.find_first_not_of(" ");
					rinex_obs.rinex_ver = line[start_pos:start_pos+3];	
				}
				
				if (line.find("APPROX POSITION") != -1)
				{
					std::string:size_type sz; 
					approx_pos_xyz["x"] = std::stod(line, &sz);
					std::string appx_rest = line.substr(sz);
					approx_pos_xyz["y"] = std::stod(appx_rest, &sz);
					approx_pos_xyz["z"] = std::stod(appx_rest.substr(sz));
				}

				if(line.find("OBS TYPES") != -1)
				{
					if (line[0] == "G")
					{
						
					}	
				}
				if (line.find("APPROX POSITION") != -1)
				{
					
				}
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
