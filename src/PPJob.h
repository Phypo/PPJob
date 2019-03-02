#ifndef H__PPJOB__H
#define H__PPJOB__H



#include <string>
#include <ucontext.h>

#include "PPList.h"
#include "PPUtils.h"


class PPJob;

typedef  void (*PPInternalJobFunction)(void) ; 
typedef  int (*PPJobFunction)(PPJob*) ;

#include <iomanip>


//********************************************
class PPJob : public PPIntrusive::PPListElement<PPJob>
{
 public:
  enum class RequestState{ REQUEST_STATE_NONE=0, 
		     REQUEST_STATE_WAIT,
		     REQUEST_STATE_RUNNING,
		     REQUEST_STATE_FINISH,
		     REQUEST_STATE_ERROR
  }; 

 private:

  ucontext_t     cContext;
  ucontext_t     cMyContextExecutor;  // Le context de l'executor

  RequestState   cRequestState;
  std::string    cName;
  //ulonglong    cJobId;

  PPJobFunction  cMyFonction;
  int            cResult;
  bool           cException;
  uint           cStackSize;  


	PPUtils::PPTimeMicro70  cLastTimeBegin;
	PPUtils::PPTimeMicro70  cLastTimeEnd;
	PPUtils::PPTimeMicro70  cLastDuration;
	PPUtils::PPTimeMicro70  cTotalDuration;


 public:
										
  PPJob(	const char* pName,
		PPJobFunction pMyFonction,
		unsigned int pStackSize);


  virtual ~PPJob();
	
  static void InternalRun( PPJob* pJob );
  virtual int run()         { return 0; }
									
  void        yield();

  const std::string&  getName() const { return cName;}
  static const char*  StatusToString( RequestState pType);

  friend class PPJobExecutor;
	friend class PPJobManager;

  std::ostream & externalPrint( std::ostream &pOs, bool pTitle, bool pOnlyData );

};
//********************************************




#endif
