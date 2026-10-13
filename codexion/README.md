*This project has been created as part of the 42 curriculum by skheir.*

# Description

Codexion is a concurrency project that simulates several coders working with shared dongles. Each coder compiles, debugs and refactors while competing for two dongles. The project uses POSIX threads and synchronization mechanisms to safely manage shared resources.

The project supports FIFO and EDF scheduling, dongle cooldowns, burnout detection and thread-safe logging.

# Instructions

Compile the project with:

```bash
make
```

Run the program with:

```bash
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

Example:

```bash
./codexion 5 2000 200 200 200 10 0 fifo
```

The scheduler can be `fifo` or `edf`.

# Resources

- POSIX Threads documentation (`pthread_mutex`, `pthread_cond`, `pthread_create`, `pthread_join`)
- Linux pthread manual pages
- Valgrind and Helgrind documentation
- Documentation about concurrency, deadlocks and Coffman's conditions
- Documentation about FIFO and EDF scheduling

Example of memory leak testing:

```bash
valgrind --leak-check=full ./codexion 5 2000 200 200 200 10 0 fifo
```

Example of thread synchronization testing:

```bash
valgrind --tool=helgrind ./codexion 5 2000 200 200 200 10 0 fifo
./codexion 5 3000 200 200 200 6 800 fifo | grep "is compiling" | awk '{printf $2" "}'
./codexion 5 2000 200 200 200 7 0 fifo | grep -c "is compiling"
```

The tests were used to check memory leaks and synchronization errors.

AI was used as a learning and development assistant to understand pthread synchronization, analyze race conditions and Valgrind/Helgrind results, review synchronization logic, and help design tests and documentation. The code was tested and adapted manually.

# Blocking cases handled

- **Deadlock prevention:** coders use a controlled dongle acquisition order to prevent circular waiting and break the Coffman circular-wait condition.
- **Starvation prevention:** FIFO and EDF queues provide an ordering policy for waiting coders.
- **Cooldown handling:** each dongle stores its release time and cannot be reused before its cooldown expires.
- **Precise burnout detection:** the monitor checks each coder's last compilation start time and ignores coders that have completed the required compilations.
- **Log serialization:** a mutex protects logging so messages from different threads cannot be mixed.

# Thread synchronization mechanisms

The project uses:

- `pthread_mutex_t` to protect dongles, coder state, logging and monitor state.
- `pthread_cond_t` to make coders wait for available dongles and wake them when dongles are released.
- A custom heap-based event/priority queue to manage waiting coders according to FIFO or EDF scheduling.

For example, `compiles_done` and `last_compile_start` are protected by each coder's mutex, while the monitor uses the same mutex when reading them. This prevents race conditions.

The `stop` state is protected by `stop_lock`, allowing coders and the monitor to communicate safely.