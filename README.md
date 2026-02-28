# HIMS : Hospital Information Management System

A robust, console-based Hospital Information Management System (HIMS) written in C. This project features a modern CLI interface with ANSI colors, smooth animations, and a centralized management system for various medical departments.

![HIMS Logo](https://img.shields.io/badge/Interface-CLI-blue)
![Language](https://img.shields.io/badge/Language-C-green)


---

## 🚀 Features

- **Patient Management**: Complete record keeping, admission, and discharge flows.
- **Doctor Management**: Scheduling and doctor profiles.
- **Department Management**: Management of hospital units.
- **Billing System**: Integrated invoicing and payment tracking.
- **Pharmacy & Inventory**: Medication tracking and inventory management.
- **Appointments**: Scheduling system for patient-doctor consultations.
- **Reporting & Analytics**: Summary reports for hospital operations.
- **Data Persistence**: Automated backup and data saving system.

## 🎨 UI & UX Highlights

- **Dynamic ASCII Logo**: A perfectly aligned "HIMS" header on every screen.
- **ANSI Color Support**: Color-coded menus (Yellow), success messages (Green), and errors (Red).
- **Smooth Animations**: Loading bars and animated text for a premium terminal feel.
- **Intelligent Centering**: Responsive text alignment based on terminal width.

---

## 🛠 Prerequisites

- **Compiler**: `gcc` (installed via Scoop, MSYS2, or Mingw-w64).
- **Build Tool**: `make` (optional) or manual command line.

## 📦 Building the Project

### Using Makefile
If you have `make` installed:
```powershell
make
```

### Manual Compilation
Alternatively, use this one-liner to compile and link all modules:
```powershell
gcc -o hims.exe main.c auth.c patient.c doctor.c department.c billing.c report.c file.c utils.c pharmacy.c appointment.c -I.
```

---

## 🖥 How to Run

1. Open your terminal in the project directory.
2. Execute the binary:
   ```powershell
   ./hims.exe
   ```
3. **Admin Login**:
   - **Username**: `admin`
   - **Password**: `admin123`

---

## ⚙️ Project Structure

- `main.c`: Application entry point and main menu.
- `auth.c`: Admin authentication logic.
- `utils.c`: UI engine, colors, animations, and helper functions.
- `*.c` & `*.h`: Domain-specific modules (Patient, Billing, etc.).
- `data/`: Directory where system data is persisted.
