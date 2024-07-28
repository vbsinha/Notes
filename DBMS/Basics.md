**Data Definition Language** Notation for defining 
* db schema with type of attribute
* integrity constraints
* indices
* authorization - who can access what
* physical storage structure on disk

**Types**
1. `char(n)` fixed length char string of size `n`
2. `varchar(n)` char string of max size `n`
3. `int` / `smallint`
4. `numeric(p,d)` total `p` precision including `d` decimal digit (like `44.5` for `numeric(3, 1)`)
5. `date` / `time` / `timestamp` / `interval`
6. `blob` (binary large object)
7. `clob` (character large object)
8. arrays
9. JSON 

PostgreSQL provides enum, XML, UUID, types of network artifacts like macadd etc., geometric types, composite types etc. too.

`null` valid value for all fields.
Queries return pointer to blobs / clobs instead of data
Latest SQL standards have support to query within JSON

`CREATE TYPE dollars AS NUMERIC(12, 2) FINAL`

```sql
CREATE DOMAIN degree_level varchar(10)
CONSTRAINT degree_level_test
CHECK (VALUE IN ('Bachelors', 'Masters', 'Doctorate'));
```

```sql
CREATE TABLE instructor (
    id          char(5),
    name        varchar(20) not null,
    dept_id     varchar(20),
    salary      numeric(8,2)
    primary key (ID)
    foreign key (dept_id) references department
)

CREATE INDEX ins_index ON instructor (name)
```

DBMS may provide auto-generated id fields too.

**Data Manipulation Language** Language for accessing and updating data. Contains query language.

```sql
INSERT INTO instructor VALUES ('1011', 'Smith', 'BIOID1', '234.32')

insert into course (course_id, title, dept_name, credits)
values ('CS-437', 'Database Systems', 'Comp. Sci.', 4);

INSERT INTO table SELECT ... FROM ... WHERE ...
```

```sql
UPDATE instructor
SET salary = salary * 1.05
WHERE salary < (SELECT avg (salary)
                FROM instructor);

update instructor
set salary = case
                when salary <= 100000 then salary * 1.05
                else salary * 1.03
             end
```


```sql
drop table r

alter table T add A D 
-- A attribute, D type, column filled with nulls

alter table T drop A
-- Many DBMS do not support dropping columns
```

`DELETE FROM student WHERE ...` `DELETE` semantics similar to `SQL` without `SELECT`

**SQL** (Structured Query Language) - declarative case-insensitive DML.

1. `DISTINCT` (`ALL` is the opposite)
```sql
SELECT DISTINCT dept_name
FROM instructor

SELECT ALL dept_name
FROM instructor
```

2. `*` for all columns

3. Attribute can be literal. `AS`. Attributes can have expressions.
```sql
SELECT '422' AS FOO -- Table with one row and column

SELECT 'A' FROM dept -- Table with no. of rows = dept's rowcount

SELECT salary/12 AS monthly_salary FROM ...
```

4. String operations (can be used in `SELECT` and `WHERE`):
  * `LIKE`: `%` matches any string 
  * `_` matches any char
  * `SUBSTRING(name, 1, 5)` 
  * `UPPER(name)`
  * `||` concatenates according to standard - `+` / `CONCAT` in `MySQL`

5. `ORDER BY col1 ASC, col2 DESC`. `ORDER BY colname DESC NULLS [FIRST | LAST]`

6. `UNION`, `INTERSECT`, `EXCEPT`, `UNION ALL`
```sql
(select course_id from section where sem = 'Fall')
UNION -- or INTERSECT / EXCEPT
(select course_id from section where sem = 'Spring')
```

7. `IS NULL`, `IS NOT NULL`: `SELECT * FROM dept WHERE name IS NULL`

8. Aggregates: `COUNT`, `SUM`, `MAX`, `MIN`, `AVG`: `SELECT COUNT(DISTINCT id) FROM ...`. `COUNT`, `AVG`, `SUM` support `DISTINCT` (except `COUNT(*)`)

9. Aggregate with `GROUP BY`. Attributes not in aggregate functions must appear in `GROUP BY`. `HAVING` is applied after from groups, `WHERE` is applied before forming groups.
```sql
SELECT AVG(salary) as avg_salary, dept_name
FROM instructor
GROUP BY dept_name
HAVING AVG(salary) > 42000

-- multi aggregates
SELECT AVG(gpa), COUNT(sid)
FROM student
```

10. `LIMIT` / `OFFSET` / `FETCH {FIRST|NEXT}` 
`SELECT sid, name FROM student WHERE login LIKE '%@cs' LIMIT 20 OFFSET 10;`

**Subqueries**

```sql
SELECT A1, A2 -- As can be subquery that returns a single value
FROM R1, R2 -- Rs can be valid subquery
WHERE P -- can be of the form "B <op> subquery" where B is attribute
```

*Subqueries in `WHERE`*

1. `IN` / `NOT IN`
```sql
SELECT DISTINCT course_id
FROM section
WHERE semester = 'Fall' 
  AND year = 2017 
  AND course_id IN (SELECT course_id
              FROM section
              WHERE semester = 'Spring' AND year = 2018);
```

2. `SOME` / `ALL`
```sql
select name
from instructor
where salary > some (select salary
                     from instructor
                     where dept name = 'Biology');
```

3. `EXISTS` / `NOT EXISTS`
```sql
select course_id
from section as S
where semester = 'Fall' and year = 2017 and
exists (select *
        from section as T
        where semester = 'Spring' and year= 2018
        and S.course_id = T.course_id);


-- All students who have taken all bio courses
select distinct S.ID, S.name
from student as S
where not exists ( (select course_id
                    from course
                    where dept_name = 'Biology')
                  except
                    (select T.course_id
                     from takes as T
                     where S.ID = T.ID));
```

4. `UNIQUE`: True if subquery has no duplicate tuples
```sql
-- All courses offered at most once in 2017
select T.course_id
from course as T
where unique ( select R.course_id
               from section as R
               where T.course_id = R.course_id
               and R.year = 2017);
```

*Subqueries in `FROM`*

1. Allows us to circumvent `HAVING`

```sql
select dept_name, avg_salary
from (  select dept_name, avg (salary) as avg_salary
        from instructor
        group by dept_name)
where avg_salary > 42000;

select dept_name, avg_salary
from (  select dept_name, avg (salary)
        from instructor
        group by dept_name)
        as dept_avg (dept_name, avg_salary)
where avg_salary > 42000;
```

2. `WITH` gives temporary tables (Common Table Expression)
```sql
-- WITH max_budget AS (works without column names too)
WITH max_budget (value) AS -- WITH ctename (col1)
    (SELECT MAX(budget)
    FROM department)
SELECT department.name
FROM department, max_budget
WHERE department.budget = max_budget.value;
```

*Subquery in `SELECT`*

*Scalar* subquery: query which returns 1 value

Runtime error if more than >1 result tuple is returned by subquery in `SELECT`.

```sql
select dept_name,
    (select count(*)
     from instructor
     where department.dept_name = instructor.dept_name)
    as num_instructors
from department;
```

**Joins**

Join type determines how records form one table that are not matched to another are treated.

| course_id | title|
|-----------|------|
| 1         | CS   |
| 2         | MA   |

| course_id | prereq_id |
|-----------|-----------|
| 1         | 3         |
| 4         | 5         |

*Inner Join*

```sql
course INNER JOIN prereq ON
  course.course_id == prereq.course_id
```

| course_id | title| course_id | prereq_id |
|-----------|------|-----------|-----------|
| 1         | CS   | 1         | 3         |

*Left Outer Join*

```sql
course LEFT OUTER JOIN prereq ON
  course.course_id == prereq.course_id
```

| course_id | title| course_id | prereq_id |
|-----------|------|-----------|-----------|
| 1         | CS   | 1         | 3         |
| 2         | MA   | NULL      | NULL      |


*Right Outer Join*

```sql
course NATURAL RIGHT OUTER JOIN prereq
```

| course_id | title| prereq_id |
|-----------|------|-----------|
| 1         | CS   | 3         |
| 4         | NULL | 5         |

*Full Outer Join*

```sql
course FULL OUTER JOIN prereq USING (course_id)
```

| course_id | title| prereq_id |
|-----------|------|-----------|
| 1         | CS   | 3         |
| 2         | MA   | NULL      |
| 4         | NULL | 5         |


*Natural Join*

Matches tuples with same values for all common attributes and retains one copy of common attributes

```sql
SELECT name, course_id
FROM student NATURAL JOIN takes; -- default JOIN is often INNER JOIN
```

*Cartesian Product / Cross Join*

```sql
SELECT ...
FROM table1, table2
...

SELECT ...
FROM table1 CROSS JOIN table2 ON ...
....
```

**Lateral Join**
`LATERAL` allows a nested query to reference attributes in other nested queries that precede it.
Like a for loop that allows you to invoke another query for each tuple in a table.

```sql
-- number of students in each course and their gpa
SELECT * FROM course AS c
LATERAL (SELECT COUNT(*) AS cnt FROM enrolled
          WHERE enrolled.cid = c.cid) AS t1,
LATERAL (SELECT AVG(gpa) AS avg FROM student AS s
          JOIN enrolled AS e ON s.sid = e.sid
          WHERE e.cid = c.cid) AS t2;
```

**View**

Mechanism to hide data - virtual relation

`CREATE VIEW v AS <query>` Does not evaluate the query immediately, just defines `v` which can be used later. 

A view can depend on another view and can be recursive.

**Materialized View**

View relations that are physically stored. 

If original relations are updated materialized views become out of date - need to maintain view.

DBMS may allow updates on view directly if view is simple: `FROM` has one relation, no aggregates, `DISTINCT`, `GROUP BY`, `HAVING`, etc., attributes not in `SELECT` are nullable, etc.

**Transaction** Unit of work: sequence of query / updates. Must end with *commit* or *rollback*. *Atomic*

**Data Integrity**
1. `NOT NULL` (`name varchar(20) NOT NULL`)
2. primary key
3. unique `UNIQUE(col1, col2)` - candidate key
4. check(P) `CHECK(time_slot_id IN (SELECT id FROM time_slot))`

`CREATE ASSERTION <assertion-name> CHECK <predicate>`

**Referential Integrity**
`FOREIGN KEY (dept_name) REFERENCES department` - by default refers to PK of `department`
```sql
FOREIGN KEY (dept_name) REFERENCES department (dept_name) (
  ON UPDATE CASCADE, -- OR SET NULL / SET DEFAULT / default - reject update
  ON DELETE CASCADE
)
```

**Output Redirection**

```sql
-- Syntax various with DBMS
SELECT DISTINCT cid INTO course_ids
FROM enrolled
```

**Window Function**

Conceptual execution:
1. Partition data
2. Sort each partition
3. for each partition create window
4. compute answer for each window

`ROW_NUMBER()`, `RANK()`, aggregation functions
```sql
-- Student with second highest grade in each course
SELECT * FROM (
  SELECT *, RANK(PARTITION BY course_id ORDER BY grade ASC) AS rank 
  FROM enrolled) AS ranking
WHERE ranking.rank = 2
```

`RANK` may leave gaps (2 people can get rank 1). `DENSE_RANK` does not leave gaps.

```sql
SELECT date, SUM(value) OVER
(ORDER BY date BETWEEN ROWS 1 PRECEDING AND 1 FOLLOWING)
FROM sales
```

```sql
-- Window examples
BETWEEN ROWS UNBOUNDED PRECEDING AND CURRENT
ROWS UNBOUNDED PRECEDING
RANGE BETWEEN 10 PRECEDING AND CURRENT ROW
RANGE INTERVAL 10 DAY PRECEDING
```

Window functions work with `PARTITION` too.

```sql
-- For each transaction show the balance after transaction
SELECT account_number, date_time,
  SUM (value) OVER
    (PARTITION BY account_number
    ORDER BY date_time
    ROWS UNBOUNDED PRECEDING)
  AS balance
FROM transaction
ORDER BY account_number, date_time
```

**Three valued logic**

`TRUE`, `FALSE`, `UNKNOWN`

If `NULL` affects the result of a logical expression `UNKNOWN` is returned since `NULL` means absent data.

*Comparison to NULL*
Examples of expression returning `UNKNOWN`:
1. `NULL = 1`
2. `NULL <> 1`
3. `NULL > 1`
4. `NULL = NULL`. Nothing equals `NULL`, not even `NULL`.

Use `IS NULL` / `IS NOT NULL` predicate in where clause. 

`IS [NOT] DISTINCT FROM` returns `TRUE` when comparing `NULL` with `NULL`. 
`IS NOT DISTINCT FROM` is same as
```sql
CASE WHEN (a = b) OR (a IS NULL AND b IS NULL)
  THEN TRUE
  ELSE FALSE
END
```

*Logical operations with UNKNOWN*

Replace each `NULL` with `random()`. If the result can change based on the random value returned value is `UNKNOWN`, otherwise `TRUE` or `FALSE`.

`(NULL = 1) OR (2 = 2)` is `TRUE` not `UNKNOWN`.

*Rules for `WHERE`, `HAVING`, `WHEN`*

`WHERE`, `HAVING`, `WHEN` require `TRUE` condition. (Not `FALSE` is not enough).

`(col = NULL) OR NOT (col = NULL)` evaluates to `UNKNOWN`

`WHERE 1 NOT IN(NULL)` is `UNKNOWN` (replace `NULL` by `random()`). `NOT IN (NULL, ...)` never returns `TRUE`.

*`CHECK` constraints*

`CHECK` rejects `FALSE` allows `TRUE` and `UNKNOWN` 

```sql
-- a can have a value > 10 if b is NULL
CREATE TABLE t (
    a NUMERIC CHECK (a >= 0),
    b NUMERIC CHECK (b >= 0),
    CHECK ( a + b <= 10 )
)
```

*`IS [NOT] (TRUE|FALSE|UNKNOWN)`*  never returns `UNKNOWN`

**Recursive Queries**

TODO

**References**
1. [Three valued logic](https://modern-sql.com/concept/three-valued-logic)