---
pdf_options:
  format: A4
  margin: 18mm 16mm 20mm 16mm
  outline: true
  displayHeaderFooter: true
  headerTemplate: "<span></span>"
  footerTemplate: "<div style='width:100%; font-size:9px; font-family:Arial; color:#666; text-align:center;'>xp_threadSynchro – <span class='pageNumber'></span> / <span class='totalPages'></span></div>"
---

<style>
.cover-title { padding-top: 80px; }
.toc { margin-top: 40px; }
/* Long code examples: smaller code blocks, so that lines are not wrapped */
pre, pre code { font-size: 8.5pt; tab-size: 4; -moz-tab-size: 4; }
</style>

<div class="cover-title">
<div class="cover-heading">xp_threadSynchro</div>
<div class="cover-version">V1.0.2 – 2026-10-18 by Septirage</div>
</div>

<div class="toc">

<div class="toc-title">Table of contents</div>

<!-- TOC -->

</div>

<div class="pagebreak"></div>

# Guide for Admin

## What is xp_threadSynchro

The NWN2 server runs the scripts on a single thread, its main thread. A plugin that works in another thread (a web request, a database query…) cannot run a script itself without risking a crash.

xp_threadSynchro lets such a plugin have a script executed by the main thread, safely. It has no nwscript function: it is a building block for other plugins and systems. Install it when a plugin or a system asks for it.

## Requirements

To use the xp_threadSynchro plugin you will need:

- [Nwnx4](https://github.com/nwn2dev/nwnx4)

## Installation of xp_threadSynchro

Put `xp_threadSynchro.dll` and `xp_threadSynchro.ini` in your nwnx4 folder.

Look at our website (<https://septirage.com/nwn2/>) for an overview.

## xp_threadSynchro.ini

<div class="options">

| Option | Default | Description |
|---|---|---|
| `loglevel` | 3 | Log verbosity, written to `xp_threadSynchro.txt`: 0 none, 1 error, 2 warning, 3 info, 4 debug, 5 trace. |
| `AllowThreading` | 0 | Maximum number of threads that can wait at the same time to have a script executed, from 1 to 10. 0 (or absent) disables the plugin. The provided file sets 5. |

</div>

<div class="pagebreak"></div>

# Guide for Developers

## How it works

A script is executed in two steps.

1. **Capture**, on the main thread, during a script: your plugin arms the capture, then the script calls `AssignCommand`. The action given to `AssignCommand` is not queued: the plugin keeps it and gives it a number (handler).
2. **Insertion**, later, from any thread: your plugin waits for its turn, then inserts the action with its handler. The main thread executes it at its next loop.

To let the threads in safely, the main thread pauses once per loop. A thread that wants to insert waits for this pause; up to `AllowThreading` threads can wait at the same time, the others wait before.

## Functions

The functions are C functions (`__cdecl`) exported by `xp_threadSynchro.dll`. Load them with `DllImport` from C#, or with `GetProcAddress` from a plugin.

<!-- MINITOC -->

#### PrepareSaveContext

```c
int PrepareSaveContext();
```

Main thread only. Arms the capture: the next `AssignCommand` is captured instead of being queued.

Returns 1, or 0 when called from another thread.

#### GetContextHandler

```c
uint32_t GetContextHandler();
```

Main thread only, just after the `AssignCommand`. Returns the handler of the captured action.

Returns 0 when nothing was captured (`AssignCommand` on an invalid object, or not called), or when called from another thread. If `AssignCommand` was not called, it also disarms the capture.

#### BeginContextInsertion

```c
int BeginContextInsertion();
```

Any thread. Waits until the thread can insert: at most one loop of the main thread. From the main thread, it returns immediately.

Returns 1: call `InsertContext`, then `EndContextInsertion`. Returns 0: the plugin is disabled (`AllowThreading = 0`) or the server is shutting down; do not insert, and do not call `EndContextInsertion`.

#### InsertContext

```c
int InsertContext(uint32_t iHandler);
```

Between `BeginContextInsertion` and `EndContextInsertion`. Inserts the captured action: it is executed on the module, at the next loop of the main thread. A handler can only be inserted once.

Returns 1, -1 when not called between `BeginContextInsertion` (returning 1) and `EndContextInsertion`, -2 for an unknown handler (already inserted, or never captured).

#### EndContextInsertion

```c
int EndContextInsertion();
```

After `BeginContextInsertion` returned 1. Gives the hand back to the main thread.

Returns 1, 0 when the server is shutting down, -1 when not called after `BeginContextInsertion`.

## Examples

### In C# (CLR)

In C#, the action is a delegate: it carries everything it needs (the result, the objects…), and the C# code calls `AssignCommand` itself. This is the code used by SEPTI (`SeptiThreading.cs`): `NWNActionHandle` keeps the handler, and is invalidated once inserted, so that it cannot be inserted twice.

<div class="keep">

```csharp
public class NWNActionHandle
{
	private uint _handle;

	internal NWNActionHandle(uint handle) { _handle = handle; }

	internal uint Handle { get { return _handle; } }
	public bool IsValid { get { return _handle > 0; } }

	internal void Invalidate() { _handle = 0; }

	public static readonly NWNActionHandle Invalid = new NWNActionHandle(0);
}
```

</div>

<div class="keep">

```csharp
public class NWNScriptThreadingUtilities
{
	[DllImport("xp_threadSynchro.dll")] private static extern int BeginContextInsertion();
	[DllImport("xp_threadSynchro.dll")]
	private static extern int InsertContext(uint iHandler);
	[DllImport("xp_threadSynchro.dll")] private static extern int EndContextInsertion();
	[DllImport("xp_threadSynchro.dll")] private static extern int PrepareSaveContext();
	[DllImport("xp_threadSynchro.dll")] private static extern uint GetContextHandler();

	// On the main thread, in a script: capture the action
	public static NWNActionHandle PrepareAssignCommandContext(
		CLRScriptBase.NWAction myAction, CLRScriptBase script)
	{
		if (PrepareSaveContext() != 1)
			return NWNActionHandle.Invalid;
		script.AssignCommand(script.GetModule(), myAction);
		return new NWNActionHandle(GetContextHandler());
	}

	// Later, in any thread: have the action executed by the main thread
	public static int ExecuteAssignCmdContext(NWNActionHandle handler)
	{
		if (!handler.IsValid)
			return 0;	// invalid handle
		if (BeginContextInsertion() == 0)
			return -1;	// shutdown asked

		// Begin succeeded: a failure here means the handle is no longer valid.
		// EndContextInsertion must still be called.
		int iRes = InsertContext(handler.Handle);

		handler.Invalidate();	// the handle is consumed

		if (EndContextInsertion() != 1)
			return -1;	// shutdown asked

		return (iRes == 1) ? 1 : 0;
	}
}
```

</div>

### In a native plugin

The functions can also be used from a native plugin (`GetProcAddress`). The action must still be captured by a real `AssignCommand` in a script, for example a small script run with `ExecuteScriptEnhanced`.

## Rules of use

- **Same script.** `PrepareSaveContext`, `AssignCommand` and `GetContextHandler` must follow each other in the same script, on the main thread. Always call `GetContextHandler`, even when the `AssignCommand` was skipped: it disarms the capture.
- **The target of AssignCommand** must exist, or nothing is captured. It is not used afterwards: the action is always executed on the module (`OBJECT_SELF` is the module). `GetModule()` is a good choice.
- **Insert every capture.** A captured action stays in memory until it is inserted.
- **Keep the insertion short.** Between `BeginContextInsertion` and `EndContextInsertion`, the main thread is paused: only call `InsertContext` (once or several times), never a blocking call.
- **Always close.** After `BeginContextInsertion` returned 1, always call `EndContextInsertion`, or the server stays paused.
- **Shutdown.** When the server shuts down, `BeginContextInsertion` returns 0 and the waiting threads are released. The plugin waits at most 1 second for them.
- **NWScript JIT.** The plugin works with the NWScript JIT plugin: the captured action already contains its state. A `CLEAR SCRIPT CACHE` between the capture and the insertion has not been tested.

<div class="pagebreak"></div>

<!-- INCLUDE CHANGELOG.md -->
