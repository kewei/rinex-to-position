#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <cmath>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "read_obs_rinex.h"
#include "str_trim.cc"
#include "date2GpsTime.h"
#include "obsData.h"

std::mutex mtx;
std::condition_variable header;


// read rinex obs file
ObsData ReadObsRinex::read_rinex_obs(std::string obs_file_name)
{
	ObsData obs_data;
	struct RinexObsHeader rinex_obs_header;
	std::vector<std::vector<double> > obsGps;
	std::vector<std::vector<double> > obsGalileo;
	std::vector<std::vector<double> > obsBeidou;
	std::vector<std::vector<double> > obsGlonass;
	std::vector<std::vector<double> > obsQzss;

	std::string line;
	std::ifstream obs_file(obs_file_name);
	if (obs_file.is_open())
	{
		/* read obs header*/
		while (getline(obs_file, line) && line.find("END OF HEADER") == std::string::npos)
		{
			if (line.find("RINEX VERSION") != -1)
			{
				int start_pos;
				start_pos = line.find_first_not_of(" ");
				rinex_obs_header.rinex_v = line.substr(start_pos, 3);
			}

			if (line.find("APPROX POSITION") != -1)
			{
				std::string::size_type sz;
				rinex_obs_header.approx_pos_xyz['x'] = std::stod(line, &sz);
				std::string appx_rest = line.substr(sz);
				rinex_obs_header.approx_pos_xyz['y'] = std::stod(appx_rest, &sz);
				rinex_obs_header.approx_pos_xyz['z'] = std::stod(appx_rest.substr(sz));
			}

			if (line.find("OBS TYPES") != -1)
			{
				//if (line[0].compare("G") == 0)
				if (line[0] == 'G')
				{
					int num_gps_type = std::stoi(line.substr(4, 2));
					for (int i = 0; i < num_gps_type; i++)
					{
						rinex_obs_header.obs_type_gps.push_back(line.substr(7 + i * 4, 3));
					}
				}
				//if (std::strcmp(line[0], "R") == 0)
				if (line[0] == 'R')
				{
					int num_glonass_type = std::stoi(line.substr(4, 2));
					for (int i = 0; i < num_glonass_type; i++)
					{
						rinex_obs_header.obs_type_glonass.push_back(line.substr(7 + i * 4, 3));
					}
				}
				//if (std::strcmp(line[0], "E") == 0)
				if (line[0] == 'E')
				{
					int num_galileo_type = std::stoi(line.substr(4, 2));
					for (int i = 0; i < num_galileo_type; i++)
					{
						rinex_obs_header.obs_type_galileo.push_back(line.substr(7 + i * 4, 3));
					}
				}
				//if (std::strcmp(line[0], "C") == 0)
				if (line[0] == 'C')
				{
					int num_beidou_type = std::stoi(line.substr(4, 2));
					for (int i = 0; i < num_beidou_type; i++)
					{
						rinex_obs_header.obs_type_beidou.push_back(line.substr(7 + i * 4, 3));
					}
				}
				//if (std::strcmp(line[0], "J") == 0)
				if (line[0] == 'J')
				{
					int num_qzss_type = std::stoi(line.substr(4, 2));
					for (int i = 0; i < num_qzss_type; i++)
					{
						rinex_obs_header.obs_type_qzss.push_back(line.substr(7 + i * 4, 3));
					}
				}
			}
			if (line.find("TIME OF FIRST OBS") != -1)
			{
				std::vector<long> time_first_obs; // <secGps, week, DOY>
				std::string::size_type sz;
				rinex_obs_header.t_first_obs["year"] = std::to_string(std::stoi(line, &sz));
				std::string t_d_rest = line.substr(sz);
				rinex_obs_header.t_first_obs["month"] = std::to_string(std::stoi(t_d_rest, &sz));
				t_d_rest = t_d_rest.substr(sz);
				rinex_obs_header.t_first_obs["day"] = std::to_string(std::stoi(t_d_rest, &sz));
				t_d_rest = t_d_rest.substr(sz);
				rinex_obs_header.t_first_obs["hour"] = std::to_string(std::stoi(t_d_rest, &sz));
				t_d_rest = t_d_rest.substr(sz);
				rinex_obs_header.t_first_obs["min"] = std::to_string(std::stoi(t_d_rest, &sz));
				t_d_rest = t_d_rest.substr(sz);
				rinex_obs_header.t_first_obs["sec"] = std::to_string(std::stof(t_d_rest, &sz));
				t_d_rest = t_d_rest.substr(sz);
				time_first_obs = date2GpsTime(rinex_obs_header.t_first_obs["year"], rinex_obs_header.t_first_obs["month"],\
				rinex_obs_header.t_first_obs["day"], rinex_obs_header.t_first_obs["hour"], 0, 0);
				rinex_obs_header.t_first_obs["DOY"] = time_first_obs[2];
				ltrim(t_d_rest);
				rinex_obs_header.t_first_obs["system"] = t_d_rest.substr(0, t_d_rest.find(" "));
			}
		}

		int featuresGps = rinex_obs_header.obs_type_gps.size();
		int featuresGalileo = rinex_obs_header.obs_type_galileo.size();
		int featuresBeidou = rinex_obs_header.obs_type_beidou.size();
		int featuresGlonass = rinex_obs_header.obs_type_glonass.size();
		int featuresQzss = rinex_obs_header.obs_type_qzss.size();

		float obs_records_gps;
		/* read obs data */
		getline(obs_file, line);
		while (!obs_file.eof())
		{
			if (line.empty())
			{
				getline(obs_file, line);
			}
			else
			{
				std::vector<long> gps_time; // <secGps, week, DOY>
				int num_sats;
				if (line[0] == '>')
				{
					int year = std::stoi(line.substr(2, 4));
					int month = std::stoi(line.substr(6, 3));
					int day = std::stoi(line.substr(9, 3));
					int hour = std::stoi(line.substr(12, 3));
					int minute = std::stoi(line.substr(15, 3));
					float second = std::stof(line.substr(18, 11));
					int flag = std::stoi(line.substr(29, 3));
					num_sats = std::stoi(line.substr(32, 3));
					gps_time = date2GpsTime(year, month, day, hour, minute, second);
				}
				for (int j = 0; j < num_sats; j++)
				{
					getline(obs_file, line);
					int line_len = line.size();
					int num_data = std::floor(line_len/16);  // each data is 16
					std::vector<double> each_record;
					each_record.push_back(gps_time[0]);
					each_record.push_back(gps_time[1]);
					
					switch (line[0])
					{
						
						case 'G': 
								each_record.push_back(std::stoi(line.substr(1,2)) + 1000); // prn
								for (int i=0; i<num_data; i++)
								{
									try{
										each_record.push_back(std::stod(line.substr(3+i*16, 16)));
									}
									catch (std::invalid_argument ) {
										each_record.push_back(0);
									}
									
								}
								obsGps.push_back(each_record);
								break;
						case 'E': each_record.push_back(std::stoi(line.substr(1,2)) + 4000); // prn
								for (int i=0; i<num_data; i++)
								{
									try{
										each_record.push_back(std::stod(line.substr(3+i*16, 16)));
									}
									catch (std::invalid_argument ) {
										each_record.push_back(0);
									}
									
								}
								obsGalileo.push_back(each_record);
								break;
						case 'R': each_record.push_back(std::stoi(line.substr(1,2)) + 2000); // prn
								for (int i=0; i<num_data; i++)
								{
									try{
										each_record.push_back(std::stod(line.substr(3+i*16, 16)));
									}
									catch (std::invalid_argument ) {
										each_record.push_back(0);
									}
									
								}
								obsGlonass.push_back(each_record);
								break;
						case 'C': each_record.push_back(std::stoi(line.substr(1,2)) + 3000); // prn
								for (int i=0; i<num_data; i++)
								{
									try{
										each_record.push_back(std::stod(line.substr(3+i*16, 16)));
									}
									catch (std::invalid_argument ) {
										each_record.push_back(0);
									}
								}
								obsBeidou.push_back(each_record);
								break;
						case 'J': each_record.push_back(std::stoi(line.substr(1,2)) + 5000); // prn
								for (int i=0; i<num_data; i++)
								{
									try{
										each_record.push_back(std::stod(line.substr(3+i*16, 16)));
									}
									catch (std::invalid_argument ) {
										each_record.push_back(0);
									}

								}
								obsQzss.push_back(each_record);
								break;
						default: break;		

					}
				}
			}
			getline(obs_file, line);
		}
	}

	obs_data.header = rinex_obs_header;
	obs_data.obsGPS = obsGps;
	obs_data.obsGAL = obsGalileo;
	obs_data.obsBEIDOU = obsBeidou;
	obs_data.obsGLO = obsGlonass;
	obs_data.obsQZSS = obsQzss;

	return obs_data;
}

