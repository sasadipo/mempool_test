#include <mutex> 
#include <vector>

//@template memory pool for single type of object - thread safe 
template <class ELEM_TYPE> class MemoryPool
{
public:
	// creates new memory pool
	// @param count the number of ELEM_TYPE values this pool can hold (default is 200)
	MemoryPool(std::size_t count = 200) : m_pElems(new ELEM_TYPE[count])
	{
		m_ElemPool.reserve(count);
		for (std::size_t _it = 0; _it < count; ++_it)
		{
			m_ElemPool.push_back(&m_pElems[_it]);
		}
	}

	~MemoryPool(void)
	{
		delete[] m_pElems;
	}

	// @return borrowed memory item from pool or null if no more items available from pool
	// caller must handle null value being returned
	ELEM_TYPE* alloc() 
	{
		ELEM_TYPE *t = nullptr;
		std::lock_guard<std::mutex> lock(m_pLock);
		if (!m_ElemPool.empty())
		{
			t = m_ElemPool.back();
			m_ElemPool.pop_back();
		}
		return t;
	}

	// return item back to pool
	void free(ELEM_TYPE* p)
	{
		std::lock_guard<std::mutex> lock(m_pLock);
		m_ElemPool.push_back(p);		
	}

private:
	// pointers to each element in the memory pool
	std::vector<ELEM_TYPE *> m_ElemPool;
	// pointer to first element in the container
	ELEM_TYPE *m_pElems;
	// lock for thread safety
	std::mutex m_pLock;
};