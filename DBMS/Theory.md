**Storage Hierarchy**

1. CPU Registers
2. CPU Caches
3. DRAM --- Volatile (Byte addressible)
4. SSD --- Non Volatile (Block addressible)
5. HDD
6. Network Storage (S3 etc.)

1. L1 (1 ns)
2. L2 (4 ns)
3. DRAM (100 ns)
4. SSD (16,000 ns)
5. HDD (2,000,000 ns)
6. Network Storage (~50,000,000 ns)

Sequential reads/writes are faster than Random.

### Data Storage On Disk

*Storage Manager* is responsible for maintaining a database's file.  The files are stored as a collection of pages.

A *page* is a fixed-size block of data. May contain tuples, metadata, indexes, logs etc.
Pages do not mix data - a page will only data about one object / index etc.
DBMS has an indirection layer to map page IDs to physical locations.

Three types of pages:
1. Hardware Page (4 kB) - largest block of data OS writes atomically
2. OS Page (4 kB) pages at the OS level
3. Database Page (512 B - 32 kB)

#### Page Storage Architecture

Different ways DBMS manage pages in files on disk.

1. Heap File Organization (Most common)
2. Tree File Organization
3. Hashing File Organization

A *heap file* is an unordered collection of pages with tuples that are stored in random order.
* Easy to find pages if the database is stored as a single file.
* E.g.: File 1: Page 1, 2, 3, ... File 2: Page 1, 2, 3, ...
* Special pages that tracks the location of data pages in the database files.

#### Page Layout

Every page contains a header of metadata: Page size, checksum, DBMS version, schema information, compression / encoding metadata, data summary / sketches etc.

Row oriented storage model for tuples:
1. Tuple oriented storage
2. Log structured storage
3. Index organized storage

**Tuple oriented storage - Slotted Pages**

E.g. PostGre, Oracle

Page:
Header
Slot array:
    Position of Tuple 1
    Position of Tuple 2
    ... Grows down

... Grows up
Tuple 2
Tuple 1

Takes care of variable length tuples. While deleting tuples - upper records do not need to be slided but can be.


```SQL
-- In PostGRE
-- (1,a), (2,b), (3,c) was inserted, and then (2,b) was deleted
SELECT r.ctid, r.* FROM r;

ctid | id | val
---------------
(0,1)| 1  | a
(0,3)| 2  | c

VACCUM FULL r;
ctid | id | val
---------------
(0,1)| 1  | a
(0,2)| 2  | c
```

*Problems of Tuple oriented storage*

1. FRagmentation: Page is not fully utilized.
2. Useless Disk I/O: Entire page to update a tuple.
3. Random Disk I/O: Updating multiple tuples on separate pages will be expensive.
4. Some object stores, HDFS etc. do not allow page overwrites, only creating new page is allowed.

**Log Structured Storage**

E.g. RocksDB (Facebook - Fork of LevelDB), LevelDB (Google)

Only allowed Operations: PUT & DELETE. Each log record has tuple's unique identifier.

`PUT #103 {val=a1}`, `DEL #102` (`a1` encapsulates all columns)

Keep appending log entries to in-memory buffer and then sequentially write changes to disk.

Faster writes, slower reads.
For reads: scan newest to oldest logs. Or maintain an index from tuple id to location of record.

DBMS will periodically compact pages, keep only latest data.
After compaction, each id appears only once in a page, so the page can be sorted of efficient look-ups. Called *Sorted String Tables* (SSTables). Embed indexes in header to reduce search time.

*Universal Compaction* (RocksDB)
File1 File2 File3 File4
Compacted12 File3 File4
Compacted123 File4

*Level Compaction* (LevelDB)
Level1: File1 File2 File3 File4
Level2: Compacted12 Compacted34
Level3: Compacted1234

*Problems of Log oriented storage*

1. Reads are slower.
2. Compaction is expensive.
3. Write Amplification - For every logical write of record in an application how many writes actually happen. Can be a lot due to compaction.

**Index Organized Storage**

E.g. SQLite, can be turned on in Oracle etc.

Both log and record structured storage rely on index to figure out where to find tuple.

Assuming the index is B-Tree, log and record storage leaf nodes of B Tree store location of tuple. In index organized storage, leaf nodes itself store the data. The leaf look like slotted pages, but within page tuples are sorted.

**Tuple Layout**

Header | Attribute Data
Does not store table metadata
DBMS interprets the bytes with appropriate types.
All attributes should be word aligned.
Approaches for Word Alignment:
1. Padding: Pad with 0.
2. Reordering: Reorder columns to make them better aligned, might still need padding.

Varchar / Varbinary / Text / Blob: Header with length, followed by data bytes OR pointer to another page / offset with data.
Timestamps etc. Micro / Milli sec since Jan 1, 1970 in GMT.
Numeric / Decimal: No rounding error 

Null value approaches:
1. Header stores a bitmap which attribute is NULL
2. Special value in type's range is designated as NULL
3. Per attribute NULL flag (bad choice) - bad for word alignment

### Database workloads

1. *On-Line Transaction Processing (OLTP)*: Read / Update a small amount of data
2. *On-Line Analytical Processing (OLAP)*: Queries to read a lot of data to compute aggregates
3. *Hybrid Transaction + Analytical Processing (HTAP)*: OLTP + OLAP on same db instance  

Column Store system is used in OLAP.

*Storage Model* specifies how tuples are organized on disk.

**N-ary Storage Model (NSM)**

Row Stores - (almost) all attributes of a single tuple stored contiguously in a single page. Ideal for OLTP.

*Disadvantages*
1. Not good fo scanning large portions of the table and / or a subset of attributes.
2. Terrible memory locality - within a tuple, need to jump to a particular attribute.
3. Not ideal for compression because of multiple value domains in a page.

**Decomposition Storage Model (DSM)**

Column Store - Store a single attribute for all tuples contiguously. Ideal for OLAP. DBMS is responsible for combining / splitting attributes. SQL remains same.

Store attributes and metadata in separate arrays of *fixed length*. Identify tuples using offset. Maintain a separate file per attribute for metadata for the column.

Separate handling for variable length attributes.

*Page*:
header
null bitmap
R1C1
R2C1
...

Might not have usual indexes

*Tuple Identification*

1. Fixed-length offset: For every attribute same about of space is reserved. Tuples can be identified by offsets. Common approach.
* Variable length attributes uses *dictionary compression*.
2. Embedded Tuple Ids: Each value is stored with id. Old approach. Maintain index for (row, column).

Columnar File format: Parquet, ORC. (Actually PAX)

*Disadvantages*

1. Slow for point queries, inserts, updates and deletes.

**Hybrid Storage Model (PAX)**

Partition rows into groups. Then partition their attributes into columns and store columns contiguously. 

*PAX file*:
header
Row Group:
header (might be stored as footer due to file immutability)
R1C1 R2C1 R3C1
R1C2 R2C2 R3C2
Row Group:
header
R4C1 R5C1 R6C1
R4C2 R5C2 R6C2

#### Database Compression

Goals:
1. Produce fixed-length values - except for var char stored in separate pool.
2. Postpone decompression during query execution.
3. Lossless

Granularity:
1. Block-level: Compress block of tuples. Zip each file. DB cannot make sense of zipped directly.
2. Tuple-level: Compress tuple (NSM only)
3. Attribute level: Single attribute within tuple.
4. Column-level: DSM-only