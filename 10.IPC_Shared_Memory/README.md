# POSIX Shared Memory – Producer & Consumer Demo

## 🧠 What
A simple demo showing **inter-process communication** using **POSIX Shared Memory**.  
The *producer* writes product info into shared memory, and the *consumer* reads it.

---

## 💡 Why
To practice:
- Using `shm_open`, `mmap`, `ftruncate`, and `shm_unlink`
- Understanding `MAP_SHARED` vs `MAP_PRIVATE`
- Sharing data between processes efficiently (without pipes or sockets)

---

## 📂 Where
```
10.IPC_Shared_Memory/
│
├── src/
│   ├── posix_shm_producer.c
│   └── posix_shm_consumer.c
│
├── Makefile
└── README.md
```

---

## ⚙️ How

### Build
```bash
make
```

### Run
Open two terminals:

**Terminal 1:**
```bash
./posix_shm_producer
```

**Terminal 2:**
```bash
./posix_shm_consumer
```

The consumer will display the product written by the producer.

### Clean
```bash
make clean
```

---

## 🧩 Notes
- Uses `MAP_SHARED` for real shared memory between processes.  
- The producer removes the shared memory with `shm_unlink()` when done.  
- Works on **Linux / POSIX-compliant systems**.

---

## ✅ Example Output
```
Consumer: Read product info from shared memory
  ID: 101
  Name: Laptop Dell XPS 13
  Price: 25999.99
```
