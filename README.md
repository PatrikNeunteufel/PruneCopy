# PruneCopy

**Selective file copier for developers.**  
A powerful CLI utility for pattern-based file copying and filtering – ideal for post-build tasks and deployment preparation.

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

---

## 🛠️ Example usage

```bash
prunecopy ./build ./release --types *.h *.cpp --exclude-dirs test build --log-dir ./logs --log-open
```

---

## 🚧 Coming Soon

- Parallel execution (`--parallel-async`, `--parallel-thread`, `--parallel-openMP`)
- Flattened copy output (`--flatten`, `--flatten-suffix`)
- Optional installer (with PATH integration)

These options are visible or planned but currently not implemented.

---

## 💖 Support the Project

If you find this tool useful, consider supporting development:

- `--donate` shows donation options (e.g. [Patreon](https://patreon.com/deinname), [Ko-Fi](https://ko-fi.com/prunecopy))
- `--sponsors` shows the current supporter list
- Become a named supporter in the CLI via [`sponsors.json`](https://raw.githubusercontent.com/PatrikNeunteufel/PruneCopy/main/sponsors/sponsors.json)

---

## 📦 Installation

1. Clone the repository  
2. Run `vcpkg install` to install dependencies (`cpr`, `nlohmann-json`)  
3. Open the solution in Visual Studio 2022  
4. Build & go 🚀

Installer is planned for future releases.

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

