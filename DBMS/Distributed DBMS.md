Nodes are far from each other. Need to communicate over public network which can have network error.

#### Architectures

1. Shared Everything: 1 node with HDD, RAM, and CPU.
2. Shared Nothing: each node has HDD, RAM, and CPU. Communicate over network. E.g. Teradata
    a. Better efficiency and performance.
    b. Harder to scale capacity and ensure consistency.
3. Shared Disk: each node has RAM, and CPU. Communicate over network. HDD is shared. Popular in cloud.
4. Shared Memory: each node has a CPU. HDD and RAM is shared. Communicate over network. Mostly only in HPC etc, not in DBMS.

**Shared Disk**

Scale execution layer independently of storage layer.
Facilitates data lakes and serverless systems - can decommission execution nodes when not needed, and execution nodes become stateless storing no data.

#### Design Decisions

**Homogenous vs Heterogenous**

*Homogenous Nodes*: Every node in the cluster can perform the same set of tasks (on potentially different partitions of data). Makes provisioning and failover easier. Cassandra and other key value stores.
*Heterogenous Nodes*: Nodes are assigned specific tasks. Can allow a single physical node to host multiple "virtual" node types for dedicated tasks. More common on cloud services like Snowflake, Databricks etc. Can have coordinator nodes, catalog (metadata) nodes etc.

**Data Transparency**

Application should not be required to know where the data is stored in a distributed system. Query on single node DBMS should produce the same result as in distributed DBMS.

**Database partitioning**

Called *sharding* in NoSQL. DBMS can partition the DB physically (shared nothing) or logically (shared disk).

*Naive Table Partitioning* Assign entire table to a single node and assume that each node has enough storage for the entire table. Ideal if queries do not join, and access patterns are uniform. MongoDB

*Vertical Partitioning* Split table's attributes into separate partitions (for e.g. three columns in one node, two on another). Must store tuple information to reconstruct the tuple.

*Horizontal Partitioning* (Sharding is generally this) Split table's tuples based on partitioning key /scheme. Use hashing / ranges / predicates / round robin. In shared disk all data is kept together, but each node is responsible for some partition.

* *Hashing*: Hash a (few) column (partition key) mod number of nodes. Needs much more re-balancing.
* *Consistent Hashing*: Lesser re balancing. Only the range whose assigned node has changed needs to be rebalanced. For replication, copy data in n next nodes (next in range).

**Single node vs distributed**

A *Single node* txn accesses data only on one partition.
A *distributed* txn accesses data on multiple partitions. Requires expensive coordination.

**Transaction coordination**

Required to support distributed txn.
*Centralized* Global "traffic cop"
*Decentralized* Nodes organize themselves

Generally a hybrid. Periodically elect some temporary coordinator node.

**Centralized Coordinator**

Application server -> Coordinator (coordinator locks data on multiple partitions) -> returns ack to server
Application server does txn on different partitions
Application server sends commit request to coordinator, which asks partitions if it is safe to commit. Partitions acks back, and then coordinator acks back.

**Centralized Coordinator with middleware**

Server does not talk directly to nodes only talks to middleware.
Server -> Middleware (query). Middleware gets the query executed between nodes, and acks back to server.

**Federated DB**

DB is split across disparate DBMS. Server talks to middleware, which then talks to individual DBMS.
Nobody does it well. Lot of data copying.

### Decentralized coordinator

Server contacts the leader node first. 
Then does txns with other nodes. (App sends queries directly to individual nodes).
Then it contacts the leader to commit. 
Leader checks with everyone if it is safe to commit. 
And then leader acks.


*Assumption* No node is nefarious. Ow *Byzantine Fault Tolerant* systems are needed (blockchain).

#### Replication

DBMS can replicate a database across redundant nodes to increase availability.
Can do whether partitioned or not. Can off load read queries.

**Replica Configurations**

*Primary Replica* (Most common) *Leader / Follower* or  *Master / Slave*
* All updates go to a designated primary for each object.
* The primary propagates updates to its replicas without an atomic commit protocol.
* Read only txns are allowed to read replicas. Might not be the latest version.
* If the primary goes down, then hold an election to select a new primary.

Can use write ahead logs.

*Multi-Primary*
* Txns can update data objects at any replica.
* Replicas must synchronize with each other using an
atomic commit protocol.

**K safety**

K represents the number of replicas per
data object that must always be available.

If the number of replicas goes below this
threshold, then the DBMS halts execution and takes itself offline.

**Propagation Scheme**

When a txn commits on a replicated database, the DBMS decides whether it must wait for that txn's changes to propagate to other nodes before it can send the acknowledgement to application.

*Strong consistency* / *Synchronous*
All replicas must be updated before acknowledgement.

*Eventual consistency* / Asynchronous*
Ack might be sent before all replicas updated.

The node who gets the update request, flushes it, sends update request to replicas and returns acks (some replicas might not have acked).

**Propagation Timing**

*Continuous* 
DBMS sends log messages immediately as it
generates them. Also need to send commit / abort message. Common.

*On commit*
DBMS only sends the log messages for a txn to the replicas once the txn commits.
Do not waste time sending log records for aborted txns.
Assumes that a txn's log records fits entirely in memory.

**Active vs Passive**

*Active-Active* 
txn executes at each replica independently. Need to check at the end whether the txn ends up with
the same result at each replica.

*Active-Passive*
txn executes at a single location and propagates the changes to the replica.
Can either do physical or logical replication. Common.

#### Atomic Commit Protocol

Coordinating the commit order of txns across nodes in a distributed DBMS.

**Two Phase Commit**

1. App sends request to Node 1 (Coordinator). 
2. Phase 1: Coordinator sends prepare request to all participants. Participants ack back.
3. Phase 2: Coordinator sends commit request to all participants. Participants ack back.
4. Coordinator responds success to app.

When coordinator crashes: system becomes unavailable. Participants wait till timeout and then abort.

When participant crashes: After timeout, coordinator assumes this was an abort.

Early ack optimization: Coordinator sends ack to app after first phase success.

**PAXOS**

A newer improved version is *Raft*.

If majority of participants ack, then coordinator considers it acked. The minority have to crash.

Does not block if a majority of participants are available and has provably minimal message delays in the best case.

Often single leader is selected for some time. The system periodically renews the leader (known as a *lease*) using another Paxos round.

#### CAP Theorem

A distributed database cannot always be:
1. Consistent: App receives ack only after all replicas have been updated. No intermediate states are visible.
2. Always Available: Data is available to app no matter how many nodes go down (as long as network between nodes are alive).
3. Network Partition Tolerant: PAXOS is will not be consistent when network fails between nodes. Each node can become leader and lead to inconsistent data.

*PACELC* includes consistency vs latency trade offs:
1. Partition Tolerant
2. Always available
3. Consistent
4. Else, choose during normal operations
5. Latency
6. Consistency

For strong consistency, need acks from all, which suffers due to latency delays.

For OLTP, distributed RDBMS prefer strong consistency over availability. While NoSQL prefer availability while losing multi-node consistency.