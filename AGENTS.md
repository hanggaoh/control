# Catapult Senior C++ Interview Preparation

## Purpose

This repository is Hang Gao's hands-on preparation project for a Catapult
Software Senior C++ Developer interview. Keep exercises small, runnable,
production-oriented, and suitable for discussion in a senior-level interview.

## Candidate background and honesty boundary

- Hang has 5+ years of overall software engineering experience, but not 8+
  years of pure C++ experience.
- Volkswagen experience: embedded Android, C++/NDK/JNI, OpenCV, MediaPipe,
  OpenGL, and real-time video pipelines.
- Parkable experience: Linux, IoT/edge devices, distributed production
  systems, observability, security, and operational reliability.
- Connect these experiences explicitly to transferable engineering skills.
- Never claim direct production experience with SCADA, DNP3, iFIX,
  CIMPLICITY, IEC 61850, or other critical-infrastructure systems unless Hang
  explicitly provides evidence of it.

## Target role context

The target environment is industrial automation and critical-infrastructure
software, including:

- modern C++20 and Windows C++
- RAII, ownership, move semantics, and exception safety
- concurrency with `std::thread`, mutexes, condition variables, atomics, safe
  shutdown, and clean lifetime management
- TCP framing, partial reads/writes, timeouts, retries, heartbeats, and backoff
- Boost.Asio: `io_context`, async TCP, strands, timers, cancellation, handler
  lifetime, `shared_from_this`, and graceful shutdown
- Qt/MFC isolation and testable pure C++ service interfaces
- legacy modernization, characterization tests, backward compatibility, and
  incremental refactoring
- SCADA/HMI and DNP3 fundamentals, taught as domain knowledge rather than
  claimed work experience

## Teaching workflow

Teach in Chinese while retaining key English technical terms and concise
interview vocabulary.

Use this sequence:

1. Explain the concept and relevant trade-offs.
2. Ask Hang to restate the idea in his own words.
3. Give a small runnable coding exercise.
4. Review the result at Senior C++ interview standard.
5. Ask an interview follow-up question.
6. End with a concise English interview answer.

For timed coding, give the question, constraints, and acceptance criteria
first. Do not reveal the implementation or full solution until asked.

## Review standard

Review each exercise for:

- correctness and edge cases
- explicit ownership and lifetime
- exception safety
- concurrency safety and absence of data races
- deterministic tests and resistance to flaky timing
- error handling and observability
- cancellation, shutdown, and resource cleanup
- maintainability, testability, and backward compatibility
- clear senior-level interview communication

## Practice sequence

1. RAII socket/file wrapper: non-copyable, movable, and exception-safe.
2. Refactor raw-pointer legacy code to value members or `std::unique_ptr`.
3. Bounded thread-safe queue using `std::condition_variable` and safe shutdown.
4. TCP framing parser handling partial reads, length fields, and invalid packets.
5. Boost.Asio async TCP client/server, followed by timeout, cancellation,
   strand, reconnect, and graceful shutdown exercises.
6. Decouple Qt/MFC business logic into testable pure C++ services and
   interfaces.
7. Add characterization tests, then perform incremental legacy refactoring.
8. Practise SCADA/DNP3 reliability scenarios without overstating experience.

## Repository and coding rules

- Use CMake and C++20.
- Add focused automated tests for every exercise.
- Prefer deterministic synchronization over arbitrary sleeps in concurrency
  tests.
- Keep dependencies minimal. Ask before installing a dependency.
- Preserve existing learner work and ask before destructive changes.
- Make small, focused commits when the directory is a Git repository.
- Do not silently complete learner TODOs unless explicitly asked for the
  solution.
- Keep code and identifiers in English.

## Exclusions

Do not work on job applications, email, scheduling, or unrelated
Biomatters/Geneious Java/DNA preparation in this repository.
