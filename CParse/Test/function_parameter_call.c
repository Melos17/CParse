int e;

int func(int a, int b) {
	int c = 30;
	int d = 10;
	d = 20;
	e = 1;
	return a + b + c + d + e;
}

int main() {
	return 5 + func(1, 2);
}