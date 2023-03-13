#include "entropy.hpp"
#include <vector>
#include <format>

///  function under test which is implemented
int times(int i, int j) { return i * j; }

TEST("advanced", [](entropy::context<void> const & ctxt) {
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

TEST("simple", [](entropy::context<void> const & ctxt) {
	namespace e = entropy;

	// Create a data driven suite as a nested loop
	// over two vectors. 
	for (auto i : std::vector<int>{ 1, 3, 8, 12 }) {
		for (auto j : std::vector<int>{ 21, 55 }) {

			// Generate the label for this section
			auto path = std::format("{}.{}", i, j);

			// Code inside the scope block will only execute if it's
			// name matches the filter passed on the command line
			ctxt.scope(path + ".correct", [&](auto& scope) {
				std::cout << "running " << scope.name() << std::endl;
				scope.expect(times(i, j) == i * j);
			});

			ctxt.scope(path + ".incorrect", [&](auto& scope) {
				std::cout << "running " << scope.name() << std::endl;
				scope.expect(times(i, j) == i + j);
			});
		}
		
	}
});

