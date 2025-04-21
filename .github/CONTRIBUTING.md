# Contributing to PruneCopy

Thank you for considering contributing to **PruneCopy**! This project thrives on clean design, useful features, and well-tested code. Whether you're fixing bugs, suggesting improvements, or submitting pull requests – you're awesome. 🚀

---

## 💡 How to Contribute

There are many ways to contribute:

- 🐞 **Bug Reports**: Open an issue with a clear reproduction case.
- 💬 **Feature Requests**: Got a cool idea? Open an issue with justification.
- 🔧 **Code Contributions**: Fix bugs, improve performance, add CLI features, or enhance test coverage.
- 🧪 **Testing**: Extend unit tests (see `/Source/test`) or improve test utilities.
- 📚 **Documentation**: Help improve the README or add usage examples.

---

## 🧰 Development Setup

1. **Clone the repository**:
   ```bash
   git clone https://github.com/PatrikNeunteufel/PruneCopy.git
   ```

2. **Open the solution in Visual Studio (Community Edition works)**

3. **Build configuration**: Use `Release` or `Debug` as appropriate

4. **Run tests**:
   - Built-in test runner: Call `TestRunner::run()` from `main.cpp`
   - Unit tests are located under `Source/test/`

---

## 🧼 Code Style & Structure

- **Language**: C++
- **Compiler**: MSVC (Visual Studio)
- **Encoding**: UTF-8
- **Header Style**: `#pragma once` (no include guards)
- **Member Variables**: Use `m_` prefix
- **Enums**: Prefer `enum class` with descriptive names
- **Logging**: Use `LogManager` (not `std::cout`)
- **Test Assertions**: Use `TestUtils::{assertTrue, assertFalse, assertEqual, assertContains}`

> Follow the architecture and conventions you see in `Source/core/`, `Source/cli/`, and `Source/test/`.

---

## ✅ Pull Request Checklist

- [ ] Code compiles and passes all tests
- [ ] No use of global variables or hardcoded paths
- [ ] No use of `printf` / `cout` (use `LogManager`)
- [ ] Includes necessary comments and documentation
- [ ] Adds tests for new features or behaviors
- [ ] Uses consistent style (variable names, indentation, etc.)

---

## 🤝 Code of Conduct

Please read and follow our [Code of Conduct](./CODE_OF_CONDUCT.md).

---

## 🧠 Need help?

Feel free to [open an issue](https://github.com/PatrikNeunteufel/PruneCopy/issues) or start a discussion!

---

**Happy coding – and thanks for helping make PruneCopy better!**