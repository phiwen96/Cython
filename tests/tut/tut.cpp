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

struct default_sentinel_t { };

template <class T>
requires requires (typename T::promise_type p) {
    p.value;
}
struct generator_iter
{
    using promise_type = typename T::promise_type;
public:
    void operator++() {
        handle.resume();
    }
    auto operator*() -> decltype (auto) {
        return handle.promise().value;
    }
    bool operator==(default_sentinel_t) const {
        return !handle || handle.done();
    }
    explicit generator_iter (coroutine_handle <promise_type> handle) : handle{handle} {}

private:
    coroutine_handle <promise_type> handle;
};

template <class T>
struct generator
{
    struct promise
    {
        T value;
        
        auto get_return_object () -> decltype (auto) {
            return generator {coroutine_handle<promise>::from_promise(*this)};
        }
        static constexpr auto initial_suspend () noexcept {
            return suspend_always{};
        }
        static constexpr auto final_suspend () noexcept {
            return suspend_always{};
        }
        [[nodiscard]] auto yield_value (auto&& u) noexcept {
            value = forward <decltype (u)> (u);
            return suspend_always{};
        }
        constexpr void return_void () {}
        void await_transform() = delete;
        [[noreturn]] static void unhandled_exception () {
            throw;
//            terminate();
        }
        [[nodiscard]] static void* operator new (size_t sz) {
//            cout << "allocation heap for coroutine" << endl;//: " << sz << " bytes" << endl;
            return ::operator new (sz);
        }
        [[nodiscard]] static void operator delete (void* ptr) {
            ::operator delete (ptr);
//            cout << "deallocating heap for coroutine" << endl;
        }
    };
    
    using promise_type = promise;
    coroutine_handle <promise_type> handle;
    
    explicit generator (coroutine_handle <promise_type> h) : handle {h} {
        
    }
    generator (generator&& other) : handle {exchange (other.handle, {})} {
        
    }
    generator& operator=(generator&& other) noexcept {
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
    ~generator () {
        // destroy the coroutine
        if (handle)
        {
            handle.destroy();
        }
    }
    generator(generator const&) = delete;
    generator& operator=(const generator&) = delete;
    explicit operator bool() {
            return !handle.done();
        }
    auto operator () () -> decltype (auto) {
        // will allocate a new stack frame and store the return-address of the caller in the
        // stack frame before transfering execution to the function.
        
        // resumes the coroutine from the point it was suspended
        if (handle.done())
        {
            throw runtime_error ("no, coroutine is done");
        }
        
        handle.resume();
        return handle.promise().value;
    }
    
    // Range-based for loop support.
//       class Iter {
//       public:
//           void operator++() {
//               handle.resume();
//           }
//           auto operator*()  -> decltype (auto) {
//               return handle.promise().value;
//           }
//           bool operator==(default_sentinel_t) const {
//               return !handle || handle.done();
//           }
//           explicit Iter (coroutine_handle <promise_type> handle) : handle{handle} {}
//
//       private:
//           coroutine_handle <promise_type> handle;
//       };
    
    generator_iter<generator<T>> begin() {
        if (handle) {
            handle.resume();
        }
        return generator_iter<generator<T>>{handle};
    }
    default_sentinel_t end() {
        return {};
    }

};


template <coroutine T>
T coro_function ()
{
    cout << "0" << endl;
//    co_await suspend_always {};
   
    co_yield 6;
    cout << "1" << endl;
}


template<typename T>
generator<T> range(T first, const T last) {
    while (first < last) {
        co_yield first++;
    }
}

int main(int argc, char const *argv[])
{
    auto res = coro_function <generator<int>>  ();
    cout << res() << endl;

    
    for (char i : range(65, 91)) {
            std::cout << i << ' ' << endl;
    }
    
//	int result = Catch::Session().run( argc, argv );
//	return result;
    cout << "===============================================================================" << endl;
    return 0;
}
