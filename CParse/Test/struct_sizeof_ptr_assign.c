
struct A1 {
	char a;
};

struct A2 {
	int a;
};

struct B {
	char a;
	int b;
	char c;
};

int main() {
	A1 a1;
	A2 a2;
	B b;
	A1* pa1 = &a1;
	A2* pa2 = &a2;
	B* pb = &b;
	int size = sizeof(A1);
	
	a1.a = 'b';
	a2.a = 3;
	b.a = 'c';
	b.b = 5;
	b.c = 'd';
	
	if (size != 1) return -1;
	
	size = sizeof(A2);
	if (size != 4) return -2;
	
	size = sizeof(B);
	if (size != 12) return -3;
	
	if (a1.a != 'b') return -4;
	if (a2.a != 3) return -5;
	if (b.a != 'c') return -6;
	if (b.b != 5) return -7;
	if (b.c != 'd') return -8;
	
	pb->c = 'd';
	pb->b = 5;
	pb->a = 'c';
	pa2->a = 3;
	pa1->a = 'b';
	
	if (pa1->a != 'b') return -11;
	if (pa2->a != 3) return -12;
	if (pb->a != 'c') return -13;
	if (pb->b != 5) return -14;
	if (pb->c != 'd') return -15;
	
	return 0;
}
