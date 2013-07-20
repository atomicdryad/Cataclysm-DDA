#include <cstdlib>
#include <sys/time.h>

enum pfs {
  pf0,pf1,pf2,pf3,pf4,pf5,pf6,n_pf
};

class Timer {
    timeval timer[2];

  public:

    timeval start()
    {
        gettimeofday(&this->timer[0], NULL);
        return this->timer[0];
    }

    timeval stop()
    {
        gettimeofday(&this->timer[1], NULL);
        return this->timer[1];
    }

    int diff() const
    {
        int secs(this->timer[1].tv_sec - this->timer[0].tv_sec);
        int usecs(this->timer[1].tv_usec - this->timer[0].tv_usec);

        if(usecs < 0)
        {
            --secs;
            usecs += 1000000;
        }

        return static_cast<int>(secs * 1000 + usecs / 1000.0 + 0.5);
    }
    int done() {
        this->stop(); return this->diff();
    }
};


struct pfents {
  int times[n_pf];
  int count[n_pf];
  Timer timers[n_pf];
  pfents () {
    for( int i=0;i<n_pf;i++ ) {
      times[i]=0;
      count[i]=0;
    }
  };
  void start(int i) {
    timers[i].start();
  }
  void a (int i, int T) {
    count[i]++;
    times[i] += T;
  };
  void stop(int i) {    
    times[i] += timers[i].done();
    count[i]++;
  }
  int get(int i) {
    return times[i];
  }
  void reset(int i) {
      times[i]=0;
      count[i]=0;
  }
//    int& operator[] (pfs i) { return times[i]; };
//    int& operator[] (int i) { return times[i]; };
};
extern pfents pf;

