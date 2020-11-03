
int main() {
	int arr[256];
	int i = 0;
	int* p = malloc(1024);
	
	memset(p, 0, 1024);
	
	for (; i < 256; ++i) {
		arr[i] = 0;
	}
	
	if (memcmp(arr, p, 1024) != 0) {
		return -1;
	}
	
	free(p);
	return 0;
}
