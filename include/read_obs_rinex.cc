#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "read_obs_rinex.h"
#include "str_trim.cc"

std::mutex mtx;
std::condition_variable header;

struct RinexObs
{
	public:
		std::string rinex_ver;
		std::map<char, double> approx_pos_xyz;
		std::vector<std::string> obs_type_gps;
		std::vector<std::string> obs_type_glonass;
		std::vector<std::string> obs_type_galileo;
		std::vector<std::string> obs_type_beidou;
		std::vector<std::string> obs_type_qzss;
		std::map<std::string, std::string> t_first_obs;
};

// read rinex obs file
	void ReadObsRinex::read_rinex_obs(std::string obs_file_name) 
	{	
		struct RinexObs rinex_obs;
		std::string line;
		std::ifstream obs_file(obs_file_name);
		if (obs_file.is_open()) 
		{
			/* read obs header*/
			while (getline(obs_file, line) && line.find("END OF HEADER") != -1) 
			{
				if (line.find("RINEX VERSION") != -1)
				{
					int start_pos;
					start_pos = line.find_first_not_of(" ");
					rinex_obs.rinex_ver = line.substr(start_pos, 3);	
				}
				
				if (line.find("APPROX POSITION") != -1)
				{
					std::string::size_type sz; 
					rinex_obs.approx_pos_xyz['x'] = std::stod(line, &sz);
					std::string appx_rest = line.substr(sz);
					rinex_obs.approx_pos_xyz['y'] = std::stod(appx_rest, &sz);
					rinex_obs.approx_pos_xyz['z'] = std::stod(appx_rest.substr(sz));
				}

				if(line.find("OBS TYPES") != -1)
				{
					//if (line[0].compare("G") == 0)
					if (line[0] == 'G')
					{
						int num_gps_type = std::stoi(line.substr(4, 2));
						for (int i=0; i<num_gps_type; i++)
						{
							rinex_obs.obs_type_gps.push_back(line.substr(7+i*4, 3));
						}
					}
					//if (std::strcmp(line[0], "R") == 0)
					if (line[0] == 'R')
					{
						int num_glonass_type = std::stoi(line.substr(4, 2));
						for (int i=0; i<num_glonass_type; i++)
						{
							rinex_obs.obs_type_glonass.push_back(line.substr(7+i*4, 3));
						}
					}
					//if (std::strcmp(line[0], "E") == 0)
					if (line[0] == 'E')
					{
						int num_galileo_type = std::stoi(line.substr(4, 2));
						for (int i=0; i<num_galileo_type; i++)
						{
							rinex_obs.obs_type_galileo.push_back(line.substr(7+i*4, 3));
						}
					}
					//if (std::strcmp(line[0], "C") == 0)
					if (line[0] == 'C')
					{
						int num_beidou_type = std::stoi(line.substr(4, 2));
						for (int i=0; i<num_beidou_type; i++)
						{
							rinex_obs.obs_type_beidou.push_back(line.substr(7+i*4, 3));
						}
					}
					//if (std::strcmp(line[0], "J") == 0)
					if (line[0] == 'J')
					{
						int num_qzss_type = std::stoi(line.substr(4, 2));
						for (int i=0; i<num_qzss_type; i++)
						{
							rinex_obs.obs_type_qzss.push_back(line.substr(7+i*4, 3));
						}
					}	
				}
				if (line.find("TIME OF FIRST OBS") != -1)
				{
					std::string::size_type sz;
					rinex_obs.t_first_obs["year"] = std::stoi(line, &sz);
					std::string t_d_rest = line.substr(sz);
					rinex_obs.t_first_obs["month"] = std::stoi(t_d_rest, &sz);
					t_d_rest = line.substr(sz);
					rinex_obs.t_first_obs["day"] = std::stoi(t_d_rest, &sz);
					t_d_rest = line.substr(sz);
					rinex_obs.t_first_obs["hour"] = std::stoi(t_d_rest, &sz);
					t_d_rest = line.substr(sz);
					rinex_obs.t_first_obs["min"] = std::stoi(t_d_rest, &sz);
					t_d_rest = line.substr(sz);
					rinex_obs.t_first_obs["sec"] = std::stof(t_d_rest, &sz);
					t_d_rest = line.substr(sz);
					ltrim(t_d_rest);
					rinex_obs.t_first_obs["system"] = t_d_rest.substr(0, t_d_rest.find(" "));
				}
			}


			/* read obs data */
		}
				    

	}

	
	/* // read observation nav file
	void ReadObsRinex::read_rinex_nav(std::string nav_file) {
    

	}
	
	// get nav file from ftp server
	void ReadObsRinex::get_nav_ftp(std::string date_time) {
    

	} */

