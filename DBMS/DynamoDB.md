1. **Fully managed** AWS takes care of all operational tasks — hardware provisioning, configuration, patching, scaling etc.
2. **Highly scalable** Automatically scales up or down to adjust to your application's needs, without any downtime or performance degradation.
3. **Document DB**

### Data Model

1. **Tables** Top level data structure with mandatory primary key. Supports secondary indexes.
2. **Items** Corresponds to rows in relational model. Each item has a primary key and can support up to 400KB of data.
3. **Attributes** Key-value pairs that consist the data within item. Can have scalar type (strings, numbers, booleans) and set types (string sets, number sets). Attributes can also be nested, allowing for complex data structures within a single item.

DynamoDB is schema-less, no need to define a schema before inserting data. Items in the same table can have different sets of attributes, and new attributes can be added to items at any point without affecting existing items. Data validation has to be done at the application level.

Storage format of DynamoDB is proprietary, but interacts using JSON.

```json
// users table
{
  "PersonID": 101,
  "LastName": "Smith",
  "FirstName": "Fred",
  "Phone": "555-4321"
},
{
  "PersonID": 102,
  "LastName": "Jones",
  "FirstName": "Mary",
  "Address": {
    "Street": "123 Main",
    "City": "Anytown",
    "State": "OH",
    "ZIPCode": 12345
  }
},
{
  "PersonID": 103,
  "LastName": "Stephens",
  "FirstName": "Howard",
  "Address": {
    "Street": "123 Main",
    "City": "London",
    "PostalCode": "ER3 5K8"
  },
  "FavoriteColor": "Blue"
}
```

### Partition Key and Sort Key

1. **Partition Key** Attribute that, along with sort key (if present), uniquely identifies items in the table. 
* DynamoDB uses the hash of partition key to determine the partition where the data is stored.
* Consistent hashing is used.
2. **Sort Key** (Optional) Additional attribute that, when combined with the partition key, forms a composite primary key. 
* The sort key is used to order items with the same partition key value, enabling efficient range queries and sorting within a partition.
* B-Tree is used, which indexes sort key allowing efficient range queries and sorted intervals.

Primary Key = Partition Key + Sort Key
E.g. Partition Key: `chat_id` (per chat group), Sort Key: `timestamp`
### Secondary Indexes


|  | **GSI** | **LSI** | 
|-|-|-|
| Definition | Index with a different partition key than the main table| Index with the same partition key as the main table but a different sort key |   
| When to Use | Query on attributes that are not part of the primary key | Additional sort keys for querying within the same partition key |
| Size Restrictions | No size restrictions on items in the index | Limited to 10 GB per partition key |
| Throughput | Separate read/write capacity units from the base table | Shares the read/write capacity units of the base table |
| Consistency | Can be eventually consistent or strongly consistent | Always strongly consistent, same as the base table |
| Deletion | Deleting a GSI does not affect the base table items |Deleting an LSI is not possible without deleting the base table |
| Maximum Count | Up to 20 GSIs per table | Up to 5 LSIs per table |
| Use Case | Use GSI for global search across all partitions, such as searching by email in a user database |  Use LSI for local search within partitions, such as finding recent orders within a customer partition |

Secondary indexes are implemented as separate tables that are automatically maintained by the system:
1. **Global Secondary Indexes (GSIs)**:
* Each GSI is a separate table with its own partition scheme.
* When an item is added, updated, or deleted in the main table, DynamoDB asynchronously updates the GSI.
* GSIs use the same consistent hashing mechanism as the main table, but with different partition and sort keys.
* This allows for efficient querying on non-primary key attributes across all partitions.
2. **Local Secondary Indexes (LSIs)**:
* LSIs are co-located with the main table's partitions, sharing the same partition key.
* They maintain a separate B-tree structure within each partition, indexed on the LSI's sort key.
* Updates to LSIs are done synchronously with the main table updates, ensuring strong consistency.
3. **Index Maintenance**:
* DynamoDB automatically propagates changes from the main table to all secondary indexes.
* For GSIs, this propagation is eventually consistent, while for LSIs it's strongly consistent.
* The system manages the additional write capacity required for index updates.
4. **Query Processing**:
* When a query uses a secondary index, DynamoDB routes the query to the appropriate index table (for GSIs) or index structure (for LSIs).
It then uses the index's partition and sort key mechanics to efficiently retrieve the requested data.

### Accessing data

1. **Scan Operation** Read every item in table or index and return paginated. Avoid scans if possible.
2. **Query Operation** Retrieves items based on the primary key or secondary index key attributes.

Entire item is always read, cannot be filtered to a few fields. Normalize data to avoid expensive reads.

### CAP Theorem

1. **Eventual consistency** Default consistency model, offers highest availability, and lowest latency, but can cause stale reads. Dynamo behaves like an AP system displaying BASE properties.
2. **Strong consistency** All reads reflect latest write. Dynamo behaves like an CP system displaying ACID properties.

1. **Data Replication**:
* DynamoDB replicates data across multiple nodes in different Availability Zones within a region
* Each item is typically replicated to at least three nodes.
2. **Eventual Consistency**:
* Write operations are considered successful when acknowledged by a majority of replicas.
* Reads may be served by any replica, which might not have the latest update yet.
* Background processes continuously synchronize data across replicas.
3. **Strong Consistency**:
* Strongly consistent reads are routed to the leader node for the partition.
* The leader ensures it has the most up-to-date data before responding.
* This may involve waiting for replication to complete or querying other nodes.
4. **Quorum-Based Approach**:
* DynamoDB uses a quorum-based system for both reads and writes.
* For writes, a quorum of nodes must acknowledge the write for it to be considered successful.
* For strongly consistent reads, a quorum of nodes is queried to ensure the latest data is retrieved.
5. **Conflict Resolution**:
* In case of conflicts (e.g., simultaneous writes to different replicas), DynamoDB uses a "last writer wins" approach based on timestamps.
6. **Transaction Support**:
* For transactions, DynamoDB uses a two-phase commit protocol across multiple partitions.
* This ensures that all parts of a transaction are completed atomically.

### Scalability

DynamoDB scales through auto-sharding and load balancing. Consistent hashing ensures even distribution across nodes, balancing traffic and load.

Global Tables allow real-time replication across regions, enabling local read/write operations worldwide. This reduces latency and improves user experience.

### Security

Data is encrypted at rest by default in DynamoDB. Can also enable encryption in transit.

DynamoDB integrates with *AWS Identity and Access Management (IAM)* to provide fine-grained access control over your data.

Use *Virtual Private Cloud (VPC)* endpoints to securely access DynamoDB from within your VPC without exposing your data to the public internet.

#### DAX (DynamoDB Accelerator)

Built-in, **in-memory read-through** and **write-through** cache for Dynamo. Automatically caches read results from DynamoDB tables. Writes data to both the cache and the underlying DynamoDB table.

Can be configured as item cache or query cache

#### Streams

Dynamo also has built-in support for **Change Data Capture (CDC)** through DynamoDB Streams.

Uses:
1. **Consistency with Elasticsearch** Can be used to keep an Elasticsearch index in sync.
2. **Real-time analytics** By processing DynamoDB Streams with Kinesis Data Firehose, you can load data into Amazon S3, Redshift, or Elasticsearch for real-time analytics.
3. **Change Notifications** You can use DynamoDB Streams to trigger Lambda functions in response to changes in the database. 

### Limitations

1. **Cost efficiency** Might get expensive for high-volume workloads.
2. **Complex Query Patterns** For joins / multi table transactions use RDBMS.
3. **Data modelling constraints** If many GIS / LIS are required use RDBMS.
4. **Vendor Lock-In**