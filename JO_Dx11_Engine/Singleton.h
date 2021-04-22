#pragma once

template<typename T>
class TemplateSingleton
{
public:
	static T&	GetInstance()
	{
		static T	instance;
		return instance;
	}
	///////////////////////////////////////////////
};

template<typename T>
class TemplateSingletonA
{
	static T *m_instance;
public:
	static T&	GetInstance()
	{
		if (m_instance == nullptr)
		{
			m_instance = new T;
		}
		return *m_instance;
	}
	void	Destroy()
	{
		delete m_instance;
		m_instance = nullptr;
	}
	///////////////////////////////////////////////
};

template<typename T>
T *TemplateSingletonA<T>::m_instance = nullptr;