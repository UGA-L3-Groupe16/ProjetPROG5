#include <stdio.h>
#include "../util.h"

int main(int argc, const char *argv[]) {
	printf("%d", get_bits(0b110010101, 4, 0));
	return 0;
}
