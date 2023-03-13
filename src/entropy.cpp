#include "entropy.hpp"

int main(int argv, char** argc) {
	namespace e = entropy;

	// Create the root context and parse the command line options
	auto ctxt = e::create(argv, argc);

	auto const& tcs = e::test();
	for (auto const& tc : tcs)
		tc.runner(entropy::context<void>( ctxt.results(), tc.name, ctxt.runtime()));

	// Print out all the results
	for (auto scope : *ctxt.results()) {
		std::cout << scope << std::endl;

	}
}

std::string entropy::detail::filter(int argc, char** argv) {
	std::string filter;

	for (int i = 1; i < argc; i++) {
		if (std::string(argv[i]) == "--filter" && i + 1 < argc) {
			filter = argv[i + 1];
			break;
		}
	}

	return filter;
}


entropy::context<void> entropy::create(int argv, char** argc) {
	std::string filter = detail::filter(argv, argc);
	std::cout << "filter is " << filter << std::endl;
	std::shared_ptr<results> r = std::make_shared<results>();
	std::shared_ptr<runtime> rt = std::make_shared<runtime>(".*" + filter + ".*");
	return context<void>(r, "root", rt);
}

entropy::test_cases const& entropy::test(
	std::string name,
	std::optional<test_case::runner_fn> t,
	std::source_location location
)
{
	location;

	static test_cases ts;
	if (t)
		ts.push_back({  name, *t, location });
	return ts;
}

