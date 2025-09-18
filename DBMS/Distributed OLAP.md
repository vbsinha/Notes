***OLAP / BI (Business Intelligence dbms) / DSS (Decision Support System)***

Many Databases - (ETL) &rarr; Data warehouse

1. *Extract* data from database
2. *Transform* data to match warehouse's schema. Clean up data.
3. *Load* into the warehouse.

Tools for ETL e.g. Informatica

Modern version: ELT. Load first and transform in the warehouse. E.g. dbt, Airbyte

#### Schema

1. *Star Schema* Middle (fact) table has FKs, to outer (dimension) tables.
2. *Snowflake Schema* (newer & more general) More normalized. Tables in hierarchy.

*Normalization* Snowflake takes less space. Denormalized data may incur integrity and consistency violations.
*Query Complexity* Snowflake requires more joins. Queries are generally faster on star schema.

#### Query Execution

*Pull data to query* or *Push query to data* (or both)

**Push query to data** Perform filtering and processing on data before transmitting over network.
**Pull data to query** Necessary when no compute resources at storage node. Clouds allow a little filtering while fetching data from storage nodes.

#### Distributed Join Algorithms

Options:
1. Copy the dimension table to all nodes. No extra data transfer needed.
2. Each node has matching partitions for fact and dimension tables. No extra data transfer needed.
3. If one table is small, it is broadcasted to all nodes. Called *broadcast join*.
4. If tables are not partitioned on the join keys, then data has to shuffled through the nodes to compute the join. Called *shuffle join*.

#### Cloud systems

Cloud systems offer Database as a Service.

Approaches
1. *Managed DBMS* No significant modification to the DBMS to be aware that it is running on cloud. Most vendors do this.
2. *Cloud native DBMS* System designed explicitly to run in a cloud environment. Generally shared disk. Snowflake, Google Big Query etc.

#### Serverless databases

**Normal** Application -> [Node with CPU & Memory + DBMS]
**Serverless** Application -> [Node with CPU & Memory] -> DBMS. Node can be deprovisioned during low traffic. CockroachDB, Neon (based on Postgress) etc.

### Data Lakes

Repository to store large amounts of structured, semi-structured and unstructured data without having to define a schema or ingest data into proprietary internal formats.

Application can put all sorts of data like csv, json, parquet etc. Databricks, Snowflake, trino, presto, Redshift, Google bigquery etc.

#### Data File Formats

Apache Parquet: Compressed columnar storage 
Apache ORC, Apache Iceberg, Apache Arrow etc.