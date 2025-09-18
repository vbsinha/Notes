MQs are a way to transfer information between two systems.

Advantages of MQs (*Decoupling*):
1. Sender does not care about the receiver - one less responsibility.
2. Sender does not care *when* the receiver will receive.
3. Sender can push message as fast as they like.
4. Receiver will receive at a speed comfortable to it.
5. Sender & Receiver are not concerned about how the other works or what is their *capacity* / *load* or where they are.

MQs are not very useful when 
1. Decoupling systems do not make sense - one cannot work without the other - application server & data service (OLTP).
2. Communication adds more more overhead and complexity. One system is less complex than two.
3. Time predictability is lost. Consumer might process late. If predictability is more important than throughput, do not use queue.


MQ is *durable*, it will not forget a message.

### Delivery guarantees
MQ will guarantee that each message will be delivered *at-least*, *at-most* or *exactly* once.

#### At least once

If a message is read but not deleted, it will be kept being redelivered.

If the receiver crashes before acknowledging the message, the message will be delivered again.

Receiver has to plan for multiple copies of same message incase, the acknowledgement doesn't reach the queue / the queue restarted before it could keep track of what's been sent.

To account for message duplication / repetition, receiver needs to store the message ID in transactional ACID db. This is called *idempotency token* or *_tombstone-_*.
If duplications are handled or messages are naturally resistant to duplication, the system is called *idempotent*.

Senders can also send the same message multiple times, in case they fail to receive the acknowledgement from queue. The senders are responsible for using the same tombstone or idempotency token.

#### At-most once

Used when duplication is explosive or messages unimportant - better to not send message rather than send twice.

#### Exactly once

Impossible because senders / receivers / network is imperfect.


### Ordering vs Parallelism

If strict ordering is enforced, there can be only one receiver. 
If there are two, second receiver could finish processing second message and start on third, while first receiver is still working on the first message. 
To avoid this a distributed lock can be used, but that effectively reduces to have one agent, and one agent crashing would result in deadlock.
FIFO queue will only supply the second message after the first one has been acknowledged.

FIFO queue are generally more expensive than parallel since work needs to be constrained to one server which needs to be kept reliable. For e.g. AWS SQS (Simple Queue Service) FIFO queue is about expensive than the parallel queue - supports about few hundred message per sec.

FIFO can be enforced per group: All payment message must be FIFO, all order messages must be FIFO, but they need not be FIFO to each other.

In random queues, *best-effort* is put to maintain ordering, most messages will be ordered. Imagine multiple pipes, sometimes branching too.
This allows scale up, pushing many messages will add more pipes.

FIFO requirements can sometimes be parallelized using timestamp - caring about only the most recent message (like download percent - 43% 41% 42% is 43%), or using monotonically increasing counter into messages.

Systems should try to be idempotent & allow out-of-order delivery.

### Fan Out / In

Fan Out: Multiple receivers want to receive a message - for audit log / archive etc.

Fan In: Receiver wants many kinds of receive: notification service / audit-log.

1. Publish every message to one appropriate topic.
2. Create a queue for each receiver. 
3. Link each receiver's queue to the topics the receiver is interested in.

AWS SNS (Simple Notification Service), Google's Pub/Sub, Apache Kafka.

### Poison Pills and Dead Letters

If a message is sent out for processing repeatedly & unsuccessfully, system will move it to *dead letter queue*. Monitor this queue for problems.

If a receiver crashes due to a message it is called a *poison pill*. Poison pills will eventually be promoted to dead letter queue.

### AWS SNS & SQS

SQS: Pure MQ to create / send / receive. *ReceiveMessage* API is pull-only. Keep calling with wait time of 20 secs.

SNS: Supports topics.

AWS uses groupId for FIFO.

### Google Pub/Sub

Topic + Subscription (i.e. queue). Pub/Sub allows *webhook* style POST of message apart from pull / poll mechanism. Can replay messages too. Ordering Key for FIFO.

### AWS EventBridge

Managed Event bus: no visible topic based separation, all messages are posted into single bus. Every message needs to be structured according to a standard format based on topic. Bus reads message and routes to subscribers interested in that topic.

### Redis Streams

TODO
*Consumer groups* 

### Kafka

Publish *events* to *topics*. Events are split into partitions based on partition key. FIFO is maintained in each partition.

Open source -> complicated to install and maintain.

### Rabbit MQ

Supports *topics* and *queues*.

### NSQ & NATS

Distributed message queue.

References
1. [link](https://sudhir.io/understanding-connections-pools)