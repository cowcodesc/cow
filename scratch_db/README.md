# Scratch DB
A database made from scratch in C, while following the book *"Build your own Database from Scratch"*, which is written for GoLang.

According to the book, there are three main pillars of a DB.
1. Persistance
2. Indexing
3. Concurrency

## Persistance
Normal file writes are prone to corruption or incomplete write. To fix this and make data writes atomic, we can use two methods:
1. Write all the data to a temporary file, flush it using `fsync` and then rename the file to original file. Renaming is always guranteed to be atomic by the file-system.
2. Use append-only logs, append only logs appends data to a file, without overwritting previous data. It also maintains write order.
