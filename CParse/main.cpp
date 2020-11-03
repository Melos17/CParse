#include <iostream>

#include "test.h"

using std::cin;
using std::cout;
using std::endl;

int main(int argc, const char** argv) {
	string file;
	cin >> file;
	if (file.find(".txt") != string::npos) {
		TestManager::getInstance().run("Test/main.txt");
	}
	else if (file.find(".c") != string::npos) {
		std::ifstream ifs(file);
		string codes((istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
		CParse parse(codes);
		parse.init();
		parse.parse();

		shared_ptr<Command> command = parse.fetchCommand();
		VirtualMachine vm(*command);

		// need to converse to 64bits if you run at 32bits
		if (1) // special for c4 case
		{
			const char** argv2 = new const char*[argc + 1];
			argv2[0] = argv[0];
			for (int i = 0; i < argc; ++i) {
				argv2[i + 1] = argv[i];
			}
			BaseValueType ret = vm.run(argc, argv2);
			cout << "result:" << ret << endl;
		}
		else {
			const char** argv2 = new const char*[argc *2];
			argv2[0] = argv[0];
			for (int i = 0; i < argc; ++i) {
				argv2[i * 2] = argv[i];
			}
			BaseValueType ret = vm.run(argc, argv2);
			cout << "result:" << ret << endl;
		}

	}
	else {
		cout << "invalid path!" << endl;
	}
	system("pause");
	return 0;
}
