**Triggers**

```sql
CREATE TRIGGER trigger_name AFTER UPDATE OF column_name ON (table_name)
REFERENCING NEW ROW AS nrow
REFERENCING OLD ROW AS orow
...
```

Disable triggers before bulk loading data, replicating data etc.

**Transactions**

1. When interacting directly with db (for e.g. using CLI), start transactions, and run SQLs. Rollback if something goes wrong.
2. Intermediate states between transactions are not visible to concurrent users. Transactions are *atomic*. PostgreSQL treats every individual statement as being executed in a transaction.
```sql
BEGIN;
-- sql statements
COMMIT; -- ROLLBACK;
```
3. Savepoints allow to selectively discard parts of transactions. Everything is inside transaction so none of the sps are visible to concurrent requests, visible only after committing.
```sql
BEGIN;
-- stmt
SAVEPOINT mysp;
-- stmt
ROLLBACK TO mtsp;
-- stmt
ROLLBACK TO mtsp; -- sp continues to be defined even after ROLLBACK
-- Rolling back to a sp will release all sps defined after it
-- If sp is no longer, it can be released - will release all sps defined after it
-- stmt
COMMIT;
```

**Database Partitioning**
[TODO](https://www.postgresql.org/docs/16/ddl-partitioning.html)

**Indexes**

`CREATE INDEX index_name ON tablename(columnn1, column2)`
`CREATE INDEX index_name ON tablename(columnn1 DESC NULLS LAST)`

1. Run `ANALYZE` command regularly for database to regularly update statistics.
2. Indexes are used in queries (where), joins, updates, and deletes.
3. By default PostgreSQL creates B-Tree based indexing.
4. B-Tree based indexing supports `<`, `<=`, `=`, `=>`, `>`, `LIKE word%` (not `%word`), `BETWEEN`, `IN`, `IS [NOT] NULL`, `ORDER BY` [with `LIMIT`].
5. Hash based indexing supports only `=`. In PostgreSQL does not support multi-column index.
5. PostgreSQL: In multi-column index, equality constraints on leading column, inequality constraint on the first column that does not have equality will be used. `(a,b,c)` `WHERE a = 5 AND b >= 42 AND c < 77` will use index on `a` and `b`.
6. Indexes with three or more column is unlikely to be useful. Prefer one column indexes.
8. Database can combine multiple indexes to produce result. Index on `(a,b)` is useful for `a = 1 AND b = 2` but not for `a = 1 OR b = 2`.
9. Use `EXPLAIN` on individual queries to see if index is being used.

`CREATE UNIQUE INDEX index_name ON table(column)`
1. In PostgreSQL only B-Tree indexes can be declared unique.
2. By default, null values are considered distinct, allowing multiple nulls in unique column. `NULLS NOT DISTINCT` modifies this.
3. PostgreSQL automatically creates a unique index when a unique constraint or primary key is specified. Do not create index explicitly - that will duplicate.

`CREATE INDEX name ON people ((first_name || ' ' || last_name))` index on expression

Partial indexes can also be setup which index only if condition is satisfied.

All indexes in PostgreSQL are secondary indexes: each index is stored separately from table's main data.