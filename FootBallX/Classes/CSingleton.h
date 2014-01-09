//
//  Singleton.h
//  TheForce
//
//  Created by Ray M on 13-1-28.
//
//

#ifndef TheForce_Singleton_h
#define TheForce_Singleton_h
#include <iostream>

template<class T> class CSingleton
{
public:
	virtual ~CSingleton(){spInstance_=nullptr;}
	static T* getInstance()
	{
		if (spInstance_==nullptr)
		{
			spInstance_ = new T;
		}
		return spInstance_;
	}
    
protected:
	CSingleton(){}
    
private:
    static T* spInstance_;
};

#define IMPLEMENT_SINGLETON(__c) template<> __c* CSingleton<__c>::spInstance_ = nullptr;

#endif
