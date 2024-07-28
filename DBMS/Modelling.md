* *Super Key* set of attributes that uniquely identify a row in a table.
* *Candidate Key* Super Key that cannot be reduced to simpler super key by removing attributes.
* Many to many relationships have to be decomposed into one to many.

#### Normalization

* De normalized schemas can be used for better performance.
* Higher normalizations cannot be achieved until lower are satisfied.

**1 NF** 
A field may not contain a set of values or a nested record. Do not store arrays in fields - create another table to model the one to many relationship.

**2 NF**
Every non candidate key must depend on the whole of the candidate key not just a part of it.

Book table: (Title, Format (Ebook / Hardcover), Price, Author) has candidate key (Title, Format), but Author depends only on Title. Break table into two (Title, Author), and (Title (as FK), Format, Price).

**3 NF**
Eliminate transitive functional dependencies. 

Book table: (Title, Author, Author Country, Publisher, Publisher Country) should be decomposed to (Title, Author, Publisher), (Author, Author Country), (Publisher, Publisher Country)

**BCNF**
Table in 3 NF is almost always in BCNF.
TODO: When is it not?

Higher forms of normalization is of academic interest mostly.

#### References
1. [Database normalization wikipedia](https://en.wikipedia.org/wiki/Database_normalization)