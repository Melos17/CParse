
int main(int argc, char** argv) {
	// type conversion
	int a = (char)(0x80);
	if (a != -128) return -1;
	
	a = (unsigned char)a;
	if (a != 0x80) return a;
	
	a = (char)(0x7f) + (char)(0x1);
	if (a != 128) return -3;
	
	a = (short)(0x8000);
	if (a != -32768) return -4;
	
	a = (short)(0x7fff) + (short)(0x1);
	if (a != 32768) return -5;
	
	// type conversion
	a = (int)(unsigned char)0xff; // 255
	if (a != 255) return -51;
	
	a = (unsigned int)(char)0xff; // 4294967295
	if (a != -1) return -52;
	
	a = (int)(char)0xff ; // -1
	if (a != -1) return -53;
	
	a = (unsigned int)(unsigned char)0xff; // 255
	if (a != 255) return -54;
	
	return 0;
}
