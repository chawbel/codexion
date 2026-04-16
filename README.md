*This project has been created as part of the 42 curriculum by cbahry.*

# Codexion

## Description
Codexion is a concurrency simulation project that models coders working in a shared hub, competing for limited USB dongles. Each coder needs two dongles to compile their code. The goal is to coordinate multiple threads (coders) and a monitor thread to ensure everyone compiles regularly, avoids burnout (death), and manages shared resources without deadlocks or race conditions. This project serves as a practical implementation of classic synchronization problems like the Dining Philosophers, with added complexity from custom scheduling and cooldown mechanics.

## Instructions
### Compilation
The project uses a Makefile with standard rules. To build the executable:
```bash
make
```
This will produce the `codexion` binary using the flags `-Wall -Wextra -Werror -pthread`.

### Execution
Run the simulation with the following mandatory arguments:
```bash
./codexion <number_of_coders> <time_to_burnout> <time_to_compile> <time_to_debug> <time_to_refactor> <number_of_compiles_required> <dongle_cooldown> <scheduler>
```
- `scheduler`: Must be either `fifo` or `edf`.

Example:
```bash
./codexion 5 800 200 200 200 7 100 EDF
```

## Resources
### Classic References
- **Dining Philosophers Problem**: Foundations of synchronization and deadlock prevention.
- **POSIX Threads Programming**: Guide on `pthread` library usage.
- **Coffman Conditions**: Understanding the four conditions necessary for deadlock.
- **Earliest Deadline First (EDF) Scheduling**: Real-time scheduling theory.

### AI Usage
AI was used in this project for the following tasks:
- **Bug Root-Cause Analysis**: Identifying subtle data races on simulation state and timestamp variables.
- **Refactoring for Norm Compliance**: Splitting functions that exceeded the 25-line limit (notably the `monitor_routine`).
- **Test Automation**: Generating Python scripts to stress-test the simulation with varied arguments and Helgrind validation.
- **Documentation**: Assisting in the structured drafting of this README to ensure all subject requirements were met.

## Blocking cases handled
- **Deadlock Prevention & Coffman's Conditions**:
    - **Circular Wait**: Prevented by enforcing a strict lock ordering (coders always pick up the dongle with the lower ID first).
    - **Hold and Wait**: Coders only proceed to compile once both resources are secured; if the simulation ends, they release any held resources.
- **Starvation Prevention**: The `EDF` (Earliest Deadline First) scheduler ensures coders closest to burnout get priority for dongles. The `FIFO` scheduler ensures fairness based on arrival time.
- **Precise Burnout Detection**: A dedicated monitor thread checks all coders every 500ms, ensuring burnout logs are printed within the 10ms tolerance required by the subject.
- **Log Serialization**: All console output is protected by a `log_mutex` and uses `write` syscalls via helper functions to ensure lines from different threads never interleave.
- **Cooldown Handling**: Dongles track a `cooldown_until` timestamp, and the `wait_for_turn` logic uses `pthread_cond_timedwait` to respect this period efficiently.

## Thread synchronization mechanisms
- **pthread_mutex_t**:
    - `dongle->lock`: Protects the state of each individual dongle and its associated wait queue.
    - `sim->stop_mutex`: Protects global state variables like `sim_ended` and `compile_count` to prevent race conditions during simulation shutdown.
    - `sim->log_mutex`: Ensures that logs are printed atomically.
- **pthread_cond_t**:
    - Used to implement a non-busy wait system. Coders sleep on the dongle's condition variable and are woken up via `pthread_cond_broadcast` only when the dongle they need becomes available or the simulation ends.
- **Race Condition Prevention**:
    - Every access to shared variables (e.g., `last_compile_start`, `sim_ended`) is wrapped in a mutex lock/unlock pair.
    - Race conditions were verified and eliminated using `valgrind --tool=helgrind`.
- **Thread-Safe Communication**:
    - The monitor thread communicates a "simulation end" event by setting the `sim_ended` flag under `stop_mutex` and broadcasting to all dongle condition variables to wake any waiting threads for a clean exit.
