#pragma once

class ResponseProtocol
{
public:
	virtual void selected(bool var) = 0;
	virtual void activate() = 0;
};