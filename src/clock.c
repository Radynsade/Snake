int64_t currentMicroseconds() {
	struct timespec tms;
	clock_gettime(CLOCK_REALTIME, &tms);
	int64_t microseconds = tms.tv_sec * 1000000;
	microseconds += tms.tv_nsec/1000;

	return microseconds;
}

float microsecondsToSeconds(int64_t microseconds) {
	return (float)microseconds / 1000000.0F;
}
