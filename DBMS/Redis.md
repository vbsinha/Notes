**Re**mote **Di**ctionary **S**erver

Single node key-value system written in C
All data resides **in memory**
**Single-threaded** execution engine

Values can be:
1. Strings
2. Hashes (objects)
3. Lists
4. Sets
5. Sorted sets (priority queues)
6. Bloom filters
7. Geospatial indexes
8. Time series

Specific commands for each value type.
```bash
SET foo 1
GET foo # Returns 1
INCR foo # Returns 2
XADD mystream * name Sara surname OConnor # Adds an item to a stream
```
Doesn't support SQL by default

Mostly used as in-memory cache

For persistence: Periodic snapshots + WAL (of the commands sent to Redis)
Uses chained hashing
No secondary indexes, schemas, or constraints

#### Infrastructure Configuration

1. **Single node**
2. **Replicated** (Main -> Secondary) for high availability. Main writes successful commands to a WAL, and Secondary keeps catching up. Like Change Data Capture.
3. **Cluster** Redis clients needs to know which key resides on which node. Clients connect to the node that has data. Each individual primary node in a cluster could also be replicated by secondaries. Primary nodes know about others - use gossip protocol.

*Important to choose key structuring for good Redis performance.* 

#### Performance

Redis can handle O(100k) writes per second and read latency is often in the microsecond range. 

### Capabilities

#### Redis as a Cache

Use the key-value map as a Cache. Add **TTL** to each key. Redis guarantees values cannot be read after TTL has expired. TTL is also used to decide which keys to evict when more items need to be cached than memory allows.

Service can first check cache. If cache does not have data, it can read from DB.

Instead of using TTL, Redis can be configured to run in **LRU** mode.

#### Redis as a Distributed Lock

`INCR` uses default value `0` if key does not exist.
1. When trying to acquire lock `INCR lockname`. 
    a. If Redis returns 1, lock is acquired. 
    b. Otherwise keep trying, as someone else is holding the lock.
2. `DEL` lock when done using.

#### Redis for leaderboards

Can use sorted sets for leaderboards. Sorted sets guarantee log time operations.

#### Redis for rate limiting

Can use as a *fixed-window* rate limiter. Keep incrementing counter and serving requests until `N` requests, after that do not serve. After `W` minutes, expire the counter key.

#### Redis for proximity search

```bash
GEOADD key longitude latitude member # Adds "member" to the index at key "key"
GEORADIUS key longitude latitude radius # Searches the index at key "key" at specified position and radius
```

The search command runs in `O(N+log(M))` time where `N` is the number of elements in the radius and `M` is the number of members in our index.

#### Redis for event sourcing

Redis' streams are append-only logs similar to Kafka's topics.

Stream is a queue with items, added using `XADD`. Create a consumer group which connects to a bunch of workers. Consumer group reads items from the queue and the worker to process them. If a worker fails it provides a way for a new worker to claim (`XCLAIM`) and restart processing the message.

### Shortcomings and remediations

#### Hot key issue

In a Redis cluster some keys could requested very often and increase load on one node. To remedy:
1. Add in-memory cache to clients to not request so many times.
2. Store same data in multiple keys and randomize requests to spread across cluster. Multiple keys are created by appending something to the original key.
3. Add read replica and dynamically scale with load.