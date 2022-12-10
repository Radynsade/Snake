// Including [min; max].
int randomInt(int min, int max) {
	return rand() % (max + 1 - min) + min;
}
