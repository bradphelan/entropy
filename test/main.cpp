#include "entropy.hpp"
#include <vector>

int times(int i, int j) { return i + j; }

int main(int argv, char** argc) {

	namespace e = entropy;

	// Create the root context and parse the command line options
	auto ctxt = e::create(argv, argc);

	// Create a data driven suite as a nested loop
	// over two vectors. 
	for (e::context<int> i : ctxt.range(std::vector<int>{ 1, 3, 8, 12 })) {
		for (e::context<int> j : i.range(std::vector<int>{ 21, 55 })) {
			// Code inside the scope block will only execute if it's
			// name matches the filter passed on the command line
			j.scope("correct", [&](auto & scope) {
				scope.expect(times(i.value, j.value) == i.value * j.value);
			});

			j.scope("incorrect", [&](auto & scope) {
				scope.expect(times(i.value, j.value) == i.value + j.value);
			});
		}
	}

	// Print out all the results
	for (auto scope : *ctxt.results()) {
		std::cout << scope << std::endl;

	}
}
