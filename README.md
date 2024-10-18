# Bank Management System

This project is a **Bank Management System** implemented in **C++**, demonstrating various object-oriented programming concepts such as **inheritance**, **polymorphism**, **templates**, and **dynamic casting**. The system manages different types of bank accounts, allowing you to create, store, and display information related to these accounts.

## Features

- Create and manage different types of accounts:
  - Savings Account
  - Salary Account
  - Other custom account types
- Display account details based on account type using **templates** and **dynamic casting**.
- Simple, flexible architecture that allows easy extension to new account types.
- Supports listing and filtering of accounts by specific types.

## Project Structure

```
/BankManagementSystem
│
├── src/
│   ├── main.cpp           # Main application entry point
│   ├── Account.h          # Base class for Account
│   ├── SavingsAccount.h   # Derived class for Savings Account
│   ├── SalaryAccount.h    # Derived class for Salary Account
│   ├── AccountFactory.h   # Factory for creating accounts
│   └── AccountManager.h   # Handles operations on accounts
│
├── README.md              # Project description and guide
└── Makefile               # Makefile for compiling the project
```

## Installation

1. **Clone the repository:**

```bash
git clone https://github.com/your-username/BankManagementSystem.git
```

2. **Navigate to the project directory:**

```bash
cd BankManagementSystem
```

3. **Build the project:**

Use the provided `Makefile` to build the project (if applicable):

```bash
make
```

Alternatively, compile manually using a C++ compiler (e.g., `g++`):

```bash
g++ -o bankSystem src/*.cpp
```

4. **Run the application:**

```bash
./bankSystem
```

## Usage

1. **Creating Accounts**:
   - Use the `createAccount` function template to create accounts of different types. For example, to create a `SavingsAccount`:

     ```cpp
     Account* savings = createAccount<SavingsAccount>(/* account details */);
     ```

2. **Displaying Accounts**:
   - The `displayAccounts` function template allows you to filter and display accounts of a specific type:

     ```cpp
     displayAccounts<SavingsAccount>(accounts);
     ```

3. **Dynamic Casting**:
   - The system dynamically checks the account type during runtime using `dynamic_cast`, ensuring that only the correct account types are processed.

### Example:

```cpp
list<Account*> accounts;
accounts.push_back(createAccount<SavingsAccount>("John Doe", 1000.0));
accounts.push_back(createAccount<SalaryAccount>("Jane Smith", 2000.0));

displayAccounts<SavingsAccount>(accounts);  // Display only savings accounts
```

## Dependencies

- C++11 or higher
- Standard Template Library (STL) for managing lists and dynamic memory.

## Contributions

Contributions are welcome! Feel free to open issues or submit pull requests to help improve the project.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Contact

If you have any questions or suggestions, feel free to reach out at [tahamustafahaidery@gmail.com](mailto:tahamustafahaidery@gmail.com).