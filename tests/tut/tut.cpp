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









template<typename T>
struct coreturn {
    static inline int coro_nr = 0;
    int coro_int = coro_nr;
    struct promise;
    friend struct promise;
    using handle_type = coroutine_handle<promise>;
    coreturn(const coreturn &) = delete;
    coreturn(coreturn &&s)
    : coro(s.coro) {
//        cout << "Coreturn wrapper moved" << endl;
        s.coro = nullptr;
        ++coro_nr;
    }
    ~coreturn() {
//        cout << "Coreturn wrapper gone" << endl;
        if ( coro ) coro.destroy();
    }
    coreturn &operator = (const coreturn &) = delete;
    coreturn &operator = (coreturn &&s) {
        coro = s.coro;
        s.coro = nullptr;
        return *this;
    }
    struct promise {
        friend struct coreturn;
        int coro_int = coro_nr;
        promise() {
//            cout << "Promise created" << endl;
        }
        ~promise() {
//            cout << "Promise died" << endl;
        }
        
        /**
         co_return, whether or not we want to suspend
         the coroutine after it returns a value to us
         */
        auto return_value(T v) {
            cout << coro_int << ":Got an answer of " << v << endl;
            value = v;
            return suspend_never{};
        }
        
        /**
         Now that we've finished the coroutine,
         we need to say whether we want to stop here or not
         */
        auto final_suspend() noexcept {
            cout << coro_int << ":Finished the coro with value " << value << endl;
            
            return suspend_always{};
        }
        
        /**
         What should happen if an exception tries
         to escape from the coroutine?
         */
        void unhandled_exception() {
            exit(1);
        }
        auto yield_value(T value) {
            cout << coro_int << ":Yielded " << value << endl;
            this->value = value;
            return suspend_always{};
        }
//    private:
        T value;
    };
protected:
    
    /**
     needs to pull the value from
     the coroutine_handle
     */
    T get() {
        return coro.promise().value;
    }
    coreturn(handle_type h)
    : coro(h) {
        ++coro_nr;
//        cout << "Created a coreturn wrapper object" << endl;
    }
    handle_type coro;
};


template<typename T>
struct eager : public coreturn<T> {
    using coreturn<T>::coreturn;
    using handle_type = typename coreturn<T>::handle_type;
    T get() {
//        cout << "We got asked for the return value..." << endl;
        if ( not this->coro.done() ) this->coro.resume();
        return coreturn<T>::get();
    }
    struct promise_type : public coreturn<T>::promise {
        auto get_return_object() {
//            cout << "Send back a sync" << endl;
            return eager<T>{handle_type::from_promise(*this)};
        }
        auto initial_suspend() {
//            cout << "Started the coroutine, don't stop now!" << endl;
            return suspend_never{};
        }
    };
};


template<typename T>
struct lazy : public coreturn <T> {
    using parent = coreturn <T>;
    using coreturn<T>::coro_int;
    using coreturn<T>::coro;
    using coreturn<T>::coreturn;
    using handle_type = typename coreturn<T>::handle_type;
//    using coreturn<T>::coro;
    
    /**
     resume the execution of the
     coroutine at a time of our choosing.
     */
    T get() {
        cout << coro_int << ":We got asked for the return value..." << endl;
        
        /**
         make sure we only resume
         the coroutine the first
         time get is called
         
                done will return true only when
                the coroutine is at its final
                suspension point
         */
        if ( not parent::coro.done() )
        {
            parent::coro.resume();
        }
        
        return parent::get();
    }
    
    
//    bool resume () {
//        cout << coro_int << ":We are resuming" << endl;
//
//        if (not coreturn<T>::coro.done()) {
//            coreturn<T>::coro.resume();
//        }
//        return coreturn<T>::coro.done();
//    }
    
    bool await_ready() {
        const auto ready = coro.done();
        std::cout << coro_int << ":Await " << (ready ? "is ready" : "isn't ready") << std::endl;
        return coro.done();
    }
    void await_suspend(std::experimental::coroutine_handle<> awaiting) {
        std::cout << coro_int << ":About to resume the lazy" << std::endl;
        coro.resume();
        std::cout << coro_int << ":About to resume the awaiter" << std::endl;
//        awaiting.resume();
        return awaiting;
    }
    auto await_resume() {
        const auto r = coro.promise().value;
        std::cout << coro_int << ":Await value is returned: " << r << std::endl;
        return r;
    }
    
    struct promise_type : public coreturn<T>::promise
    {
        
        /**
         We tell the promise_type how to construct
         the sync object that the coroutine returns
         */
        auto get_return_object()
        {
//            cout << "Send back a lazy" << endl;
            return lazy<T>{handle_type::from_promise(*this)};
        }
        
        /**
         whether we want to start on the body
         straight away or not.
         */
        auto initial_suspend()
        {
//            cout << "Started the coroutine, put the brakes on!" << endl;
            return suspend_always{};
        }
    };
};



eager<int> eager_answer() {
//    cout << "Thinking deep thoughts..." << endl;
    co_return 42;
}

lazy<int> lazy_answer() {
//    cout << "Thinking deep thoughts..." << endl;
    cout << "a0" << endl;
    co_yield 62;
    cout << "a1" << endl;
    co_return 42;
}

lazy<int> await_lazy_answer() {
//    cout << "Started await_answer" << endl;
//    auto a = lazy_answer();
//    cout << "Got a coroutine, let's get a value" << endl;
    cout << "b0" << endl;
    int v = co_await lazy_answer();
    cout << "b1" << endl;

//    cout << "And the coroutine value is: " << v << endl;
//    v = co_await a;
//    cout << "And the coroutine value is still: " << v << endl;
    co_return v;
}

template <class T>
concept lazy_coroutine = requires (typename T::promise_type p) {
    {p.initial_suspend ()} -> convertible <suspend_always>;
};

int main(int argc, char const *argv[])
{
    auto a = await_lazy_answer();
    cout << a.get() << endl;
    cout << a.get() << endl;
    cout << a.get() << endl;
    cout << a.get() << endl;
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
