# International Shipping Company Management System

A CLI-based shipping management system built in C++ as a Data Structures course project.

**Course:** Data Structures — BSSE Semester III  
**University:** Riphah International University, Islamabad  
**Instructor:** Dr. M Islam Abbasi  

**Team:**
- Abdalrahman Alashi (71580) — Team Lead
- Ahmed Hamdan (71648)
- Mostafa Hassouna (71601) 


---

## Overview

Simulates the operations of an international shipping company. The system handles package tracking, priority-based processing, and shipment management through a command-line interface — no GUI, no database, no networking.

---

## Features

### Main Menu
| Option | Description |
|--------|-------------|
| Admin Login | Access the full management panel (password protected) |
| Track a Shipment | Enter a shipment ID to view its status — no login needed |
| Exit | Close the application |

### Admin Panel
| Option | Description |
|--------|-------------|
| Add Shipment | Register a new shipment with sender, receiver, destination, weight, and priority |
| Delete Shipment | Remove a shipment by ID (supports undo) |
| Display All Shipments | View all shipments in a formatted table |
| Process Next Shipment | Process the next shipment from the queue (high priority first) |
| Mark as Delivered | Update a shipment's status to Delivered |
| Sort Shipments | Sort by weight, priority, or destination |
| Undo Last Delete | Restore the most recently deleted shipment |

### Public Tracking
Anyone can track a shipment using only its ID — no account required.

---

## Data Structures Used

| Structure | Purpose |
|-----------|---------|
| **Linked List** | Dynamic storage for all shipment records |
| **Priority Queue** | Ensures high-priority shipments are processed first |
| **Queue (FIFO)** | Handles normal shipments in order of arrival |
| **Stack** | Powers the undo feature for deleted shipments |
| **Vector + Sort** | Used for sorting shipments by different criteria |

---

## How to Run

### Requirements
- C++17 or later
- g++ compiler (or Visual Studio)

### Compile & Run (terminal)
```bash
g++ -std=c++17 -o shipping_system shipping_system.cpp
./shipping_system
```

### Visual Studio
Open `shipping_system.cpp` and press `Ctrl + F5`.

---

## Usage Example

```
==========================================
  International Shipping Management System
==========================================

+==========================+
|        MAIN MENU         |
+==========================+
|  1. Admin Login          |
|  2. Track a Shipment     |
|  0. Exit                 |
+==========================+
```

**Admin password:** `admin123`

Shipment IDs start from `1001` and increment automatically.

---

## Project Structure

```
shipping_system.cpp   # All source code (single file)
README.md             # This file
```

---

## License

Academic project — Riphah International University, 2026.
