class Person
{
public:
	char szFirstName[128];
	char szLastName[128];
	int  nSocialSecurityNumber;
};

MemoryPool<Person> ppl;

void print_thread_id(int id) {
	for (int i = 0; i < 1000; i++)
	{
		Person * me = ppl.alloc();
		if (me != nullptr)
		{
			memset(me, 1, sizeof(Person));
			printf("\r\nid: %d - number of free:%lu ", id, ppl.get_free_pool());
			ppl.free(me);
		}

	}
}
int main()
{
	
	std::thread threads[100];
	// spawn 10 threads:
	for (long int i = 0; i<100; ++i)
		threads[i] = std::thread(print_thread_id, i);

	for (auto& th : threads) th.join();

	return 0;
}
