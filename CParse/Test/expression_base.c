int arr[50];
int *pa;

int main() {
	int a = 1;
	if (++a != 2) return -1;
	if (a++ != 2) return -2;
	
	arr[2] = ++a;
	if (arr[2] != 4) return -11;
	
	pa = arr;
	pa[3] = ++a;
	if (pa[3] != 5) return -12;
	
	return 0;
}