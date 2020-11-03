
void test(int a[5][5]) {
	int i;
	int j;
	for (i = 4; i >= 0; --i) {
		for (j = 4; j >= 0; --j) {
			a[i][j] = i * 5 + j;
		}
	}
}

int get(int a[5][5]) {
	return sizeof(a);
}

int main(int argc, char** argv) {
	int a[5][5]; // = {1,2,3,4,5,6,7,8,9,10,};
	int sum = 0;
	int i;
	int j;
	
	if (sizeof(int [5][5]) != 100) {
		printf("%d\n", sizeof(int [5][5]));
		return -1;
	}
	if (get(a) != 8) { // 8 in 64bits
		printf("%d\n", get(a));
		return -2;
	}
	
	for (i = 0; i < 5; ++i) {
		for (j = 0; j < 5; ++j) {
			a[i][j] = i * 5 + j;
		}
	}
	for (i = 0; i < 5; ++i) {
		for (j = 0; j < 5; ++j) {
			sum += a[i][j];
		}
	}
	if (sum != 300) {
		printf("%d\n", sum);
		return -3;
	}
	
	sum = 0;
	for (i = 4; i >= 0; --i) {
		for (j = 4; j >= 0; --j) {
			a[i][j] = i * 5 + j;
		}
	}
	for (i = 0; i < 5; ++i) {
		for (j = 0; j < 5; ++j) {
			sum += a[i][j];
		}
	}
	if (sum != 300) {
		printf("%d\n", sum);
		return -4;
	}
	
	sum = 0;
	for (i = 0; i < 5; ++i) {
		for (j = 0; j < 5; ++j) {
			a[i][j] = 0;
		}
	}
	test(a);
	for (i = 0; i < 5; ++i) {
		for (j = 0; j < 5; ++j) {
			sum += a[i][j];
		}
	}
	if (sum != 300) {
		printf("%d\n", sum);
		return -5;
	}
	
	return 0;
}