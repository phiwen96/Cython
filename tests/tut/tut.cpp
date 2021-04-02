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
#include <semaphore>
#include <condition_variable>
#include <exception>

#include <cppcoro/task.hpp>
#include <cppcoro/sync_wait.hpp>
#include <ph_color/color.hpp>
#include <ph_coroutines/handle.hpp>
//#include <ph_coroutines/timer.hpp>

using namespace std;
using namespace experimental;
using namespace ph::concepts;
using namespace chrono_literals;





    











struct resumable {
#define class_name "resumable"
    struct promise_type {
        coroutine_handle <> m_continuation;
        resumable get_return_object () {
            return resumable {*this};
        }
        auto initial_suspend () {
            thread{[&] () mutable {
                resume();
            }}.detach();
            return suspend_always {};
        }
        
        auto final_suspend () noexcept {
            
            struct awaitable {
                bool await_ready () noexcept {
                    return false;
                }
                coroutine_handle<> await_suspend (coroutine_handle <promise_type> thisCoro) noexcept {
                    auto& promise = thisCoro.promise();

                    if (promise.m_continuation) {
                        return static_cast <coroutine_handle<>> (promise.m_continuation);
                    }
                    return noop_coroutine();
                }
                void await_resume () noexcept {
                    
                }
            };
            return awaitable {};
        }
        template <typename T>
        auto yield_value (T&& lambda) {
            struct awaitable {
                bool await_ready () noexcept {
                    return false;
                }
                coroutine_handle<> await_suspend (coroutine_handle <promise_type> thisCoro) noexcept {
//                    thread {[this, thisCoro]()mutable{
//                        thisCoro.resume();
//                    }}.detach();

//                    cout << "fdfd" << endl;
                    auto& promise = thisCoro.promise();
                    if (promise.m_continuation) {
                        return static_cast <coroutine_handle<>> (promise.m_continuation);
                    }
                    return noop_coroutine();
                }
                void await_resume () noexcept {
                    
                }
            };
            return awaitable {};
        }
        auto return_value (int i) {
            
        }
        auto unhandled_exception () {
            throw runtime_error ("oops");
        }
        bool done () {
            return coroutine_handle<promise_type>::from_promise(*this).done();
//            return m_handle.done();
        }
        bool resume () {
            if (not coroutine_handle<promise_type>::from_promise(*this).done())
                coroutine_handle<promise_type>::from_promise(*this).resume();
            
            return not coroutine_handle<promise_type>::from_promise(*this).done();
        }
    };
    auto resume () {
        return m_promise.resume();
    }
    
    bool await_ready () {
        return m_promise.done();
    }
    auto await_suspend (coroutine_handle <> continuation) noexcept {
        m_promise.m_continuation = continuation;
        return true;
//        return coroutine_handle<promise_type>::from_promise (m_promise);
    }
    void await_resume () {
        
    }
    promise_type& m_promise;
    resumable (resumable const&) = delete;
    resumable (promise_type& promise) : m_promise {promise} {}
    resumable (resumable&& o) : m_promise {o.m_promise} {}
    resumable& operator=(resumable&&) = delete;
    resumable& operator=(resumable const&) = delete;
};




#define class_name "FUNCTION"
#define PRINT(x) debug_class_print_called_from (yellow, 0, string (yellow) + x);

resumable counter4 (debug_called_from) {
//    debug_class_print_called_from (yellow, 0)
    co_await suspend_never{};
//    co_await suspend_always {};
    
    cout << "hi" << endl;
    cout << "counter4..." << endl;
    co_return 2;
}
resumable counter3 (debug_called_from) {
//    debug_class_print_called_from (yellow, 0)
    co_await suspend_never{};
//    co_await suspend_always {};

    cout << "hi" << endl;
    cout << "counter3..." << endl;
    co_return 2;
}

resumable counter2 (debug_called_from) {
//    debug_class_print_called_from (yellow, 0)
    co_await counter3();
    co_await counter4();
    cout << "hi" << endl;
    cout << "counter2..." << endl;
    co_return 2;
}

resumable counter (debug_called_from) {
//    debug_class_print_called_from (yellow, 0)

    
    co_await counter2 ();
    
    cout << "yo" << endl;
//    co_yield 3;
//    co_await suspend_always {};


//    co_await *a; // creates a callable object coroutine_handle <> whose invocation continues execution of the current function
      /**
       The compiler creates a coroutine handle and passes
       it to the method a.await_suspend (coroutine_handle).
       */
    cout << "counter..." << endl;
    co_return 2;
}



int main(int argc, char const *argv[]) {
 
    string info {""};
    debug_called_from_none
    


    
    char const* lines = "================================================================================================================";
//    cout << white << lines << info << endl << white << lines << endl;// << red << lines << white << info  << red << lines << endl << endl;
    cout << red << lines << endl << endl;

    cout << white;
//    coroutine_handle<ReturnObject::promise_type> my_handle;
    int i = 0;
    resumable r = counter ();
    cout << green << lines << endl << white;
    this_thread::sleep_for(1s);
//    r.resume();
//    r.resume();

//    r ();
    
//    cout << counters_handle.called_from_function << endl;
//    counters_handle ();
//    counters_handle ();
//    counters_handle ();

//    if (i <= 0){
//        ++i;
//        counters_handle ();
//    }
//    counters_handle ();
//    cout << endl << lines << endl << endl;
    for (int i = 0; i < 2; ++i) {
//        debug_class_print_called_from(yellow, 0)
//        debug_class_print_called_from (yellow, 0, counters_handle.promise().value, string (yellow) + "waking up counter!")
//        counters_handle ();
//        h ();
//        cout << h.promise().value << endl;
    }

    cout << red << endl << lines << endl;



    return 0;
}
