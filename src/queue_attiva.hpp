#include <iostream>
#include <deque>
#include <mutex>

// CODA usata per comunicare coi thread, attesa attiva
// utilizzato dalla versione main.thr.bin

template <typename T>
class queue_q
{
private:
  mutex              d_mutex;
  deque<T>           d_queue;
  volatile bool      empty;
public:

  queue_q() {empty = true;}

   void push(T const& value) {
       {
         unique_lock<mutex> lock(this->d_mutex);
         d_queue.push_front(value);
         empty = false;
       }
   }

   T pop() {
      while(empty);
      {
         unique_lock<mutex> lock(this->d_mutex);
         T rc(move(this->d_queue.back()));
         this->d_queue.pop_back();
         if(d_queue.size() == 0) empty = true;
         return rc;
      }
  }
};

