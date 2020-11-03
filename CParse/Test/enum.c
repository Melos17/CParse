
enum {A1, A2, A3};
enum B {B1 = 5, B2, B3};
enum C {C1 = 3, C2 = 1, C3};

int main() {
	if (A1 != 0) {
		printf("%d", A1);
		return -1;
	}
	if (A2 != 1) {
		printf("%d", A2);
		return -2;
	}
	if (A3 != 2) {
		printf("%d", A3);
		return -3;
	}
	
	if (B1 != 5) {
		printf("%d", B1);
		return -11;
	}
	if (B2 != 6) {
		printf("%d", B2);
		return -12;
	}
	if (B3 != 7) {
		printf("%d", B3);
		return -13;
	}
	
	if (C1 != 3) {
		printf("%d", C1);
		return -21;
	}
	if (C2 != 1) {
		printf("%d", C2);
		return -22;
	}
	if (C3 != 2) {
		printf("%d", C3);
		return -23;
	}
	
	return 0;
}
