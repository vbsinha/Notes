1. Use `UNION ALL` instead of `UNION` wherever possible
2. Use `UNION` where instead of `WHERE` ... or ... query. When using `UNION` each subquery can use the index of its search, and then combine result. Often works when condition involves scan on two different columns.
3. `LENGTH()` measures length in bytes - can count more than one byte per char, while `CHAR_LENGTH()` measures length in characters.
4. Generally default `JOIN` is `INNER JOIN`.
5. Join ON conditions can be more complex than just `=`. For example `W1.date = W2.date - 1`, `DATEDIFF(W2.date, W1.date) = 1`.
6. Divison will be integer division if both operators as integers. Use `CAST(col AS DECIMAL(6,2))`
7. To use a group by on a field after applying a function to it, define the field in `SELECT`. Applying function with `AS` in `GROUP BY` will not work.
```sql
// TO_CHAR converts date trans_date to string of 'YYYY-MM'
SELECT TO_CHAR(trans_date, 'YYYY-MM') AS month,  COUNT(*) AS trans_count
FROM Transactions
GROUP BY month
```


**Useful Functions**
1. `IFNULL(col, 0)`
2. `COALESCE(col, 2, ...)` returns the first non null value in the list
3. `ROUND(val, 3)` precision `3` - 3 digits after decimal
4. `COUNT(colname)` gives the count of rows with non null values
5. `IF(condition, true value, false value)` (not available in Postgre)
```sql
-- In PL/pgSQL
IF condition THEN
    statement
ELSIF condition THEN
    statement
ELSE
    statement
END IF
```
6. `BETWEEN (CAST('2019-07-27' AS DATE) - INTERVAL '29' DAY) AND '2019-07-27'`