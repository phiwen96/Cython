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
#include <ph_coroutines/task.hpp>
//#include <ph_coroutines/timer.hpp>

using namespace std;
using namespace experimental;
using namespace ph::concepts;
using namespace chrono_literals;





    














template <bool threading = true>
task <string, threading> async_read_file (filesystem::path const& p) {
    auto ss = ostringstream {};
    ifstream input_file(p);
    if (!input_file.is_open()) {
        cerr << "Could not open the file - '"
             << p << "'" << endl;
        exit (EXIT_FAILURE);
    }
    ss << input_file.rdbuf();
    co_return ss.str();
}

task <void> async_write_file (filesystem::path const& p, string const& s) {
    ofstream output_file;
    output_file.open (p);
    if (!output_file.is_open()) {
        cerr << "Could not open the file - '"
             << p << "'" << endl;
        exit (EXIT_FAILURE);
    }
    output_file << s;
}


#define class_name "FUNCTION"
#define PRINT(x) debug_class_print_called_from (yellow, 0, string (yellow) + x);

task <int>  counter4 (debug_called_from) {
    debug_class_print_called_from (yellow, 0)
    co_await suspend_never{};
//    co_await suspend_always {};
    this_thread::sleep_for(1s);

    cout << "hi" << endl;
    cout << "counter4..." << endl;
    co_return 2;
}
task <int> counter3 (debug_called_from) {
    debug_class_print_called_from (yellow, 0)
    co_await suspend_never{};
//    co_await suspend_always {};

    cout << "hi" << endl;
    cout << "counter3..." << endl;
    co_return 2;
}

task <int> counter2 (debug_called_from) {
    debug_class_print_called_from (yellow, 0)
    co_await counter3();
    co_await counter4();
    
    cout << "hi" << endl;
    cout << "counter2..." << endl;
    co_return 2;
}

task <int> counter (debug_called_from) {
    debug_class_print_called_from (yellow, 0)

    
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


task <int, false> run () {
    cout << "hi" << endl;
    task <string> t1 = async_read_file ("/Users/philipwenkel/GitHub/Cython/tests/apptest/sources/a.hpp.in");
    string s1 = co_await t1;
    cout << "hi" << endl;
//    string s2 = co_await async_read_file <false> ("/Users/philipwenkel/GitHub/Cython/tests/apptest/sources/a.hpp.in");
    
    
    cout << s1 << endl;
//    cout << s2 << endl;

    
//    task r = counter ();
//    task r2 = counter ();
//    co_await r;
//    co_await r2;
    cout << "done" << endl;
}

int main(int argc, char const *argv[]) {
 
    
    string info {""};
    debug_called_from_none
    


    
    char const* lines = "================================================================================================================";
//    cout << white << lines << info << endl << white << lines << endl;// << red << lines << white << info  << red << lines << endl << endl;
    cout << red << lines << endl << endl;

    cout << white;
//    coroutine_handle<ReturnObject::promise_type> my_handle;
        
    run ();
    cout << green << lines << endl << white;
    counter();
    this_thread::sleep_for(2s);
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
