#include <string>
#include <map>
#include <vector>

class ReadObsRinex
{
	public:
		void read_rinex_obs(std::string osb_file_name);
		void read_rinex_nav(std::string nav_file_name);
		void get_nav_ftp(std::string d_t);
};

