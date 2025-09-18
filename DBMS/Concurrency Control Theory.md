**Transaction**: BEGIN, COMMIT / ABORT

**ACID**

1. *Atomicity*: All actions in txn happen, or nothing happens.
2. *Consistency*: If each txn is consistent, and the DB starts consistent, then it ends up consistent. Consistent means constraints, checks, integrity constraints are maintained.
3. *Isolation*: Execution of one txn is isolated from that of other txns. Give an illusion that each txn is executing sequentially.
4. *Durability*: If a txn commits, its effect persists. Use non volatile memory.

**Atomicity**

*Logging Approach*
Log all actions to undo. Undo records are stored both in memory and disk.

Logging is used by almost every DBMS: Audit Trail

*Shadow Paging*
Old approach. Make copies of pages and txns and make changes to copies. Only make it visible after commit.

**Consistency**

DBMS ensures all integrity constraints are true before and after transaction.

Nothing special: ensure AID, consistency ensured.

*Eventual Consistency*
Eventual effect of all transactions will take place. No guarantee of when the effect will appear.
Move away from such model.

**Isolation**

Each txn executes as if it were running by itself.

*Concurrency Control* protocol to decide proper interleaving of operations.

1. Pessimistic: Don't let problems arise.
2. Optimistic: Assume rare conflicts, deal when they happen.

The execution schedule should produce result equivalent to some serial execution of the transactions.

*Conflicts*
1. Read-Write
```
Unrepeatable Read
T1          T2
BEGIN
R(A) 10
            BEGIN
            R(A) 10
            W(A) 19
            COMMIT
R(A) 19 <- Should have been 10
COMMIT
```
2. Write-Read
```
Write-Read (Dirty Read)
T1          T2
BEGIN      
R(A) 10    
W(A) 12
            BEGIN
            R(A) 12 <- Should not have used 12
            W(A) 14
            COMMIT
ABORT
```
3. Write-Write 
```
Overwrite uncommitted txn
T1          T2
BEGIN
W(A) 10
            BEGIN
            W(A) 19
            W(B) Bob
            COMMIT
W(B) Alice <- Alice is associated with 19, should have been (Alice, 10), or (Bob, 19)
COMMIT

```
**Types of Serializability**

*Conflict Serializability* 
* Supported by Databases
* Two schedules are __conflict equivalent__ iff they involve same actions, and every pair of conflicting actions are ordered in the same way.

**Durability**

Use logging or shadow paging.

#### Locks (& Latches) - A Pessimistic approach to isolation

Locks protect database objects while latches protect in-memory data structures.

|             | Locks       | Latches       |
| ----------- | ----------- |---------------|
| Separates...| User txn    | Threads | 
| Protects...| DB contents     | In - memory data structures |
| During...| Entire txn|Critical section - programmer writes this|
| Modes...| Shared, Exclusive, Update, Intention | Read, Write |
| Deadlock | Detection & Resolution | Avoidance |
| "prevented by | Waits for, timeout, aborts | Coding discipline of programmer |
| Kept in | Lock manager | Protected Data Structure | 

*S-LOCK* (Shared lock) for reads, allows other shared locks to be held
*X-LOCK* (Exclusive lock) for wirtes, no other lock can be held

Lock manager keeps track of which txn holds the lock.
Application programmer generally does not lock / unlock manually, but they can give hints.

```sql
-- Not standard SQL, postgre syntax
LOCK TABLE t IN m MODE -- m is SHARE / EXCLUSIVE

SELECT * FROM t WHERE q FOR UPDATE; -- hint
```

**Two-phase locking**

Does not need to know all queries a txn will execute ahead of time.

Phase 1: Growing
Txn requests locks it needs. Lock mgr grants / denies.

Phase 2: Shrinking
Txn is only allowed to release locks. Cannot acquire new locks.

*Cascading locks problem*
```
T1              T2
BEGIN
X-LOCK(A)
X-LOCK(B)
R(A)
W(A)
UNLOCK(A)
                BEGIN
                X-LOCK(A)
                R(A) <- Incorrect value read, since T1 will get aborted
                W(A)
                ...
ABORT                            
```

*Solution*: *Strong Strict 2PL* or *Rigorous 2PL*. Release locks only at the end (COMMIT / ABORT)

*Problem*: Deadlock

*Solution 1*: Deadlock Detection. DBMS creates waits for graph (T1 is waiting for T2 to release a lock). When a cycle is detected, select a victim txn and either restart or abort (more common) it. Rollback txn completely or to a save-point. 

*Solution 2*: Deadlock Prevention. When a txn tries to acquire a lock held by another txn, database kills one of them. No waits for graph is needed.

*Wait-Die* (Old wait for young): If Requesting txn has higher priority than holding txn, requesting txn waits for holding txn. Ow requesting txn aborts.
*Wound-wait* (Young waits for old): If Requesting txn has higher priority than holding txn, holding txn aborts and releases lock. Ow requesting txn waits.

**Hierarchial locks** 

Locks at table, page, tuple, attribute (rare) level.

*Intention-Shared locks (IS)* Add IS locks at higher levels of tree and add S locks to lower level which you are reading. IS means I intend to read something in the subtree below.

*Intention-Exclusive locks (IX)* Similar to IS.

*Shared+Intention-Exclusive locks (SIX)* SIX (essentially S) lock entire subtree, X locks at lower levels (read all, update some)

#### Timestamp Ordering - Another pessimistic approach to isolation

Each txn T is assigned a unique timestamp TS(T) which is increasing. Txn read / write object without locks.

Every object X is tagged with timestamp of the last txn that successfully read / wrote it.
*W-TS(X)* - Write timestamp on X
*R-TS(X)* - Read timestamp on X

If a txn tries to access an object from the future, it is aborted and restarted.

If TS(T) < W-TS(X), abort T and restart with new TS.
Else read X, set R-TS(X) = max(R-TS(X), TS(T)), and make a local copy of X to ensure repeatable reads for X.

If TS(T) < min(R-TS(X), W-TS(X)), abort T and restart.
Else update X, W-TS(X), and make a local copy of X to ensure repeatable reads for X.

*Thomas Write Rule*
If TS(T) < R-TS(X), abort and restart T.
Else if TS(T) < W-TS(X), do not update X, but make a local copy of X to ensure repeatable read for T.
Else update X, W-TS(X).

No deadlock, but can cause starvation.
No DBMS uses basic T/O, but is used in OCC / MVCC.

Most txns are short lived, so locks cause much more overhead.

#### Optimistic Concurrency Control (OCC) - An optimistic approach to isolation

Each txn has a private workspace. An object read is copied to the workspace. Modifications are applied to the workspace. At commit, DBMS compares the write set to see if it conflicts with other transactions. If there are no conflicts, write set is installed into global database.

*Read Phase* Track read/write and store writes in private workspace.
*Validation Phase* Check for conflict with other txn
*Write Phase* If validation succeeds, ply changes to db. Ow abort and restart txn.

**Phantom Problem**

```
T1                      T2
BEGIN
SELECT COUNT(age)
FROM people
-- 99
                    BEGIN
                    INSERT INTO people (age=30)
                    COMMIT
SELECT COUNT(age)
FROM people
-- 100 
COMMIT
```

There are different isolation levels which can be specified in transactions. Highest to lowest:
1. Strict Serializable
2. Serializable
3. Repeatable Read
4. Read Committed
5. Read uncommitted

#### Multi version concurrency control

Writers do not block readers. Readers do not block writers.

### Reasons for failures

1. Logical error during transaction (integrity constraint violation)
2. Internal state error (deadlock etc.)
3. Software failure due to bugs in OS, DBMS
4. Hardware failure like power failure etc.
5. Storage Media Failure like failure during disk write etc.