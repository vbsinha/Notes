Challenges in building distributed system:
1. Communication
2. Coordination: 2 generals problem (2 generals have to coordinate the time to attack)
3. Scalability: System should be able to handle higher *loads*
    a. *Throughput* is the number of requests processed per second by the application.
    b. *Response time* is the time elapsed between sending a request and receiving a response.
    c. *Scaling up (Vertical)* means adding more hardware to the same node.
    d. *Scaling out (Horizontal)* means more machines working together.
4. Resiliency: System continues to work even when failures happen.
    a. *Availability* is the percentage time system is available for use: *uptime / (uptime + downtime)*
    b. 3 nines *99.9% - 1.44 min per day* is acceptable. Above 4 is highly available.
    c. Use *redundancy*, *fault isolation*, *self-healing mechanisms*.
5. Maintainability

Distributed system: group of processes communicating via inter process communication like HTTP / a group of loosely coupled components that communicate via API.

A *service* implements one specific part of overall system's capabilities. Core is business logic, which exposes interfaces to communicate with the outside world.
Process running a service is *server*, process sending request is *client*. A process can be both a client and a server.
*Dependency Inversion* Technical details depend on business logic, not vice versa.

*Ports and adapter (Hexagonal) architecture* 
Processes can’t call each other’s interfaces directly, *adapters* are needed to connect IPC mechanisms to service interfaces. An *inbound adapter* is part of the service’s *Application Programming Interface* (API); it handles the requests received from an IPC mechanism, like HTTP, by invoking operations defined in the service interfaces. *Outbound adapters* grant the business logic access to external services, like data stores. 
