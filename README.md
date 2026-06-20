# libposture

A lightweight, dependency-free C++17 library for real-time squat analysis:
joint-angle math, signal smoothing, and rep counting. Built to be bridged into
**Posture AI**, an iOS app that uses ARKit body tracking to coach squat form.

The library has no Apple dependencies, so all logic is built and unit-tested on
the desktop with [GoogleTest](https://github.com/google/googletest).

## Modules

| Module | Status | Purpose |
|--------|--------|---------|
| `geometry` | ✅ | 3D joint-angle math (e.g. knee angle) |
| `filter` | planned | Smooth noisy joint/angle signals |
| `rep_counter` | planned | Count squat reps via a state machine |
| `form` | planned | Flag form errors (depth, torso, knees) |

## Build & test

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

GoogleTest is fetched automatically by CMake, so no manual install is needed.
