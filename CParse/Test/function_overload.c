
int f1(int a, int b, short c) {
	return a * b / c;
}

int f1(int a, int b) {
	return a + b;
}

int main() {
	return f1(2, 3) + f1(2, 8, (char)3);
}
