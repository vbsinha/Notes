#### Domain Name System

A distributed, hierarchial, and eventually consistent key-value store.

4 main systems: 
    1. DNS resolver (typically hosted by ISP)
    2. Root name server (NS) - maps top-level domain (like `.com`) to address of name server responsible for it
    3. TLD (Top Level Domain) nameserver
    4. Authoritative nameserver.

Used to use UDP, but now TLS is used.

8 Step process (without caching):
1. Local sends ex.com to DNS resolver.
2. Resolver queries root nameserver.
3. Root nameserver returns TLD for `.com`.
4. Resolver queries TLD with `ex.com`.
5. TLD returns IP of Authoritative ns for `ex.com`
6. Resolver queries Authoritative ns with `www.ex.com` (or `news.ex.com`).
7. Authoritative ns returns IP of `www.ex.com` (or `news.ex.com`).
8. Resolver returns IP.

For subdomains like `news.ex.com` more queries can be needed

DDoS attack on Root ns not successful due to traffic filtering. DNS Spoofing attack (send bogus IP)

Chrome DNS lookup: `chrome://net-internals/#dns`
DNS resolver list from terminal: `scutil --dns`
