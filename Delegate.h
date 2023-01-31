#pragma once
#include <vector>

template <typename ...Ts>
class DelegateListenerBase abstract
{
public:
	virtual ~DelegateListenerBase() = default;

	virtual void Invoke(const Ts& ...params) const = 0;

	virtual bool IsSameFunction(DelegateListenerBase* dlb) const = 0;
};

/**
 * \brief Class for storing a function and do some testing on it
 * \tparam Ts List of every parameters types
 */
template <typename ...Ts>
class DelegateListener final : public DelegateListenerBase<Ts...>
{
public:
	DelegateListener(void(*function)(Ts... params));

	void Invoke(const Ts& ...params) const override;
	
	bool IsSameFunction(DelegateListenerBase<Ts...>* dlb) const override;

private:
	void(*m_function)(Ts... params);
};

/**
 * \brief Class for storing a member function and do some testing on it
 * \tparam Ts List of every parameters types
 */
template <typename I, typename ...Ts>
class DelegateListenerMember final : public DelegateListenerBase<Ts...>
{
public:
	DelegateListenerMember(I* instance, void(I::* function)(Ts... params));

	void Invoke(const Ts& ...params) const override;
	
	bool IsSameFunction(DelegateListenerBase<Ts...>* dlb) const override;

private:
	I* m_instance;
	void(I::*m_function)(Ts... params);
};

/**
 * \brief Delegate implementation, store a list of DelegateListener
 * that are called on () operator
 * \tparam Ts List of every parameters types
 */
template <typename ...Ts>
class Delegate final
{
public:
	~Delegate();

	void Subscribe(void(* function)(Ts... params));
	void UnSubscribe(void(* function)(Ts... params));
	template <typename I>
	void Subscribe(I* instance, void(I::* function)(Ts... params));
	template <typename I>
	void UnSubscribe(I* instance, void(I::* function)(Ts... params));
	void Clear();
	
	void operator()(const Ts& ...params) const;

private:
	std::vector<DelegateListenerBase<Ts...>*> m_listeners;

	void Subscribe(DelegateListenerBase<Ts...>* dlb);
	void UnSubscribe(DelegateListenerBase<Ts...>* dlb);

public:
	Delegate& operator=(const Delegate&) = delete;
};
#include "Delegate.hxx"