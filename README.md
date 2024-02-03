# Log-Structured-KV-Storage-Engine

* Implemented a very basic log-structured key-value storage engine in C++.

## Features
* Lightning-fast writes since data is added in the binary file in append mode.
* Reads are also super fast since it uses an in-memory index to store the byte offset of the latest record in data file.
* Fault-tolerant, since data and index both are persisted in the file system.
* Supports crash recovery. In case the system crashes during persisting the index, it can be reconstructed using "write-ahead logging" with the data file.

## TODO
* For handling large data, store the data in multiple files and write compression algorithms which runs in background to merge these files (since data file is append only, a key can be present in multiple files with latest record in one of the new files)
* Implement LSM tree.

#### Running Project
* Create a bin folder in root directory if it doesn't exists. It will store the data and index file binaries.
