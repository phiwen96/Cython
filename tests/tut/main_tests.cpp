#include <catch2/catch.hpp>
#include <experimental/coroutine>
#include <concepts>
#include <cython/state_machine.hpp>
namespace {
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
    
//    variant <state0, state1> v = state1{};
//    v = visit (overload{[&](state0&)->state1{cout << "state0" << endl; return {};}, [&](state1&)-> state1{cout << "state1" << endl; return {};}}, v);
//    overload ([](int i){cout << "int" << endl;}, [](string s){cout << "string" << endl;})(v);
}

}





struct event
{
    struct l_paran {
        friend ostream& operator<< (ostream& os, l_paran const&) {
            os << "(";
            return os;
        }
    };
    struct r_paran {
        friend ostream& operator<< (ostream& os, r_paran const&) {
            os << ")";
            return os;
        }
    };
    struct l_bracket {
        friend ostream& operator<< (ostream& os, l_bracket const&) {
            os << "{";
            return os;
        }
    };
    struct r_bracket {
        friend ostream& operator<< (ostream& os, r_bracket const&) {
            os << "}";
            return os;
        }
    };
    struct dollar {
        friend ostream& operator<< (ostream& os, dollar const&) {
            os << "$";
            return os;
        }
    };
    struct alpha {
        friend ostream& operator<< (ostream& os, alpha const&) {
            os << "@";
            return os;
        }
    };
    struct hashtag {
        friend ostream& operator<< (ostream& os, hashtag const&) {
            os << "#";
            return os;
        }
    };
};



template <int a, int...>
struct s {
    template <typename machine>
    auto execute (machine&&) -> decltype (auto){
        cout << "state " << a << ": working on machine" << endl;
    }
    
    friend ostream& operator<< (ostream& os, s const&) {
        os << a;
        return os;
    }
};

template <>
struct s <2> : s <2, 0>
{
    
    
//    template <typename machine>
//    [[noreturn]] auto execute (machine&&) -> decltype (auto){
////        return {};
//    }
    template <typename event>
    auto handle (event&&) -> decltype (auto) {
        return s <2> {};
    }
};

template <>
struct s <1> : s <1, 0>
{
//    template <typename machine>
//    auto execute (machine&&) -> decltype (auto) {
////        return {};
//    }
    
   
    auto handle (auto&&) -> decltype (auto) {
        return transition_to <s <1>> {};
    }
    auto handle (event::dollar) -> decltype (auto) {
        return transition_to <s <2>> {};
    }
};



template <>
struct s <3> : s <3, 0>
{
//    template <typename machine>
//    auto execute (machine&&) -> decltype (auto) {
////        return {};
//    }

    auto handle (auto&&) -> decltype (auto) {
        return s <3> {};
    }
};

template <>
struct s <4> : s <4, 0>
{
//    template <typename machine>
//    [[noreturn]] auto execute (machine&&) -> decltype (auto) {
////        return {};
//    }

    auto handle (auto&&) -> decltype (auto) {
        return s <4> {};
    }
};







//#define DECL(z, n, text) struct  s ## n {};
//
//BOOST_PP_REPEAT(20, DECL, _);













TEST_CASE ("")
{
    string input = "$(0 x 2)";
    state_machine <s <1>, s <2>, s <3>> machine;
    for (char i : input)
    {
        switch (i) {
            case '$':
                machine.handle (event::dollar {});
                break;
                
            default:
                break;
        }
    }
        
    
    
}










