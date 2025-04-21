# PruneCopy

**Selective file copier for developers.**  
A powerful CLI utility for pattern-based file copying and filtering – ideal for post-build tasks and deployment preparation.

the idea for this tool was born out of the need to copy only specific files from a source directory to a target directory, while excluding certain directories and files based on patterns. This is particularly useful in scenarios where you want to prepare a subset of files for deployment or distribution, such as copying only header files or excluding implementation files. e.g. the includes to use a dll does not need the implementation files as `*Impl.hpp`.

The tool is designed to be flexible and extensible, allowing users to customize the file selection process according to their specific needs.


> 🆕 **v1.0.4 Released!**  
> Download the latest version as a ZIP or Installer [from GitHub Releases](https://github.com/PatrikNeunteufel/PruneCopy/releases).  


---

## ✨ Features (Active)

- Filter files by extension (`--types`)
- Exclude directories (`--exclude-dirs`) and files (`--exclude-files`)
- Dry-run mode (`--dry-run`) to simulate without copying
- Optional logging to file (`--log-dir`, `--log-open`)
- Console output control (`--cmdln-out-off`, `--log-level`, `--color`)
- Overwrite behavior (`--no-overwrite`, `--force-overwrite`)
- Delete target before copy (`--delete-target-first`)
- Random supporter display (`--about` with optional `--no-network`)
- Full sponsor listing (`--sponsors`)
- Donate and contact info (`--donate`, `--contact-dev`)

new features in v1.0.2:
- multiple source folders (`--source <sourcepaths>` instead of `<sourcepath>`)
- multiple destination folders (`--destination <destpaths>` instead of `<destpath>`)
- check for updates (`--update`)

new features in v1.0.3:
- added `--preset` to load a preset file
- added `--save-preset` to save the current command line arguments as a preset
- added `--show-preset` to show the presets command line arguments
- added `--list-presets` to list all available presets
- added more log levels, they now work hirarchically All > Standard > Info > Warning > Error > None
- added deprecated info when using deprecated flags

new features in v1.0.4:
- added `--flatten` to flatten the directory structure in the destination
- added `--flatten-suffix` to add a suffix to the flattened files
- added `--flatten-auto-rename` to automatically rename files if they already exist in the destination

deprecated features:
- `--cmdln-out-off`: replaced by `--log-level none`

---

## 🛠️ Example usage

```bash
prunecopy ./source ./build/includes --types *.h *.hpp --exclude-dirs pch --exclude-files *Impl.* --log-dir ./logs --log-open
```

---

## 🚧 Coming Soon

- Parallel execution (`--parallel-async`, `--parallel-thread`, `--parallel-openMP`)
- Flattened copy output (`--flatten`, `--flatten-suffix`)
- only copy newer files (`--only-newer`)
- eventually `--remote` for copying to remote servers via SSH/SFTP

These options are visible or planned but currently not implemented.

---

## 💖 Support the Project

If you find this tool useful, consider supporting development:

- `--donate` shows donation options (e.g. [Patreon](https://patreon.com/PruneCopy), [Ko-Fi](https://ko-fi.com/prunecopy))
- `--sponsors` shows the current supporter list
- Become a named supporter in the CLI via [`sponsors.json`](https://raw.githubusercontent.com/PatrikNeunteufel/PruneCopy/master/sponsors/sponsors.json)

---

## 📦 Installation

1. Clone the repository  
2. Run `vcpkg install` to install dependencies (`cpr`, `nlohmann-json`)  
3. Open the solution in Visual Studio 2022  
4. Build & go 🚀

or just get the PrunCode.zip or Installer [from GitHub Releases](https://github.com/PatrikNeunteufel/PruneCopy/releases).

---

## 📄 License

This project is **dual-licensed** under either:

- [MIT License](./LICENSE)
- [Apache License 2.0](./LICENSE)

You may choose the license that best fits your use case.

---

## 👤 Author

**Patrik Neunteufel**  
📫 Contact via `--contact-dev` or GitHub issues.

