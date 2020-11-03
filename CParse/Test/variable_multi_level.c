int e;

int func(int a, int b) {
	int c = 30;
	int d = 10;
	d = 20;
	e = 1;
	{
		int e = 50;
		int f;
		f = 10;
		return a + b + c + d + e;
	}
	return 0;
}

int func2(int a, int b) {
	int c = 30;
	int d = 10;
	d = 20;
	e = 1;
	{
		int e = 50;
		int f;
		f = 10;
		b = 100;
	}
	return a + b + c + d + e;
}

int main() {
	int a = 5;
	return a + func(1, 2) + func2(1, 2);
}
