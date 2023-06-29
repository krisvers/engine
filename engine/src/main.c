#include <stdio.h>
#include <defines.h>
#include <test.h>

int main(void) {
	printf("Hello, World!\nPlatform: %s\n", KPLATFORM_STRING);
	print_int(69420);

	return 0;
}
