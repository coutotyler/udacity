// Priority Readers and Writers
//
// Write a multi-threaded C program that gives readers priority over writers concerning a shared (global) variable. 
// Essentially, if any readers are waiting, then they have priority over writer threads -- writers can only write 
// when there are no readers. This program should adhere to the following constraints:
//
// Multiple readers/writers must be supported (5 of each is fine)
// Readers must read the shared variable X number of times
// Writers must write the shared variable X number of times
// Readers must print:
// 	The value read
// 	The number of readers present when value is read
// Writers must print:
// 	The written value
// 	The number of readers present were when value is written (should be 0)
// Before a reader/writer attempts to access the shared variable it should wait some random amount of time
// Note: This will help ensure that reads and writes do not occur all at once
// Use pthreads, mutexes, and condition variables to synchronize access to the shared variable

void main() {
	int NUM_WRITE_THREADS = 5;
	int NUM_READ_THREAS = 5;
	int shared_var = 0;
	int num_readers = 0;
	pthread_mutex_t m_shared_var = init_mutex();
	pthread_mutex_t m_num_readers = init_mutex();
	pthread_cond_t write_ready;
	pthread_cond_t read_ready;
	pthread_t *writers[NUM_WRITE_THREADS]; 
	pthread_t *readers[NUM_READ_THREADS];

	// Create write threads
	for (i=0; i < 5; i++) {
		if ( pthread_create(&writers[i], NULL, &producer, NULL) != 0 ) { 
			printf("Couldn't create write thread number %d", i);
		}
	}

	// Create read threads
	for (i=0; i < 5; i++) {
		if ( pthread_create(&readers[i], NULL, &producer, NULL) != 0 ) { 
			printf("Couldn't create read thread number %d", i);
		}
		else num_readers += 1; 
	}

	// Join write threads
	for (i=0; i < 5; i++) { 
		pthread_join(writers[i]);
	}

	// Join read threads
	for (i=0; i < 5; i++) {
		pthread_join(readers[i]);
	}
	
	return 0;
}

void writer() {
	// Check predicate
	while (num_readers != 0) {
		pthread_cond_wait(&write_ready, &m_shared_var);
	} 

	// Lock and modify
	pthread_mutex_lock(&m_shared_var);
		shared_var += 1; 	
	pthread_mutex_unlock(&m_shared_var);

	// Signal next writer
	pthread_signal(&write_ready);
}

void reader() {
	pthread_mutex_lock(&m_shared_var, 'r')
		printf("Value of shared_var: %d\n", shared_var);
		printf("Number of readers: %d\n", num_readers);
	pthread_mutex_unlock(&m_shared_var)

	while {
		pthread_cond_wait(&c_num_reader, &m_num_reader); 
	}
	pthread_mutex_lock(&m_num_reader);
		num_reader -= 1;
	pthread_mutex_unlock(&m_num_reader);

	if num_reader == 0 {
		pthread_signal(&write_ready)
	}
}
