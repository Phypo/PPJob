#ifndef H__PPINTRUSIVE_LIST__H
#define H__PPINTRUSIVE_LIST__H



#include <iostream>

namespace PPIntrusive{

  template <class T>
	class PPList;
  
  //********************************************
  template <class T>
	class PPListElement{

    PPListElement*  cPreviousElement;
    PPListElement*  cNextElement;

  public:
		PPListElement():
			cPreviousElement(nullptr),
      cNextElement(nullptr)
		{;} 
		
  private:	
    void addAfter( PPListElement* pToAdd  )
    {
      if( cNextElement != nullptr )
				{
					cNextElement->cPreviousElement = pToAdd;
				}
      pToAdd->cNextElement     = cNextElement;
      cNextElement             = pToAdd;
      pToAdd->cPreviousElement = this;
    }
  public:
    void removeMe ()
    {
      if( cNextElement != nullptr )
				{
					cNextElement->cPreviousElement = cPreviousElement;
				}
    
      if( cPreviousElement != nullptr )
				{
					cPreviousElement->cNextElement = cNextElement;
				}    
    }
    T* next()     { return  static_cast<T*>(cNextElement); }
    T* previous() { return  static_cast<T*>(cPreviousElement); }
    
    friend PPIntrusive::PPList<T>;
  };




  //**************************
  template <class T>
	class PPList{
    
  protected:
    PPListElement<T>* cBegin;
    PPListElement<T>* cEnd;
    unsigned int cSize;
    
  public:
		PPList() 
			: cBegin(nullptr)
      ,cEnd(nullptr)
      ,cSize(0)
		{
		}
    //-----------------------------
    void addBegin( PPListElement<T>* pElem )
    {
      if( cBegin != nullptr )
				{
					cBegin->cPreviousElement = pElem;
				}
      else
				cEnd = pElem;
	  
      pElem->cNextElement = cBegin;
      cBegin = pElem;
      
      cSize++;
    }
    //-----------------------------
    void addEnd( PPListElement<T>* pElem )
    {
      if( cEnd != nullptr )
				{
					cEnd->cNextElement = pElem;
				}
      else
				cBegin = pElem;
	  
      pElem->cPreviousElement = cEnd;
      pElem->cNextElement = nullptr;
	
      cEnd = pElem;
      cSize++;
    }
    //-----------------------------
    // On ne verifie pas si l'obet appartient à la liste !
    
    void remove(  PPListElement<T>* pElem )
    {
      if( cBegin == pElem )
				{
					cBegin = pElem->cNextElement;
				}
      
      if( cEnd == pElem )
				{
					cEnd = pElem->cPreviousElement;
				}
      
      pElem->removeMe();
      cSize--;
      //      std::cout << "remove size:" << cSize << std::endl;
    }
    
    T*           begin()       { return static_cast<T*>(cBegin);  } 
    T*           end()         { return static_cast<T*>(cEnd);  } 
    unsigned int size() const  { return cSize; }
    
    //-----------------------------
    void addListEnd( PPList & pSrc )
    {
      if( cEnd != nullptr )
				{
					cEnd->cNextElement = pSrc.cBegin;
					if( pSrc.cBegin != nullptr )
						{
							pSrc.cBegin->cPreviousElement = cEnd;
						}
	   
				}
      else
				{
					cBegin = pSrc.cBegin;
					cEnd = pSrc.cEnd;
				}
      cSize += pSrc.cSize;
      pSrc.cBegin = pSrc.cEnd = nullptr;
      pSrc.cSize = 0;
      
    }  

    //--------------------------------
    T* popBegin()
    {
      PPListElement<T>* lTmp = cBegin;
      if( lTmp == nullptr )
				{
					return  static_cast<T*>(nullptr); 
				}
		
      remove( cBegin );
      return static_cast<T*>(lTmp); 
    }

		
  };
}; //fin namespace PPIntrusive


#endif
