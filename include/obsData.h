#ifndef OBSDATA_H
#define OBSDATA_H

#include <string>
#include <map>
#include <vector>

struct RinexObsHeader
{
public:
	std::string rinex_v;
	std::map<char, double> approx_pos_xyz;
	std::vector<std::string> obs_type_gps;
	std::vector<std::string> obs_type_glonass;
	std::vector<std::string> obs_type_galileo;
	std::vector<std::string> obs_type_beidou;
	std::vector<std::string> obs_type_qzss;
	std::map<std::string, std::string> t_first_obs;
};

struct  ObsData
{
    public:
	    RinexObsHeader header;
	    std::vector<std::vector<double> > obsGPS;
	    std::vector<std::vector<double> > obsGAL;
	    std::vector<std::vector<double> > obsBEIDOU;
	    std::vector<std::vector<double> > obsGLO;
	    std::vector<std::vector<double> > obsQZSS;
};

#endif