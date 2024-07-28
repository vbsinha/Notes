## Database

UI -> Backend -> (JDBC) -> Database

Establish connection to database and run SQL without caring about db specific details.

```java
Connection conn = DriverManager.getConnection(url, username, password);
Statement stat = conn.createStatement();
String command = "UPDATE Books"
   + " SET Price = Price - 5.00"
   + " WHERE Title NOT LIKE '%Introduction%'";
stat.executeUpdate(command);
stat.close();
conn.close();
```

```java
ResultSet rs = stat.executeQuery("SELECT * FROM Books");
while (rs.next())
{
   look at a row of the result set
}
rs.close();
```

`SQLException`

Query path is decided by the prepared statement, recomputation is not required after setting params.
```java
String publisherQuery
   = "SELECT Books.Price, Books.Title"
   + " FROM Books, Publishers"
   + " WHERE Books.Publisher_Id = Publishers.Publisher_Id AND Publishers.Name = ?";
PreparedStatement stat = conn.prepareStatement(publisherQuery);
stat.setString(1, publisher);
```

BLOB: Binary Large Object
CLOB: Character Large Object