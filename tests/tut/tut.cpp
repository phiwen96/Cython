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



#include <cassert>
#include <version>

#if defined(__cpp_lib_semaphore)
#include <semaphore>
#else
// Fallback if your standard lib doesn't provide <semaphore> yet
#include <condition_variable>
#include <mutex>
#endif



namespace detail {

template <typename T>
class SyncWaitTask { // A helper class only used by sync_wait()

  struct Promise {
    SyncWaitTask get_return_object() noexcept { return SyncWaitTask{*this}; }
    auto initial_suspend() noexcept { return suspend_always{}; }
    void unhandled_exception() noexcept { error_ = current_exception(); }
    auto yield_value(T&& x) noexcept { // Result has arrived
      value_ = addressof(x);
      return final_suspend();
    }

    auto final_suspend() noexcept {
      struct Awaitable {
        bool await_ready() noexcept { return false; }
        void await_suspend(coroutine_handle<Promise> h) noexcept {

#if defined(__cpp_lib_semaphore)
          h.promise().semaphore_.release(); // Signal!
#else
          {
            auto lock = unique_lock{h.promise().mtx_};
            h.promise().ready_ = true;
          }
          h.promise().cv_.notify_one();
#endif
        }
        void await_resume() noexcept {}
      };
      return Awaitable{};
    }

    void return_void() noexcept { assert(false); }

    T* value_ = nullptr;
    exception_ptr error_;

#if defined(__cpp_lib_semaphore)
    binary_semaphore semaphore_;
#else
    bool ready_{false};
    mutex mtx_;
    condition_variable cv_;
#endif
  };

  coroutine_handle<Promise> h_;
  explicit SyncWaitTask(Promise& p) noexcept
      : h_{coroutine_handle<Promise>::from_promise(p)} {}

public:
  using promise_type = Promise;
  SyncWaitTask(SyncWaitTask&& t) noexcept : h_{exchange(t.h_, {})} {}
  ~SyncWaitTask() {
    if (h_)
      h_.destroy();
  }

  // Called from sync_wait(). Will block and retrieve the
  // value or error from the task passed to sync_wait()
  T&& get() {
    auto& p = h_.promise();
    h_.resume();

#if defined(__cpp_lib_semaphore)
    p.semaphore_.acquire(); // Block until signal
#else
    {
      auto lock = unique_lock{p.mtx_};
      p.cv_.wait(lock, [&p] { return p.ready_; });
    }
#endif

    if (p.error_)
      rethrow_exception(p.error_);
    return static_cast<T&&>(*p.value_);
  }
};

} // namespace detail

template <typename T>
using Result = decltype(declval<T&>().await_resume());

template <typename T>
Result<T> sync_wait(T&& task) {
  if constexpr (is_void_v<Result<T>>) {
    struct Empty {};
    auto coro = [&]() -> detail::SyncWaitTask<Empty> {
      co_await forward<T>(task);
      co_yield Empty{};
      assert(false); // Coroutine will be destroyed
    };               // before it has a chance to return.
    (void)coro().get();
    return; // Result<T> is void
  } else {
    auto coro = [&]() -> detail::SyncWaitTask<Result<T>> {
      co_yield co_await forward<T>(task);
      assert(false);
    };
    return coro().get(); // Rerturn value
  }
}

template <typename T>
class [[nodiscard]] Task {

  struct Promise {
    variant<monostate, T, exception_ptr> result_;
    coroutine_handle<> continuation_; // Awaiting coroutine
    auto get_return_object() noexcept { return Task{*this}; }
    
      void yield_value (auto&& v) {
          result_.template emplace <1> (forward<decltype (v)> (v));
      }
    void return_value(T value) {
      result_.template emplace<1>(move(value));
    }
    void unhandled_exception() noexcept {
      result_.template emplace<2>(current_exception());
    }
    auto initial_suspend() { return suspend_always{}; }
    auto final_suspend() noexcept {
      struct Awaitable {
        bool await_ready() noexcept { return false; }
        auto await_suspend(coroutine_handle<Promise> h) noexcept {
          return h.promise().continuation_;
        }
        void await_resume() noexcept {}
      };
      return Awaitable{};
    }
  };

  coroutine_handle<Promise> h_;
  explicit Task(Promise & p) noexcept
      : h_{coroutine_handle<Promise>::from_promise(p)} {}

public:
  using promise_type = Promise;
  Task(Task && t) noexcept : h_{exchange(t.h_, {})} {}
  ~Task() {
    if (h_)
      h_.destroy();
  }

  // Awaitable interface
  bool await_ready() { return false; }
  auto await_suspend(coroutine_handle<> c) {
    h_.promise().continuation_ = c;
    return h_;
  }
  T await_resume() {
    auto&& r = h_.promise().result_;
    if (r.index() == 1) {
      return get<1>(move(r));
    } else {
      rethrow_exception(get<2>(move(r)));
    }
  }
};

// Template Specialization for Task<void>
template <>
class [[nodiscard]] Task<void> {

  struct Promise {
    exception_ptr e_;
    coroutine_handle<> continuation_; // Awaiting coroutine
    auto get_return_object() noexcept { return Task{*this}; }
    void return_void() {}
    void unhandled_exception() noexcept { e_ = current_exception(); }
    auto initial_suspend() { return suspend_always{}; }
    auto final_suspend() noexcept {
      struct Awaitable {
        bool await_ready() noexcept { return false; }
        auto await_suspend(coroutine_handle<Promise> h) noexcept {
          return h.promise().continuation_;
        }
        void await_resume() noexcept {}
      };
      return Awaitable{};
    }
  };

  coroutine_handle<Promise> h_;
  explicit Task(Promise & p) noexcept
      : h_{coroutine_handle<Promise>::from_promise(p)} {}

public:
  using promise_type = Promise;

  Task(Task && t) noexcept : h_{exchange(t.h_, {})} {}
  ~Task() {
    if (h_)
      h_.destroy();
  }

  // Awaitable interface
  bool await_ready() { return false; }
  auto await_suspend(coroutine_handle<> c) {
    h_.promise().continuation_ = c;
    return h_;
  }
  void await_resume() {
    if (h_.promise().e_)
      rethrow_exception(h_.promise().e_);
  }
};

Task<int> width () {
    co_return 11;
}

Task<int> height () {
    co_return 12;
}

Task<int> area () {
    co_return co_await width() * co_await height();
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

//            return awaitable {};
            return suspend_always {};
        }
        auto final_suspend () noexcept
        {
            struct awaitable
            {
//                coroutine_handle<>& coro;
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
//            return suspend_always {};
        }
        auto yield_value (int i)
        {
            value = i;
            
            struct awaitable
            {
                coroutine_handle<promise_type> co;
                auto await_ready ()
                {
                    return true;
                }
                void await_suspend (coroutine_handle <promise_type> current_coro)
                {
//                    co = current_coro;
//                    return current_coro.promise().awaiting_coro;
//                    auto precursor = current_coro.promise().awaiting_coro;
//                    if (precursor)
//                    {
//                      return precursor;
//                    }
//                    return noop_coroutine();
//                    return current_coro;
//                    return suspend_always {};

                }
                auto await_resume ()
                {
                    return 8;
//                    return co;
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

co_task c ()
{
    cout << "c0" << endl;
    cout << "c1" << endl;
    co_return;
}

co_task b ()
{
    cout << "b0" << endl;
    int i = co_yield 11;
    cout << i << endl;
    co_await c ();
    cout << "b1" << endl;
    
    co_return;
}

co_task a ()
{
    cout << "a0" << endl;
//    cout << co_await run0 () << endl;
//    co_yield 10;
    co_await b();
//    co_await b();
    cout << "a1" << endl;
    co_return;
}


int main(int argc, char const *argv[])
{

    co_task coro = a();
    cout << (bool) coro << endl;
//    cout << (bool) coro << endl;
//    cout << (bool) coro << endl;
//    cout << (bool) coro << endl;
    
    

    
//	int result = Catch::Session().run( argc, argv );
//	return result;
    return 0;
}
