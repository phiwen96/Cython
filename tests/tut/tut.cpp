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





    














task <string> async_read_file (filesystem::path const& p) {
//    auto ss = ostringstream {};
//    ifstream input_file(p);
//    if (!input_file.is_open()) {
//        cerr << "Could not open the file - '"
//             << p << "'" << endl;
//        exit (EXIT_FAILURE);
//    }
//    ss << input_file.rdbuf();
//    co_return ss.str();
    co_return "bajs";
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

task <int>  counter5 (debug_called_from) {
    debug_class_print_called_from (yellow, 0)
//    co_await suspend_never{};
//    co_await suspend_always {};
    this_thread::sleep_for (3s);

//    cout << "hi" << endl;
    cout << "counter5..." << endl;
    co_return 2;
}

task <int>  counter4 (debug_called_from) {
    debug_class_print_called_from (yellow, 0)
//    co_await suspend_never{};
//    co_await suspend_always {};
    this_thread::sleep_for (2s);

//    cout << "hi" << endl;
    cout << "counter4..." << endl;
    co_return 2;
}

task <int> counter3 (debug_called_from) {
    debug_class_print_called_from (yellow, 0)
//    co_await suspend_never{};
    this_thread::sleep_for (1s);

//    co_await suspend_always {};

//    cout << "hi" << endl;
    cout << "counter3..." << endl;
    co_return 2;
}

task <int> counter2 (debug_called_from) {
    debug_class_print_called_from (yellow, 0)
    Timer<true> t {"hej"};
//    co_await counter5();
//    co_await counter4();
//    co_await counter3();
    auto c5 = counter5 ();
    auto c4 = counter4 ();
    auto c3 = counter3 ();

    co_await c5;
    cout << "yay5" << endl;
    co_await c4;
    cout << "yay4" << endl;
    co_await c3;
    cout << "yay3" << endl;

    
    
//    cout << "hi" << endl;
    cout << "counter2..." << endl;
    co_return 2;
}



task <int> counter (debug_called_from) {
    debug_class_print_called_from (yellow, 0)
    co_await counter2 ();

//    co_await *a; // creates a callable object coroutine_handle <> whose invocation continues execution of the current function
      /**
       The compiler creates a coroutine handle and passes
       it to the method a.await_suspend (coroutine_handle).
       */
    cout << "counter..." << endl;
    co_return 2;
}

task <int> counterr (debug_called_from) {
    debug_class_print_called_from (yellow, 0)
    co_await counter2 ();

//    co_await *a; // creates a callable object coroutine_handle <> whose invocation continues execution of the current function
      /**
       The compiler creates a coroutine handle and passes
       it to the method a.await_suspend (coroutine_handle).
       */
    cout << "counterr..." << endl;
    co_return 2;
}


task <int> run () {
    cout << "run" << endl;
    task <string> t1 = async_read_file ("/Users/philipwenkel/GitHub/Cython/tests/apptest/sources/a.hpp.in");
    string s1 = co_await t1;
    cout << "run." << endl;
//    string s2 = co_await async_read_file <false> ("/Users/philipwenkel/GitHub/Cython/tests/apptest/sources/a.hpp.in");
    
    
    cout << s1 << endl;
//    cout << s2 << endl;

    
//    task r = counter ();
//    task r2 = counter ();
//    co_await r;
//    co_await r2;
    cout << "done" << endl;
}

struct Sleep {
    bool await_ready () {
        return duration == duration.zero ();
    }
    void await_suspend (coroutine_handle<> coro) {
        this_thread::sleep_for (duration);
        coro ();
    }
    void await_resume () {
        cout << "done!" << endl;
    }
    chrono::seconds duration;
};

task <int> testing () {
    co_await Sleep {2s};
}





int main(int argc, char const *argv[]) {
    char const* lines = "================================================================================================================";

    
//    testing().wait();
//    cout << lines << endl;
//    return 0;
//
//    string info {""};
//    debug_called_from_none
    


    
//    cout << white << lines << info << endl << white << lines << endl;// << red << lines << white << info  << red << lines << endl << endl;
    cout << red << lines << endl << endl;

    cout << white;
//    coroutine_handle<ReturnObject::promise_type> my_handle;
        
//    run ();
    cout << green << lines << endl << white;
//    counter();
    auto t = counter();
    t.wait();
//    for (auto& th : threadss)
//        th.join();
    
//    threadss.clear();
//    this_thread::sleep_for(3s);
    cout << yellow << lines << endl << white;
//    counterr();
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

//    cout << red << endl << lines << endl;



    return 0;
}
