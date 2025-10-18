# 📨 IPC Message Queues – Task Dispatcher & Worker

## 🧭 What
A simple Linux project demonstrating **POSIX Message Queues (mqueue)** for inter-process communication (IPC).  
It includes two programs:
- **task_dispatcher.c** – sends tasks with a given priority.
- **task_worker.c** – receives and processes tasks from the message queue.

---

## 💡 Why
To learn how POSIX message queues enable:
- Communication between independent processes.
- Priority-based message handling.
- A producer–consumer model in Linux.

---

## 🕒 When
Use this example when studying or testing IPC mechanisms on Linux, or when designing systems that require task queues with priority scheduling.

---

## 📍 Where
Runs on any **Linux system** that supports POSIX message queues.  
Requires the **librt** library for compilation.

### Folder Structure
```
8.IPC_Message_Queues/
├── src/
│   ├── task_dispatcher.c
│   └── task_worker.c
└── Makefile
```

## ⚙️ How

### 🔧 Build
From the project root directory:
```bash
make
```

Or manually:
```bash
gcc src/task_worker.c -o task_worker -lrt
gcc src/task_dispatcher.c -o task_dispatcher -lrt
```

### ▶️ Run

1. Start the worker:
   ```bash
   ./task_worker
   ```

2. In another terminal, send tasks:
   ```bash
   ./task_dispatcher 10 "Update weekly report"
   ./task_dispatcher 30 "CRITICAL: Restart web server"
   ./task_dispatcher 20 "Backup user database"
   ```

**Note:**  
- When sending tasks one by one, they are processed in **send order**.  
- When multiple messages exist in the queue before the worker runs, they are processed by **priority (high → low)**.

### 🧪 Test Priority Order
```bash
./task_dispatcher 10 "Low priority"
./task_dispatcher 30 "High priority"
./task_dispatcher 20 "Medium priority"
```

**Expected Output:**
```
Processing task (Priority: 30): High priority
Processing task (Priority: 20): Medium priority
Processing task (Priority: 10): Low priority
```

**Actual Output (when sending one by one):**
```
Processing task (Priority: 10): Low priority
Processing task (Priority: 30): High priority
Processing task (Priority: 20): Medium priority
```

---

## ⚠️ Note on Priority Behavior

**Q:** Why does `task_worker` process messages in send order instead of priority order  
when I send tasks with priorities 10, 20, and 30?

**A:**  
POSIX message queues *do* support priorities — but only when multiple messages exist in the queue **at the same time**.  
When `mq_receive()` is called in **blocking mode**, the worker immediately retrieves the first available message as soon as it arrives.

In this project:
- `task_worker` is always waiting on `mq_receive()`.
- `task_dispatcher` sends messages one by one, with short delays between sends.

So at each moment, there is **only one message** in the queue.  
No comparison by priority occurs, since the queue never holds multiple messages simultaneously.

**In short:**
| Step | Queue State | Actor | Result |
|------|--------------|--------|---------|
| Send task #1 (prio=10) | Queue empty → received immediately | Worker waiting | Receives prio=10 |
| Send task #2 (prio=30) | Queue empty again | Worker waiting | Receives prio=30 |
| Send task #3 (prio=20) | Same situation | Worker waiting | Receives prio=20 |

→ There’s never a moment when the queue has multiple messages to sort by priority.

---

### 🧹 Cleanup
Press `Ctrl + C` in the worker to exit:
```
Shutting down task_worker...
Queue /my_task_queue removed.
```

---

## 📚 References
- `man mq_overview`
- `man mq_send`
- `man mq_receive`
