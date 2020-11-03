
int c(int*, int) {
	return 1;
}

typedef int (*pc)(int*, int);

pc ffa(int) {
	return 0;
}



int main(int a) {
	int(*(ffa(int))) (int*, int);
	int(*((*ff)(int))) (int*, int) = ffa;
}

