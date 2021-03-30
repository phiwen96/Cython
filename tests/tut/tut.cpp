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







template <typename R, typename promise_type, typename T>
struct co_promise_type {
    T value;
    coroutine_handle <> awaiting_coro;
    
    co_promise_type () {}
    co_promise_type (co_promise_type const&) = delete;
    co_promise_type& operator= (co_promise_type const&) = delete;
    co_promise_type& operator= (co_promise_type&&) = delete;
    
    struct awaitables {
        struct initial_suspend {
            auto await_ready () {return false;}
            auto await_suspend (coroutine_handle <> awaiting_coro) {}
            auto await_resume () {}
        };
        struct final_suspend {
            promise_type const& f;
            auto await_ready () noexcept {return false;}
            coroutine_handle <> await_suspend (coroutine_handle <promise_type> current_coro) noexcept {
//                auto precursor = current_coro.promise().awaiting_coro;
//                if (precursor) return precursor;
//                return noop_coroutine();
            }
            auto await_resume () noexcept {}
        };
        struct yield_value {
            promise_type& p;
            auto await_ready () {return false;}
            void await_suspend (coroutine_handle <promise_type> current_coro) {
                auto precursor = current_coro.promise().awaiting_coro;
                if (precursor) precursor.resume();//return noop_coroutine();
            }
            auto await_resume () {}
        };
    };
    
    auto get_return_object () {return R {static_cast<promise_type&>(*this)};}
    auto initial_suspend () {return awaitables::initial_suspend();}
    auto final_suspend () noexcept {return awaitables::final_suspend(*this);}
    auto yield_value (auto&& lambda) {return awaitables::yield_value (*this, index);}
    auto return_value (auto&& v) {value = forward<decltype(v)>(v);}
    [[noreturn]] auto unhandled_exception () {terminate();}
};



int aaa = 0;


template <typename T>
struct [[nodiscard]] co_future <T>
{
    co_future () = delete;
    co_future (co_future const&) = delete;
    co_future& operator= (co_future const&) = delete;
    co_future& operator= (co_future&&) = delete;
    
    struct promise_type : co_promise_type <co_future, promise_type, T> {
        using base = co_promise_type <co_future, promise_type, T>;
        int index;
        T value;
        struct awaitables {
            using base = typename base::awaitables;
            struct initial_suspend : base::initial_suspend {
                using base = typename base::initial_suspend;
                int index;
                auto await_ready (debug_called_from) -> decltype (base::await_ready()){
                    debug_print_called_from(yellow, index)
                    return base::await_ready ();
                }
                auto await_suspend (coroutine_handle <> awaiting_coro, debug_called_from) -> decltype (base::await_suspend(awaiting_coro)){
                    debug_print_called_from(yellow, index)
                    EWRITE(yellow, index, suspending...)
                    return base::await_suspend(awaiting_coro);
                }
                auto await_resume () -> decltype (base::await_resume()){
                    BWRITE(yellow, index, resuming...)
                    D01(yellow, index)
                    return base::await_resume ();
                }
            };
            struct final_suspend : base::final_suspend {
                using base = typename base::final_suspend;
                promise_type const& f;
                auto await_ready () noexcept -> decltype (base::await_ready ()) {
                    return base::await_ready ();
                }
                auto await_suspend (coroutine_handle <promise_type> current_coro, debug_called_from) noexcept -> decltype (base::await_suspend (current_coro)){
                    debug_print_called_from(yellow, f.index)
                    EWRITE(yellow, f.index, finally suspending...)
                    cout << f.value << ":" << current_coro.promise().value << endl;
                    return base::await_suspend (current_coro);
                }
                auto await_resume () noexcept -> decltype (base::await_resume ()) {
                    return base::await_resume ();
                }
            };
            struct yield_value : base::yield_value {
                promise_type& p;
                int index;
                auto await_ready (debug_called_from){
                    debug_print_called_from(yellow, index);
                    return false;
                }
                auto await_suspend (coroutine_handle <promise_type> current_coro, debug_called_from) -> decltype (base::await_suspend (current_coro)) {
                    debug_print_called_from(yellow, index);
                    return base::await_suspend (current_coro);
                }
                auto await_resume () -> decltype (base::await_resume ()) {
                    D01(yellow, index);
                    return base::await_resume ();
                }
            };
        };
        promise_type (debug_called_from) : index {antal++} {++lol; debug_print_called_from (yellow, index)}
        ~promise_type () {--lol;}
        auto get_return_object (debug_called_from){debug_print_called_from(yellow, index); return base::get_return_object();}
        typename awaitables::initial_suspend initial_suspend (debug_called_from){return  {{}, {index}};}
        typename awaitables::final_suspend final_suspend () noexcept{return  {{*this}, {*this}};}
        typename awaitables::yield_value yield_value (int i, debug_called_from) {debug_print_called_from(yellow, index); return {{*this}, {*this, index}};}
        auto return_value (auto&& v, debug_called_from){debug_print_called_from(yellow, index); base::return_value (forward<decltype(v)>(v));}
        [[noreturn]] auto unhandled_exception (){base::unhandled_exception ();}
    };
    promise_type& promise () {return coro.promise();}
    int index;
    coroutine_handle <promise_type> coro;
    co_future (promise_type& p) : coro {coroutine_handle<promise_type>::from_promise (p)} {
//        cout << index << endl;
//        cout << aaa++ << endl;
        ++co_future<>::current_threads;
        index = p.index;
    }
    co_future (co_future&& other) : coro {exchange (other.coro, {})} {
        index = other.index;
        ++co_future<>::current_threads;
        coro.promise().index = antal;
    }
    ~co_future (){
        if (not coro) throw runtime_error (":O");
        if (coro) coro.destroy();
    }
    operator bool (){
        if (not coro.done ()) coro.resume ();
        return not coro.done ();
    }
    auto get () -> decltype (auto) {return coro.promise().value;}
    struct awaitable;
    friend class awaitable;
    auto operator co_await () {
        BWRITE(green, index, co_awaited...)
        return awaitable {coro, index};
    }
};


template <typename T, typename Promise>
struct co_awaitable {
    using promise_type = Promise;
    using return_type = T;
    coroutine_handle <promise_type> h;
    co_awaitable (coroutine_handle <promise_type> h) :  h (h) {}
    bool await_ready (){return false;}
    void await_suspend (coroutine_handle <> awaiting_coro){h.promise().awaiting_coro = awaiting_coro;}
    [[nodiscard]] return_type await_resume (){return h.promise().value;}
};


template <class T>
struct co_future<T>::awaitable : co_awaitable <T, promise_type> {
    using parent = co_awaitable <T, promise_type>;
    int index;
    awaitable (coroutine_handle <promise_type> h, int index) : parent {h}, index {index} {}
    bool await_ready (If (Debug) (debug_called_from)()) {
        If (Debug) (debug_print_called_from (green, index)) ()
        return parent::await_ready ();
    }
    void await_suspend (coroutine_handle <> awaiting_coro If (Debug) (,debug_called_from) ()){
        
        If (Debug) (debug_print_called_from (green, index)  EWRITE (green, index, getting a handle to the coroutine that will be suspended. suspending...)) ()
        parent::await_suspend (awaiting_coro);
    }
    [[nodiscard]] T await_resume (If(Debug)(debug_called_from)()){
        If (Debug) (debug_print_called_from (green, index)   EWRITE (green, index, resuming parent...))()
        if constexpr (not is_same_v<T, void>){
            return parent::await_resume ();
        } else{
            parent::await_resume ();
        }
    }
};

/**
 await_ready()
 
 hallå är du redo eller ska jag vänta???!!!
 
 okej, här får du mitt nummer hör av dig när du är klar.
 
 sen await_resume
 */








co_future<int> C () {
    co_return 4;
}

co_future<int> G () {
    co_await C ();
    co_return 3;
}

co_future<int> Q () {
    co_await G ();
    co_return 2;
}

co_future<int> run () {
    co_await Q ();
    co_return 1;
}

string info =
R"V0G0N(
await_ready     always called
await_suspend   only called if about to suspend
await_resume    called when resuming coro
)V0G0N";

template <typename... T>
struct co_handle;

template <typename T>
struct co_handle <T> : coroutine_handle <T> {
    
};

template <>
struct co_handle <> : coroutine_handle <> {
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
//        cout << called_from_function << endl;
    }
    
//
    template <class T>
    co_handle (T&& h, debug_called_from) : base {forward<T>(h)}, called_from_function {move (_called_from_function)}, called_from_line {_called_from_line} {
//        cout << called_from_function << endl;
//        cout << "ohh assigning handle" << endl;
//        cout << typeid(h).name() << endl;
    }

};

struct ReturnObject {
  struct promise_type {
    ReturnObject get_return_object() { return {}; }
    suspend_never initial_suspend() { return {}; }
    suspend_never final_suspend() noexcept { return {}; }
    void unhandled_exception() {}
      void return_void () {}
  };
};

struct Awaitable {
    co_handle <> * hp_;
    bool await_ready(debug_called_from) {
//        debug_print_called_from (yellow, 0)
        return false;
    }
    void await_suspend (co_handle <> h, debug_called_from) {
//        out("hej", text{"kuk", green, white}, "hora");
        cout << "storing {" << text {_called_from_function, blue} << "}'s handle into {" << blue << hp_ -> called_from_function << white << "}'s handle" <<  _called_from_function + "::" + to_string (_called_from_line) << endl;
//        D1(yellow, 0)
        //        cout << "FFFF" << endl;
//        hp_->resume();
        *hp_ = h;
    }
    void await_resume (debug_called_from) {
        debug_print_called_from (yellow, 0)
    }
};

ReturnObject counter(co_handle<>& a) {
    cout << "...counter" << endl;

//  Awaitable a {continuation_out};
    for (int i = 0; ; ++i){
        
        co_await Awaitable {&a};
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
//    cout << "tji" << endl;
//    out (0, "hej", "då");
//    return 0;
    co_handle <> h;

    counter(h);
    cout << "hi" << endl;
    h.resume(); // resume can only be called on suspended coroutines
//    cout << a.hp_ -> done() << endl; // done can only be called on suspended coroutines
//    a.hp_ -> resume();
//     for (int i = 0; i < 3; ++i) {
//       std::cout << "In main1 function\n";
//       h();
//     }

    return 0;
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
