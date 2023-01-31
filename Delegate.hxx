#pragma once
#include <cassert>

#include "Delegate.h"

//////////////////////////////////////////////////
// DelegateListener                             //
//////////////////////////////////////////////////

template <typename ... Ts>
DelegateListener<Ts...>::DelegateListener(void(*function)(Ts... params)) :
	m_function(function)
{}

template <typename... Ts>
void DelegateListener<Ts...>::Invoke(const Ts&... params) const { m_function(params...); }

template <typename ... Ts>
bool DelegateListener<Ts...>::IsSameFunction(DelegateListenerBase<Ts...>* dlb) const
{
	assert(dlb != nullptr);
	auto* dl = dynamic_cast<DelegateListener*>(dlb);
	return dl && dl->m_function == m_function;
}

//////////////////////////////////////////////////
// DelegateListenerMember                       //
//////////////////////////////////////////////////

template <typename I, typename... Ts>
DelegateListenerMember<I, Ts...>::DelegateListenerMember(I* instance, void(I::* function)(Ts... params)) :
	m_instance(instance),
	m_function(function)
{}

template <typename I, typename... Ts>
void DelegateListenerMember<I, Ts...>::Invoke(const Ts&... params) const
{
	if (m_instance) (m_instance->*m_function)(params...);
}

template <typename I, typename... Ts>
bool DelegateListenerMember<I, Ts...>::IsSameFunction(DelegateListenerBase<Ts...>* dlb) const
{
	assert(dlb != nullptr);
	auto* dlm = dynamic_cast<DelegateListenerMember*>(dlb);
	return dlm && dlm->m_instance == m_instance && dlm->m_function == m_function;
}

//////////////////////////////////////////////////
// Delegate                                     //
//////////////////////////////////////////////////

template <typename... Ts>
Delegate<Ts...>::~Delegate() { Clear(); }

template <typename... Ts>
void Delegate<Ts...>::Clear()
{
	for (auto* listener : m_listeners)
	{
		delete listener;
		listener = nullptr;
	}
	m_listeners.clear();
}

template <typename... Ts>
void Delegate<Ts...>::Subscribe(void(*function)(Ts... params))
{
	Subscribe(new DelegateListener<Ts...>(function));
}
template <typename... Ts>
void Delegate<Ts...>::UnSubscribe(void(*function)(Ts... params))
{
	UnSubscribe(new DelegateListener<Ts...>(function));
}
template <typename... Ts>
template <typename I>
void Delegate<Ts...>::Subscribe(I* instance, void (I::* function)(Ts... params))
{
	assert(instance != nullptr);
	Subscribe(new DelegateListenerMember<I, Ts...>(instance, function));
}
template <typename... Ts>
template <typename I>
void Delegate<Ts...>::UnSubscribe(I* instance, void (I::* function)(Ts... params))
{
	assert(instance != nullptr);
	UnSubscribe(new DelegateListenerMember<I, Ts...>(instance, function));
}

template <typename... Ts>
void Delegate<Ts...>::operator()(const Ts&... params) const
{
	for (auto* listener : m_listeners)
		listener->Invoke(params...);
}

template <typename... Ts>
void Delegate<Ts...>::Subscribe(DelegateListenerBase<Ts...>* dlb)
{
	assert(dlb != nullptr);
	// Test if the listener already in our vector
	for (auto* listener : m_listeners)
		if (listener->IsSameFunction(dlb))
		{
			// Delete listener since it already exist in our vector
			delete dlb;
			dlb = nullptr;
			return;
		}

	m_listeners.push_back(dlb);
}
template <typename... Ts>
void Delegate<Ts...>::UnSubscribe(DelegateListenerBase<Ts...>* dlb)
{
	assert(dlb != nullptr);
	for (auto it = m_listeners.begin(); it != m_listeners.end(); ++it)
	{
		DelegateListenerBase<Ts...>* listener = *it;
		if (dlb->IsSameFunction(listener))
		{
			delete listener;
			listener = nullptr;
			m_listeners.erase(it);
			break;
		}
	}
}