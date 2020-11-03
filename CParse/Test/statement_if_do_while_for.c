
int main(int argc, char** argv) {
	int a = 0;
	int sum = 0;
	// if
	if (a) return -1;
	
	// while
	a = 11;
	while (--a) {
		sum += a;
	}
	if (sum != 55) return -2;
	
	// do-while
	a = 10;
	sum = 0;
	do {
		sum += a;
	} while(--a);
	if (sum != 55) return -3;
	
	// for
	for (sum = 0; a <= 10; ++a) {
		sum += a;
	}
	if (sum != 55) return -4;
	
	// break
	for (sum = 0, a = 0; a <= 20; ++a) {
		sum += a;
		if (a == 10) break;
	}
	if (sum != 55) return -5;
	
	// continue
	for (sum = 0, a = 0; a <= 10; ++a) {
		if (a % 2) continue;
		sum += a;
	}
	if (sum != 30) return -6;
	
	/* still no support
	// switch
	switch (a) {
	case 1:
	case 2:
	case 10:
		return -7;
	case 11:
		a = 99;
	case 12:
		a += 1;
		break;
	default:
		return -8;
	}
	if (sum != 100) return -9;
	
	// enable FOR local variable
	sum = 0;
	for (int a = 0; a <= 10; ++a) {
		sum += a;
	}
	if (sum != 55) return -10;
	*/
	
	return sum;
}

