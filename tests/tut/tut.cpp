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
#include <ph_debug/debug.hpp>


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
generator <ph::promise <generator, gen_iterator, T>> range(T first, const T last) {
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
    
    struct _promise
    {
        void return_void () {
            cout << "return_void ()" << endl;
        }
        coro  get_return_object () {
            cout << "get_return_object" << endl;
            return coroutine_handle <_promise> :: from_promise (*this);
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
    using promise_type = _promise;
    coroutine_handle <_promise> handle;
    coro (coroutine_handle<_promise> h) : handle {h} {}
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

struct phthread : thread
{
    using thread::thread;
    
    string started_time;
    string started_by;
    
    
    void join (string s = __builtin_FUNCTION ()) {
        start (move (s));
        thread::join();
    }
    void detach (string s = __builtin_FUNCTION ()) {
        start (move (s));
        thread::detach();
    }
    ~phthread () {
//        cout << "~phthread ()" << endl;
        cout << "========================================" << endl;
        cout << started_by << "\n\t time: " << started_time << "~ " << now () << "\n\tthread: " << this_thread::get_id() << endl;
        cout << "========================================" << endl;
        thread::~thread();
    }
    
private:
    void start (string&& started_by) {
        started_time = now ();
        this->started_by = move (started_by);
    }
};


coro hej () {
    cout << "begin" << endl;
    co_await awaitablee {};
    cout << "end" << endl;
}


void fun () {
//    auto _d = debug (__PRETTY_FUNCTION__);
//    debug (1)
    this_thread::sleep_for(2s);
    cout << this_thread::get_id() << endl;
    
//    this_thread::sleep_for(2s);
}

int heavy_work (int a, int b) {
    this_thread::sleep_for(1s);
    return a + b;
}

void thread_based () {
    auto task = packaged_task <decltype (heavy_work)> {heavy_work};
    auto futur = task.get_future ();
    thread {move (task), 1, 2}.detach();
    cout << futur.get() << endl;
}

void task_based () {
    cout << async (heavy_work, 1, 2).get() << endl;
}



int main(int argc, char const *argv[])
{
//    debug (1)
    auto futur = async (heavy_work, 1, 2);
    auto futur2 = async (heavy_work, 2, 3);
    cout << futur.get() << endl;
    cout << futur2.get() << endl;
//    phthread th1 {fun};
//
//    th1.join();
//    cout << this_thread::get_id() << endl;
//    this_thread::sleep_for(2s);

    
//    auto c = hej ();
//    c.go();

//    std::this_thread::sleep_for(std::chrono::seconds(5));
//    auto res = coro_function <generator<int>>  ();
//    int a = res;
//    cout << res() << endl;
//
//
//    for (char i : range(65, 91)) {
//            std::cout << i << ' ' << endl;
//    }
    
//	int result = Catch::Session().run( argc, argv );
//	return result;
    return 0;
}
