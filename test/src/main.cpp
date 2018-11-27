#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cpptest.h>

#include "util/ToolsUnitTest.h"

// Show usage for cmd line
static void usage() {
	std::cout << "usage: mytest [MODE]\n"
		 << "where MODE may be one of:\n"
		 << "  --compiler\n"
		 << "  --html\n"
		 << "  --text-terse (default)\n"
		 << "  --text-verbose\n";
	exit(0);
}

// Command line output
static std::auto_ptr<Test::Output> cmdline(int argc, char* argv[]) {
	if (argc > 2)
		usage(); // will not return

	Test::Output* output = 0;

	if (argc == 1)
		output = new Test::TextOutput(Test::TextOutput::Verbose);
	else {
		const char* arg = argv[1];
		if (strcmp(arg, "--compiler") == 0)
			output = new Test::CompilerOutput;
		else if (strcmp(arg, "--html") == 0)
			output =  new Test::HtmlOutput;
		else if (strcmp(arg, "--text-terse") == 0)
			output = new Test::TextOutput(Test::TextOutput::Terse);
		else if (strcmp(arg, "--text-verbose") == 0)
			output = new Test::TextOutput(Test::TextOutput::Verbose);
		else {
			std::cout << "invalid commandline argument: " << arg << std::endl;
			usage(); // will not return
		}
	}

	return std::auto_ptr<Test::Output>(output);
}

// Unit tests start here
int main(int argc, char* argv[]) {
  try {
		// Load in all test suites
		Test::Suite ts;
		ts.add(std::auto_ptr<Test::Suite>(new ToolsUnitTest));

		// Run the tests
		std::auto_ptr<Test::Output> output(cmdline(argc, argv));
		ts.run(*output, true);

		// Output to html
		Test::HtmlOutput* const html = dynamic_cast<Test::HtmlOutput*>(output.get());
		if (html)
			html -> generate(std::cout, true, "JumpingJimothyTests");
	}
	catch (...) {
		std::cout << "unexpected exception encountered\n";
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
