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

#include <stdio.h>
#include <pthread.h>

void* writer();
void* reader();

int shared_var = 0;
int num_readers = 0;
pthread_mutex_t* m_shared_var;
pthread_mutexattr_t* m_attr_shared_var;
pthread_cond_t* write_ready;

int main(void) {
	int NUM_WRITE_THREADS = 5;
	int NUM_READ_THREADS = 5;
	//pthread_mutex_t m_num_readers = init_mutex();
	//pthread_cond_t read_ready;
	pthread_t *writers[NUM_WRITE_THREADS]; 
	pthread_t *readers[NUM_READ_THREADS];

	// Initialize mutex
	pthread_mutexattr_init(m_attr_shared_var);
	pthread_mutex_init(m_shared_var, m_attr_shared_var);

	// Create write threads
	for (int i=0; i < 5; i++) {
		if ( pthread_create(writers[i], NULL, writer, NULL) != 0 ) { 
			printf("Couldn't create write thread number %d", i);
		}
	}

	// Create read threads
	for (int i=0; i < 5; i++) {
		if ( pthread_create(readers[i], NULL, reader, NULL) != 0 ) { 
			printf("Couldn't create read thread number %d", i);
		}
		else num_readers += 1; 
	}

	// Join write threads
	for (int i=0; i < 5; i++) { 
		pthread_join(*writers[i], NULL);
	}

	// Join read threads
	for (int i=0; i < 5; i++) {
		pthread_join(*readers[i], NULL);
	}
	
	return 0;
}

void* writer() {
	// Check predicate
	while (num_readers != 0) {
		pthread_cond_wait(write_ready, m_shared_var);
	} 

	// Lock and modify
	pthread_mutex_lock(m_shared_var);
		shared_var += 1; 	
	pthread_mutex_unlock(m_shared_var);

	// Signal next writer
	pthread_cond_signal(write_ready);
	
	pthread_exit(NULL);
}

void* reader() {
	// Print values
	printf("Value of shared_var: %d\n", shared_var);
	printf("Number of readers: %d\n", num_readers);

	// Decrement number of readers
	num_readers -= 1;

	// Signal writers if no more readers
	if (num_readers == 0) {
		pthread_cond_signal(write_ready);
	}

	pthread_exit(NULL);
}
