# SerendipityBookstore
# Serendipity Booksellers - Point-of-Sale & Inventory Management System

Point-of-Sale (POS) and inventory database system designed for a fictional bookseller. This system simulates real-world workflow, enabling cashiers to process customer transactions and managers to modify the book database, manage stock thresholds, and generate financial asset reports.

## ⚙️ Core Functional Modules

The application is structured into three main operational subsystems:

* **Cashier Module (POS):** Handles real-time customer checkouts. It prompts for a target book title or ISBN, verifies stock on hand, calculates subtotal, applies Maryland sales tax (6%) parameters, prints a receipt, and automatically updates inventory stock every transaction.
  
* **Inventory Database Module:** For inventory tracking. Authorized users can look up existing records or change specific database fields, including:
  * Book Title & International Standard Book Number (ISBN)
  * Author & Publisher details
  * Wholesale Cost and Retail Price
  * Date Added & Current Quantity on Hand

* **Reports Module:** Generates automated bookkeeping and business analytics dashboards for management, including:
  * Inventory List
  * Total Wholesale and Retail value
  * Quantity-on-Hand (identifying low-stock items)
  * Chronological sorting

## 🛠️ Technical Design & Data Persistence

* **Structured Data :** Uses object structures to encapsulate all attributes of a book record into clean, modular instances.
* **File-System Storage & Persistence:** Implements File I/O. The database fully populates itself by reading from `inventory.txt` when the application boots, and safely updates all  modifications, transaction logs, and additions when the system exits.
* **Array Optimization & Sorting Algorithms:** Utilizes sorting and searching algorithms (Bubble Sort and Selection Sort) to dynamically reorganize database records in memory.

## 🚀 Execution Instructions

1. Use a standard modern C++ compiler (such as `g++` or Visual Studio) on your host environment.
2. Compile the source file bundle:
   ```bash
   g++ mainmenu.cpp -o SerendipityPOS
