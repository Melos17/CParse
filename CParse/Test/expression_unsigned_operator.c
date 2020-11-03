
int main(int argc, char** argv) {
	int a;
	
	// shift
	a = 2 << -1;
	if (a != 0) return -11;
	
	a = 2 >> -1;
	if (a != 0) return -12;
	
	a = (unsigned int)-1 >> 1;
	if (a != 2147483647) return -13;
	
	a = -1 >> 1;
	if (a != -1) return -14;
	
	a = (unsigned int)-1 << 1;
	if (a != -2) return -15;
	
	a = -1 << 1;
	if (a != -2) return -16;
	
	// div
	a = (unsigned int)-1 / 2;
	if (a != 2147483647) return -21;
	
	a = -1 / (unsigned int)2;
	if (a != 2147483647) return -22;
	
	a = -1 / 2;
	if (a != 0) return -23;
	
	// mod
	a = (unsigned int)-1 % 7;
	if (a != 3) return -31;
	
	a = -1 % (unsigned int)7;
	if (a != 3) return -32;
	
	a = -1 % 7;
	if (a != -1) return -33;
	
	// cmp
	a = (unsigned int)-1 > 2;
	if (a != 1) return -41;
	
	a = -1 > (unsigned int)2;
	if (a != 1) return -42;
	
	a = -1 > 2;
	if (a != 0) return -43;
	
	return 0;
}
