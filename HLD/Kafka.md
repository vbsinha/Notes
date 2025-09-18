**Kafka** is an open-source distributed event streaming platform that can be used either as a **message queue* or as a *stream processing system*. 
In *MQ* consumers read messages and ack that they processed the message.
In *stream* consumers read and process messages but do not ack that they have processed.
It delivers high performance, scalability, and durability.

### Basic Terminology and Architecture

**Broker**
A Kafka cluster is made up of multiple **brokers**, which are just individual physical or virtual servers. Each broker is responsible for storing data and serving clients.

**Partitions**
Each broker has a number of **partitions**. Each partition is an ordered, immutable sequence of messages that is continually appended to, essentially an append-only log file. So Kafka is described as distributed commit log.

1. **Immutability** Once written, messages in a partition cannot be altered or deleted.
2. **Efficiency** Minimizes disk seek time, since messages are always written at the end.
3. **Scalabiltiy** Partitions allow Kafka to scale horizontally. Partitions are replicated across multiple brokers for fault tolerance. 

**Topic**
Logical grouping of partitions. Messages are published to and consumed from topics. Topics can have zero or more producers.

**Producers**
Write data to topics.

**Consumers**
Read data from topics. Consumers can be grouped into **consumer groups**. Each message is guaranteed to only be processed by one consumer in the group.

**Message**

A message consists of required **value**, and optional: **key**, **timestamp**, **headers**. Key is used to determine which partition message is sent to. Timestamp is used to order messages within a partition. Headers are key-value pairs used to store metadata about the message. Messages are sent to topics.

If a key is not assigned Kafka will randomly pick a partition to assign it to. To process keys in order provide an appropriate key.

### How Kafka works

When a message is published to Kafka, it first determines the appropriate partition for the message using the key. Messages with same keys go to the same partition. Then Kafka decides which broker has the partition using the mapping managed by Kafka cluster metadata, which is maintained by Kafka controller (a role within broker cluster). 

Each message in a partition is assigned a unique offset indicating its position in the partition. Consumers track progress using these offsets and commit their current offset back to Kafka. This helps consumers resume reading in case of failure or restart.

It is better to keep consumer's work small since if the work is large, there is more rework whenever when consumer fails and reworks on a message.

Consumers can read messages from Kafka when they arrive (**push model**), or poll Kafka for new messages (**pull model**).

#### Durability and Availability in Kafka

1. **Leader replica assignment** Each partition has a leader which resides on a broker. Leader is responsible for all read and write requests for the partition. Assignment of leader replica is managed by central cluster controller. 

2. **Follower Replication** Several followers exist for each partition, residing on different brokers. They do not handle direct client requests, instead passively replicate data from leader. When a leader fails, a follower is promoted to a leader. Kafka can be set to ack to a producer's message enqueue operation only after message has been replicated on all followers.

3. **Controller's role** Monitors health of all brokers and manages leadership and replication.

Replication factor of 3 is common: 1 primary + 2 replicas.

Common wisdom: Kafka is always available and sometimes consistent.

#### When to use Kafka

**As an MQ**

1. For async processing.
2. To ensure messages are processed in correct order.
3. To decouple producer and consumer to scale them independently.

**As a stream**

1. Continuous and immediate processing of data, as in ad click aggregator.
2. To process messages by multiple consumers simultaneously.

#### Scalability

Max message size is configurable, but ideally messages should be kept small under 1 MB. On good hardware, a single broker can store around 1TB of data and handle around 10,000 messages per second.

To *scale*:
1. **Horizontal scaling with more brokers** Add more brokers to the cluster. This helps with both scaling and fault tolerance since partitions can be split across brokers. 
2. **Partitioning Strategy** Choose the partition key wisely to avoid skewed traffic. 

To handle *hot partitions*:
1. **Random partitioning with no key** Kafka will distribute messages evenly across partitions but ordering will be lost.
2. **Random Salting** Salt the key, but makes aggregation complicated on the consumer side.
3. **Use a compound key** Use ID with another attribute like geo data.
4. **Back pressure** Managed Kafka services allow back pressure. For own cluster, producer can check lag on partition and slow down if it's high.

#### Handling retries and errors

**Producer Retries**
Kafka libraries support automatic producer retries. Keep the messages idempotent to avoid duplication.

**Consumer Retries**
Kafka does not actually support retries for consumers out of the box (but AWS SQS does!) so we need to implement our own retry logic. One common pattern is to set up a custom topic that we can move failed messages to and then have a separate consumer that processes these messages. We can retry messages as many times as we want without affecting the main consumer. If a given message is retried too many times, we can move it to a dead letter queue (DLQ).

#### Performance Optimizations

1. Producer should send messages in batch.
2. Messages can be zipped.
3. Choose partition key wisely.

#### Retention Policies

Kafka topics have configurable retention policies. Default is 7 days or until it reaches 1 GB.

#### References

1. [Hellointerview](https://www.hellointerview.com/learn/system-design/deep-dives/kafka)