#ifndef TIME_H
#define TIME_H

#include <chrono>

class TimeUtil {
public:
	const static inline std::chrono::system_clock::time_point now()
	{
		return std::chrono::system_clock::now();
	}
	const static inline double timeFrom
		(const std::chrono::system_clock::time_point &start)
	{
		auto delta = std::chrono::system_clock::now() - start;
		auto deltaSeconds = std::chrono::duration_cast
			<std::chrono::milliseconds>(delta).count() / 1000.0;

		return deltaSeconds;
	}
};

#endif