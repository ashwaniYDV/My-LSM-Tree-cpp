# Log-Structured-KV-Storage-Engine

* Implemented a Log-Structured Key-Value Storage Engine in C++, drawing inspiration from databases like Bitcask, LevelDB, Google BigTable, RockDB, and Cassandra.

## Features
* Lightning-fast writes by appending data directly to a binary file, optimizing write performance.
* Reads are highly efficient, leveraging an in-memory index to store byte offsets of the latest records in the data file. 
* It is fault-tolerant, persisting both data and index to the file system.
* Supports crash recovery through "write-ahead logging," allowing the reconstruction of the index in case of system crashes during the persistence process.

## TODO
* For handling large data, store the data in multiple files and write compression algorithms which runs in background to merge these files (since data file is append only, a key can be present in multiple files with latest record in one of the new files)
* Implement LSM tree.

#### Running Project
* Create a bin folder in root directory if it doesn't exists. It will store the data and index file binaries.
