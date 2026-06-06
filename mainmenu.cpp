#include <iostream>
#include <iomanip>
#include <string>
#include <cctype>
#include <cstring>
#include <fstream>
#include <cstdlib>
#include <new>
#include "mainmenu.h"
#include "InventoryManager.h"

using namespace std;

//bookstore cashier module, inventory module, and reports module. Let's user buy books, annd keep and edit book inventory. Can show specific book information

const int SIZE = 20;

double SoldBook::taxRate = 0.06;
double SoldBook::total = 0.0;

int main() {
    int choice = 0;
    //try to open inventory.txt in input, output, and binary mode. Put main menu logic below bec the file will be destroyed after try block
	try {
		InventoryManager supplyFile("inventory.txt");

        //main menu
        while (choice != 4) {
            cout << "\n\tSerendipity Booksellers\n\t\tMain Menu\n";
            cout << "\t1. Cashier Module\n";
            cout << "\t2. Inventory Database Module\n";
            cout << "\t3. Report Module\n";
            cout << "\t4. Exit\n";
            cout << "\n\tEnter Your Choice: ";

            cin >> choice;

            //validate input
            while (choice < 1 || choice > 4) {
                cout << "\n\tPlease enter a number in the range of 1-4.";
                cout << "\n\tEnter Your Choice: ";
                cin >> choice;
            }

            cin.ignore();


            //open different modules based on user input
            switch (choice) {
                case 1:
                    cashier(supplyFile);
                    break;
                case 2:
                    invmenu(supplyFile);
                    break;
                case 3:
                    reports(supplyFile);
                    break;
            }
        }

	}
	catch (string errorMessage) {
		cout << errorMessage << endl;
        exit(EXIT_FAILURE);
	}
    return 0;
}

void cashier(InventoryManager& supplyFile) {
    //class object
    InventoryBook book;

	//user input and computing variables
    char date[11], isbnUser[14];
    int numBooks;

	//keeps subtotal and tax totals for all books in the cart
	double preTaxTotal = 0.0;
	double totalTaxed = 0.0;

    //set total to 0 every time the function is called
    SoldBook::setTotal(0.0);

	//clear error characters and reset to the beginning of the file
    supplyFile.reset();

	//cashier module
    cout << "\nSerendipity Booksellers\nCashier Module\n";

	//get date from user
    cout << "Date (MM/DD/YYYY): ";
    cin.getline(date, 11);

    cout << "How many different titles are being purchased? ";
    cin >> numBooks;
    cin.ignore();

	while (numBooks< 1 || numBooks > SIZE) {
		cout << "Please enter a number between 1 and " << SIZE << ": ";
		cin >> numBooks;
		cin.ignore();
	}
    //keeps track of user's cart

    SoldBook* cart = nullptr;
    try {
        cart = new SoldBook[numBooks];
    }
    catch (bad_alloc) {
		cout << "MEMORY ALLOCATION FAILED. UNABLE TO CREATE CART." << endl;
        exit(EXIT_FAILURE);
    }

	// loop until user is done buying or cart is full
    for (int index = 0; index < numBooks; index++) {

		//get isbn from user
        cout << "ISBN: ";
        cin.getline(isbnUser, 14);
        
		//find the book in the file
        long pos = supplyFile.findISBN(isbnUser, book);

		//if book isn't found, ask user if they want to try again
        if (pos == -1) {
            cout << "ISBN not found.\n";

            //so invalid input doesn't count.
            index--;

            continue;
        }
        else{
            int qty;

		//ask user for quantity and check if enough stock
            cout << "Qty: ";
            cin >> qty;
            cin.ignore();

			//if user tries to buy more than what's in stock, show error message and exit the function
            if (qty > book.getQty()) {
                cout << "Not enough stock.\n";
                index--;
                continue;
            }
            else {
				//update quantity in file
                book.setQty(book.getQty() - qty);
                supplyFile.updateBook(pos, book);

				//add book to cart
                cart[index].setTitle(book.getTitle());
                cart[index].setISBN(book.getISBN());
				cart[index].setQtySold(qty);
				cart[index].setRetail(book.getRetail());

                //calculate tax and subtotal of the book and update total.
                cart[index].calcPrice(qty);

				//add to pre-tax total
				preTaxTotal += qty * book.getRetail();
				totalTaxed += cart[index].getTax();
            }
        }
    }


	//print receipt
    cout << "\nSerendipity Book Sellers" << endl;
    cout << "\nDate: " << date << endl;

    cout << "Qty" << setw(7) << "ISBN" << setw(25) << "Title"
        << setw(48) << "Price" << setw(19) << "Total" << endl;

    cout << fixed << setprecision(2);

    for (int index = 0; index < numBooks; index++) {

        for (int i = 0; i < 106; i++) {
            cout << "_";
        }

        cout << endl;

        cout << cart[index].getQtySold() << " " << cart[index].getISBN()
            << setw(25) << " " << cart[index].getTitle()
            << setw(14) << "$" << setw(6) << cart[index].getRetail()
            << setw(19) << "$" << setw(6) << cart[index].getSubtotal() << endl;
    }

    cout << "\n" << setw(31) << "Subtotal"
        << setw(64) << "$" << setw(6) << preTaxTotal << endl;

    cout << setw(31) << "Tax"
        << setw(64) << "$" << setw(6) << totalTaxed << endl;

    cout << setw(31) << "Total"
        << setw(64) << "$" << setw(6) << SoldBook::getTotal() << endl;

    cout << "\nThank You for Shopping at Serendipity!" << endl;

    //delete dynamic array to free up memory
    delete[] cart;
}


void invmenu(InventoryManager& supplyFile) {
    int choice = 0;

	//inventory menu
    while (choice != 5) {
        cout << "\n\tSerendipity Booksellers\n";
        cout << "\t\tInventory Database" << endl;
        cout << "\t1. Look Up a Book" << endl;
        cout << "\t2. Add a Book" << endl;
        cout << "\t3. Edit a Books's Record" << endl;
        cout << "\t4. Delete a Book" << endl;
        cout << "\t5. Return to the Main Menu" << endl;

        cout << "\n\tEnter Your Choice: ";
        cin >> choice;

		//validate input
        while (choice < 1 || choice > 5) {
            cout << "\n\tPlease enter a number in the range of 1-5." << endl;
            cout << "\n\tEnter Your Choice: ";
            cin >> choice;
        }

		//open different inventory functions based on user input
        switch (choice) {
            case 1:
                lookUpBook(supplyFile);
                break;
            case 2:
                addBook(supplyFile);
                break;
            case 3:
                editBook(supplyFile);
                break;
            case 4:
                deleteBook(supplyFile);
                break;
        }
    }
}

void lookUpBook(InventoryManager& supplyFile) {
    InventoryBook book;
    char searchTitle[51];
    char choice;
    long currentPos = 0;
    bool found = false;

	//clear error characters and reset to the beginning of the file
    supplyFile.reset();

	//look up book by title
    cin.ignore();
    cout << "\n\tEnter the title of the book you would like to look up: ";
    cin.getline(searchTitle, 51);
    strUpper(searchTitle);

	//search file for book title and show information if found, otherwise show error message
    while ((currentPos = supplyFile.findBook(searchTitle, book, currentPos)) != -1) {
		// ask user if the book found is the one they are looking for
        cout << "Is " << book.getTitle() << " the book you have been searching for(y/n)? ";
        cin >> choice;
        cin.ignore();

		// if user confirms, show book information and exit the function
        if (toupper(choice) == 'Y') {
            bookInfo(book);
            found = true;
            break;
        }
		// if user doesn't confirm, keep searching for the book title
        currentPos += sizeof(InventoryBook);
    }

	// if book isn't found, show error message
    if (!found) {
        cout << "\n\tThis book isn't in the inventory" << endl;
    }
}

void addBook(InventoryManager& supplyFile) {
	// class objects
    InventoryBook book;
    InventoryBook newBook;

	// user input variables
    int qty;
    double wholesaleUser, retailUser;
    char title[51], isbn[14], author[31], publisher[31], date[11];

    //flag and position variables
    bool foundEmpty = false;
    long position = 0;

	// clear error characters and reset to the beginning of the file
    supplyFile.reset();

    // scan file sequentially
    while (supplyFile.readNext(book)) {
		// if an empty record is found, set flag and break loop
        if (book.isEmpty()) {
            foundEmpty = true;
            break;
        }
		// if not empty, move to the next record
        position += sizeof(InventoryBook);
    }

	// get book information from user
    cin.ignore();
    cout << "\n\tEnter the book title: ";
    cin.getline(title, 51);
    strUpper(title);
    newBook.setTitle(title);

    cout << "\n\tEnter the ISBN: ";
    cin.getline(isbn, 14);
    strUpper(isbn);
    newBook.setISBN(isbn);

    cout << "\n\tEnter the author's name: ";
    cin.getline(author, 31);
    strUpper(author);
    newBook.setAuthor(author);

    cout << "\n\tEnter the publisher: ";
    cin.getline(publisher, 31);
    strUpper(publisher);
    newBook.setPub(publisher);

    cout << "\n\tEnter the date added: ";
    cin.getline(date, 11);
    newBook.setDateAdded(date);

    cout << "\n\tEnter quantity on hand: ";
    cin >> qty;
    newBook.setQty(qty);

    cout << "\n\tEnter wholesale cost: ";
    cin >> wholesaleUser;
    newBook.setWholesale(wholesaleUser);

    cout << "\n\tEnter retail price: ";
    cin >> retailUser;
    newBook.setRetail(retailUser);

    // write to correct location
    if (foundEmpty) {
		//full an empty spot in the file
        supplyFile.updateBook(position, newBook);
    }
    else {
		//add to the end of the file
        supplyFile.addNewBook(newBook);
    }
}

void editBook(InventoryManager& supplyFile) {
	// class object
    InventoryBook book;

	// user input variables
    char searchTitle[51];
    char confirm;

    int editChoice;
    int qty;
    double price;
    char temp[51];

	// flag variable
    bool found = false;

    cout << "\n\tSerendipity Booksellers\n";
    cout << "\t\tEdit Book\n";

	//ask user to enter the title of the book they want to edit and search for it in the file.
    cin.ignore();
    cout << "\n\tEnter book title: ";
    cin.getline(searchTitle, 51);
    strUpper(searchTitle);

	//clear error characters and reset to the beginning of the file
    supplyFile.reset();

    long currentPos = 0;

	//search file for book title and ask user if the book found is the one they want to edit. If user confirms, show edit menu, otherwise keep searching for the book title. If book isn't found, show error message.
    while ((currentPos = supplyFile.findBook(searchTitle, book, currentPos)) != -1) {
		// ask user if the book found is the one they want to edit
        cout << "Is " << book.getTitle() << " the book you want to edit(y/n)? ";
        cin >> confirm;
        cin.ignore();

        // if user confirms, show edit menu and update the file after each edit until the user selects 0 to exit
        if (toupper(confirm) == 'Y') {
            found = true;


            do {
				// edit menu
                cout << "\n\tWhat would you like to modify(1: ISBN, 2: Title, 3: Author, 4: Publisher, 5: Date, 6: Quantity, 7: Wholesale, 8: Retail, 0 : Exit): ";
                cout << "\n\tEnter choice: ";
                cin >> editChoice;
                cin.ignore();

				//edit book information based on user input
                switch (editChoice) {

                    case 1:
                        cout << "New ISBN: ";
                        cin.getline(temp, 14);
                        strUpper(temp);
                        book.setISBN(temp);
                        break;

                    case 2:
                        cout << "New Title: ";
                        cin.getline(temp, 51);
                        strUpper(temp);
                        book.setTitle(temp);
                        break;

                    case 3:
                        cout << "New Author: ";
                        cin.getline(temp, 31);
                        strUpper(temp);
                        book.setAuthor(temp);
                        break;

                    case 4:
                        cout << "New Publisher: ";
                        cin.getline(temp, 31);
                        strUpper(temp);
                        book.setPub(temp);
                        break;

                    case 5:
                        cout << "New Date: ";
                        cin.getline(temp, 11);
                        book.setDateAdded(temp);
                        break;

                    case 6:
                        cout << "New Quantity: ";
                        cin >> qty;
                        cin.ignore();
                        book.setQty(qty);
                        break;

                    case 7:
                        cout << "New Wholesale: ";
                        cin >> price;
                        cin.ignore();
                        book.setWholesale(price);
                        break;

                    case 8:
                        cout << "New Retail: ";
                        cin >> price;
                        cin.ignore();
                        book.setRetail(price);
                        break;

                    case 0:
                        break;
                    default:
                        cout << "\nInvalid selection.\n";
                }

                // ONLY update if user didn't choose exit
                if (editChoice != 0) {
                    supplyFile.updateBook(currentPos, book);
                }

            } while (editChoice != 0);

            break;
        }
		// if user doesn't confirm, keep searching for the book title
        currentPos += sizeof(InventoryBook);
    }
	// if book isn't found, show error message
    if (!found) {
        cout << "\n\tThis book isn't in the inventory.\n";
    }
}

void deleteBook(InventoryManager& supplyFile) {
	// class object
    InventoryBook book;
    char search[51], confirm;
    long currentPos = 0;

    cout << "\n\tSerendipity Booksellers\n";
    cout << "\t\tDelete Book\n";

	//ask user to enter the title of the book they want to delete.
    cin.ignore();
    cout << "\n\tEnter book title: ";
    cin.getline(search, 51);
    strUpper(search);

	//clear error characters and reset to the beginning of the file
    supplyFile.reset();

	//search file for book title and ask user if the book found is the one they want to delete. setting the book title to empty and updating the file with the new information.
    while ((currentPos = supplyFile.findBook(search, book, currentPos)) != -1) {
		// ask user if the book found is the one they want to delete
        cout << "\nIs " << book.getTitle() << " the book you would like to delete(y/n)? ";
        cin >> confirm;

		// if user confirms 
        if (toupper(confirm) == 'Y') {
			// ask user to confirm deletion
            cout << "\nAre you sure(y/n)? ";
            cin >> confirm;

			// if user confirms
			if (toupper(confirm) == 'Y') {
				// set book title to empty and update the file with the new information
                book.removeBook();
                supplyFile.updateBook(currentPos, book);
                return;
			}
        }
		// if user doesn't confirm, keep searching for the book title
        currentPos += sizeof(InventoryBook);

    }
    //errror message
    cout << "\n\tBook isn't in inventory.\n";
};


void reports(InventoryManager& supplyFile) {
    int choice = 0;

	//reports menu
    while (choice != 7) {
        cout << "\n\tSerendipity Booksellers\n";
        cout << "\t\tReports\n";
        cout << "\t1. Inventory Listing\n";
        cout << "\t2. Inventory Wholesale Value\n";
        cout << "\t3. Inventory Retail Value\n";
        cout << "\t4. Listing by Quantity\n";
        cout << "\t5. Listing by Cost\n";
        cout << "\t6. Listing by Age\n";
        cout << "\t7. Return to Main Menu\n";

        cout << "\n\tEnter Your Choice: ";
        cin >> choice;

		//validate input
        while (choice < 1 || choice > 7) {
            cout << "\n\tPlease enter 1-7: ";
            cin >> choice;
        }
		//open different report functions based on user input
        switch (choice) {
            case 1: repListing(supplyFile);
                break;
            case 2: repWholesale(supplyFile);
                break;
            case 3: repRetail(supplyFile);
                break;
            case 4: repQty(supplyFile); 
                break;
            case 5: repCost(supplyFile);
                break;
            case 6: repAge(supplyFile); 
                break;
        }
    }
}

void repListing(InventoryManager& supplyFile) {
	// class object
    InventoryBook book;

	// clear error characters and reset to the beginning of the file
    supplyFile.reset();

    cout << "\n\tSerendipity Booksellers\n";
    cout << "\t\tInventory Listing\n";

	// read each book from the file and if it's not empty, print the book's information
    while (supplyFile.readNext(book)) {
        if (!book.isEmpty()) {
            bookInfo(book);
            cout << endl;
        }
    }

    cout << "\n\tPress Enter to continue";
    cin.ignore();
    cin.get();
}

void repWholesale(InventoryManager& supplyFile) {
	// class objects
    InventoryBook list[SIZE];
    InventoryBook book;

	// index and computing variables
    int count = 0;
    double wholesaleSum = 0.0;

	// clear error characters and reset to the beginning of the file
    supplyFile.reset();

	// read each book from the file and if it's not empty, save it to the class array
    while (supplyFile.readNext(book) && count < 20) {
        if (!book.isEmpty()) {
            list[count++] = book;
        }
    }

    cout << "\n\tSerendipity Booksellers\n";
    cout << "\t\tWholesale Information\n";

	// print the book's title, wholesale cost, and compute the total wholesale value of all the books in the inventory
    for (int index = 0; index < count; index++) {
        cout << "\n\tTitle: " << list[index].getTitle() << endl;
        cout << "\tISBN: " << list[index].getISBN() << endl;
        cout << "\tQuantity: " << list[index].getQty() << endl;
        cout << "\tWholesale: $" << list[index].getWholesale() << endl;

        wholesaleSum += list[index].getQty() * list[index].getWholesale();
    }

    cout << "\n\tTotal Wholesale Value: $" << wholesaleSum << endl;

    cout << "\n\tPress Enter to continue";
    cin.ignore();
    cin.get();
}

void repRetail(InventoryManager& supplyFile) {
	// class objects
    InventoryBook list[SIZE];
    InventoryBook book;

	// index and computing variables
    int count = 0;
    double retailSum = 0.0;

	// clear error characters and reset to the beginning of the file
    supplyFile.reset();

	// read each book from the file and if it's not empty, save it to the class array
    while (supplyFile.readNext(book) && count < 20) {
        if (!book.isEmpty()) {
            list[count++] = book;
        }
    }

    cout << "\n\tSerendipity Booksellers\n";
    cout << "\t\tRetail Information\n";

	// print the book's title, retail price, and compute the total retail value of all the books in the inventory
    for (int index = 0; index < count; index++) {
        cout << "\n\tTitle: " << list[index].getTitle() << endl;
        cout << "\tISBN: " << list[index].getISBN() << endl;
        cout << "\tQuantity: " << list[index].getQty() << endl;
        cout << "\tRetail: $" << list[index].getRetail() << endl;
        
        retailSum += list[index].getQty() * list[index].getRetail();
    }

    cout << "\n\tTotal Retail Value: $" << retailSum << endl;

    cout << "\n\tPress Enter to continue";
    cin.ignore();
    cin.get();
}

void repCost(InventoryManager& supplyFile) {
    // class objects
    InventoryBook tempBookList[SIZE];
    InventoryBook book;

    // index for class array
    int count = 0;

    // clear error characters and reset to the beginning of the file
    supplyFile.reset();

    // read each book from the file and if it's not empty, save it to the class array
    while (supplyFile.readNext(book) && count < SIZE) {
        if (!book.isEmpty()) {
            tempBookList[count] = book;
            count++;
        }
    }

    // sort the class array in descending order of wholesale cost
    for (int start = 0; start < count - 1; start++) {
        int maxIndex = start;
        for (int index = start + 1; index < count; index++) {
            if (tempBookList[index].getWholesale() > tempBookList[maxIndex].getWholesale())
                maxIndex = index;
        }
        // swap the books at start and maxIndex
        InventoryBook temp = tempBookList[start];
        tempBookList[start] = tempBookList[maxIndex];
        tempBookList[maxIndex] = temp;
    }

    cout << "\n\tSerendipity Booksellers\n";
    cout << "\t\tCost Information\n";
    // print the book's title and wholesale cost
    for (int index = 0; index < count; index++) {
        cout << "\n\tTitle: " << tempBookList[index].getTitle() << endl;
        cout << "\tWholesale: $" << tempBookList[index].getWholesale() << endl;
    }
    cout << "\n\tPress Enter to continue";
    cin.ignore();
    cin.get();
}

void repAge(InventoryManager& supplyFile) {
    // class objects
    InventoryBook tempBookList[SIZE];
    InventoryBook book;

    // index for class array
    int count = 0;

    // clear error characters and reset to the beginning of the file
    supplyFile.reset();

    // read each book from the file and if it's not empty, save it to the class array
    while (supplyFile.readNext(book) && count < SIZE) {
        if (!book.isEmpty()) {
            tempBookList[count] = book;
            count++;
        }
    }

    // sort the class array in descending order of date added
    for (int start = 0; start < count - 1; start++) {
        int maxIndex = start;
        for (int index = start + 1; index < count; index++) {
            if (strcmp(tempBookList[index].getDateAdded(), tempBookList[maxIndex].getDateAdded()) > 0)
                maxIndex = index;
        }
        // swap the books at start and maxIndex
        InventoryBook temp = tempBookList[start];
        tempBookList[start] = tempBookList[maxIndex];
        tempBookList[maxIndex] = temp;
    }

    cout << "\n\tSerendipity Booksellers\n";
    cout << "\t\tDate Added Information\n";
    // print the book's title and date added
    for (int index = 0; index < count; index++) {
        cout << "\n\tTitle: " << tempBookList[index].getTitle() << endl;
        cout << "\tDate Added: " << tempBookList[index].getDateAdded() << endl;
    }
    cout << "\n\tPress Enter to continue";
    cin.ignore();
    cin.get();
}


void repQty(InventoryManager& supplyFile) {
    // class objects
    InventoryBook tempBookList[SIZE];
    InventoryBook book;

    // index for class array
    int count = 0;

    // clear error characters and reset to the beginning of the file
    supplyFile.reset();

    // read each book from the file and if it's not empty, save it to the class array
    while (supplyFile.readNext(book) && count < SIZE) {
        if (!book.isEmpty()) {
            tempBookList[count] = book;
            count++;
        }
    }

    // sort the class array in descending order of quantity on hand
    for (int start = 0; start < count - 1; start++) {
        int maxIndex = start;
        for (int index = start + 1; index < count; index++) {
            if (tempBookList[index].getQty() > tempBookList[maxIndex].getQty())
                maxIndex = index;
        }
        // swap the books at start and maxIndex
        InventoryBook temp = tempBookList[start];
        tempBookList[start] = tempBookList[maxIndex];
        tempBookList[maxIndex] = temp;
    }

    cout << "\n\tSerendipity Booksellers\n";
    cout << "\t\tBooks by Quantity\n";
    // print the book's title and quantity on hand
    for (int index = 0; index < count; index++) {
        cout << "\n\tTitle: " << tempBookList[index].getTitle() << endl;
        cout << "\tQuantity: " << tempBookList[index].getQty() << endl;
    }
    cout << "\n\tPress Enter to continue";
    cin.ignore();
    cin.get();
}

// Change the parameters to one const reference
void bookInfo(InventoryBook& book) {
    cout << "\n\tSerendipity Booksellers\n";
    cout << "\t\tBook Information\n";
    cout << "\n\tISBN: " << book.getISBN() << endl;
    cout << "\tTitle: " << book.getTitle() << endl;
    cout << "\tAuthor: " << book.getAuthor() << endl;
    cout << "\tPublisher: " << book.getPub() << endl;
    cout << "\tDate Added: " << book.getDateAdded() << endl;
    cout << "\tQuantity-On-Hand: " << book.getQty() << endl;
    cout << "\tWholesale Cost: $" << book.getWholesale() << endl;
    cout << "\tRetail Price: $" << book.getRetail() << endl;
}
// convert a string to uppercase
void strUpper(char* str) {
    while (*str != '\0') { 
        *str = toupper(*str); 
        str++; 
    }
}
