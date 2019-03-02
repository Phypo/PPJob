#ifndef H__PPUtils__H
#define H__PPUtils__H

#include <mutex>
#include <thread>
#include <queue>

#include <atomic>

#include <boost/utility.hpp>
#include <sys/time.h>

namespace PPUtils{ 

  typedef  uint64_t PPTimeMicro70;

  
	extern  PPTimeMicro70 GetMicroTime70();
    
	//************************************

	class PPThread : public std::thread {

	public :
		PPThread(){;}

	};


	//************************************
	class PPMutex : public std::mutex {
	public:
		PPMutex() {;}
	};
	//************************************
	class PPAutoLockMutex {
		//   PPMutex &cMyMutex;

		std::mutex  & cMyMutex;
	public:
		PPAutoLockMutex(  std::mutex & pMutex )
			:cMyMutex( pMutex )
		{
			cMyMutex.lock();
		}
		~PPAutoLockMutex(){
			cMyMutex.unlock();
		}
	};
	//************************************
	template <class T>
	class PPGardedQueuePtr :  boost::noncopyable{

		PPMutex cMutexQueue;
	
		std::queue<T*> cMyQueue;
	
	public:
		void push( T* pVal) {
		
			PPAutoLockMutex pLock( cMutexQueue );
			cMyQueue.push(pVal);
		}
	
		T* pop() {
		
			PPAutoLockMutex pLock( cMutexQueue );
			T* lTmp = cMyQueue.front();
			cMyQueue.pop();
			return lTmp;
		}

		bool empty() {
			PPAutoLockMutex pLock( cMutexQueue );
			return cMyQueue.empty();

		}
		typename std::queue<T*>::size_type size() {
			PPAutoLockMutex pLock( cMutexQueue );
			return cMyQueue.size();

		}
	};

	//************************************
	class PPAtomicSpinLock {
		std::atomic_flag cFlag = ATOMIC_FLAG_INIT;

	public:
		void lock()     noexcept { while(cFlag.test_and_set(std::memory_order_acquire)); }
		void unlock()   noexcept { cFlag.clear(std::memory_order_release);         }
		bool try_lock() noexcept { return !cFlag.test_and_set(std::memory_order_acquire);  }
	};

	//************************************
	class PPAtomicSpinLockGuard {
		std::atomic_flag& cFlag;

	public:
		PPAtomicSpinLockGuard( std::atomic_flag& pFlag)
			:cFlag(pFlag)
		{
			while(cFlag.test_and_set(std::memory_order_acquire));
		}

		PPAtomicSpinLockGuard( std::atomic_flag& pFlag, uint pTimeSleepMicro )
			:cFlag(pFlag)
		{
			while(cFlag.test_and_set(std::memory_order_acquire)) { ::usleep(pTimeSleepMicro); }
		}

		~PPAtomicSpinLockGuard()
		{
		 	cFlag.clear();
		}
	};

	//************************************


}; // End namespace


#endif
