
int main(int argc, char** argv) {
	int a = 0;
	int* pa = &a;
	int** ppa = &pa;
	*pa = 10;
	**ppa = 1;
	return a;
}
