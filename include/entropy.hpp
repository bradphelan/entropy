#include <string>
#include <string_view>
#include <ranges>
#include <iostream>
#include <source_location>
#include <memory>
#include <vector>
#include <regex>

namespace entropy {

	namespace detail {
        std::string filter(int argc, char** argv) {
			std::string filter;

			for (int i = 1; i < argc; i++) {
				if (std::string(argv[i]) == "--filter" && i + 1 < argc) {
					filter = argv[i + 1];
					break;
				}
			}

			return filter;
		}
	}

	template <typename T> class context;

	struct result {
		std::string name;
		std::source_location location;
		bool passed;
	};

	inline std::ostream & operator <<(std::ostream& os, const result& r)
	{
		os << r.name << ":" << r.location.file_name() << ":" << r.location.line() << ":" << (r.passed ? "passed" : "failed");
		return os;
	}

	using results = std::vector<result>;
	using results_ptr = std::shared_ptr<results>;
	using results_cptr = std::shared_ptr<const results>;

	class runtime {
	public:
		std::regex filter;
		runtime(std::string filter) :filter(filter) {}
	};

	using runtime_ptr = std::shared_ptr<runtime>;
	using runtime_cptr = std::shared_ptr<const runtime>;

	class context_base {
	protected:
		std::string m_name;
		results_ptr m_results;
		runtime_ptr m_runtime;
	public:
		context_base(results_ptr const& r, std::string const& name, runtime_ptr const& runtime) :
			m_results(r), 
			m_name(name), 
			m_runtime(runtime) {}

		context_base(context_base const& c) = delete;

		template <typename TRange>
		auto range(TRange&& range) const
		{
			return std::forward<TRange>(range) | std::views::transform([results = m_results, m_name = m_name, m_runtime=m_runtime]<typename T>(T v) {
				std::string name = m_name + "." + std::to_string(v);
				return context(v, results, name, m_runtime);
			});
		}

		void expect(bool state, std::source_location location = std::source_location::current()) {
			m_results->push_back({ m_name, location, state });
		}

		template <typename TF>
		void scope(TF fn) {
			if(std::regex_match(m_name,m_runtime->filter))
				fn();
		}


		results_cptr results() const {
			return m_results;
		}
	};

	template <typename T>
	class context : public context_base {
	public:

		T value;

		context(T value, results_ptr const& r, std::string const& name, runtime_ptr const& runtime) : 
			context_base(r, name, runtime), 
			value(value) {}

		template <typename TF>
		void scope(std::string name, TF fn) {
			context<T> ctxt = context(value, m_results, m_name + "." + name, m_runtime);
			ctxt.context_base::scope([&] {fn(ctxt); });
		}

	};

	template <>
	class context<void> : public context_base {
	public:
		context(results_ptr const& r, std::string const& name, runtime_ptr const& runtime) : context_base(r, name, runtime) {}

	};

	context<void> create(int argv, char** argc) {
		std::string filter = detail::filter(argv, argc);
		std::cout << "filter is " << filter << std::endl; 
		std::shared_ptr<results> r = std::make_shared<results>();
		std::shared_ptr<runtime> rt = std::make_shared<runtime>(".*"+filter+".*");
		return context<void>(r, "root", rt);
	}
}
