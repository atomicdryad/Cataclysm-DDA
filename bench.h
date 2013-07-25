#include <cstdlib>
#include <sys/time.h>
#define pfnewmethod 1

enum pfs {
  pf0,pf1,pf2,pf3,pf4,pf5,pf6,pfm0,pfm1,pfm2,pfm3,pfdout,
pg0,pg1,pg2,pg3,pg4,pg5,pg6,pg7,pg8,pg9,pgwtf,
lm0,lm1,lm2,lm3,lm4,lm5,lm6,lm7,lm8,lm9,
fd0,fd1,fd2,fd3,fd4,fd5,fd6,fd7,fd8,fd9,
n_pf
};

class Timer {
    timeval timer[2];

  public:
    Timer() {}
    void reset() {}
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

    int diff()
// const
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
    int diff2()
    {
        int secs(this->timer[1].tv_sec - this->timer[0].tv_sec);
        int usecs(this->timer[1].tv_usec - this->timer[0].tv_usec);

        if(usecs < 0)
        {
            --secs;
            usecs += 1000000;
        }

        return static_cast<long int>(secs * 1000000 + usecs);
    }
    int done1() {
        stop(); return diff();
    }
    int done() {
        stop(); return diff2();
    }
};


struct pfents {
  int times[n_pf];
  int count[n_pf];
  long int utimes[n_pf];
  Timer timers[n_pf];
  pfents () {
    for( int i=0;i<n_pf;i++ ) {
      timers[i].reset();
      times[i]=0;
      utimes[i]=0;
      count[i]=0;
    }
  };
  void start(int i) {
    timers[i].start();
  }
  void a (int i, int T) {
    count[i]++;
    times[i] += T;
    utimes[i] += (T*1000);
  };
  void stop(int i) {
#ifdef pfnewmethod
    utimes[i] += timers[i].done();
#else
    times[i] += timers[i].done1();
#endif
    count[i]++;
  }
  int get(int i) {
#ifdef pfnewmethod
    return static_cast<int>( utimes[i] / 1000.0 + 0.5 );
#else
    return times[i];
#endif
  }

  int getcount(int i) {
    return count[i];
  }
  void reset(int i) {
      times[i]=0;
      utimes[i]=0;
      count[i]=0;
  }
};
extern pfents pf;

