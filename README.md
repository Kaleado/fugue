# Fugue

## Overview
An all-in-memory key-value store written in C++.

## Goals
- [ ] Implement a B+ tree for the underlying storage.
- [ ] Implement a text-based communication layer using TCP (which will be used for all IPC).
- [ ] Implement an API (perhaps in Python or Node).
- [ ] Implement a simple challenge-response authentication mechanism.

## Comparison to Memcached

- Memcached offers a similar facility, but it uses a large distributed hash table.
- Memcached API calls appear like so:

```
function get_foo(int userid)
     /* first try the cache */
     data = memcached_fetch("userrow:" + userid)
     if not data
         /* not found : request database */
         data = db_select("SELECT * FROM users WHERE userid = ?", userid)
         /* then store in cache until next get */
         memcached_add("userrow:" + userid, data)
     end

     return data
```

- Memcached provides both a binary protocol and a text protocol (both over TCP). For this project, a text protocol is
  all that will be necessary.
- I could implement only a small subset of commands to begin with, e.g. `get`, `set` and `delete`.
- Memcached allows for multiple servers to be used to store cache data by hashing the key being accessed to get an index
  into a list of servers. Because the servers do not provide any guarantee of reliability, the expectation is that a SQL
  query will be made if the server is inaccessible.
- Memcached uses a LRU cache when memory is full, which can operate in 'flat' mode or in 'temperature' mode.
- Flat mode is a conventional LRU cache, temperature mode aims to balance between 'hot', 'warm' and 'cold' cache
  entries.
- Memcached uses its own memory allocator to allocate portions of a massive hash map (not a B+ tree).
- Memcached tracks statistics, which can be retrieved using the `stat` command.
- Perhaps the main draw could be ease of use?