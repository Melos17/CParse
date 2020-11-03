
int main() {
	int fd = open("Test/system_func_file_input.txt", "r");
	char str[100];
	int i;
	
	if (fd < 0) {
		printf("open fail\n");
		return -1;
	}
	
	if ((i = read(fd, str, 99)) <= 0) {
		printf("read fail\n");
		return -2;
	}
	str[i] = 0;
	
	printf("read result:%s\n", str);
	
	close(fd);
	
	return 0;
}
