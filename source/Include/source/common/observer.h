/*!
\file
\brief Заголовочный файл observer.h

Определяет базовые структуры для реализации уведомлений отношений один ко многим.
Множество объектов-наблюдателей (Observer) следят за одним наблюдаемым объектом (Observable)

*/
#pragma once
#include "base_class.h"
#include <memory>
#include <mutex>
#include <functional>
/**
* \interface ICommand
* Абстрактный тап наблюдателя
*/
struct IObserver
{
};

/**
* \класс Observable параметризованный сигнатурой функтора-уведомления
* Регистрирует наблюдателей для рассылки им уведомлений с заданной в параметрах сигнатурой .
* Следит за временем жизни наблюдателей и отписывает их в случае "смерти".
* Работает в многопоточном режиме.
*
* \tparam Lock  Тип класса для блокировки для работы в многопоточном режиме. ( NoLock - без блокировки )
* \tparam Ret  возвращаемое значение функтора-уведомления
* \tparam Args аргументы функтора-уведомления
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
	Регистрирует наблюдателя с функтором для вызова уведомления
	\param[in] wpObserver Указатель для контроля времени жизни наблюдателя
	\param[in] pObserveFunctor Функтор наблюдателя с заданной сигнатурой для вызова уведомления:
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
	Оповещает наблюдателей вызовом  функтора Ret ObserverClass::Fun( Args...args )
	Обнаружение в списке подписчиков наблюдателя без сильной ссылки означает, что объекта-наблюдателя больще не существует.
	Такие наблюдатели удаляются из списка подписчиков.
	*/
	Ret Notify(Ret ret_default, Args... args)
	{
		Ret ret = ret_default;
		std::vector<ObserveFunctor> targets;					//список "живых" наблюдателей
		{
			std::lock_guard< SpinLock >lock(m_SpinLock);
			m_observers.erase(std::remove_if(m_observers.begin(), m_observers.end(),
				[&targets](const auto& o)
				{
					auto ptr = o.first.lock();
					if (ptr)
					{
						targets.push_back(o.second);  //есть сильная ссылка - значит наблюдатель "живой" 
						return false;
					}
					else return true;					//нет сильной ссылки - удаляем из списка 
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
	ObserversContainer m_observers;  ///< контейнер наблюдателей
	SpinLock m_SpinLock;
};