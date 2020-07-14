#include <iostream>
#include <fstream>
#include "pro_rinex.h"


void to_kml() 
{
	
	return 0;
}

void main(int argc, char *argv[]) 
{
	bool header = false;
	if (arc > 2 or arc < 1)
	{
		std::cout << "It requires at least one parameter for observation file or at most two parameters for observation file and navigation file" << std::endl;
		return 0;
	}
	else if (arc == 2)
	{
		Pro_rinex rinex_worker;
    	rinex_worker.read_rinex_obs(argv[1], header);
		rinex_worker.read_rinex_nav(argv[2]);	
	}
	else if (arc == 1)
	{
		Pro_rinex rinex_worker;
    	rinex_worker.read_rinex_obs(argv[1], header);
    	rinex_worker.get_nav_ftp(header);
	}	
	
	return 0;
}




