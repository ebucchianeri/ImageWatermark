#define cimg_display 0
#include <iostream>
#include <mutex>
#include <condition_variable>
#include <deque>


// CODA usata per comunicare coi thread, no attesa attiva
// utilizzato dalla versione main.thrnorr.bin

template <typename T>
class queue
{
private:
  mutex              d_mutex;
  condition_variable d_condition;
  deque<T>           d_queue;
public:

  queue() {}

  void push(T const& value) {
    {
      unique_lock<mutex> lock(this->d_mutex);
      d_queue.push_front(value);
    }
    this->d_condition.notify_one();
  }

  T pop() {
   unique_lock<mutex> lock(this->d_mutex);
    this->d_condition.wait(lock, [=]{ return !this->d_queue.empty(); });
    T rc(move(this->d_queue.back()));
    this->d_queue.pop_back();
    return rc;
  }
};
