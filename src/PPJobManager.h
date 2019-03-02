#ifndef H__PPJOBMANAGER__H

#define H__PPJOBMANAGER__H


#include "PPJobExecutor.h"

//********************************************
class PPJobManager {
  volatile bool cFlagContinue;

  
  // std::atomic_flag cExternalProtection;
  std::mutex  cExternalProtection; // a remplacer par PPAtomicBoolean ?
  //  PPUtils::PPAtomicSpinLock cExternalProtection;

  PPIntrusive::PPList<PPJob> cJobListToRun;
  PPIntrusive::PPList<PPJob> cJobListRunning;
  PPIntrusive::PPList<PPJob> cJobListFinish;

  
  std::vector<PPJobExecutor*> cExecutors;
  
	int cMinExecutor;
	int cMaxExecutor;
	int cMaxJobByExecutor;

  std::thread cMyThread;
  
 public:
  PPJobManager( int pMinExecutor, int pMaxExecutor, int pMaxJobByExecutor );
  
  

  void run();
  void runInThread();
	void addExecutor( int pNb );


  std::ostream & externalPrint( std::ostream &pOut, bool pLight, bool pOnlyData );

  bool   externalAddJob( const char* pName,  PPJobFunction pMyFonction, unsigned int pStackSize=8192 );

  PPJob* executorCallGetJobForExecutor();
  void   executorCallReleaseJob( PPJob* pJob );

};
//********************************************

#endif
