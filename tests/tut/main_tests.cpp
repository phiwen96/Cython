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


//template <int, int, class...>
//struct s;



//template <int a, class... T>
//struct s <0, a, T...>  : overload <T...>
//{
//    using overload <T...>::overload;
//
//
//    template <typename machine>
//    auto execute (machine&&) -> decltype (auto) {
//        cout << "state " << a << ": working on machine" << endl;
//    }
//
//    auto handle (auto&&) -> decltype (auto) {
//        cout << "state " << a << ": working on machine" << endl;
//    }
//
//    friend ostream& operator<< (ostream& os, s const&) {
//        os << a;
//        return os;
//    }
//};
//
//template <class... T>
//struct s <2, 0, T...> : s <0, 2, T...>
//{
//    using s <0, 2, T...>::s;
////    template <typename machine>
////    [[noreturn]] auto execute (machine&&) -> decltype (auto){
//////        return {};
////    }
//    auto handle (auto&&) -> transition_to <s <2, 0, T...>> {
//        return transition_to <s <2, 0, T...>> {};
//    }
//};
//
//template <class... T>
//struct s <1, 0, T...> : s <0, 1, T...>
//{
//    using s <0, 1, T...>::s;
////    template <typename machine>
////    auto execute (machine&&) -> decltype (auto) {
//////        return {};
////    }
//
//
////    auto handle (auto&&) -> decltype (auto) {
////        return transition_to <s <1>> {};
////    }
//    auto handle (event::dollar const& e) -> decltype (auto) {
//        s <0, 1, T...>::handle (e);
//        return transition_to <s <1, 0, T...>> {};
//    }
//};



//template <>
//struct s <3> : s <3, 0>
//{
////    template <typename machine>
////    auto execute (machine&&) -> decltype (auto) {
//////        return {};
////    }
//
//    auto handle (auto&&) -> decltype (auto) {
//        return s <3> {};
//    }
//};
//
//template <>
//struct s <4> : s <4, 0>
//{
////    template <typename machine>
////    [[noreturn]] auto execute (machine&&) -> decltype (auto) {
//////        return {};
////    }
//
//    auto handle (auto&&) -> decltype (auto) {
//        return s <4> {};
//    }
//};







//#define DECL(z, n, text) struct  s ## n {};
//
//BOOST_PP_REPEAT(20, DECL, _);






template <class... T>
struct state1 : T...
{
    state1 (T&&... t) : T {forward <T> (t)}...
    {
        
    }
};


template <class... T>
struct state2 : T...
{
    state2 (T&&... t) : T {forward <T> (t)}...
    {
        
    }
};



template <typename... T>
struct machine
{
    tuple <T...> av_states;
    variant <T...> state;
    
    
    machine (T&&... t) : av_states {forward <T> (t)...}, state {get <0> (av_states)}
    {
        
    }
};

TEST_CASE ("")
{
    string input = "$(0 x 2)";
//    overload d {[](){return 2;}};
//    s<1, 0> s{[](){return 2;}};
//    state_machine <s <1>, s <2>> machine;
    machine m
    {
        state1
        {
            [](event::dollar, auto&& m)
            {
                
            },
            
            []<typename T>(T&&, ...)
            {
                
            }
        },
        
        state2
        {
            [](event::dollar, auto&& m)
            {
                
            },
            
            []<typename T>(T&&, ...)
            {
                
            }}
    };
    
    
//    machine.current_state = visit (overload {[](auto&&)->s<1>{cout << "hola s1" << endl;return{};}}, machine.current_state);
//
//    for (char i : input)
//    {
//        switch (i) {
//            case '$':
//                machine.handle (event::dollar {});
//                break;
//
//            default:
//                break;
//        }
//    }
        
    
    
}










