#include "PPJob.h"



//************************************
const char* 
PPJob::StatusToString( RequestState pType)
{
  switch( pType )
    {
    case RequestState::REQUEST_STATE_NONE:       return "None";
    case RequestState::REQUEST_STATE_WAIT:       return "Waiting";
    case RequestState::REQUEST_STATE_RUNNING:    return "Running";
    case RequestState::REQUEST_STATE_FINISH:     return "Finish";
    case RequestState::REQUEST_STATE_ERROR:      return "Error";
    }
  return "UNKNOWN";
}
//-----------------------------------
PPJob::PPJob(  const char*   pName,
	     PPJobFunction pMyFonction,
	     unsigned int  pStackSize)
  :cRequestState(RequestState::REQUEST_STATE_NONE),
   cName(pName),
   cMyFonction( pMyFonction ),
   cResult(0),
   cException( false ),
	 cStackSize(pStackSize),

	 cLastTimeBegin(0),
	 cLastTimeEnd(0),
	 cLastDuration(0),
	 cTotalDuration(0)
{
  ::getcontext(&cContext);
  cContext.uc_stack.ss_sp    = ::malloc(cStackSize); 
  cContext.uc_stack.ss_size  = cStackSize;
  cContext.uc_stack.ss_flags = 0;
  cContext.uc_link            = &cMyContextExecutor;
  ::makecontext(&cContext, (PPInternalJobFunction)InternalRun, 1, this );
  cRequestState=RequestState::REQUEST_STATE_WAIT;
}
//-----------------------------------
PPJob::~PPJob()
{
  ::free( cContext.uc_stack.ss_sp );
}
//-----------------------------------
void
PPJob::InternalRun( PPJob* pJob )
{
  pJob->cResult = false;
  try {
		//    std::cout << "*** JobInternalRun for Job:" << pJob->cName << std::endl;
    pJob->cRequestState=RequestState::REQUEST_STATE_RUNNING;
			
    if( pJob->cMyFonction != nullptr )
      pJob->cResult = pJob->cMyFonction( pJob );
    else
      pJob->cResult = pJob->run();
			
    pJob->cRequestState=RequestState::REQUEST_STATE_FINISH;
		//    std::cout << "*** JobInternalRun for Job:" << pJob->cName << " finish "
		//<<	pJob->cResult <<  std::endl;
			
  }
  catch( ... )
    {		 
      pJob->cException = true;
      pJob->cRequestState=RequestState::REQUEST_STATE_ERROR;
      std::cout << "*** JobInternalRun for Job:"<< pJob->cName
								<< " catch  "<<	pJob->cResult <<  std::endl;
    }
		
}

//-----------------------------------
void
PPJob::yield()
{
  ::swapcontext( &cContext,  &cMyContextExecutor );
}
//-----------------------------------
std::ostream & 
PPJob::externalPrint( std::ostream &pOut, bool pWantTitle, bool pOnlyData )
{
	if( pWantTitle  )
		{
			pOut << std::setw( 32) << " Name"         << '|'
					 << std::setw( 15) << " Status "      << '|'
					 << std::setw( 8) << " Result "       << '|'
					 << std::setw( 11) << " Exception "    << '|'
				
					 << std::setw( 19) << " Last begin " << '|'
					 << std::setw( 19) << " Last end "   << '|'
					 << std::setw( 12) << " Last duration"    << '|'
					 << std::setw( 15) << "Total duration"    << '|'
					 << std::endl;
			return pOut;
		}

	if( pOnlyData )
		{
				pOut <<  cName                         << '|'
						 <<   StatusToString(cRequestState ) << '|'
						 << cResult       << '|'
						 << cException    << '|'				
						 << cLastTimeBegin << '|'
						 << cLastTimeEnd << '|'
						 << cLastDuration    << '|'
						 << cTotalDuration    << '|'
						 << std::endl;
		}
	else
		{
			pOut << std::setw( 32)  <<  cName                         << '|'
					 << std::setw( 15)  << StatusToString(cRequestState ) << '|'
					 << std::setw( 8) << cResult       << '|'
					 << std::setw( 11) << cException    << '|'
				
					 << std::setw( 19) << cLastTimeBegin << '|'
					 << std::setw( 19) << cLastTimeEnd << '|'
					 << std::setw( 12) << cLastDuration    << '|'
					 << std::setw( 12) << (cTotalDuration/1000000.0 )   << '|'
					 << std::endl;
		}
	return pOut;
}
