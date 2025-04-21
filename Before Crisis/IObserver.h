#pragma once
#include <any>

class IObserver {
public:
	virtual ~IObserver() = default;
	virtual void Update(const std::any message) = 0;
};

