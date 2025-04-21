#pragma once
#include "IObserver.h"
#include <any>
#include <memory>


class ISubject {
public:
	virtual ~ISubject() = default;
	virtual void Attach(std::shared_ptr<IObserver> observer) = 0;
	virtual void Detach(std::shared_ptr<IObserver> observer) = 0;
	virtual void Notify(std::any message) = 0;
};
