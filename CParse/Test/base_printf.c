
int main(int argc, char** argv) {
	printf("%s" "%d\n", "pass if shows 0:", 0);
	while (argc-- > 0) {
		printf("%s\n", *argv++);
	}
	return 0;
}
