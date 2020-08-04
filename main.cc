#include <iostream>
#include <fstream>
#include <string>
//#include "/Users/keweizhang/Documents/rinex-to-position/include/read_obs_rinex.h"
#include "read_obs_rinex.h"


int main(int argc, char *argv[]) 
{
	bool header = false;
	std::string rinex_obs;
	std::string rinex_nav;
	if (argc > 3 || argc < 2)
	{
		std::cout << "It requires at least one parameter for observation file or at most two parameters for observation file and navigation file" << std::endl;
	}
	else if (argc == 3)
	{
		ReadObsRinex rinex_worker;
		rinex_obs = argv[1];
		rinex_nav = argv[2];
    	rinex_worker.read_rinex_obs(rinex_obs);
		// rinex_worker.read_rinex_nav(rinex_nav);	
	}
	else if (argc == 2)
	{
		ReadObsRinex rinex_worker;
		std::string d_t;
		rinex_obs = argv[1];
    	rinex_worker.read_rinex_obs(rinex_obs);
    	// rinex_worker.get_nav_ftp(d_t);
	}

	return 0;
}




