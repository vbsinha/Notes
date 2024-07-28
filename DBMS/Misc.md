*Result Set* is the result of a query, a kind of in-memory table.

All database elements created via SQL schema statements are stored in a special set of tables called the *data dictionary*.

*SQL* is used for database manipulation and querying
*PL/SQL* extends the functionality with procedural programming capabilities:
* Allows writing functions which returns primitives as well as tables
* Can declare variables in method / loop over etc.
* Can call external methods written in other languages like C / Java.

*ORM (Object Relational Mapping)* mapping between objects and records
* Hibernate ORM for Java
* Django ORM for Python

1. Create an index on huge tables (>1.000.000) rows
2. Use EXIST() instead of COUNT() to find an element in the table
3. SELECT fields instead of using SELECT *
4. Avoid Subqueries in WHERE Clause
5. Avoid SELECT DISTINCT where possible
6. Use WHERE Clause instead of HAVING
7. Create joins with INNER JOIN (not WHERE)
8. Use LIMIT to sample query results
11. Run your query during off-peak hours
12. Avoid using OR in join queries
14. Choose GROUP BY over window functions
15. Use derived and temporary tables
16. Drop the index before loading bulk data
16. Use materialized views instead of views
17. Avoid != or <> (not equal) operator
18. Minimize the number of subqueries
19. Use INNER join as little as possible when you can get the same output using LEFT/RIGHT join.
20. Frequently try to use temporary sources to retrieve the same dataset.
21. Use Prepared Statement with user input.
22. Strive to compute answer in a single query.


**OLTP (On-Line Transactional Processing)**:
1. Supports large number of short CRUD, used in operation of a system
2. Fast query time, data integrity, and high transactions per second
3. 3NF
4. Requests like updating email of a user

**OLAP (On-Line Analytical Processing)**:
1. Historical / Archival data, often data source is OLTP
2. Low volume of queries, queries are complex with aggregations
3. Response time for queries important
4. Star schema
5. Used for decision making, planning etc.

[Link](https://www.datawarehouse4u.info/OLTP-vs-OLAP.html) for differences between OLAP and OLTP.

**General Advice / Information**
 
1. Cursor is in a sense a pointer to a row.
2. All tables should have a primary key except for special cases like table to store logs etc.
3. All tables should have a id field, except:
    * many-to-many link tables where primary key is composite.
    * primary key is natural like 2 letter state code which never change.
4. A PostgreSQL database cluster contains one or more named databases. A database contains one or more schemas, which in turn contain tables. Refer to as `database.schema.table`.
