#include <catch2/catch.hpp>
#include <experimental/coroutine>
#include <concepts>

template <class T, class U>
concept convertible = std::is_convertible_v<T, U>;
using namespace std;
using namespace experimental;


template <typename T>
struct Generator
{
	struct promise_type;
	using handle_type = coroutine_handle<promise_type>;

	struct promise_type
	{
		T value_;
		exception_ptr exception_;

		Generator get_return_object()
		{
			return Generator(handle_type::from_promise(*this));
		}
		suspend_always initial_suspend() { return {}; }
		suspend_always final_suspend() noexcept { return {}; }
		void unhandled_exception() { exception_ = current_exception(); }
		template <convertible<T> From> // C++20 concept
		suspend_always yield_value(From &&from)
		{
			value_ = forward<From>(from);
			return {};
		}
		void return_void() {}
	};

	handle_type h_;

	Generator(handle_type h) : h_(h) {}
	~Generator() 
	{ 
		h_.destroy(); 
	}
	explicit operator bool()
	{
		fill();
		return !h_.done();
	}
	T operator()()
	{
		fill();
		full_ = false;
		return move(h_.promise().value_);
	}

private:
	bool full_ = false;

	void fill()
	{
		if (!full_)
		{
			h_();
			if (h_.promise().exception_)
				rethrow_exception(h_.promise().exception_);
			full_ = true;
		}
	}
};

Generator<unsigned>
counter6()
{
	for (unsigned i = 0; i < 3;)
		co_yield i++;
}

TEST_CASE()
{
	auto gen2 = counter6();
//	cout << gen2() << endl;
    
//	while (gen2)
//		cout << "counter6: " << gen2() << endl;
    
    
	
}









template <class... T>
auto foreach2(T... args)
{
    bool b[] = { [&args](){
      return true;
    }()... };
}

TEST_CASE ("")
{
    foreach2(0, 1, 2);
}




template <class... T>
struct overload : T...
{
    overload (T&&... t) : T{forward <T> (t)}...
    {

    }
};
template<class... Ts> overload(Ts...) -> overload<decay_t<Ts>...>;


struct state0
{
    
};

struct state1
{
    
};




TEST_CASE ("")
{
    variant <state0, state1> v = state1{};
    v = visit (overload{[&](state0&)->state1{cout << "state0" << endl; return {};}, [&](state1&)-> state1{cout << "state1" << endl; return {};}}, v);
//    overload ([](int i){cout << "int" << endl;}, [](string s){cout << "string" << endl;})(v);
}
