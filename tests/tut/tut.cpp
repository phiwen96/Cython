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

using namespace std;
using namespace experimental;
using namespace ph::concepts;
using namespace chrono_literals;

namespace Color {
    enum Code {
        FG_RED      = 31,
        FG_GREEN    = 32,
        FG_BLUE     = 34,
        FG_DEFAULT  = 39,
        BG_RED      = 41,
        BG_GREEN    = 42,
        BG_BLUE     = 44,
        BG_DEFAULT  = 49
    };
    class Modifier {
        Code code;
    public:
        Modifier(Code pCode) : code(pCode) {}
        friend std::ostream&
        operator<<(std::ostream& os, const Modifier& mod) {
            return os << "\033[" << mod.code << "m";
        }
    };
}


    

class Semaphore {
    const size_t num_permissions;
    size_t avail;
    mutex m;
    condition_variable cv;
public:
    /** Default constructor. Default semaphore is a binary semaphore **/
    explicit Semaphore(const size_t& num_permissions = 1) : num_permissions(num_permissions), avail(num_permissions) { }

    /** Copy constructor. Does not copy state of mutex or condition variable,
        only the number of permissions and number of available permissions **/
    Semaphore(const Semaphore& s) : num_permissions(s.num_permissions), avail(s.avail) { }

    void acquire() {
        unique_lock<mutex> lk(m);
        cv.wait(lk, [this] { return avail > 0; });
        avail--;
        lk.unlock();
    }

    void release() {
        m.lock();
        avail++;
        m.unlock();
        cv.notify_one();
    }

    size_t available() const {
        return avail;
    }
};



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







struct [[nodiscard]] co_task
{
    co_task () = delete;
    co_task (co_task const&) = delete;
    co_task& operator= (co_task const&) = delete;
    co_task& operator= (co_task&&) = delete;
    struct promise_type
    {
        int value {10};
        coroutine_handle <> awaiting_coro;
        auto get_return_object ()
        {
            return co_task {*this};
        }
        auto initial_suspend ()
        {
 
            struct awaitable
            {
                auto await_ready ()
                {
                    return false;
                }
                auto await_suspend (coroutine_handle <> awaiting_coro)
                {
                    
                }
                auto await_resume ()
                {
                    
                }
            };

            return suspend_always {};
        }
        auto final_suspend () noexcept
        {
            struct awaitable
            {
                auto await_ready () noexcept
                {
                    return false;
                }
                coroutine_handle <> await_suspend (coroutine_handle <promise_type> current_coro) noexcept
                {
                    auto precursor = current_coro.promise().awaiting_coro;
                    if (precursor)
                    {
                      return precursor;
                    }
                    return noop_coroutine();
                }
                
                auto await_resume () noexcept
                {

                }
            };

            return awaitable {};
        }
        auto yield_value (int i)
        {
            value = i;
            
            struct awaitable
            {
                auto await_ready ()
                {
                    return true;
                }
                void await_suspend (coroutine_handle <promise_type> current_coro)
                {

                }
                auto await_resume ()
                {
                    return 8;
                }
            };
            return awaitable {};
//            return suspend_always {};
        }
        auto return_void ()
        {
            
        }
        [[noreturn]]
        auto unhandled_exception ()
        {
            terminate();
        }
    };
    
    coroutine_handle <promise_type> coro;
    co_task (promise_type& p) : coro {coroutine_handle<promise_type>::from_promise (p)}
    {
        
    }
    co_task (co_task&& other) : coro {exchange (other.coro, {})}
    {
        
    }
    ~co_task ()
    {
//        cout << "~co_task" << endl;
        if (not coro)
            throw runtime_error (":O");
        coro.destroy();
    }
    operator bool ()
    {
        if (not coro.done ())
        {
            coro.resume ();
        }
        return not coro.done ();
    }
    auto operator () ()
    {
        if (not coro.done ())
        {
            coro.resume ();
        }
        else
        {
            throw runtime_error ("coro already done");
        }
    }

    auto operator co_await ()
    {
        struct awaitable
        {
            coroutine_handle <promise_type>& coro;
            auto await_ready ()
            {
                return false;
            }
            auto await_suspend (coroutine_handle <> awaiting_coro)
            {
                coro.promise().awaiting_coro = awaiting_coro;
                return coro;
            }
            auto await_resume ()
            {
                return coro;
            }
        };
        
        return awaitable {coro};
    }
};





template <typename...>
struct co_future;

template <>
struct co_future <>
{
    inline static atomic <int> current_threads = -1;
};

struct thr : thread
{
//    using thread::thread;
    inline static atomic <int> current_threads = 0;
    int m_thread;
    
    thr() noexcept : thread {}, m_thread {current_threads} {
        cout << "::" << m_thread << endl;
//        ++current_threads;
    }
    thr( thr&& other ) noexcept : thread {(thread&&)other}, m_thread {current_threads} {
        cout << "::" << m_thread << endl;

//        ++current_threads;
    }
    template< class Function, class... Args >
    explicit thr( Function&& f, Args&&... args ) : thread {forward<Function>(f), forward<Args>(args)...}, m_thread {current_threads} {
        cout << "::" << m_thread << endl;

//        ++current_threads;
    }
    
    ~thr () {
//        cout << "~::" << m_thread << endl;
    }
};

void out (string&& s, string&& s2, int index) {
    for(int i = 0; i < index; ++i)
    {
        cout << "\t";
    }
    cout << "[" << index << "] " << s << setw(20) << s2 << endl;
}
string info =
R"V0G0N(
await_ready     always called
await_suspend   only called if about to suspend
await_resume    always called
)V0G0N";

#define D0 string h = to_string (__LINE__), string s = __builtin_FUNCTION(), int l = __builtin_LINE()
#define D1(index) out(string (__FUNCTION__) + string ("::") +  h, s + "::"  + to_string(l), index);
#define D01(index) out(string (__FUNCTION__), to_string(__LINE__), index);
atomic <bool> running {true};
atomic <int> antal = 0;
atomic <int> lol = 0;
struct Run
{
    Run(Run const&) = delete;
    Run(Run&&) = delete;
    Run& operator=(Run const&) = delete;
    Run& operator=(Run &&) = delete;

    Run(){
        ++co_future<>::current_threads;
    }
    ~Run () {
        --co_future<>::current_threads;
//        running.store(false);
    }
};

template <typename T>
struct [[nodiscard]] co_future <T>
{
 
    
    co_future () = delete;
    co_future (co_future const&) = delete;
    co_future& operator= (co_future const&) = delete;
    co_future& operator= (co_future&&) = delete;
    struct promise_type
    {
        
        promise_type (promise_type const&) = delete;
        promise_type& operator= (promise_type const&) = delete;
        promise_type& operator= (promise_type&&) = delete;
        promise_type (D0) : index {antal++} {
            ++lol;
//            out("promise_type::" +  h +          "              <--  \t\t" + s + "::"  + to_string(l));
            D1 (index)
        }
        
        ~promise_type () {
            --lol;
//            out("~promise_type");
        }
        
        int index;
        T value;
        coroutine_handle <> awaiting_coro;
        vector <coroutine_handle <>> queues;
        inline static int nr = -1;
        char i = ++nr + 97;
        auto get_return_object (D0)
        {
            D1(index)
//            out("get_return_object");
            return co_future {*this};
        }
        auto initial_suspend (D0)
        {
            D1(index)
            struct awaitable
            {
                int index;
                auto await_ready (D0)
                {
                    D1(index)
                    return false;
                }
                auto await_suspend (coroutine_handle <> awaiting_coro, D0)
                {
                    D1(index)
                }
                auto await_resume ()
                {
                    D01(index)
                }
            };

//            return suspend_always {};
            return awaitable {index};
        }
        auto final_suspend () noexcept
        {
            struct awaitable
            {
                auto await_ready () noexcept
                {
                    return false;
                }
                coroutine_handle <> await_suspend (coroutine_handle <promise_type> current_coro) noexcept
                {
//                    cout << "hi" << endl;
                    auto precursor = current_coro.promise().awaiting_coro;
                    if (precursor)
                    {
//                        cout << current_coro.promise().i << endl;
//                        async(launch::async, [&precursor]{precursor.resume(); cout << "hi" << endl;});
//                        precursor.resume();
                        return precursor;
                    }
                    return noop_coroutine();
                }
                
                auto await_resume () noexcept
                {
//                    return 3;
                }
            };

            return awaitable {};
        }
        
        auto yield_value (int i, D0) {
            D1(index)
//            return suspend_always{};
            return suspend_never{};
        }
        
        auto yield_value (auto&& lambda, D0)
        requires requires(){
            lambda();
        }
        {
            D1(index)
            thr{forward<decltype(lambda)>(lambda)}.detach();
//            lambda();
//            async (launch::async, forward<decltype(lambda)>(lambda)).get();
//            ++co_future<>::current_threads;
//            thr{forward<decltype(lambda)>(lambda)}.detach();
//            --co_future<>::current_threads;

            cout << "cool " << i << endl;
            value = i;
            
            struct awaitable
            {
                promise_type& p;
                int index;
                auto await_ready (D0)
                {
                    D1(index)
                    return false;
                }
                void await_suspend (coroutine_handle <promise_type> current_coro, D0)
                {
                    D1(index)
//                    if (current_coro.address() == p.awaiting_coro.address())
//                        cout << "dmfkmdkfmdkmf" << endl;
                    auto precursor = current_coro.promise().awaiting_coro;
                    if (precursor)
                    {
//                        cout << current_coro.promise().i << endl;
//                        async(launch::async, [&precursor]{precursor.resume(); cout << "hi" << endl;});
//                        precursor.resume();
//                        return precursor;
                    }
                    
//                    return noop_coroutine();
                }
                auto await_resume ()
                {
                    D01(index)
//                    return 8;
                }
            };
            return awaitable {*this, index};
//            return suspend_always {};
//            return suspend_never {};
        }
        
        
        auto return_value (auto&& v, D0)
        {
            D1(index)
            value = forward<decltype(v)>(v);
        }
        [[noreturn]]
        auto unhandled_exception ()
        {
            terminate();
        }
    };
    promise_type& promise () {
        return coro.promise();
    }
    int index;
    coroutine_handle <promise_type> coro;
    co_future (promise_type& p) : coro {coroutine_handle<promise_type>::from_promise (p)}
    {
//        promise().out("co_future");
        ++co_future<>::current_threads;
//        p.index = antal;
        index = p.index;
    }
    co_future (co_future&& other) : coro {exchange (other.coro, {})}
    {
        index = other.index;
//        promise().out("co_future");
        ++co_future<>::current_threads;
        coro.promise().index = antal;
    }
    ~co_future ()
    {
//        promise().out("~co_future");
//        --co_future<>::current_threads;
//        cout << "~co_task" << coro.promise().i << endl;
        if (not coro)
            throw runtime_error (":O");
        coro.destroy();
    }
    void run ()
    {
        if (not coro.done ())
        {
            coro.resume ();
        }
    }
    operator bool ()
    {
        if (not coro.done ())
        {
            coro.resume ();
        }
        return not coro.done ();
    }
    auto operator () ()
    {
        if (not coro.done ())
        {
            coro.resume ();
        }
        else
        {
            throw runtime_error ("coro already done");
        }
    }
    auto get () -> decltype (auto)
    {
        return coro.promise().value;
    }

    bool ready = false;
    
    
    void do_work ()
    {
        
    }
    
    struct awaitable
    {
        co_future& f;
        
        auto await_ready (D0)
        {
            D1(f.index)
//            f.promise().out("await_ready");
            return false;
        }
//            auto await_suspend (coroutine_handle <> awaiting_coro)
//            {
//                coro.promise().awaiting_coro = awaiting_coro;
////                return coro;
//            }
//#define SLOW
        auto await_suspend (coroutine_handle <> awaiting_coro, D0)
        {
            D1(f.index)

            f.coro.promise().awaiting_coro = awaiting_coro;
        }
        auto await_resume (D0)
        {
            D1(f.index)
            return f.coro.promise().value;
//            return f.coro;
        }
    };
    friend class awaitable;
    awaitable operator co_await ()
    {
//        thread{[]{this_thread::sleep_for(2s);cout << "hi" << endl;}}.join();
//        this_thread::sleep_for(2s);
        
        return awaitable {*this};
    }
};

co_future<int> D ()
{
    
    cout << "****" << endl;
    cout << "D:" << this_thread::get_id() << endl;
    cout << "~****" << endl;
    co_yield []()noexcept{this_thread::sleep_for(2s);};
//
//    this_thread::sleep_for(2s);
    co_return 5;
}
co_future<int> C ()
{
    cout << "*" << endl;
    cout << "C:" << this_thread::get_id() << endl;
    cout << "~*" << endl;
    co_yield []{this_thread::sleep_for(2s);};
//    this_thread::sleep_for(2s);

//    this_thread::sleep_for(2s);
    co_return 6;
}
co_future <int> B ()
{
    cout << "**" << endl;
//    int i = co_await C();
    
    cout << "~**" << endl;
//    co_yield []{this_thread::sleep_for(2s);};

    co_yield []{++co_future<>::current_threads;this_thread::sleep_for(2s);--co_future<>::current_threads;};
//    cout << "mythread:" << this_thread::get_id() << endl;
//    this_thread::sleep_for(2s);
    co_return co_await C() + co_await D();
}

co_future<int> A ()
{

//    co_yield []{++co_future<>::current_threads;this_thread::sleep_for(2s);--co_future<>::current_threads;};

//    ++co_future<>::current_threads;

//    thr::current_threads;
    cout << "***" << endl;
//    cout << "mythread:" << this_thread::get_id() << endl;
//    co_await D();
//    cout << "mythread:" << this_thread::get_id() << endl;
//    co_await B();
    cout << "mythread:" << this_thread::get_id() << endl;
//    co_await B();
    cout << "A:" << this_thread::get_id() << endl;
    
    
    cout << "~***" << endl;
    co_return co_await B() + 2;
}

co_future<int> Q () {
    cout << "Q" << endl;
//    co_yield []{this_thread::sleep_for(2s);};
    cout << "QQQQQQ" << endl;
    co_yield 4;
    
    co_return 3;
}

co_future<int> run () {
    
//    cout << "run" << endl;
    int aa = co_await  Q ();
    cout << "1111111111" << endl;

//    int bb = co_await Q ();
    cout << aa << "==" << endl;
//    cout << bb << "==" << endl;
    co_return 0;
}

int main(int argc, char const *argv[])
{
    cout << info << endl << "================================================================================================================" << endl << endl;
    {
        auto aa = run();
        (bool)aa;
    }
    
//    (bool)aa;
//    Timer<true> t ("time");
//    thread t{[]{co_future a = A(); a.run();}};
//    t.join();
//    run();
    
    
//    return 0;
//    cout << "baaaaajs" << endl;
    while (lol) {
        cout << lol << endl;
//        cout << co_future<>::current_threads << endl;
//        this_thread::sleep_for(1ms);
//        cout << "threads:" << current_threads << endl;
    }
//    cout << "......" << endl;
//    cout << aa.get() << endl;
//    cout << "......" << endl;

//    (bool)a;
//    async ([a = A()]()mutable ->co_future{(bool)a;}).get();

//    cout << "---------------" << endl;
//    (bool)a;
//    (bool)a;
//    co_task coro = a();
//    (bool) coro;
//    cout << "-----------" << endl;
//    (bool) coro;
//    cout << (bool) coro << endl;
//    cout << (bool) coro << endl;
//    cout << (bool) coro << endl;
//    cout << (bool) coro << endl;
    
    

    
//	int result = Catch::Session().run( argc, argv );
//	return result;
    cout << endl << "================================================================================================================" << endl << endl;
    return 0;
}
