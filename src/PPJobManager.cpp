#include "PPJobManager.h"



//************************************
PPJobManager::PPJobManager( int pMinExecutor, int pMaxExecutor, int pMaxJobByExecutor )
  :cMinExecutor(pMinExecutor),
   cMaxExecutor(pMaxExecutor),
   cMaxJobByExecutor(pMaxJobByExecutor),
   cMyThread()//DON'T RUN THE THREAD !!!
{
}
//-----------------------------------
void
PPJobManager::runInThread()
{
  cMyThread = std::thread(&PPJobManager::run, this); 
}
//-----------------------------------
void
PPJobManager::run()
{
  // cExternalProtection.clear();
  addExecutor( cMinExecutor );

  // NE SERT A RIEN POUR LE MOMENT
  // GERER LE NOMBRE DE THREAD ACTIF EN FONCTION DE LA CHARGE
  /*
    while( cFlagContinue )
    {
			
    uint lNbExecutor = cExecutors.size();
    if( lNbExecutor )
    {
    std::lock_guard<std::mutex>  lGuard( cExternalProtection );				
    }
				
    ::usleep( 1000 ); // METTRE UNE variable      
			
    }
  */
}
#define PROTECT std::lock_guard<std::mutex>  lGuard( cExternalProtection );
#define UNPROTECT
#define PROTECTbbb PPUtils::PPAtomicLock lGuard( cExternalProtection, 100 );
//#define PROTECT cExternalProtection.lock();
//#define UNPROTECT cExternalProtection.unlock();

//-----------------------------------
void 
PPJobManager::addExecutor( int pNb )
{	
  std::cout << "PPJobManager::addExecutor " << pNb << std::endl;
  for( int i=0 ; i <  pNb; i++ )
    {
      PPJobExecutor* lExec =  new PPJobExecutor( *this );
      PROTECT

      cExecutors.push_back( lExec );
      UNPROTECT
      lExec->runInThread();
    }
}
//-----------------------------------
bool
PPJobManager::externalAddJob(  const char* pName,  PPJobFunction pMyFunction, unsigned int pStackSize)
{
  // std::cout << "PPJobManager::addJob - " << pName << std::endl;
  PROTECT
    
  cJobListToRun.addEnd( new PPJob( pName, pMyFunction, pStackSize ));

  UNPROTECT
  //	if( cExecutors.size() == 0 )
  //		{
  //			//			addExecutor( 1) ;
  //	}	

  return true;
}
//-----------------------------------

PPJob*
PPJobManager::executorCallGetJobForExecutor()	
{
  PROTECT
  PPJob*  lJob = cJobListToRun.popBegin();
  if( lJob != nullptr )
    {
      cJobListRunning.addEnd( lJob );
    }
  UNPROTECT
  
  return lJob;
}
//-----------------------------------
void
PPJobManager::executorCallReleaseJob( PPJob* pJob )
{	

  switch( pJob->cRequestState )
    {
    case PPJob::RequestState::REQUEST_STATE_NONE:
    case PPJob::RequestState::REQUEST_STATE_WAIT:
    case PPJob::RequestState::REQUEST_STATE_RUNNING:
      {
	PROTECT
	cJobListRunning.remove( pJob );
	cJobListToRun.addEnd( pJob );
	UNPROTECT
      }
      break;
      
    case PPJob::RequestState::REQUEST_STATE_FINISH:
    case PPJob::RequestState::REQUEST_STATE_ERROR:
      {
        PROTECT
	cJobListRunning.remove( pJob );
	cJobListFinish.addEnd( pJob );
	UNPROTECT
      }
      break;
    }
}
//-----------------------------------
std::ostream & 
PPJobManager::externalPrint( std::ostream &pOut, bool pLight, bool pOnlyData )
{
 
  PROTECT
 
  std::cout << "EXECUTORS:" << cExecutors.size() << std::endl;
	
  bool lFirst = true;
  if( ! pLight) 
    for( std::vector<PPJobExecutor*>::iterator  lIter = cExecutors.begin(); lIter != cExecutors.end(); ++lIter )
      {
	PPJobExecutor*  lExec = *lIter;

	if( lFirst )
	  {

	    lExec->externalPrint( pOut, true, pOnlyData );
	    lFirst = false; 
	  }

	(*lIter)->externalPrint( pOut, false, pOnlyData );			
      }
	
  lFirst = true;
  std::cout << "=== RUNNING JOBS " << cJobListRunning.size() << std::endl;

  if( ! pLight) 	
    for( PPJob* lCurrentJob = cJobListRunning.begin(); lCurrentJob != nullptr; lCurrentJob = lCurrentJob->next())
      {
	if(lFirst )
	  {
	    lCurrentJob->externalPrint( pOut, true, pOnlyData );
	    lFirst = false; 
	  }

	lCurrentJob->externalPrint( pOut, false, pOnlyData );
      }
 
  lFirst = true;
  std::cout << "=== WAITING JOBS " << cJobListToRun.size() << std::endl;
  if( ! pLight) 	
    for( PPJob* lCurrentJob = cJobListToRun.begin(); lCurrentJob != nullptr; lCurrentJob = lCurrentJob->next())
      {
	if(  lFirst )
	  {
	    lCurrentJob->externalPrint( pOut, true, pOnlyData );
	    lFirst = false; 
	  }

	lCurrentJob->externalPrint( pOut, false, pOnlyData );
      }
	
  lFirst = true;
	
  std::cout << "=== FINISH JOBS ===" << cJobListFinish.size() << std::endl;
	if( ! pLight) 
  for( PPJob* lCurrentJob = cJobListFinish.begin(); lCurrentJob != nullptr; lCurrentJob = lCurrentJob->next())
    {
      if(   lFirst )
	{
	  lCurrentJob->externalPrint( pOut, true, pOnlyData );
	  lFirst = false; 
	}

      lCurrentJob->externalPrint( pOut, false, pOnlyData );
    }

  UNPROTECT

  return pOut;
}

