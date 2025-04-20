
## 🔍 Use Case Comparison Table: PruneCopy vs. CLI Copy Tools

| Use Case                                                                 | PruneCopy          | Robocopy | rsync  | FastCopy | TeraCopy | UltraCopier | Copy Handler | RichCopy | GS RichCopy 360 |
|--------------------------------------------------------------------------|--------------------|----------|--------|----------|----------|--------------|---------------|----------|-----------------|
| **Basic copy of files or folders**                                      | ✅                 | ✅       | ✅     | ✅       | ✅       | ✅           | ✅            | ✅       | ✅              |
| **Recursive directory copy**                                            | ✅                 | ✅       | ✅     | ✅       | ✅       | ✅           | ✅            | ✅       | ✅              |
| **File type filtering (e.g. only *.h)**                                 | ✅                 | ✅       | ✅     | ✅       | ⚠️       | ⚠️           | ⚠️            | ❌       | ✅              |
| **Excluding files/folders with wildcards**                              | ✅ (with wildcards)| ⚠️       | ✅     | ⚠️       | ❌       | ⚠️           | ⚠️            | ❌       | ✅              |
| **Copy only if source file is newer**                                   | ⚠️ (planned)       | ✅       | ✅     | ✅       | ❌       | ❌           | ❌            | ✅       | ✅              |
| **Preserve directory structure**                                        | ✅                 | ✅       | ✅     | ✅       | ✅       | ✅           | ✅            | ✅       | ✅              |
| **Flattening to a single target directory**                             | ⚠️ (planned)       | ❌       | ⚠️     | ⚠️       | ❌       | ❌           | ❌            | ❌       | ❌              |
| **Filename conflict handling (suffix/overwrite)**                       | ✅ (suffix planned)| ✅       | ✅     | ⚠️       | ✅       | ⚠️           | ❌            | ⚠️       | ✅              |
| **Simulation / dry-run mode**                                           | ✅                 | ❌       | ✅     | ❌       | ❌       | ❌           | ❌            | ❌       | ✅              |
| **Logging to console & file with log level filters**                    | ✅                 | ✅       | ✅     | ⚠️       | ⚠️       | ⚠️           | ❌            | ⚠️       | ✅              |
| **Colored console output for statuses**                                 | ✅ (ANSI/Windows)  | ⚠️       | ⚠️     | ❌       | ✅       | ✅           | ❌            | ❌       | ✅              |
| **Parallel copying (threads, async, OpenMP)**                           | ⚠️ (planned)       | ✅       | ✅     | ✅       | ❌       | ❌           | ❌            | ❌       | ✅              |
| **Cross-platform support (Linux, macOS, Windows)**                      | ⚠️ (planned)       | ❌       | ✅     | ❌       | ❌       | ✅           | ❌            | ❌       | ❌              |
| **Presets / configurable profiles**                                     | ⚠️ (planned)       | ⚠️       | ✅     | ❌       | ⚠️       | ❌           | ❌            | ❌       | ✅              |
| **GUI available (optional)**                                            | ❌                 | ⚠️       | ⚠️     | ✅       | ✅       | ✅           | ✅            | ✅       | ✅              |
| **Open source**                                                         | ✅ (MIT)           | ❌       | ✅     | ✅       | ❌       | ✅           | ✅            | ❌       | ❌              |
| **Copying to remote targets (e.g. UNC, SSH, network drives)**           | ⚠️ (planned)       | ✅ (UNC) | ✅ (SSH, rsyncd) | ⚠️ (UNC only) | ❌       | ⚠️ (partial)    | ❌            | ❌       | ✅              |
| **Multiple source and destination directories**                         | ✅                 | ⚠️ (batch only) | ⚠️ (via scripts) | ❌       | ❌       | ❌           | ❌            | ❌       | ✅              |

---

## 🏁 Recommendation – Best Tools by User Group

| User Group                                   | Best Tool(s)                                            | Justification                                                             |
|---------------------------------------------|----------------------------------------------------------|---------------------------------------------------------------------------|
| **SysAdmins / DevOps (Windows-only)**       | **Robocopy**, **PruneCopy**                             | Robocopy: proven, fast, stable. PruneCopy: modern, flexible, logging      |
| **Cross-platform users (Linux/macOS/Win)**  | **rsync**, **UltraCopier**                              | Multi-platform, rsync with SSH, UltraCopier with GUI                      |
| **Power users / developers with automation**| **PruneCopy**, **rsync**, **FastCopy**                  | PruneCopy: precise & scriptable, rsync: established                       |
| **Beginners preferring GUI**                | **TeraCopy**, **FastCopy**, **UltraCopier**             | Easy to use, visual feedback                                              |
| **Enterprises with professional demands**   | **GS RichCopy 360**, **PruneCopy (with CI integration)**| GS: commercial-grade, PruneCopy: flexible CI/CD integration               |
| **Open source enthusiasts / community devs**| **PruneCopy**, **rsync**, **Copy Handler**              | Free, extensible, actively maintained                                     |
