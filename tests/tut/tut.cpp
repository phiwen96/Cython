//#define CATCH_CONFIG_RUNNER
//#include <catch2/catch.hpp>
#include <experimental/coroutine>
#include <concepts>
#include <ph_concepts/concepts.hpp>
using namespace std;
using namespace experimental;
using namespace ph::concepts;
using namespace std::chrono_literals;

template <class T, class U>
concept convertible = std::is_convertible_v<T, U>;
template <class T, class U>
concept same = std::is_same_v<T, U>;




template <class T>
struct resumable
{
    struct promise
    {
        T value;
        
        promise (string s = __builtin_FUNCTION(), int i = __builtin_LINE()) {
            cout << "promis  " << s << " " << i << endl;
        }
        ~promise () {
            cout << "~promis  " << endl;
        }
        
        auto get_return_object (string s = __builtin_FUNCTION(), int i = __builtin_LINE()) -> decltype (auto) {
            cout << "get_return_object () " << s << " " << i << endl;
            return resumable {coroutine_handle<promise>::from_promise(*this)};
        }
        static constexpr suspend_always initial_suspend () noexcept {
            return {};
        }
        static constexpr suspend_always final_suspend () noexcept {
            return {};
        }
        
        template <typename U>
        suspend_always yield_value (U&& u) noexcept {
            value = forward <U> (u);
            return {};
        }
        constexpr void return_void () {}
        
        
        
        // Disallow co_await in generator coroutines.
//        void await_transform() = delete;
        
        [[noreturn]] static void unhandled_exception () {
            throw;
//            terminate();
        }
        
        static void* operator new (size_t sz) {
            cout << "allocation heap for coroutine" << endl;//: " << sz << " bytes" << endl;
            return ::operator new (sz);
        }
        static void operator delete (void* ptr) {
            ::operator delete (ptr);
            cout << "deallocating heap for coroutine" << endl;
        }
    };
    
    using promise_type = promise;
    coroutine_handle <promise_type> handle;
    
    explicit resumable (coroutine_handle <promise_type> h, string s = __builtin_FUNCTION()) : handle {h} {
        cout << "resumable ()" << s << endl;
    }
    resumable (resumable&& other) : handle {exchange (other.handle, {})} {
        
    }
    resumable& operator=(resumable&& other) noexcept {
           if (this != &other) {
               if (handle) {
                   handle.destroy();
               }
               swap (handle, other.handle);
           } else
           {
               throw runtime_error ("coro");
           }
           return *this;
       }
    ~resumable () {
        // destroy the coroutine
        if (handle)
        {
            handle.destroy();
        }
    }
    resumable(resumable const&) = delete;
    resumable& operator=(const resumable&) = delete;
    
    bool resume () {
        
        cout << "resuming coroutine " << "\t" << endl;
        
        // resumes the coroutine from the point it was suspended
        if (not handle.done())
        {
            
            handle.resume ();
            cout << "suspending coroutine " << "\t" <<  endl;

                // will allocate a new stack frame and store the return-address of the caller in the
                // stack frame before transfering execution to the function.
            
        } else
        {
            throw runtime_error ("no, coroutine is done");
        }
        
        return not handle.done ();
    }
    
    // Range-based for loop support.
       class Iter {
       public:
           void operator++() {
               handle.resume();
           }
           const T& operator*() const {
               return *handle.promise().current_value;
           }
           bool operator==(std::default_sentinel_t) const {
               return !handle || handle.done();
           }
    
           explicit Iter(coroutine_handle <promise_type> handle) :
           handle{handle}
           {}
    
       private:
           coroutine_handle <promise_type> handle;
       };
    
       Iter begin() {
           if (handle) {
               handle.resume();
           }
           return Iter{handle};
       }
       std::default_sentinel_t end() {
           return {};
       }

};


template <coroutine T>
T coro_function ()
{
    cout << "coro_function" << endl;
    cout << "0" << endl;
    co_await suspend_always {};
    co_return;
    cout << "1" << endl;
}


int main(int argc, char const *argv[])
{
    auto res = coro_function <resumable<int>>  ();
    res.resume ();
    res.resume ();
    
    
//	int result = Catch::Session().run( argc, argv );
//	return result;
    cout << "===============================================================================" << endl;
    return 0;
}
