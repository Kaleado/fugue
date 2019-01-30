# Fugue

## Overview
An all-in-memory key-value store written in C++.

## Goals
- [ ] Implement a B+ tree for the underlying storage.
- [ ] Implement a communication layer using TCP (which will be used for all IPC).

## Research Notes

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

