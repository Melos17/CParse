int g = 3;

int get(int a) {
	int b = 2;
	int c;
	c = 3;
	return a * b / c;
}

int main(int argc, char** argv) {
	int a = 1;
	int b = 2;
	g = 4;
	return a + b + g - 5 + get(b);
}
