race condition
data race
critical section

### Basic operations on threads

```C++
void f(int i, string s, string& t) { 
    this_thread::yield();
    this_thread::get_id();
    this_thread::sleep_for(chrono::seconds(2));
}

int main() {
    string s = "ss";
    string t = "tt";
    // to pass reference use reference wrapper
    thread t {f, 0, s, ref(t)};

    // thread destructor calls terminate() on the thread
    
    t.get_id();
    if (t.joinable()) t.join();
}
```

### Mutex

```C++
mutex m, m1, m2; // shared

void worker() {
    // acquires lock and releases when lock is destroyed
    // cannot be unlocked (i.e. no lock1.unlock())
    // to unlock and relock use unique_lock
    lock_guard<mutex> lock1 {m}; 

    // C++17 same as lock_guard but can take multiple mutexes and avoid deadlocks
    scoped_lock<mutex> lock2 {m1, m2} 
}
```

```C++
mutex m, m1, m2;

void worker1() {
    // acquires lock, upon destruction will unlock
    unique_lock<mutex> lock {m};
    lock.lock();
    lock.unlock();
}

void worker2() {
    // first lock together to avoid deadlock and then adopt locks
    std::lock(m1, m2);
    unique_lock<mutex> lock1 {m1, adopt_lock};
    unique_lock<mutex> lock2 {m2, adopt_lock};
}

void worker3() {
    // same as worker2 but defer lock during construction and then acquire them together
    unique_lock<mutex> lock1 {m1, defer_lock};
    unique_lock<mutex> lock2 {m2, defer_lock};
    std::lock(m1, m2);
}

void worker4() {
    m.lock();
    // do not use raw m.lock() and unlock(). If the an exception is thrown here m will forever remain locked.
    // use unqiue_lock to lock for RAII
    m.unlock();
}
```

### Condition Variables

```C++
// shared
mutex m;
condition_variable cv;

void worker() {
    unique_lock<mutex> lock {m};

    // always use while() -> avoid if() as spurious wake ups can happen 
    // and 'if' may lead to logical errors when multiple threads run
    while (/* condition is not met */)
        cv.wait(lock);

    // same as above (while) but with lambda
    cv.wait(lock, []() {return true;});

    // can notify any one 
    // be careful to notify correct one: may need multiple cvs for that
    cv.notify_one();
    cv.notify_all();

    // lock is auto unlocked upon destruction
}
```

Always:
1. hold the lock when calling wait (mandatory) or notify (not mandatory but do this by default).
2. use while loop for condition check.
3. remember to notify correct threads.

### Semaphore

```C++

// shared
// allow 5 threads to work and then other wait
counting_semaphore<5> sem;
// binary semaphore are essentially locks
binary_semaphore b;

void worker() {
    // decrements counter: If the counter gets negative thread must wait
    sem.acquire();

    // increments counter and unblock acquirers if any of them are waiting
    sem.release();
}
```

### Building locks

```c++
// assume function runs atomically
int TestAndSet(int *old_ptr, int new) {
    int old = *old_ptr; // fetch old value at old_ptr
    *old_ptr = new; // store ’new’ into old_ptr
    return old; // return the old value
}

typedef struct __lock_t {
    int flag;
} lock_t;

void init(lock_t *lock) {
    // 0: lock is available, 1: lock is held    
    lock->flag = 0;
}

void lock(lock_t *lock) {
    while (TestAndSet(&lock->flag, 1) == 1) ; // spin-wait (do nothing)
}

void unlock(lock_t *lock) {
    lock->flag = 0;
}

// correct, but unfair - some thread can get stuck behind ones that came after it
```

```c++
int CompareAndSwap(int *ptr, int expected, int new_val) {
    int original = *ptr;
    if (original == expected)
        *ptr = new_val;
    return original;
}

void lock (lock_t *lock) {
    while (CompareAndSwap(&lock->flag, 0, 1) == 1);
}
```

### Concurrent Stack

```C++
struct Node {
    int val;
    Node* next;

    Node(_val): val{_val}, next{nullptr} {}
};

void insert(int v) {
    Node* n = new Node(v);
    lock();
    n->next = head;
    head = n;
    unlock();
}

void remove() {
    lock();
    Node* old = head;
    int v = head->val;
    head = head->next;
    unlock();
    delete old;
    return v;
}
```

```C++
int CompareAndSwap(int *address, int expected, int new) {
    if (*address == expected) {
        *address = new;
        return 1; // success
    }
    return 0; // failure
}

void insert(int value) {
    node_t *n = malloc(sizeof(node_t));
    assert(n != NULL);
    n->value = value;
    do {
        n->next = head;
    } while (CompareAndSwap(&head, n->next, n) == 0);
}
```

### Producer / Consumer problem

```C++
int buffer[MAX];
int fill_ptr = 0;
int use_ptr = 0;
int count = 0;

void put(int value) {
    buffer[fill_ptr] = value;
    fill_ptr = (fill_ptr + 1) % MAX;
    count++;
}

int get() {
    int tmp = buffer[use_ptr];
    use_ptr = (use_ptr + 1) % MAX;
    count--;
    return tmp;
}

cond_t empty, fill;
mutex_t mutex;

void *producer(void *arg) {
    int i;
    for (i = 0; i < loops; i++) {
        Pthread_mutex_lock(&mutex); // p1
        while (count == MAX) // p2
            Pthread_cond_wait(&empty, &mutex); // p3
        put(i); // p4
        Pthread_cond_signal(&fill); // p5
        Pthread_mutex_unlock(&mutex); // p6
    }
}

void *consumer(void *arg) {
    int i;
    for (i = 0; i < loops; i++) {
        Pthread_mutex_lock(&mutex); // c1
        while (count == 0) // c2
            Pthread_cond_wait(&fill, &mutex); // c3
        int tmp = get(); // c4
        Pthread_cond_signal(&empty); // c5
        Pthread_mutex_unlock(&mutex); // c6
        printf("%d\n", tmp);
    }
}
```

```c++
int buffer[MAX];
int fill = 0;
int use = 0;

void put(int value) {
    buffer[fill] = value; // Line F1
    fill = (fill + 1) % MAX; // Line F2
}

int get() {
    int tmp = buffer[use]; // Line G1
    use = (use + 1) % MAX; // Line G2
    return tmp;
}

void *producer(void *arg) {
    int i;
    for (i = 0; i < loops; i++) {
        sem_wait(&empty); // Line P1
        sem_wait(&mutex); // Line P1.5 (lock)
        put(i); // Line P2
        sem_post(&mutex); // Line P2.5 (unlock)
        sem_post(&full); // Line P3
    }
}

sem_t empty, full;
sem_init(&empty, 0, MAX); // MAX are empty
sem_init(&full, 0, 0); // 0 are full

void *consumer(void *arg) {
    int i;
    for (i = 0; i < loops; i++) {
        sem_wait(&full); // Line C1
        sem_wait(&mutex); // Line C1.5 (lock)
        int tmp = get(); // Line C2
        sem_post(&mutex); // Line C2.5 (unlock)
        sem_post(&empty); // Line C3
        printf("%d\n", tmp);
    }
}
```

### Reader Writer lock

```C++
typedef struct _rwlock_t {
    sem_t writelock;
    sem_t lock;
    int readers;
} rwlock_t;

void rwlock_init(rwlock_t *lock) {
    lock->readers = 0;
    Sem_init(&lock->lock, 1); 
    Sem_init(&lock->writelock, 1); 
}

void rwlock_acquire_readlock(rwlock_t *lock) {
    Sem_wait(&lock->lock);
    lock->readers++;
    if (lock->readers == 1)
	Sem_wait(&lock->writelock);
    Sem_post(&lock->lock);
}

void rwlock_release_readlock(rwlock_t *lock) {
    Sem_wait(&lock->lock);
    lock->readers--;
    if (lock->readers == 0)
	Sem_post(&lock->writelock);
    Sem_post(&lock->lock);
}

void rwlock_acquire_writelock(rwlock_t *lock) {
    Sem_wait(&lock->writelock);
}

void rwlock_release_writelock(rwlock_t *lock) {
    Sem_post(&lock->writelock);
}

// starves the writers
```

### Dining Philosopher

```C++
void get_forks(int p) {
    if (p == 4) {
	    Sem_wait(&forks[right(p)]);
	    Sem_wait(&forks[left(p)]);
    } else {
	    Sem_wait(&forks[left(p)]);
	    Sem_wait(&forks[right(p)]);
    }
}

void put_forks(int p) {
    Sem_post(&forks[left(p)]);
    Sem_post(&forks[right(p)]);
}
```