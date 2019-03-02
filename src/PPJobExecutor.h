#ifndef H__PPJOBEXECUTOR__H

#define H__PPJOBEXECUTOR__H

#include "PPJob.h"

#include <atomic>

#include "PPUtils.h"


class PPJobManager;

//********************************************

class PPJobExecutor {

  volatile bool cFlagContinue;
	
  PPJobManager & cMyManager;
  PPJob        * cCurrentJob;

	PPUtils::PPTimeMicro70  cLastTimeBegin;
	PPUtils::PPTimeMicro70  cLastTimeEnd;

	PPUtils::PPTimeMicro70  cLastDuration;
	PPUtils::PPTimeMicro70  cTotalDuration;

	PPUtils::PPTimeMicro70  cTotalSleep;

	std::string cNameOfCurrentJob; // FOR DEBUG ONLY !
   

	//  PPIntrusive::PPList<PPJob> cJobListRunning;
	//	//  PPIntrusive::PPList<PPJob> cJobListToRun;
	// PPIntrusive::PPList<PPJob> cJobListFinish;

  std::thread cMyThread;

public:
	enum class ExecutorState{
		INIT,
			RUN_JOB,
			FINISH_JOB,
			SLEEP
			};

  static const char*  ExecutorStateToString( ExecutorState pType);

private :
	ExecutorState cState;

 private :
  PPJobExecutor( const PPJobExecutor& ) = delete;
  const PPJobExecutor& operator =( const PPJobExecutor& ) = delete;

 public :
	
	PPJobExecutor(PPJobManager &pMyManager );
  virtual ~PPJobExecutor();
  void runInThread();

  void  stop() { cFlagContinue = false; }
  //----------------------------------------
  void internalShedule();	
	std::ostream & externalPrint( std::ostream &pOs, bool pTitle, bool pOnlyData );

};

//********************************************


#endif
