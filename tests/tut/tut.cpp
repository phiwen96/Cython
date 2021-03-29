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

//inline std::ostream& Blue(std::ostream &s){
//    s << "\033[94m";
//    return s;
//}



template <typename... T>
string format_string (T&&... s) {
    stringstream ss;
    ss << left;
    ((ss << left << setw(30) << left << forward<T>(s) << setw(30)), ...);
    return ss.str();
}
void out (auto color, string&& s, string&& s2, int index) {
    for(int i = 0; i < index; ++i)
    {
        cout << "\t";
    }
//    cout << right << setw(0) << "[" << index << "] " << s << left << "\t" << s2 << endl;
//    cout << s << internal << std::setfill('*') << setw(40) << s2 << "\n";
//    Green b;
    cout << black << " •  " << color;
    cout << left << setw(30) << s;
    cout << setw(20);
    cout  <<  setw(30) << s2  << "kuk" << "\t" << endl;
//    cout << format_string(s, "|", s2) << endl;
//         << setw(4) << hourlyRate << "\n";

//    color << "[" << index << "] " << s << setw(20) << s2 << endl;
}




#define D0 string h = to_string (__LINE__), string s = __builtin_FUNCTION(), int l = __builtin_LINE()
#define D1(color, index) cout << color; out(string (color), string (__FUNCTION__) + string ("::") +  h, s + "::"  + to_string(l), index);
#define D01(color, index) cout << color; out(string (color), string (__FUNCTION__), to_string(__LINE__), index);
#define BWRITE(color, index, ...) for(int _j = 0; _j < index; ++_j)cout << "\t"; cout << white << " B  " << color; cout << BOOST_PP_STRINGIZE (__VA_ARGS__) << endl;
#define EWRITE(color, index, ...) for(int _j = 0; _j < index; ++_j)cout << "\t"; cout << cyan << " E  " << color; cout << BOOST_PP_STRINGIZE (__VA_ARGS__) << endl;

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
struct co_promise_type{
    T value;
    co_promise_type (co_promise_type const&) = delete;
    co_promise_type& operator= (co_promise_type const&) = delete;
    co_promise_type& operator= (co_promise_type&&) = delete;
    
    auto get_return_object () {return co_future {*this};}
    auto initial_suspend () {
        struct awaitable {
            auto await_ready () {return false;}
            auto await_suspend (coroutine_handle <> awaiting_coro) {}
            auto await_resume () {return 3;}
        };
        return awaitable {index};
    }
    auto final_suspend () noexcept {
        struct awaitable {
            co_promise_type const& f;
            auto await_ready () noexcept {return false;}
            coroutine_handle <> await_suspend (coroutine_handle <co_promise_type> current_coro) noexcept {
                auto precursor = current_coro.promise().awaiting_coro;
                if (precursor) return precursor;
//                return noop_coroutine();
            }
            auto await_resume () noexcept {}
        };
        return awaitable {*this};
    }
    auto yield_value (auto&& lambda) {
        struct awaitable {
            co_promise_type& p;
            auto await_ready () {return false;}
            void await_suspend (coroutine_handle <co_promise_type> current_coro) {
                auto precursor = current_coro.promise().awaiting_coro;
                if (precursor) precursor.resume();
//                    return noop_coroutine();
            }
            auto await_resume () {}
        };
        return awaitable {*this, index};
    }
    auto return_value (auto&& v) {value = forward<decltype(v)>(v);}
    [[noreturn]] auto unhandled_exception () {terminate();}
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
            D1 (yellow, index)
        }
        ~promise_type () {
            --lol;
//            out("~promise_type");
        }
        int index;
        T value;
        coroutine_handle <> awaiting_coro;
        auto get_return_object (D0)
        {
            D1(yellow, index)
//            out("get_return_object");
            return co_future {*this};
        }
        auto initial_suspend (D0)
        {
            
            struct awaitable
            {
                int index;
                auto await_ready (D0)
                {
                    bool ready = false;
//                    if (ready){
//                        BWRITE(yellow, index, initially resumed)
//                    }
//
//                    else {
//                        BWRITE(yellow, index, initially suspended)
//                    }
                        
                    D1(yellow, index)
                    
                    return false;
                }
                auto await_suspend (coroutine_handle <> awaiting_coro, D0)
                {
                    
                    D1(yellow, index)
                    EWRITE(yellow, index, suspending...)
//                    EWRITE(yellow, index, initially suspended)
//                    cout << endl << "\t" << yellow << "initially suspended at line " << l << endl;
                }
                auto await_resume ()
                {
                    BWRITE(yellow, index, resuming...)
                    D01(yellow, index)
                    return 3;
                }
            };

//            return suspend_always {};
            return awaitable {index};
        }
        auto final_suspend () noexcept
        {
            struct awaitable
            {
                promise_type const& f;
                auto await_ready () noexcept
                {
                    return false;
                }
                coroutine_handle <> await_suspend (coroutine_handle <promise_type> current_coro, D0) noexcept
                {
                    D1(yellow, f.index)
                    EWRITE(yellow, f.index, finally suspending...)
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

            return awaitable {*this};
        }
        
        auto yield_value (int i, D0) {
            D1(yellow, index)
//            return suspend_always{};
            return suspend_never{};
        }
        
        auto yield_value (auto&& lambda, D0)
        requires requires(){
            lambda();
        }
        {
            D1(yellow, index)

            struct awaitable
            {
                promise_type& p;
                int index;
                auto await_ready (D0)
                {
                    D1(yellow, index)
                    return false;
                }
                void await_suspend (coroutine_handle <promise_type> current_coro, D0)
                {
                    D1(yellow, index)
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
                    D01(yellow, index)
//                    return 8;
                }
            };
            return awaitable {*this, index};
//            return suspend_always {};
//            return suspend_never {};
        }
        
        
        auto return_value (auto&& v, D0)
        {
            D1(yellow, index)
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
    ~co_future (){//D01(green, index)
//        promise().out("~co_future");
//        --co_future<>::current_threads;
//        cout << "~co_task" << coro.promise().i << endl;
        if (not coro)
            throw runtime_error (":O");
        if (coro)
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
    auto get () -> decltype (auto)
    {
        return coro.promise().value;
    }

    
    struct awaitable;
    friend class awaitable;
    auto operator co_await ()
    {
        BWRITE(green, index, co_awaited...)
        return awaitable {coro, index};
    }
};


template <typename T, typename Promise>
struct co_awaitable
{
    using promise_type = Promise;
    using return_type = T;
    coroutine_handle <promise_type> h;
    
    co_awaitable (coroutine_handle <promise_type> h) :  h (h) {}
    
    bool await_ready ()
    {
        return false;
    }
    void await_suspend (coroutine_handle <> awaiting_coro)
    {
        
    }
    [[nodiscard]] return_type await_resume ()
    {
        return 2;
    }
};


template <class T>
struct co_future<T>::awaitable : co_awaitable <T, promise_type>
{
    using parent = co_awaitable <T, promise_type>;
    int index;
    awaitable (coroutine_handle <promise_type> h, int index) : parent {h}, index {index} {}
  
    bool await_ready (If (Debug) (D0)())
    {
        If (Debug) (D1 (green, index)) ()
        return parent::await_ready ();
    }
    void await_suspend (coroutine_handle <> awaiting_coro If (Debug) (,D0) ())
    {
        If (Debug) (D1 (green, index)  EWRITE (green, index, suspending...)) ()
        parent::await_suspend (awaiting_coro);
    }
    [[nodiscard]] T await_resume (If(Debug)(D0)())
    {
        If (Debug) (D1 (green, index)   EWRITE (green, index, resuming parent...))()
        if constexpr (not is_same_v<T, void>)
        {
            return parent::await_resume ();
        } else
        {
            parent::await_resume ();
        }
    }
};














co_future<int> Q () {
    co_return 3;
}

co_future<int> run () {
    co_await Q ();
    co_return 0;
}

string info =
R"V0G0N(
await_ready     always called
await_suspend   only called if about to suspend
await_resume    called when resuming coro
)V0G0N";







int main(int argc, char const *argv[])
{
//    return 0;
//    LoggingOutputStreambuf logger( std::cout );
    

    cout << white << info  << red << "================================================================================================================" << endl << endl;
    cout << white;
    {
        auto aa = run();
        cout << blue << endl << "=====================================================CREATED=====================================================" << endl << endl;
        (bool)aa;
        cout << blue << endl << "=====================================================RESUME_0=====================================================" << endl << endl;
        (bool)aa;
        cout << blue << endl << "=====================================================RESUME_1=====================================================" << endl << endl;
        (bool)aa;
        cout << blue << endl << "=====================================================RESUME_2=====================================================" << endl << endl;

//        (bool)aa;
//        (bool)aa;
//        (bool)aa;
        
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
    
    

//    cout << "hej" << "kil" << right << "kuk" << endl;
//    cout << right << setw(10) << 543 << right << setw(30) << 12 << endl;
//	int result = Catch::Session().run( argc, argv );
//	return result;
//    cout  <<"hej"<< _color::red << "================================================================================================================" << endl << endl;
    cout << endl << red << "================================================================================================================" << endl << endl;

    return 0;
}
