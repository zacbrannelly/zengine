#pragma once

template<typename T>
class BufferHandle
{
public:
	T GetHandle() const;

protected:
	void SetHandleID(unsigned short idx);

private:
	unsigned short _idx;
};

template<typename T>
T BufferHandle<T>::GetHandle() const
{
	T buffer;
	buffer.idx = _idx;

	return buffer;
}

template<typename T>
void BufferHandle<T>::SetHandleID(unsigned short idx)
{
	_idx = idx;
}