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
using namespace chrono_literals;



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


















struct co_task {
    struct promise;
    using promise_type = promise;
    coroutine_handle <promise_type> handle;
    explicit co_task (coroutine_handle <promise_type> handle) noexcept : handle {handle} {}
    co_task (co_task&& other) noexcept : handle {exchange (other.handle, {})} {}
    ~co_task () {if (handle) handle.destroy();}
    
    struct awaiter;
    awaiter operator co_await () && noexcept;
    
};

struct co_task::promise {
    coroutine_handle <> continuation;
    
    struct final_awaiter {
        bool await_ready () noexcept {return false;}
        void await_suspend (coroutine_handle <promise_type> h) noexcept {h.promise().continuation.resume();}
        void await_resume () noexcept {}
    };
    
    co_task get_return_object () noexcept {
        return co_task {coroutine_handle <promise_type>::from_promise(*this)};
    }
    suspend_always initial_suspend () noexcept {return {};}
    void return_void () noexcept {}
    [[noreturn]] void unhandled_exceptions () noexcept {terminate();}
    final_awaiter final_suspend () noexcept {return {};}
};


//eager<int> eager_answer() {
////    cout << "Thinking deep thoughts..." << endl;
//    co_return 42;
//}

co_task lazy_answer() {
//    cout << "Thinking deep thoughts..." << endl;
    cout << "a0" << endl;
//    co_yield 62;
    cout << "a1" << endl;
    co_return 42;
}

co_task await_lazy_answer() {
//    cout << "Started await_answer" << endl;
//    auto a = lazy_answer();
//    cout << "Got a coroutine, let's get a value" << endl;
    cout << "b0" << endl;
    co_await lazy_answer();
//    cout << "::::" << v << endl;
    cout << "b1" << endl;

//    cout << "And the coroutine value is: " << v << endl;
//    v = co_await a;
//    cout << "And the coroutine value is still: " << v << endl;
    co_return 10;
}










//lazy <int> test ()
//{
//    cout << "t0" << endl;
//    co_await suspend_always {};
//    cout << "t1" << endl;
//    co_return 4;
//}

int main(int argc, char const *argv[])
{
//    auto coro = lazy_answer();
//    cout << coro.get() << endl;
//    cout << "=========================" << endl;
//    cout << coro.get() << endl;
//
//    return 0;

    auto a = await_lazy_answer();
    cout << a.resume() << endl;
    cout << "=========================" << endl;
//    cout << a.resume() << endl;
//    cout << a.get() << endl;
    
    
    
    
    
    
//    cout << a.get() << endl;
//    cout << a.get() << endl;
//    cout << a.get() << endl;
//    cout << a.resume() << endl;
//    cout << a.resume() << endl;
//    cout << a.resume() << endl;
    
//    return await_lazy_answer().get();
//        cout << "And the coroutine value is: " << v << endl;

//	int result = Catch::Session().run( argc, argv );
//	return result;
    return 0;
}
