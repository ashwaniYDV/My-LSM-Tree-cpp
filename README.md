# Log-Structured-KV-Storage-Engine

* Implemented a very basic log-structured key-value storage engine in C++.

### Features
* Lightning-fast writes since data is added in the binary file in append mode.
* Reads are also super fast since it uses an in-memory index to store the byte offset of the latest record in data file.
* Fault-tolerant, since data and index both are persisted in the file system.
* Supports crash recovery. In case the system crashes during persisting the index, it can be reconstructed using "write-ahead logging" with the data file.
