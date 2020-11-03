
int main() {
	char c = 'a';
	const char* str = "abcdefg";
	int i = 1;
	int i2 = 0x7fffffff;
	long long l = 0x80000000;
	
	if (c != 97) return -1;
	
	if (str[0] != 'a') return -11;
	if (str[1] != 'b') return -12;
	if (str[2] != 'c') return -13;
	if (str[3] != 'd') return -14;
	if (str[4] != 'e') return -15;
	if (str[5] != 'f') return -16;
	if (str[6] != 'g') return -17;
	
	if (i != 1) return -21;
	if (i2 != 2147483647) return -22;
	
	if (l != 2147483648) return -31;
	
	return 0;
}
