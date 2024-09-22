#ifndef _SINGLETON_H_
#define _SINGLETON_H_

//####################################################
// singleton template
//####################################################
template<typename T> class Singleton
{
	static T* m_sSingleton;

public:
	Singleton()
	{
		int nOffset = (int)(T*)1 - (int)(Singleton<T>*)(T*)1;
		m_sSingleton = (T*)((int)this + nOffset);
	}
	~Singleton(){ m_sSingleton = 0; }

	static T& GetSingleton(){ return (*m_sSingleton); }
	static T* GetSingletonPtr(){ return m_sSingleton; }
};

template<typename T> T* Singleton<T>::m_sSingleton = 0;

#endif