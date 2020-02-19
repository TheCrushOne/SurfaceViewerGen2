/*!
\file
\brief ������������ ���� observer.h

���������� ������� ��������� ��� ���������� ����������� ��������� ���� �� ������.
��������� ��������-������������ (Observer) ������ �� ����� ����������� �������� (Observable)

*/
#pragma once
#include "base_class.h"
#include <memory>
#include <mutex>
#include <functional>
/**
* \interface ICommand
* ����������� ��� �����������
*/
struct IObserver
{
};

/**
* \����� Observable ����������������� ���������� ��������-�����������
* ������������ ������������ ��� �������� �� ����������� � �������� � ���������� ���������� .
* ������ �� �������� ����� ������������ � ���������� �� � ������ "������".
* �������� � ������������� ������.
*
* \tparam Lock  ��� ������ ��� ���������� ��� ������ � ������������� ������. ( NoLock - ��� ���������� )
* \tparam Ret  ������������ �������� ��������-�����������
* \tparam Args ��������� ��������-�����������
*/
template < class Lock, typename Ret, typename... Args >
class Observable
{
	using SharedPtr = std::shared_ptr<IObserver>;
	using WeakPtr = std::weak_ptr<IObserver>;
	using ObserveFunctor = std::function<Ret(Args...)>;
	using AddrAndFun = std::pair< WeakPtr, ObserveFunctor >;
	using ObserversContainer = std::vector<AddrAndFun>;

public:
	Observable() = default;
	virtual ~Observable() = default;

	/*!
	������������ ����������� � ��������� ��� ������ �����������
	\param[in] wpObserver ��������� ��� �������� ������� ����� �����������
	\param[in] pObserveFunctor ������� ����������� � �������� ���������� ��� ������ �����������:
	Ret ObserverClass::Fun( Args...args )
	{
		...
		return Ret;
	}
	*/

	void Attach(WeakPtr&& wpObserver, ObserveFunctor&& pObserveFunctor)
	{
		std::lock_guard< SpinLock >lock(m_SpinLock);
		m_observers.emplace_back(AddrAndFun{ wpObserver, pObserveFunctor });
	}

	/*!
	��������� ������������ �������  �������� Ret ObserverClass::Fun( Args...args )
	����������� � ������ ����������� ����������� ��� ������� ������ ��������, ��� �������-����������� ������ �� ����������.
	����� ����������� ��������� �� ������ �����������.
	*/
	Ret Notify(Ret ret_default, Args... args)
	{
		Ret ret = ret_default;
		std::vector<ObserveFunctor> targets;					//������ "�����" ������������
		{
			std::lock_guard< SpinLock >lock(m_SpinLock);
			m_observers.erase(std::remove_if(m_observers.begin(), m_observers.end(),
				[&targets](const auto& o)
				{
					auto ptr = o.first.lock();
					if (ptr)
					{
						targets.push_back(o.second);  //���� ������� ������ - ������ ����������� "�����" 
						return false;
					}
					else return true;					//��� ������� ������ - ������� �� ������ 
				}
			), m_observers.end());
		}

		for (auto& target : targets)
			ret |= target(args...);
		return ret;
	}

	Observable(const Observable&) = delete;
	Observable& operator=(const Observable&) = delete;
	Observable(const Observable&&) = delete;
	Observable& operator=(const Observable&&) = delete;
private:
	ObserversContainer m_observers;  ///< ��������� ������������
	SpinLock m_SpinLock;
};