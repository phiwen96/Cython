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
#include <const_str/const_str.hpp>

#include <cppcoro/task.hpp>
#include <cppcoro/sync_wait.hpp>
#include <ph_color/color.hpp>
#include <ph_coroutines/handle.hpp>
#include <ph_coroutines/task.hpp>
#include <ph_coroutines/statemachine.hpp>
#include <ph_coroutines/i_am_co_awaited.hpp>
#include <ph_coroutines/i_was_co_awaited_and_now_i_am_suspending.hpp>
#include <ph_coroutines/co_promise.hpp>

//#include <ph_coroutines/timer.hpp>

using namespace std;
using namespace experimental;
using namespace ph::concepts;
using namespace chrono_literals;





    














auto async_read_file (auto&& p) -> task <string>
{
    auto ss = ostringstream {};
    ifstream input_file (fwd (p));
    if (!input_file.is_open()) {
        cerr << "Could not open the file - '"
             << p << "'" << endl;
        exit (EXIT_FAILURE);
    }
    ss << input_file.rdbuf();
    co_return ss.str();
}

auto async_write_file (auto&& path, auto&& str) -> task <void>
{
    ofstream output_file;
    output_file.open (fwd (path));
    if (!output_file.is_open()) {
        cerr << "Could not open the file - '"
             << path << "'" << endl;
        exit (EXIT_FAILURE);
    }
    output_file << fwd (str);
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

auto counter2 (debug_called_from) -> task <int>
{
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
//    task <string> t1 = async_read_file ("/Users/philipwenkel/GitHub/Cython/tests/apptest/sources/a.hpp.in");
    cout <<  co_await async_read_file ("/Users/philipwenkel/GitHub/Cython/tests/apptest/sources/a.hpp.in") << endl;
    cout << "run." << endl;
//    string s2 = co_await async_read_file <false> ("/Users/philipwenkel/GitHub/Cython/tests/apptest/sources/a.hpp.in");
    
    
//    cout << s1 << endl;
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

auto testing2 () -> task <int>
{
    int i = 3;
    co_return i;
}


auto testing3 () -> task <int>
{
    for (int i = 0; i < 10000; ++i)
    {
        co_await testing2 ();
    }
    co_return 4;
}

struct Open{};
struct Close{};
struct Knock{};

StateMachine getDoor (string answer)
{
    for (;;)
    {
    //closed
        auto e = co_await Event <Open, Knock> {};
        
        if (std::holds_alternative <Knock> (e))
        {
          cout << "Come in, it's open!" << endl;
        }

        else if (std::holds_alternative <Open> (e))
        { // open
            cout << "opening door" << endl;
            co_await Event <Close> {};
        }
    }
}




machine parse ()
{
    goto zero;
    
    zero:
    cout << "zero" << endl;
    
    string nr {""};
    for (;;)
    {
        char found = co_await "0123456789";//co_await ('c', '1', '2', '3', '4', '5', '6', '7', '8', '9');
//        co_yield found - '0';
        nr += found;
        goto one;
    }
    
    one:
    cout << "one" << endl;

    for (;;)
    {
        char found = co_await "+-0123456789";
        
        switch (found)
        {
            case '+':
                co_yield stoi (nr);
//                goto two;
                goto three;
                
            case '-':
                co_yield stoi (nr);
                goto four;
                
            default:
                nr += found;
                break;
        }
    }
    
    two:
    cout << "two" << endl;

    for (;;)
    {
        char found = co_await "+-0123456789";
        switch (found)
        {
            case '-':
                goto four;
                
            case '+':
                break;
                
            default:
                goto three;
                break;
        }
    }
    


    
    four:
    cout << "four" << endl;

    for (;;)
    {
        char found = co_await "+-0123456789";
        switch (found)
        {
            case '-':
                goto two;
                break;
                
            case '+':
                break;
                
            default:
                goto three;
                break;
        }
    }
    
//    two:
//
//    three:
    
three:
    cout << "three" << endl;
    
    auto child = parse ();
    
    for (;;)
    {
        
//        char found = co_await (Any {});
//        child.process (found);
//        co_yield child;
        co_await parse();
    }
    

    
}

machine parse2 (char c)
{
    machine parser = parse ();
    for (;;)
    {
        parser.process (c);
        co_await suspend_always {};
    }
//    co_await c;
}
machine helper (string const& str)
{
    auto parser = parse ();
    for (auto const& c : str)
    {
        parser.process(c);
//        co_await parse()
    }
//    cout << "::::::" << endl;
//
//    for (auto const& c : str)
//    {
//        parse2 (c);
////        co_await parse()
//    }
    
}



template <typename _value_type>
struct mytask
{
    using value_type = _value_type;
    using promise_type = co_promise <mytask>;

    struct awaitable
    {
        using initial_type = suspend_always;
        using final_type = i_was_co_awaited_and_now_i_am_suspending;
        using transform_type = i_am_co_awaited <promise_type>;
    };
 
    
    auto resume ()
    {
        if (m_coro.done())
        {
            throw runtime_error ("can't resume coro, it is already done!");
        }
        return m_coro.resume();
    }
    auto is_ready ()
    {
        return m_coro.done();
    }
    ~mytask ()
    {
        if (m_coro)
        {
            m_coro.destroy();
        }
    }
    mytask (coroutine_handle <promise_type> coro) : m_coro {coro}
    {

    }
    mytask (mytask&& other) : m_coro {exchange (other.m_coro, {})}
    {

    }
    mytask (mytask const&) = delete;
    mytask& operator=(mytask const&) = delete;
    mytask& operator=(mytask&&) = delete;

    
    /**
     I am co_awaited by another coroutine!
     */
//    auto operator co_await () const& noexcept
//    {
//        return i_am_co_awaited {m_coro};
//    }
    
        

    coroutine_handle <promise_type> m_coro;
};

mytask <int> task3 ()
{
    cout << "task3..." << endl;
//    co_await suspend_always {};
    cout << "...task3" << endl;
    co_return 3;
}

mytask <int> task2 ()
{
    cout << "task2..." << endl;
//    auto aa = task3 ();
//    co_await aa;
//    co_await suspend_always {};
    int i = co_await task3 ();
    cout << "...task2" << endl;
    co_return 2;
}

mytask <int> task1 ()
{
    cout << "task1..." << endl;
    co_await task2 ();
    cout << "...task1" << endl;
    co_return 1;
}









F <int> f3 ()
{
    cout << "f3..." << endl;
    cout << "...f3" << endl;

    co_return 3;
}

F <int> f2 ()
{
    cout << "f2..." << endl;

    int i = co_await f3 ();
    cout << "...f2" << endl;

    co_return 3;
}

F <int> f1 ()
{
    cout << "f1..." << endl;
    co_await f2 ();
    cout << "...f1" << endl;

    co_return 3;
}


auto main (int argc, char const *argv[]) -> int
{
    
    F <int> f = f1();
    f.resume();
    
    
    return 0;
    
    
    
    
    
    
    
    
    
    
    
    char const* lines = "================================================================================================================";
    cout << red << lines << white << endl;


    mytask t1 = task1();
    t1.resume ();
//    t1.resume ();
//    t1.resume ();
        
    
    return 0;
    
    
    helper ("10+3+8+7");
    
    return 0;
    
    
    
    
    auto door = getDoor ("Occupied!");
//    door.onEvent (Close {}); // Open -> Closed
    door.onEvent (Open {});

//    door.onEvent (Open {});  // Closed -> Open
    return 0;
    door.onEvent (Knock {});
    door.onEvent (Close {}); // Closed -> Closed

    return 0;

    
//    testing().wait();
//    cout << lines << endl;
//    return 0;
//
//    string info {""};
//    debug_called_from_none
    


    
//    cout << white << lines << info << endl << white << lines << endl;// << red << lines << white << info  << red << lines << endl << endl;
 
    run().wait();
//    return 0;
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
