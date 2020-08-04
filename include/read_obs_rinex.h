#ifndef READ_OBS_RINEX_H
#define READ_OBS_RINEX_H

#include <string>
#include <map>
#include <vector>
#include "obsData.h"

class ReadObsRinex
{
	public:
		ObsData read_rinex_obs(std::string osb_file_name);
};

#endif