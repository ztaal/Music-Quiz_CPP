#pragma once

#include <string>
#include <chrono>

namespace common {
	/**
	 * General time utility
	 */
	class TimeUtil
	{
	public:
		/** Deleted Constructor */
		TimeUtil() = delete;

		/** Deleted Constructor */
		~TimeUtil() = delete;

		/** 
		 * @brief Get the current time in GMT as a string of the format Www Mmm dd hh:mm:ss yyyy 
					Where Www is weekday, Mmm the month (in letters), dd the day hh:mm:ss the time, and yyyy the year.
		 *
		 * @return The current time.
		 */
		static std::string getTimeNow();
	};
}