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
    struct promise;
    friend struct promise;
    using handle_type = std::experimental::coroutine_handle<promise>;
    coreturn(const coreturn &) = delete;
    coreturn(coreturn &&s)
    : coro(s.coro) {
        std::cout << "Coreturn wrapper moved" << std::endl;
        s.coro = nullptr;
    }
    ~coreturn() {
        std::cout << "Coreturn wrapper gone" << std::endl;
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
        promise() {
            std::cout << "Promise created" << std::endl;
        }
        ~promise() {
            std::cout << "Promise died" << std::endl;
        }
        auto return_value(T v) {
            std::cout << "Got an answer of " << v << std::endl;
            value = v;
            return std::experimental::suspend_never{};
        }
        auto final_suspend() noexcept {
            std::cout << "Finished the coro" << std::endl;
            return std::experimental::suspend_always{};
        }
        void unhandled_exception() {
            std::exit(1);
        }
    private:
        T value;
    };
protected:
    T get() {
        return coro.promise().value;
    }
    coreturn(handle_type h)
    : coro(h) {
        std::cout << "Created a coreturn wrapper object" << std::endl;
    }
    handle_type coro;
};


template<typename T>
struct eager : public coreturn<T> {
    using coreturn<T>::coreturn;
    using handle_type = typename coreturn<T>::handle_type;
    T get() {
        std::cout << "We got asked for the return value..." << std::endl;
        return coreturn<T>::get();
    }
    struct promise_type : public coreturn<T>::promise {
        auto get_return_object() {
            std::cout << "Send back a sync" << std::endl;
            return eager<T>{handle_type::from_promise(*this)};
        }
        auto initial_suspend() {
            std::cout << "Started the coroutine, don't stop now!" << std::endl;
            return std::experimental::suspend_never{};
        }
    };
};


template<typename T>
struct lazy : public coreturn<T> {
    using coreturn<T>::coreturn;
    using handle_type = typename coreturn<T>::handle_type;;
    T get() {
        std::cout << "We got asked for the return value..." << std::endl;
        this->coro.resume();
        return coreturn<T>::get();
    }
    struct promise_type : public coreturn<T>::promise {
        auto get_return_object() {
            std::cout << "Send back a lazy" << std::endl;
            return lazy<T>{handle_type::from_promise(*this)};
        }
        auto initial_suspend() {
            std::cout << "Started the coroutine, put the brakes on!" << std::endl;
            return std::experimental::suspend_always{};
        }
    };
};



eager<int> answer_0() {
    std::cout << "Thinking deep thoughts..." << std::endl;
    co_return 42;
}

lazy<int> answer_1() {
    std::cout << "Thinking deep thoughts..." << std::endl;
    co_return 42;
}



int main(int argc, char const *argv[])
{


    
    auto a = answer_1();
        std::cout << "Got a coroutine, let's get a value" << std::endl;
        a.get();
//        std::cout << "And the coroutine value is: " << v << std::endl;

//	int result = Catch::Session().run( argc, argv );
//	return result;
    return 0;
}
