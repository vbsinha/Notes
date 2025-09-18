**Document**
Individual unit of data like a record in db. 
Each document has a unique id and a set of fields (key-value pair of data).

```json
{
  "id": "XYZ123",
  "title": "The Great Gatsby",
  "author": "F. Scott Fitzgerald",
  "price": 10.99,
  "createdAt": "2024-01-01T00:00:00.000Z"
}
```

**Index**

Collection of documents like a table in db.

**Mapping and Field**
*Mapping* is a schema of the index. It defines the *fields* that each index has, its data type, and any other properties like how the field is processed and indexed. Mapping determines which fields are searchable and what type of data they contain.

```json
{
  "properties": {
    // keyword - treat value as whole, not a tokenizable string
    "id": { "type": "keyword" },  
    "title": { "type": "text" },
    "author": { "type": "text" },
    "price": { "type": "float" },
    "createdAt": { "type": "date" }
  }
}
```

## Using ElasticSearch

We can use REST API to interact with Elasticsearch

#### Creating an Index

```json
// PUT /books
{
  "settings": {
    "number_of_shards": 1,
    "number_of_replicas": 1
  }
}
```

By default allows dynamic mapping (schema).

#### Set a mapping

Narrows down which fields to index.
```json
// PUT /books/_mapping
{
  "properties": {
    "title": { "type": "text" },
    "author": { "type": "keyword" },
    "description": { "type": "text" },
    "price": { "type": "float" },
    "publish_date": { "type": "date" },
    "categories": { "type": "keyword" },
    "reviews": {
      "type": "nested",
      "properties": {
        "user": { "type": "keyword" },
        "rating": { "type": "integer" },
        "comment": { "type": "text" }
      }
    }
  }
}
```

#### Add documents

```json
// POST /books/_doc
{
  "title": "The Great Gatsby",
  "author": "F. Scott Fitzgerald",
  "description": "A novel about the American Dream in the Jazz Age",
  "price": 9.99,
  "publish_date": "1925-04-10",
  "categories": ["Classic", "Fiction"],
  "reviews": [
    {
      "user": "reader1",
      "rating": 5,
      "comment": "A masterpiece!"
    },
    {
      "user": "reader2",
      "rating": 4,
      "comment": "Beautifully written, but a bit sad."
    }
  ]
}

// returns
{
  "_index": "books",
  "_id": "kLEHMYkBq7V9x4qGJOnh",
  "_version": 1, // NOTE!
  "result": "created",
  "_shards": {
    "total": 2,
    "successful": 1,
    "failed": 0
  },
  "_seq_no": 0,
  "_primary_term": 1
}
```

#### Updating documents

```json
// PUT /books/_doc/kLEHMYkBq7V9x4qGJOnh
{
  "title": "To Kill a Mockingbird",
  "author": "Harper Lee",
  "description": "A novel about racial injustice in the American South",
  "price": 13.99,
  "publish_date": "1960-07-11",
  "categories": ["Classic", "Fiction"],
  "reviews": [
    {
      "user": "reader3",
      "rating": 5,
      "comment": "Powerful and moving."
    }
  ]
}

// PUT /books/_doc/kLEHMYkBq7V9x4qGJOnh?version=1 
// Update only if _version is 1

// POST /books/_update/kLEHMYkBq7V9x4qGJOnh
{
  "doc": {
    "price": 14.99
  }
}
```

#### Search

GET request with body is accepted by Elasticseach, can use POST if needed

```json
// GET /books/_search
{
  "query": {
    "match": {
      "title": "Great"
    }
  }
}

// GET /books/_search
{
  "query": {
    "bool": {
      "must": [
        { "match": { "title": "Great" } },
        { "range": { "price": { "lte": 15 } } }
      ]
    }
  }
}

// GET /books/_search
{
  "query": {
    "nested": {
      "path": "reviews",
      "query": {
        "bool": {
          "must": [
            { "match": { "reviews.comment": "excellent" } },
            { "range": { "reviews.rating": { "gte": 4 } } }
          ]
        }
      }
    }
  }
}

// result
{
  "took": 7,
  "timed_out": false,
  "_shards": {
    "total": 5,
    "successful": 5,
    "skipped": 0,
    "failed": 0
  },
  "hits": {
    "total": {
      "value": 2,
      "relation": "eq"
    },
    "max_score": 2.1806526,
    "hits": [
      {
        "_index": "books",
        "_type": "_doc",
        "_id": "1",
        "_score": 2.1806526, // relevance of the document to the query
        "_source": {
          "title": "The Great Gatsby",
          "author": "F. Scott Fitzgerald",
          "price": 12.99
        }
      },
      {
        "_index": "books",
        "_type": "_doc",
        "_id": "2",
        "_score": 1.9876543,
        "_source": {
          "title": "Great Expectations",
          "author": "Charles Dickens",
          "price": 10.50
        }
      }
    ]
  }
}
```

#### Sort

Can use custom sorting functions or sort on nested structures too.

```json
// GET /books/_search
{
  "sort": [
    { "price": "asc" }
  ],
  "query": {
    "match_all": {}
  }
}
```

If no sort order is specified, by default results arr sorted by `_score` which is an algorithm similar to TF-IDF.

**TF-IDF** Term frequency measures how frequent a word is in a document. Inverse document frequency measures how unique a word is across documents.

#### Pagination and Cursors

Stateful pagination - Server keeps track of things so that it knows what is the next page. Uses cursors
Stateless pagination


1. From - Size
Inefficient since the server has to sort everything everytime.
```json
// GET /my_index/_search
{
  "from": 0,
  "size": 10,
  "query": {
    "match": {
      "title": "elasticsearch"
    }
  }
}
```

2. Search After
Use the last results as starting points. Efficient for deep pagination
First request does not have `search_after`
Can miss documents because index can get udpated.
```json
// GET /my_index/_search
{
  "size": 10,
  "query": {
    "match": {
      "title": "elasticsearch"
    }
  },
  "sort": [
    {"date": "desc"},
    {"_id": "desc"}
  ],
  "search_after": [1463538857, "654323"]
}
// 1463538857 is the timestamp of the last seen record, and 654323 is the doc id
```


3. Cursors

Snapshots the index

```json
// PIT - Point of Time
// POST /my_index/_pit?keep_alive=1m

// GET /_search
{
  "size": 10,
  "query": {
    "match": {
      "title": "elasticsearch"
    }
  },
  "pit": {
    "id": "46To...",
    "keep_alive": "1m"
  },
  "sort": [
    {"_score": "desc"},
    {"_id": "asc"}
  ]
}

// GET /_search
{
  "size": 10,
  "query": {
    "match": {
      "title": "elasticsearch"
    }
  },
  "pit": {
    "id": "46To...",
    "keep_alive": "1m"
  },
  "sort": [
    {"_score": "desc"},
    {"_id": "asc"}
  ],
  "search_after": [1.0, "1234"]
}

// DELETE /_pit
{
  "id" : "46To..."
}
```

## How it works

Elasticsearch is an orchestration layer for Apache Lucene, manages cluster coordination, APIs, etc. while search is handled by Lucene.

### Cluster Architecture

1. Master Node - Coordinates the cluster, adds / removes nodes / indices.
2. Data Node - stores data
3. Coordinating node - Coordinates searches, receives search requests and sends it to appropriate nodes.
4. Ingest node - Ingests data and prepares for indexing
5. Machine Learning node 

Client talks to ingest / coordinating nodes which in turn talk to data nodes.

### Summary

1. Not primary db
2. Best with read-heavy workloads
3. Must tolerate eventual consistency
4. Denormalization is key. Joins are not supported, flatten data