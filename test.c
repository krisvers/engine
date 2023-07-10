#include <stdio.h>

#define KEYCODE_MAP_GLFW_KEY(key, keycode) case GLFW_KEY_##key: return KEYCODE_##keycode;
#define KEYCODE_MAP_GLFW_KEY_SAME_NAME(key) KEYCODE_MAP_GLFW_KEY(key, key)

#define GLFW_KEY_DELETE 6
#define KEYCODE_DELETE 2

unsigned char convert(int value) {
	switch (value) {
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(DELETE);
	}
}

int main(void) {
	printf("%d\n", convert(GLFW_KEY_DELETE));
}
