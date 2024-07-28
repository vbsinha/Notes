#### Socket

Socket is a triad (Transport Protocol: TCP / UDP / raw IP, IP address, Port number - not for raw IP). A socket is identified externally by this triad.

Sockets are created during the lifecycle of an application process. A process can listen to multiple sockets, and vice versa - but the other way around is advanced usage.

In UNIX sockets behave like file descriptors.

#### Port

1. Port is associated per (Transport Protocol, IP). TCP & UDP can use same ports, which would correspond to different sockets.
2. 16 bits - 0 to 65535
3. OS takes data from all ports and transmits it. Partitions received data based on the Transport protocol, and port.
4. Servers use well-known port numbers for *listening*. Multiple clients can connect to same port since TCP connection is uniquely identified by (source IP, source port, destination IP, destination port). Clients typically use higher ports - ephemeral ports.
5. Browsers often use a different port per tab (often open multiple connections per tab: connection pool). 

Common ports

|Port|Assignment|
|----|---|
|20|FTP|
|22|SSH|
|25|SMTP|
|53|DNS|
|80|HTTP|
|110|POP3|
|143|IMAP|
|443|HTTPS|

#### HTTP

1. Stateless: No link between two successive requests on same connection.
2. Cookies used for state.
3. Client-server model: Client-server establish TCP connection, and then server sends *respond* to client's *request*. Server cannot respond without client's request. Use polling / Webhooks to fetch data from server.
    a. Open TCP connection
    b. Request (GET / POST etc.)
    c. Response
    d. Close / Reuse connection
4. HTTP/1, HTTP/2, HTTP/3 (uses QUIC instead of TCP like HTTP/1 & HTTP/2).
    a. HTTP/1 one TCP connection per request-response. HTTP/2 can accomodate multiple request-response per TCP connection.
    b. HTTP/1 uses text protocol, while HTTP/2 uses binary.
    c. HTTP/3 uses QUIC (UDP) so application layer becomes responsible for re-requesting lost packages.

##### HTTP request format

```
GET /static/img/a.png HTTP/1.1
        # Method Path (w/o protocol, domain, port) Version
Accept: text/html, ...
        # Key: value HTTP headers
(Optional) Body 
```

##### HTTP response format

```
HTTP/1.1 403 Forbidden
    # Version Status-code Status-message
Server: Apache
    # HTTP header
(Optional) Body
```

1. *XMLHttpRequest (XHR)* is javascript API to transmit request from browser to server. Can use non-XML, non-HTTP too.
2. *Fetch API* newer XHR.
3. *WWW (World Wide Web)* had four parts: HTML format, HTTP protocol, WorldWideWeb browser, and httpd server

#### Cookies

Small set of data server sends to client's browser. Client sends it back to server on later requests.

1. Session management: Logins, shopping carts, game scores, anything server should remember
2. Personalization: User preferences, themes, and other settings
3. Tracking: Recording and analyzing user behavior

Cookies were earlier used for general client-side storage. Modern APIs for client storage are the Web Storage API (localStorage and sessionStorage) and IndexedDB. Cookies are sent with every request, so they can worsen performance.

Server uses `Set-Cookie` header in response to send cookie. Can also specify expiration time.
Browser stores cookie and sends it back in Cookie header.

```
Response:

Set-Cookie: any_key=any_value; Expires=Thu, 31 Oct 2021 07:28:00 GMT; Secure; HttpOnly; SameSite=Strict

Secure: only sent over HTTPS, http sites cannot set Secure
HttpOnly: Inaccessible to JavaScript Document.cookie, only sent back to server: prevents XSS
SameSite: Strict -> Send cookies only with requests from the origin site. Lax -> Send cookies only when user navigates to site (even when coming from different site

Request - send all cookies:

Cookie: yummy_cookie=choco; tasty_cookie=strawberry
```

Session Cookie: Deleted when current session ends. Browser defines current session, can use it indefinitely too.
Permanent Cookie: Expires at expiration date

Can specify domain and path to set for which domain and path of the URL to use the cookies for.

First Party cookie: Cookie of the website a user visits
Third party cookie: Set by website user is currently on. Cookie from ad service etc, can track user behavior across multiple websites

#### HTTP methods

*Safe* methods don’t alter the state of the server
*Idempotent* Multiple same request = One request
All safe methods are idempotent

GET can have a body but it cannot add semantic meaning to the request. Not recommended, do not have a body.

| Method | Safe | Idempotent | Comment |
| ---- | ---- | ---- | ---- |
| GET | Yes | Yes |
| HEAD | Yes | Yes |
| POST | No | No | Insert new |
| PUT | No | Yes | Update if exists, create o/w |
| DELETE | No | Yes |

Conditional GET: If-modified-since HTTP header on request. Server responds with 304 Not Modified if content has not changed.

#### HTTP Response codes

* 1xx: Informational Responses
* 2xx: Successful
* 3xx: Redirects
* 4xx: Client error
* 5xx: Server error

#### URL / URI

The target of an HTTP request is called a resource.
URI: Each resource is identified by a Uniform Resource Identifier (URI)
URL: Uniform Resource Locator

```
http://www.example.com:80/path/to/myfile.html?key1=value1&key2=value2#SomewhereInTheDocument
Protocol: http
Domain Name: www.example.com
Port: 80 (Can be skipped if using default)
Path to File: path/to/myfile.html (need not be a real path)
Query: ?key1=value1&key2=value2
Fragment: #SomewhereInTheDocument (Never sent to server with request)
```

#### SOAP API (Simple Object Access Protocol)

Messaging Protocol used in API calls. Format to specify the request and response for API. Uses XML. 
Extensible, Neutral to HTTP / SMTP etc., Independent of programming model.
Cons: Verbose, Slow parsing of XML, Cannot use protocol-specific optimizations since it is protocol agnostic.
Request data generally kept as a part of the body and not URL. Use POST etc. instead of GET since GET cannot have meaningful body.
Since SOAP uses POST, and POST is generally not cached, SOAP request are not auto-cached (adv for REST).
Requires more bandwidth than REST.

```xml
<?xml version="1.0"?>
<soap:Envelope xmlns:soap="https://www.w3.org/2003/05/soap-envelope">
  <soap:Header> <!-- Header is optional -->
  </soap:Header>
  <soap:Body>
    <m:GetUser>
      <m:UserId>123</m:UserId>
    </m:GetUser>
  </soap:Body>
</soap:Envelope>
```

##### WSDL (Web Service Description Language)

Often used with SOAP. Describes what the message XML format of SOAP should be.


#### REST API REpresentational State Transfer

1. Architectural style for providing standards between computer systems on the web, making it easier for systems to communicate with each other.
2. Communicates with resources: Client asks for a resource (like a comment) using a GET API and server returns it. Uses GET, POST, PUT, DELETE. Specify which resource using the URL (and Accept header in request of HTTP).
3. Separation of Client and Server: Client / Serve only need to know about the format of the message, their implementations can change independently.
4. Stateless: No need to see previous interaction

#### GraphQL

GraphQL is a data query and manipulation language for APIs. It uses strong typing and Schema (GraphQL Schema Definition language). All types exposed are written in schema.

Pros: 
1. Allows client to define structure of data request and same format is returned.
2. Server does not return unnecessary data.
3. Lesser calls than REST, can get data of multiple resources in one call.
Generally REST API is developed to serve the views in UI, but when UI changes, new REST call needs to be created. This is avoided in GraphQL.

Cons:
1. Caching is affected since request can have many formats.
2. Complicated for building simple APIs

```
Request format:
{
    orders {
        id
        productsList {
            product {
                name
                price
            }
            quantity
        }
        totalAmount
    }
}

Response:
{
    "data": {
        "orders": [
            {
                "id": 1,
                "productsList": [
                    {
                        "product": {
                            "name": "orange",
                            "price": 1.5
                        },
                        "quantity": 100
                    }
                ],
                "totalAmount": 150
            }
        ]
    }
}
```
