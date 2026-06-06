#ifndef INVENTORYMANAGER_H
#define INVENTORYMANAGER_H

#include <fstream>
#include <string>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include "mainmenu.h"

const std::string errorMessage = "ERROR OPENING FILE.";

class InventoryManager {
    private:
		//private data members
        std::fstream file;
        std::string filename;

    public:
		//constructor
        InventoryManager(std::string name) {
            filename = name;
            
			// Try to open the file in input, output, and binary mode.
            file.open(filename, std::ios::in | std::ios::out | std::ios::binary);
                
			// If the file doesn't exist, create it and then open it again.
            if (!file) {
                file.clear();
                file.open(filename, std::ios::out | std::ios::binary);
                file.close();
                throw errorMessage;
            }
        }

        //destructor that closes file 
        ~InventoryManager() {
            if (file.is_open()) {
                file.close();
            }
        }

		// Resets the file stream to the beginning of the file and clears any error flags.
        void reset() {
            file.clear();
            file.seekg(0, std::ios::beg);
        }

		// Reads the next InventoryBook record from the file into the provided book object. Returns true if successful, false if end of file is reached or an error occurs.
        bool readNext(InventoryBook& book) {
            return (bool)file.read(reinterpret_cast<char*>(&book), sizeof(InventoryBook));
        }

		// Searches for a book by title starting from the specified position. If found, fills the provided book object with the data and returns the file position of the record. If not found, returns -1.
        long findBook( const char searchTitle[], InventoryBook& book, long startPos = 0) {
			// Clear any error flags and set the file position to the specified starting position.
            file.clear();
            file.seekg(startPos, std::ios::beg);

            //position variable
            long pos;

            //read through the file sequentially
            while (file.read(reinterpret_cast<char*>(&book), sizeof(InventoryBook))) {
				// Calculate the position of the current record in the file.
                pos = file.tellg() - (long)sizeof(InventoryBook);

				// If the book is not empty and the search title is a substring of the book's title, return the position of the record.
                if (!book.isEmpty() && strstr(book.getTitle(), searchTitle) != NULL) {
                    return pos;
                }
            }

			// If the book is not found, return -1.
            return -1;
        }

        long findISBN(const char searchISBN[], InventoryBook& book) {
			// Clear any error flags and reset the file position to the beginning of the file.
            reset();

			//position variable
            long pos;

			//read through the file sequentially
            while (file.read(reinterpret_cast<char*>(&book), sizeof(InventoryBook))) {
				// Calculate the position of the current record in the file.
                pos = file.tellg() - (long)sizeof(InventoryBook);

				// If the book is not empty and the ISBN matches the search ISBN, return the position of the record.
                if (!book.isEmpty() && strcmp(book.getISBN(), searchISBN) == 0){
                    return pos;
                }
            }
			// If the book is not found, return -1.
            return -1;
        }

        void updateBook(long position, InventoryBook& book) {
			// Clear any error flags and set the file position to the specified position.
            file.clear();
            file.seekp(position, std::ios::beg);

			// Write the updated book data to the file at the specified position.
            file.write(reinterpret_cast<char*>(&book), sizeof(InventoryBook));
        }

        void addNewBook(InventoryBook& newBook) {
			// Clear any error flags and set the file position to the end of the file.
            file.clear();
            file.seekp(0, std::ios::end);

			// Write the new book data to the end of the file.
            file.write(reinterpret_cast<char*>(&newBook), sizeof(InventoryBook));
        }
};

#endif