#include "entropy.hpp"
#include <vector>

///  function under test
int times(int i, int j) { return i + j; }

TEST("foo", [](entropy::context<void> const & ctxt) {
	namespace e = entropy;

	// Create a data driven suite as a nested loop
	// over two vectors. 
	for (e::context<int> i : ctxt.range(std::vector<int>{ 1, 3, 8, 12 })) {
		for (e::context<int> j : i.range(std::vector<int>{ 21, 55 })) {
			// Code inside the scope block will only execute if it's
			// name matches the filter passed on the command line
			j.scope("correct", [&](e::context_base & scope) {
				std::cout << "running " << scope.name() << std::endl;
				scope.expect(times(i.value, j.value) == i.value * j.value);
			});

			j.scope("incorrect", [&](auto & scope) {
				std::cout << "running " << scope.name() << std::endl;
				scope.expect(times(i.value, j.value) == i.value + j.value);
			});
		}
	}
});

TEST("bar", [](entropy::context<void> const & ctxt) {
	namespace e = entropy;

	// Create a data driven suite as a nested loop
	// over two vectors. 
	for (e::context<int> i : ctxt.range(std::vector<int>{ 1, 3, 8, 12 })) {
		for (e::context<int> j : i.range(std::vector<int>{ 21, 55 })) {
			// Code inside the scope block will only execute if it's
			// name matches the filter passed on the command line
			j.scope("correct", [&](auto & scope) {
				std::cout << "running " << scope.name() << std::endl;
				scope.expect(times(i.value, j.value) == i.value * j.value);
			});

			j.scope("incorrect", [&](auto & scope) {
				std::cout << "running " << scope.name() << std::endl;
				scope.expect(times(i.value, j.value) == i.value + j.value);
			});
		}
	}
});

