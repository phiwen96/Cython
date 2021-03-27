//#define CATCH_CONFIG_RUNNER
//#include <catch2/catch.hpp>
#include <experimental/coroutine>
#include <concepts>
#include <ph_concepts/concepts.hpp>
//#include <ph_coroutines/coroutines.hpp>
#include <ph_coroutines/generator/generator.hpp>
#include <ph_coroutines/generator/iterator.hpp>
#include <ph_coroutines/promise.hpp>
#include <ph_time/time.hpp>
#include <ph_time/Date.hpp>
#include <ph_macros/macros.hpp>

using namespace std;
using namespace experimental;
using namespace ph::concepts;
using namespace std::chrono_literals;



//struct default_sentinel_t { };







template <coroutine T>
T coro_function ()
{
    cout << "0" << endl;
//    co_await suspend_always {};
   
    co_yield 6;
    cout << "1" << endl;
}


template<typename T>
generator <promise <generator, gen_iterator, T>> range(T first, const T last) {
    while (first < last) {
        co_yield first++;
    }
}


void independentThread()
{
    std::cout << "Starting concurrent thread.\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "Exiting concurrent thread.\n";
}
 
void threadCaller()
{
    std::cout << "Starting thread caller.\n";
    std::thread t(independentThread);
    t.detach();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Exiting thread caller.\n";
}




struct [[nodiscard]] coro
{
    
    struct promise
    {
        void return_void () {
            cout << "return_void ()" << endl;
        }
        coro  get_return_object () {
            cout << "get_return_object" << endl;
            return coroutine_handle <promise> :: from_promise (*this);
        }
        auto initial_suspend () {
            return suspend_always{};
        }
        auto final_suspend () noexcept {
            return suspend_always {};
        }
        void unhandled_exception () {
        
        }
    };
    using promise_type = promise;
    coroutine_handle <promise> handle;
    coro (coroutine_handle<promise> h) : handle {h} {}
    void go () {
        
        if (handle.done())
            throw runtime_error ("");
        
        handle.resume();
        
    }
};

struct awaitablee {
    bool await_ready () {
        return false;
    }
    void await_suspend (coroutine_handle<> h) {
        cout << "hi" << endl;
        thread([]{cout << "thread" << endl;}).detach();
        
    }
    void await_resume () {
        
    }
};



coro hej () {
    cout << "begin" << endl;
    co_await awaitablee {};
    cout << "end" << endl;
}

#define DEBUGGING 1

struct _debug {
    string function_name;
    string time_called;
    __thread_id thread_id;
    
    _debug (string&& function_name) : function_name {function_name}, time_called {now ()}, thread_id {this_thread::get_id()} {
//        cout << function_name << " (...) time: " << time_called << " thread: " << thread_id << endl;
    }
    ~_debug () {
        cout << "========================================" << endl;
        cout << function_name << "\n\t time: " << time_called << "~ " << now () << "\n\tthread: " << thread_id << endl;
        cout << "========================================" << endl;
    }
};

[[nodiscard]] auto debug (string called = __builtin_FUNCTION ()) -> _debug {
    return {move (called)};
}

#define CURRENT_FUNCTION __PRETTY_FUNTION__

#define debug(x) IF_ELSE(x)(auto _##__COUNTER__ = debug (__PRETTY_FUNCTION__))();

void fun () {
//    auto _d = debug (__PRETTY_FUNCTION__);
    debug (1)
    cout << this_thread::get_id() << endl;
    
//    this_thread::sleep_for(2s);
}

int main(int argc, char const *argv[])
{

    
    
    cout << now () << endl;
    thread th1 {fun};
    th1.join();
//    cout << this_thread::get_id() << endl;
//    this_thread::sleep_for(2s);

    
    auto c = hej ();
    c.go();
//    c.go ();
    
    return 0;
    threadCaller();
    return 0;
//    std::this_thread::sleep_for(std::chrono::seconds(5));
//    auto res = coro_function <generator<int>>  ();
//    int a = res;
//    cout << res() << endl;
//
//
    for (char i : range(65, 91)) {
            std::cout << i << ' ' << endl;
    }
    
//	int result = Catch::Session().run( argc, argv );
//	return result;
    cout << "===============================================================================" << endl;
    return 0;
}
