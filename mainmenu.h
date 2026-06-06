#ifndef MAINMENU_H
#define MAINMENU_H

#include <cstring>
#include <string>

//keeps bookdata. Base class 
class BookData {
    protected:
		//only allows derived classes to access these members
        char bookTitle[51];
        char isbn[14];
        char author[31];
        char publisher[31];

    public:
        //constructor
        BookData() {
            bookTitle[0] = '\0';
            isbn[0] = '\0';
            author[0] = '\0';
            publisher[0] = '\0';
        }

        // Setters
        void setTitle(const char* t)
        {std::strcpy(bookTitle, t);
        }

        void setISBN(const char* i)
        {std::strcpy(isbn, i); }

        void setAuthor(const char* a)
        {
			std::strcpy(author, a); }

        void setPub(const char* p)
        {
			std::strcpy(publisher, p); }


        // Getters
        const char* getTitle() const
        { return bookTitle; }

        const char* getISBN() const 
        { return isbn; }

        const char* getAuthor() const
        { return author; }

        const char* getPub() const
        { return publisher; }
};

//keeps inventory info
class InventoryBook : public BookData {
	//only allows derived classes to have access to these members
protected:
	char dateAdded[11];
	int qtyOnHand;
	double wholesale;
	double retail;
public:
	//InventoryBook constructor also runs BookData Constructor
	InventoryBook() : BookData() {
		//sets members to empty values
		dateAdded[0] = '\0';
		qtyOnHand = 0;
		wholesale = 0.0;
		retail = 0.0;
	}

	// Setters
	void setDateAdded(const char* d)
	{
		std::strcpy(dateAdded, d);
	}
	void setQty(int q)
	{
		qtyOnHand = q;
	}
	void setWholesale(double w)
	{
		wholesale = w;
	}
	void setRetail(double r)
	{
		retail = r;
	}
	// Getters
	const char* getDateAdded() const
	{
		return dateAdded;
	}
	int getQty() const
	{
		return qtyOnHand;
	}
	double getWholesale() const
	{
		return wholesale;
	}
	double getRetail() const
	{
		return retail;
	}

	//flag
	bool isEmpty() const
	{
		return bookTitle[0] == '\0';
	}

	//set bookTitle to empty value
	void removeBook()
	{
		bookTitle[0] = '\0';
	}
};

//has price info for every title
class SoldBook : public InventoryBook {
private:
	static double taxRate;
	int qtySold;
	double tax;
	double subtotal;
	static double total;

public:
	//constructor
	SoldBook() : InventoryBook() {
		qtySold = 0;
		tax = 0.0;
		subtotal = 0.0;
	}

	//setters
	static void setTaxRate(double t)
	{
		taxRate = t;
	}

	void setQtySold(int q)
	{
		qtySold = q;
	}

	static void setTotal(double t) {
		total = t;
	}

	//calculate tax
	void calculateTax()
	{
		tax = qtySold * retail * taxRate;
	}

	//calculate subtotal
	void calcPrice(int qty) {
		qtySold = qty;
		tax = qtySold * retail * taxRate;
		subtotal = (retail * qtySold) + tax;
		total += subtotal;
	}


	//getters
	double getTax() const
	{
		return tax;
	}


	int getQtySold() const
	{
		return qtySold;
	}


	double getSubtotal() const {
		return subtotal;
	}

	static double getTotal() {
		return total;
	}

};

// Forward declaration of InventoryManager
class InventoryManager;

//bookstore functions
void cashier(InventoryManager&);
void invmenu(InventoryManager&);
void reports(InventoryManager&);
void lookUpBook(InventoryManager&);
void addBook(InventoryManager&);
void editBook(InventoryManager&);
void deleteBook(InventoryManager&);

void repListing(InventoryManager&);
void repWholesale(InventoryManager&);
void repRetail(InventoryManager&);
void repQty(InventoryManager&);
void repCost(InventoryManager&);
void repAge(InventoryManager&);
void bookInfo(InventoryBook&);

//converts string to uppercase
void strUpper(char*);


#endif