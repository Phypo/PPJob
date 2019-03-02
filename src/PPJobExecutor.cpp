#include "PPJobExecutor.h"

#include "PPJobManager.h"

#include <iomanip>



//********************************************

PPJobExecutor::PPJobExecutor( PPJobManager &pManager )
  :cFlagContinue(false),
   cMyManager( pManager),
   cCurrentJob(nullptr),
	 
   cLastTimeBegin(0),
   cLastTimeEnd(0),
   cLastDuration(0),
   cTotalDuration(0),
   cTotalSleep(0),
	 
   cMyThread(),  //DON'T RUN THE THREAD !!!
   cState( ExecutorState::INIT )	 
{
  
}
//------------------------
PPJobExecutor::~PPJobExecutor()
{
  //????	
}				
//----------------------------------------
const char* 
PPJobExecutor::ExecutorStateToString( ExecutorState pState)
{
  switch( pState )
    {
    case ExecutorState::INIT:          return "INIT";
    case ExecutorState::RUN_JOB:       return "RUN_JOB";
    case ExecutorState::FINISH_JOB:    return "FINISH_JOB";
    case ExecutorState::SLEEP:         return "SLEEP";
    }
  return "UNKNOWN";
}				
//----------------------------------------
void
PPJobExecutor::internalShedule()
{
  cFlagContinue = true;
	
  //  try {
		
    while( cFlagContinue )
      {     
	PPJob* pMemJob = cCurrentJob = cMyManager.executorCallGetJobForExecutor();
	if( cCurrentJob != nullptr )
	  {				
	    cNameOfCurrentJob= 	cCurrentJob->cName;
			
	    cCurrentJob->cLastTimeBegin = cLastTimeBegin = PPUtils::GetMicroTime70();
	    cState = ExecutorState::RUN_JOB;
 
			// On passe dans le context du Job !!!
	    ::swapcontext( &cCurrentJob->cMyContextExecutor,  &cCurrentJob->cContext );
			if( cCurrentJob != pMemJob )
				{
					std::cout << "*** Error in PPJobExecutor : cCurrentJob change between swapcontent" << std::endl;
				}
			
	    cCurrentJob->cLastTimeEnd = cLastTimeEnd = PPUtils::GetMicroTime70();	   
	    cCurrentJob->cLastDuration = cLastDuration = cLastTimeEnd - cLastTimeBegin;
	    cCurrentJob->cTotalDuration += cLastDuration;
	    cTotalDuration += cLastDuration;
	    
	    cState =  ExecutorState::FINISH_JOB;
		
		
	    cMyManager.executorCallReleaseJob( cCurrentJob ); // appel manager->putJob // en fonction de l'etat du job il le met dans differente queue protege par mutex
	    cCurrentJob = nullptr;	  	    	 
	    //		std::cout << "--------------------------" << std::endl;
	  }
	else {
	  cState =  ExecutorState::SLEEP;
	  cTotalSleep += 100;  //VERIFIER LE
	  ::usleep( 10 ); // METTRE UNE variable 
	}     
      }
    //	std::cout << "***********************" << std::endl;
    //  }
//  catch( ... )
//  {
//    std::cout << "*************** internalShedule::Exception ***********************" << std::endl;
//  }
}
//-----------------------------------
//  std::mutex  cExternalProtection;

void
PPJobExecutor::runInThread()
{
  cMyThread = std::thread(&PPJobExecutor::internalShedule, this); 
}
//-----------------------------------
std::ostream & 
PPJobExecutor::externalPrint( std::ostream &pOut, bool pWantTitle, bool pOnlyData )
{
  if( pWantTitle  )
    {
      pOut << std::setw( 32) << " Thread Id"         << '|'
	   << std::setw( 15) << " Status "      << '|'

	   << std::setw( 16) << " JOB "      << '|'
				
	   << std::setw( 19) << " Last begin " << '|'
	   << std::setw( 19) << " Last end "   << '|'
	   << std::setw( 12) << " Duration "    << '|'
	   << std::setw( 15) << "Total duration"    << '|'
	   << std::setw( 12) << " Total sleep"    << '|'
	   << std::endl;
      return pOut;
    }

  if( pOnlyData )
    {
      pOut << cMyThread.get_id()                     << '|'
	   << ExecutorStateToString(cState ) << '|'
				
	   << cLastTimeBegin << '|'
	   << cLastTimeEnd << '|'
	   << cLastDuration    << '|'
	   << cTotalDuration    << '|'
	   << cTotalSleep    << '|'
	   << std::endl;
    }
  else
    {
      pOut << std::setw( 32)  <<  cMyThread.get_id()  << '|'
	   << std::setw( 15)  << ExecutorStateToString(cState ) << '|'
	   << std::setw( 16) <<   cNameOfCurrentJob << '|'
				
	   << std::setw( 19) << cLastTimeBegin << '|'
	   << std::setw( 19) << cLastTimeEnd << '|'
	   << std::setw( 12) << cLastDuration    << '|'
	   << std::setw( 15) << (cTotalDuration/1000000.0)    << '|'
	   << std::setw( 12) << (cTotalSleep /1000000.0)   << '|'
	   << std::endl;
    }

  return pOut;
}

//********************************************
