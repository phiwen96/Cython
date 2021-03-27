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
#include <ph_time/timer.hpp>
#include <future>


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
    this_thread::sleep_for(10s);
    return a + b;
}

void thread_based () {
    auto task = packaged_task <decltype (heavy_work)> {heavy_work};
    auto futur = task.get_future ();
    cout << "hi" << endl;
//    thread {move (task), 1, 2}.detach();
    futur.wait();
}

void task_based () {
    async (heavy_work, 1, 2).wait();
}





auto random_int (int min, int max) {
    static thread_local auto engine = default_random_engine {random_device{}()};
    auto dist = uniform_int_distribution<>{min, max};
    return dist (engine);
}

atomic<int> goal = random_int(0, 10000);
atomic<bool> done {false};

atomic_flag flag;

template <typename T>
struct co_future
{
    struct co_awaiter
    {
        
    };
    
    struct co_promise
    {
        T value;
        
        // a reference to to the coroutine waiting for the task to complete AKA a waiting coroutine
        coroutine_handle <co_promise> continuation;
        
        // producing the value returned from the invocation of the coroutine
        co_future <T> get_return_object () {
            return co_future {*this};
        }
        
        // behaviour when the coroutine is created
        static constexpr auto initial_suspend () noexcept {
            struct awaitable
            {
                // call await_ready
                constexpr bool await_ready () const noexcept {return false;}
                
                // if not ready
                constexpr void await_suspend (coroutine_handle <>) const noexcept {}
                
                // if ready
                constexpr void await_resume () const noexcept {}
            };
            
//            return awaitable{};
//            return awaitable {};
            return suspend_always {};
//            return suspend_never {};
        }
        
        // behaviour before the coroutine is destroyed
        auto final_suspend () noexcept {
            struct awaitable
            {
                bool await_ready () noexcept {return false;}
                auto await_suspend (coroutine_handle <co_promise> h) noexcept {
                    return h.promise().continuation;
//                    return h;
                }
                int await_resume () noexcept {return 3;}
            };
            
//            return awaitable{};
//            return suspend_never {};
            return suspend_always {};

        }
        
        // customizing behaviour for when a value is yielded from the coroutine
        [[nodiscard]] auto yield_value (auto&& u) noexcept {
//            cout << "yielding " << u << endl;
            value = forward <decltype (u)> (u);
            return suspend_always{};
        }
        
        // customizing the behaviour (before) when the coroutine finally returns
        void return_value (auto&& v) {
//            cout << "yo" << endl;
//            cout << "returning " << v << endl;
            value = forward <decltype (v)> (v);
        }
        
//        void return_value () {
////            cout << "yo" << endl;
////            cout << "returning " << v << endl;
//        }
//        void return_void () {
//
//        }
        
//        auto await_transform() {
//
//        }
        
        // handling exceptions that are not handled inside the coroutine body
        [[noreturn]] static void unhandled_exception () {
            cout << "oooops" << endl;
            terminate();
        }
    };
    
    using promise_type = co_promise;
    
    coroutine_handle <promise_type> handle;
    
    co_future () = default;
    co_future (co_future const&) = delete;
    
    co_future (promise_type& p) : handle {coroutine_handle<promise_type>::from_promise (p)} {}
    co_future (co_future&& other) : handle {exchange (other.handle, {})} {}
    ~co_future () {
        if (handle)
            handle.destroy ();
    }
    
    // resumes the coroutine from the point it was suspended.
    // returns "still work to do?"
    bool resume () {
        if (not handle.done())
        {
            handle.resume();
//            throw std::runtime_error ("no, coroutine is done");
        }
//        else
//        {
//            throw runtime_error ("");
//        }
        return !handle.done();
        
//        return handle.promise().value;
    }
    T get () {
        return handle.promise().value;
    }
    
    // true -> await_suspend
    bool await_ready () const {
        cout << "await_ready" << endl;
        return false;
    }
    
    // if await_ready = false
    auto await_suspend (coroutine_handle<> h) {
        cout << "await_suspend" << endl;
        
//        handle.promise().continuation = h;
//        return h.promise().continuation;
        return handle;
    }
    
    // if awai_ready = true
    T await_resume () {
        cout << "await_resume" << endl;
        return handle.promise().value;
    }
    
};

co_future <int> a () {
    cout << "a0" << endl;
    co_yield 4;
    cout << "a1" << endl;
    co_return 4;
//    co_return 42;
//    co_return 43;
}
co_future <int> b () {
    cout << "b0" << endl;
//    auto aa = a ();
//    co_future<int> answer = a();
//    answer.resume();
    co_await a ();
    cout << "b1" << endl;
    co_return 10;
}

int main(int argc, char const *argv[])
{
//    thread_based();
    auto coro = b();
    cout << coro.resume () << endl;
    
    
//    fu.resume(); // resume from main
//    int aa = async([coro = move (coro)] () mutable {
//        this_thread::sleep_for(2s);
//        cout << coro.resume () << endl;
//        return coro.get ();
//    }).get ();
//    cout << aa << endl;
    return 0;
//    array<int, 10> dices;
//    fill (dices.begin(), dices.end(), 0);
    
//    auto l = latch {10};
    cout << goal << endl;
    
    
    
   
    
    
    
  
    
//    auto latc = latch {4};
//    latc.try_wait();
//    latc.try_wait();
////    auto bar = barrier{}.
//    auto futur = async (heavy_work, 1, 2);
//    auto futur2 = async (heavy_work, 2, 3);
//    cout << futur.get() << endl;
//    cout << futur2.get() << endl;
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
