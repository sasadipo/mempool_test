#include <mutex> 
#include <vector>

//@template memory pool for single type of object - thread safe 
template <class ELEM_TYPE> class MemoryPool
{
public:

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

	void free(ELEM_TYPE* p)
	{
		std::lock_guard<std::mutex> lock(m_pLock);
		m_ElemPool.push_back(p);
		
	}

	std::size_t get_free_pool()
	{
		std::lock_guard<std::mutex> lock(m_pLock);
		return m_ElemPool.size();
	}
private:

	std::vector<ELEM_TYPE *> m_ElemPool;

	ELEM_TYPE *m_pElems;

	std::mutex m_pLock;
	
};