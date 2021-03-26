#include <catch2/catch.hpp>
#include <experimental/coroutine>
#include <concepts>
#include <ph_concepts/concepts.hpp>
using namespace std;
using namespace experimental;
using namespace ph::concepts;

template <class T, class U>
concept convertible = std::is_convertible_v<T, U>;
template <class T, class U>
concept same = std::is_same_v<T, U>;





struct resumable
{
    struct promise
    {
        auto get_return_object () {
            return resumable {coroutine_handle<promise>::from_promise(*this)};
        }
        auto initial_suspend () {
            return suspend_always {};
        }
        auto final_suspend () noexcept {
            return suspend_always {};
        }
        void return_void () {}
        [[noreturn]] void unhandled_exception () {
            terminate();
        }
    };
    
    using promise_type = promise;
    coroutine_handle <promise_type> handle;
    
    explicit resumable (coroutine_handle <promise_type> h) : handle {h} {
        
    }
    ~resumable () {
        // destroy the coroutine
        if (handle)
        {
            handle.destroy();
        }
    }
    bool resume () {
        // resumes the coroutine from the point it was suspended
        if (not handle.done())
        {
            handle.resume ();
        } 

        return not handle.done ();
    }

};


template <coroutine T>
auto coro_function () -> T
{
    cout << "0" << endl;
    co_await suspend_always {};
    cout << "1" << endl;
}






TEST_CASE ("")
{
 
    auto res = coro_function <resumable> ();
    res.resume ();
//    res.resume ();


    
}
