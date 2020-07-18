#include <string>

class RinexObs
{
	string rinex_ver;
	std::map<string, double> approx_pos_xyz;
	vector<string> obs_type_gps;
	vector<string> obs_type_glonass;
	vector<string> obs_type_galileo;
	vector<string> obs_type_beidou;
	vector<string> obs_type_qzss;
	string t_first_obs;
}
