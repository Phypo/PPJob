#include <iostream>
#include <sstream>
#include <math.h>


#include "PPUtils.h"

#include "PPJobManager.h"




using namespace std;


unsigned long int tempo = 10;
unsigned long int tempo1 =1*tempo;
unsigned long int tempo2 =2*tempo;
unsigned long int tempo3 =3*tempo;

/*
	Avec une tempo de 10 - 9000 job en tout - 32 threads 
	on a environ plus de 45 passages par seconde. soit un passage tout les 22  millisecondes par job.
	En ne faisant rien dans la fonction (return 0) on est a 100 passages donc 10 millisecondes par job;
	Avec 30000 Job on est a peu pres a 25 passages

	Bug dans PPJob_bug , le currentJob passe a 0 lors du swap de mutex, reprise sur un mauvais Executor ????
	Trés bizarre

*/

volatile int sCptJob = 0;

volatile long int sCpt1 = 0;
volatile bool sFlagGo=false;


#define SLEEP_WAIT 0

#define BOUCLE for( int boucle=0; boucle<10000; boucle++)

double functStd( int lVar )
{
	if( sFlagGo == false )
		return 0;
	
	//	::usleep( lVar);
	double lRes =0;
	for( int i =0; i< lVar; i++ )
		{
			sCpt1++;
			lRes = sqrt( log( lVar) + log( lVar*2) +log( lVar*3 )+log( lVar*4 )+log( lVar*5 )+log( lVar*6 )); //  sqrt( lVar );
		}

	return lRes;
	
}

//********************************************

int printJobHello( PPJob* pJob ) {

	sCptJob++;
	
	BOUCLE
		{
			//  std::cout << " Hello 1 " << pJob->getName() << std::endl;
			functStd( tempo1 );

			if( pJob ) pJob->yield();
			//  std::cout << " Hello 2 " << pJob->getName() << std::endl;
			functStd( tempo1);
			if( pJob ) pJob->yield();
			// std::cout << " Hello 3 " << pJob->getName() << std::endl;
			functStd(tempo1  );
			if( pJob ) pJob->yield();
			// std::cout<< " Hello Finish " << pJob->getName() << std::endl;
			functStd( tempo1 );
			if( pJob ) pJob->yield();
		}
  return 0;
}
int printJobHelloMT() 
{

	sCptJob++;
	if( sFlagGo == false )	 ::usleep(10000);
	
	BOUCLE
		{
			functStd( tempo1 );
			functStd( tempo1 );
			functStd( tempo1 );
			//		std::cout << '.';
		}
  return 0;
}
//---------------------------------------
int printJobCoucou( PPJob* pJob ) {

	sCptJob++;	
	BOUCLE 
		{
			//  std::cout << " Coucou 1 " << pJob->getName() << std::endl;
			functStd( tempo2 );
			if( pJob ) pJob->yield();
			//  std::cout << " Coucou 2 " << pJob->getName() << std::endl;
			functStd( tempo2 );
			if( pJob ) pJob->yield();
			//  std::cout << " Coucou Finish " << pJob->getName() << std::endl;
			functStd( tempo2 );
			if( pJob ) pJob->yield();
		}
	return 1;
}
//---------------------------------------
int printJobCoucouMT() {

	sCptJob++;
	if( sFlagGo == false )	 ::usleep(10000);
	
	BOUCLE
		{
			functStd( tempo2 );
			functStd( tempo2 );
			functStd( tempo2 );
			//	std::cout << ':';
		}
	return 1;
}
//---------------------------------------
int printJobBouh( PPJob* pJob ) {

	sCptJob++;
	BOUCLE 
		{
			//  std::cout << " Bouh ! 1 " << pJob->getName() << std::endl;
			functStd( tempo3 );
			if( pJob ) pJob->yield();
			//  std::cout << " Bouh !  2 " << pJob->getName() << std::endl;
			functStd( tempo3);
			if( pJob ) pJob->yield();
			// std::cout << " Bouh !  3 " << pJob->getName() << std::endl;
			functStd( tempo3 );
			if( pJob ) pJob->yield();
			// std::cout << " Bouh !  4 " << pJob->getName() << std::endl;
			functStd( tempo3 );
			if( pJob ) pJob->yield();
			//  std::cout << " Bouh ! Finish " << pJob->getName() << std::endl;
		}
	return 2;
}
//---------------------------------------
int printJobBouhMT()
{
	sCptJob++;
	if( sFlagGo == false )	 ::usleep(10000);

	BOUCLE
		{
			functStd( tempo3 );
			functStd( tempo3);
			functStd( tempo3 );
			functStd( tempo3);
			//		std::cout << ';';
		}
	return 2;
}
//---------------------------------------
int printJobExecption( PPJob* pJob ) {

	std::cout << " Excep ! 1 " << pJob->getName() << std::endl;
	functStd( tempo3 );
  if( pJob ) pJob->yield();
	//  std::cout << " Excep !  2 " << pJob->getName() << std::endl;
	functStd( tempo3);
	throw exception() ;
  if( pJob ) pJob->yield();
	std::cout << " Excep !  3 " << pJob->getName() << std::endl;
	functStd( tempo3 );
  if( pJob ) pJob->yield();
  std::cout << " Excep !  4 " << pJob->getName() << std::endl;
	functStd( tempo3 );
	return 3;
}

int printJobSleep( PPJob* pJob ) {
  std::cout << "SLEEP 1 " << pJob->getName() << std::endl;
	::usleep( 100000000 );
	return -1;
}
//--------------------------------------
int printJobTime( PPJob* pJob ) {
	sCptJob++;

	int lCompteur =0;
	PPUtils::PPTimeMicro70 lDate = PPUtils::GetMicroTime70() ;
	while( true )
		{
			lCompteur++;
			PPUtils::PPTimeMicro70 lDateCur = PPUtils::GetMicroTime70();
			PPUtils::PPTimeMicro70 lDateDiff = lDateCur - lDate;
			
			if( lDateDiff >= 1000000)
				{
					std::cout << "*******************************************" << std::endl;
					std::cout << "*******************************************" << std::endl;
					std::cout << "Diff:" << lDateDiff << " error:" <<  (1000000.0/lDateDiff) << "  Cpt:" << lCompteur  <<  " sCpt:" << (sCpt1*1e-6)<< " job:" << sCptJob << std::endl; 
					std::cout << "*******************************************" << std::endl;
					std::cout << "*******************************************" << std::endl;
					lCompteur=0;
					lDate = lDateCur;
    		}
			if( pJob != NULL ) pJob->yield();
		}
	return -666;
}
//--------------------------------------
int printJobTimeMT( ) {

	sCptJob++;
	if( sFlagGo == false )	 ::usleep(10000);
	
	std::cout << "printJobTimeMT" << std::endl;
	
	int lCompteur =0;
	PPUtils::PPTimeMicro70 lDate = PPUtils::GetMicroTime70() ;
	while( true )
		{
			lCompteur++;
			PPUtils::PPTimeMicro70 lDateCur = PPUtils::GetMicroTime70();
			PPUtils::PPTimeMicro70 lDateDiff = lDateCur - lDate;
			
			if( lDateDiff >= 1000000)
				{
					std::cout << "*******************************************" << std::endl;
					std::cout << "*******************************************" << std::endl;
					std::cout << "Diff:" << lDateDiff << " error:" <<  (1000000.0/lDateDiff) << "  Cpt:" << lCompteur <<  " sCpt:" << (sCpt1*1e-6) << " job:" << sCptJob << std::endl;
			
					std::cout << "*******************************************" << std::endl;
					std::cout << "*******************************************" << std::endl;
					lCompteur=0;
					lDate = lDateCur;
    		}
			::usleep( 10 );
		}
	return -666;
}


//----------------------------------------

void  TestJob()
{

  std::cout << "*** MainContext 7777777 " << std::endl;


  PPJobManager  lJManager( 32, 128, 3 );
	lJManager.externalAddJob( "TIMER ", (PPJobFunction)printJobTime);

  for( int i=0; i<100000; i++)
    {
      { std::ostringstream lOs;
				lOs<< "Hello " << i ;
				lJManager.externalAddJob( lOs.str().c_str(), (PPJobFunction)printJobHello );
			}
      { std::ostringstream lOs;
				lOs<< "Coucou " << i ;

				lJManager.externalAddJob( lOs.str().c_str(), (PPJobFunction)printJobCoucou );
			}
      { std::ostringstream lOs;
				lOs<< "Bouh " << i ;

				lJManager.externalAddJob( lOs.str().c_str(), (PPJobFunction)printJobBouh);
			}
		
    }
	sFlagGo = true;


	//	lJManager.externalPrint( std::cout, true, false );

	std::cout << "BEFORE RUN" << std::endl;
  lJManager.runInThread();  
	std::cout << "AFTER  RUN" << std::endl;
  
  
	for( int i=0; i<60; i++ )
    {
			lJManager.externalPrint( std::cout, true, false );
			std::cout << "*************************************************************" << std::endl;
      ::usleep(1000000 );
    }
  

  /*
    std::cout << "before swap" <<  std::endl;
    ::swapcontext( &cMainContext, 	&Sheduling ); 
  */

  std::cout << "ending ..." << std::endl; 
	::exit(0);
} 

//----------------------------------------
		
void  TestThread() 
{	 
	std::thread a( printJobTimeMT);
			
	for( int i=0; i<10000; i++)
		{	
			std::thread* b=new 	std::thread(printJobHelloMT);				
			std::thread* c=new 	std::thread(printJobCoucouMT);
			std::thread* d=new 	std::thread(printJobBouhMT);
			if( i%1000 == 0 ) std::cout << i*3 << std::endl;
		}

	std::cout << "GO GO GO GO GO GO GO GO GO GO GO GO  " << std::endl;
	sFlagGo = true;

 
	for( int i=0; i<60; i++ )
		{
				
			std::cout << "=================================================" << std::endl;
			::usleep(1000000 );
		}
}
  


//**********************************************
void TestPPUtils()
{
	PPUtils::PPGardedQueuePtr<char> lGQueue;

	lGQueue.push( const_cast<char*>("titi") );
	lGQueue.push( const_cast<char*>("toto") );
	lGQueue.push( const_cast<char*>("tutu") );

	cout << "GQueue empty:" << lGQueue.empty() << endl;
	cout << "GQueue size:" << lGQueue.size() << endl;
	while( lGQueue.empty() == false )
		{
			cout << lGQueue.pop() << endl;
			cout << "GQueue size:" << lGQueue.size() << endl;
		}	
}
//**********************************************
int main( int argc, char* argv[] )
{

  for( int i=1; i< argc ; i++ )
    {
      char* lStr = argv[i];
      std::cout << "param " << i << '=' << lStr << std::endl;

			sFlagGo = false;
			if( *lStr == 'J' )
				{
					cout << "TestJob" << endl;
					TestJob();
				}
			else
				if( *lStr == 'T' )
					{
						cout << "TestThread" << endl;
						TestThread();
					}
			sCptJob = 0;
			sCpt1=0;
		}

  //	TestPPUtils();
}
