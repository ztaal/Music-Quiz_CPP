#include "TimeUtil.hpp"

#include <time.h>
#include <iomanip>


std::string common::TimeUtil::getTimeNow()
{
	time_t timer;
	time(&timer);
	const std::string str = static_cast<std::string>(asctime(gmtime(&timer)));
	return str.substr(0, str.length() - 1);
}