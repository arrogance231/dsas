# Movie Rental System

A Windows desktop application built with C++ and wxWidgets for managing movie rentals, customer data, and rental transactions.

## Features

- **User Authentication**: Login and signup functionality for customers and administrators
- **Movie Management**: Browse, search, and manage movie inventory
- **Rental System**: Rent and return movies with automatic fee calculation
- **Customer Management**: Track customer information and rental history
- **Admin Panel**: Administrative tools for managing the system
- **Receipt Generation**: Automatic receipt generation for rentals and returns

## Prerequisites

Before setting up this project, you'll need to install the following software:

### 1. Visual Studio 2022

1. Download Visual Studio 2022 Community Edition from [Microsoft's official website](https://visualstudio.microsoft.com/downloads/)
2. During installation, make sure to select the following workloads:
   - **Desktop development with C++** - This provides the necessary C++ compiler and build tools
   - **Universal Windows Platform development** - For Windows-specific development features
   - **Windows 10/11 SDK** - Required for Windows application development

### 2. Git

Download and install Git from [https://git-scm.com](https://git-scm.com)
- This is required for cloning the vcpkg package manager

## Installation Steps

### Step 1: Install wxWidgets using vcpkg

wxWidgets is a C++ library for creating desktop applications with native look and feel. We'll install it using vcpkg, Microsoft's C++ package manager.

Open Command Prompt or PowerShell and run the following commands:

#### 1. Clone vcpkg repository
```bash
git clone https://github.com/Microsoft/vcpkg
```
**What this does**: Downloads Microsoft's vcpkg package manager, which is a C++ library manager that simplifies the installation of C++ libraries and their dependencies.

#### 2. Navigate to vcpkg directory
```bash
cd vcpkg
```
**What this does**: Changes the current directory to the vcpkg folder where we'll run the installation commands.

#### 3. Bootstrap vcpkg
```bash
bootstrap-vcpkg.bat
```
**What this does**: Sets up vcpkg by downloading and configuring the necessary build tools. This creates the vcpkg executable and prepares the package manager for use.

#### 4. Install wxWidgets
```bash
vcpkg install wxwidgets
```
**What this does**: Downloads, compiles, and installs the wxWidgets library along with all its dependencies. This process may take several minutes as it compiles the library from source.

#### 5. Integrate vcpkg with Visual Studio
```bash
vcpkg integrate install
```
**What this does**: Configures Visual Studio to automatically find and use libraries installed through vcpkg. This allows the project to find wxWidgets headers and libraries without manual configuration.

### Step 2: Set up the project

1. Clone or download this project to your local machine
2. Open the solution file `WindowsProject1.sln` in Visual Studio 2022
3. Set the `WXWIN` environment variable to point to your wxWidgets installation:
   - Right-click on the project in Solution Explorer
   - Select "Properties"
   - Go to "Configuration Properties" → "VC++ Directories"
   - Add the wxWidgets include and library paths to "Include Directories" and "Library Directories"

### Step 3: Build and Run

1. In Visual Studio, select your desired build configuration (Debug or Release)
2. Select your target platform (x86 or x64)
3. Build the solution by pressing `Ctrl+Shift+B` or clicking "Build" → "Build Solution"
4. Run the application by pressing `F5` or clicking "Debug" → "Start Debugging"

## Project Structure

```
WindowsProject1/
├── assets/                 # Movie posters and data files
│   ├── 🌸 Anime/          # Anime movie posters
│   ├── 🎃 Horror/         # Horror movie posters
│   ├── 💌 Romance/        # Romance movie posters
│   ├── 🔥 Action - Adventure/ # Action movie posters
│   ├── *.json             # Data files (movies, customers, admins)
│   └── *.txt              # Backup data files
├── include/gui/           # Header files for GUI components
├── src/gui/               # Source files for GUI components
├── *.cpp                  # Main application source files
├── *.h                    # Header files
└── WindowsProject1.sln    # Visual Studio solution file
```

## Data Files

The application uses several JSON and TXT files to store data:
- `movies.json/txt` - Movie inventory and details
- `customers.json/txt` - Customer information
- `admins.json/txt` - Administrator accounts
- `data.json/txt` - General application data

## Troubleshooting

### Common Issues

1. **"wxWidgets not found" error**
   - Ensure vcpkg integration is properly set up
   - Check that the `WXWIN` environment variable is set correctly
   - Verify wxWidgets was installed successfully with `vcpkg list`

2. **Build errors related to missing libraries**
   - Make sure you selected "Desktop development with C++" during Visual Studio installation
   - Try rebuilding the solution after a clean build

3. **Runtime errors**
   - Ensure all data files (movies.json, customers.json, etc.) are present in the project directory
   - Check that the application has write permissions in its directory

### Getting Help

If you encounter issues:
1. Check that all prerequisites are installed correctly
2. Verify that vcpkg integration is working
3. Ensure Visual Studio is configured for C++ development
4. Try a clean rebuild of the solution

## System Requirements

- Windows 10 or later
- Visual Studio 2022 (Community Edition is sufficient)
- At least 4GB RAM
- 2GB free disk space for development tools and libraries

## License

This project is for educational purposes. Please ensure you have the necessary licenses for any third-party content used in the application. 