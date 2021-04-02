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

using namespace std;
using namespace experimental;
using namespace ph::concepts;
using namespace chrono_literals;





    











struct resumable {
    
};


#define class_name "FUNCTION"
#define PRINT(x) debug_class_print_called_from (yellow, 0, string (yellow) + x);




resumable counter (debug_called_from) {
    debug_class_print_called_from (yellow, 0)

    
    cout << "yo" << endl;
//    co_yield 3;

    

//    co_await *a; // creates a callable object coroutine_handle <> whose invocation continues execution of the current function
      /**
       The compiler creates a coroutine handle and passes
       it to the method a.await_suspend (coroutine_handle).
       */
    cout << "counter..." << endl;
}




int main(int argc, char const *argv[]) {
    string info {""};
    debug_called_from_none
    


    
    char const* lines = "================================================================================================================";
//    cout << white << lines << info << endl << white << lines << endl;// << red << lines << white << info  << red << lines << endl << endl;
    cout << red << lines << endl << endl;

    cout << white;
//    co_handle<ReturnObject::promise_type> my_handle;
    int i = 0;
//    resumable counters_handle = counter ();
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
