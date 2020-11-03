
char *p, *lp, // current position in source code
     *data;   // data/bss pointer

int *e, *le,  // current position in emitted code
    *id,      // currently parsed identifier
    *sym,     // symbol table (simple list of identifiers)
    tk,       // current token
    ival,     // current token value
    ty,       // current expression type
    loc,      // local variable offset
    line,     // current line number
    src,      // print source and assembly flag
    debug;    // print executed instructions
	
int main() {
	sym = malloc(256);
	id = sym;
	id[2] = 1;
	
	if (id[2] != 1) return -1;
	
	if ((1 && 1) || 0) {
		return -2;
	}
	
	return 0;
}