# Changelog

**v1.0.2 (2026/10/18, DLL and ini)**

- `GetContextHandler` returns 0 when nothing was captured (`AssignCommand` on an invalid object, or not called). Before, it returned the number of the previous capture.
- When `AssignCommand` is not called after `PrepareSaveContext`, `GetContextHandler` disarms the capture. Before, the next `AssignCommand` of any script was captured instead of being executed.
- On server shutdown, the plugin waits for the waiting threads to leave (at most 1 second), instead of a fixed delay.

**v1.0.1 (2025/08/29, DLL)**

- Avoided a possible starvation of the waiting threads.

**v1.0.0 (2025/08/09, DLL and ini)**

- First version.
