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

using namespace std;
using namespace experimental;
using namespace ph::concepts;
using namespace chrono_literals;





    







template <typename... promise>
struct co_handle;


template <>
struct co_handle <> : coroutine_handle <>{
    #define class_name "co_handle<>"
    using base = coroutine_handle <>;
    using base::coroutine_handle;
    using base::operator=;
    using base::operator bool;
    using base::operator();
    using base::done;
    using base::address;
    using base::from_address;
    using base::destroy;
    using base::resume;
    string called_from_function;
    int called_from_line;
    co_handle (debug_called_from) : called_from_function {move (_called_from_function)}, called_from_line {_called_from_line} {
        debug_class_print_called_from(cyan, 0);
    }
    template <class T>
    co_handle (T&& h, debug_called_from) : base {forward<T>(h)}, called_from_function {move (_called_from_function)}, called_from_line {_called_from_line} {
//        debug_class_print_called_from(cyan, 0);
    }
};

template <typename promise>
struct co_handle <promise> : coroutine_handle <promise>{
    #define class_name "co_handle<promise>"
    using base = coroutine_handle <promise>;
//    using base::coroutine_handle;
//    using base::operator=;
    using base::operator bool;
//    using base::operator();
    using base::done;
    using base::address;
    using base::from_address;
    using base::destroy;
//    using base::resume;
    string called_from_function;
    int called_from_line;
    co_handle (debug_called_from) : called_from_function {move (_called_from_function)}, called_from_line {_called_from_line} {
        debug_class_print_called_from(cyan, 0);
    }
    template <class U>
    co_handle (U&& h, debug_called_from) : base {h}, called_from_function {move (_called_from_function)}, called_from_line {_called_from_line} {
        debug_class_print_called_from(cyan, 0);
    }
    static auto from_promise (promise& p, bool write_out = true, debug_called_from) -> decltype (auto) {
        if (write_out)
            debug_class_print_called_from(cyan, 0);
        return base::from_promise (p);
    }
    
    static co_handle <promise> from_promise (promise& p, string const& called_function, int called_line) {
        return {base::from_promise (p), called_function, called_line};
    }
    
private:
    co_handle (coroutine_handle <promise> handle, string const& _called_from_function, int _called_from_line) : base::coroutine_handle {handle}, called_from_function {_called_from_function}, called_from_line {_called_from_line}{
        debug_class_print_called_from (cyan, 0)
    }
};

struct ReturnObject {
    #define class_name "ReturnObject"
    struct promise_type;
    co_handle <promise_type>& handle;
    ReturnObject (co_handle<promise_type>& handle, debug_called_from) : handle {handle} {debug_class_print_called_from (red, 0);}
    operator co_handle<> () {
        return handle;
    }
    
    using co_function_return_value = ReturnObject;
    
    struct promise_type {
        #define class_name "ReturnObject::promise_type"
        
        co_handle <promise_type> my_handle;
 
        promise_type (debug_called_from) : my_handle {co_handle<promise_type>::from_promise (*this, _called_from_function, _called_from_line)} {debug_class_print_called_from (red, 0);}
        co_function_return_value get_return_object(debug_called_from) {debug_class_print_called_from(red, 0); return my_handle; }
        suspend_never initial_suspend(debug_called_from) {debug_class_print_called_from(red, 0); return {}; }
        suspend_never final_suspend(debug_called_from) noexcept {debug_class_print_called_from(red, 0); return {}; }
        void unhandled_exception(debug_called_from) {debug_class_print_called_from(red, 0);}
        void return_void (debug_called_from) {debug_class_print_called_from(red, 0);}
    };
};

struct Awaitable {
    #define class_name "Awaitable"
    co_handle <> * hp_;
    bool await_ready(debug_called_from) {
//        debug_print_called_from (yellow, 0)
        return false;
    }
    void await_suspend (co_handle <> h, debug_called_from) {
        string color;
        if (hp_) {
            color = hp_->called_from_function == h.called_from_function ? blue : green;
        }
        else {
            color = blue;
        }
//        out("hej", text{"kuk", green, white}, "hora");
        debug_class_print_called_from(yellow, 0);
        
        cout << endl << "storing {" << text {_called_from_function, blue} << "}'s handle into {" << color << hp_ -> called_from_function << white << "}'s handle.   " <<  _called_from_function + "::" + to_string (_called_from_line) << endl << endl;
//        D1(yellow, 0)
        //        cout << "FFFF" << endl;
//        hp_->resume();
        *hp_ = h;
    }
    void await_resume (debug_called_from) {
        debug_class_print_called_from (yellow, 0)
    }
};




ReturnObject counter (debug_called_from) {
//    debug_class_print_called_from(magenta, 0);
    
    
//  Awaitable a {continuation_out};
    for (int i = 0; ; ++i){
        
        co_await suspend_always {};
    }
    
    
//    co_await Awaitable {a};
//  for (unsigned i = 0; i < 1; ++i) {
//      cout << "00" << endl;

//    co_await *a; // creates a callable object coroutine_handle <> whose invocation continues execution of the current function
      /**
       The compiler creates a coroutine handle and passes
       it to the method a.await_suspend (coroutine_handle).
       */
      

//    std::cout << "counter: " << i << std::endl;
//  }
    cout << "counter..." << endl;
    
}


int main(int argc, char const *argv[]) {
    string info {""};
    debug_called_from_none
    
#define class_name "MAIN"

    
    char const* lines = "================================================================================================================";
//    cout << white << lines << info << endl << white << lines << endl;// << red << lines << white << info  << red << lines << endl << endl;
    cout << red << lines << endl << endl;

    cout << white;
    
    co_handle <> h = counter();
//    cout << endl << lines << endl << endl;
    for (int i = 0; i < 2; ++i) {
        debug_class_print_called_from(yellow, 0)
        h ();
    }

    cout << red << endl << lines << endl;



    return 0;
}
