#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>
using namespace std;

enum class MediaType {
	Book = 1,
	Magazine,
	AudioCD,
	CDROM,
	Cassette,
	VideoCassette
};

class MediaItem {
private:
	MediaType type;
	string author;
	string title;
	int year;
	bool status; 

	string getTypeString() const {
		switch (type) {
		case MediaType::Book: return "Book";
		case MediaType::Magazine: return "Magazine";
		case MediaType::AudioCD: return "Audio CD";
		case MediaType::CDROM: return "CD-ROM";
		case MediaType::Cassette: return "Cassette";
		case MediaType::VideoCassette: return "Video Cassette";
		default: return "Unknown";
		}
	}

public:
	
	MediaItem(MediaType t, const string& a, const string& ti, int y, bool s) : type(t), author(a), title(ti), year(y), status(s) {}

	MediaItem() : type(MediaType::Book), year(1), status(true) {}

	
	MediaType getType() const {
		return type; 
	}
	string getAuthor() const { 
		return author; 
	}
	string getTitle() const {
		return title; 
	}
	int getYear() const {
		return year; 
	}
	bool isAvailable() const {
		return status; 
	}

	
	void setType(MediaType t) {
		type = t; 
	}
	void setAuthor(const string& a) {
		author = a; 
	}
	void setTitle(const string& ti) {
		title = ti; 
	}

	void setYear(int y) {
		if (y > 0) year = y;
		else throw invalid_argument("Year must be a positive number.");
	}

	void setStatus(bool s) { status = s; }

	
	friend ostream& operator<<(ostream& os, const MediaItem& item) {
		os << "Type: " << item.getTypeString() << "\n"
			<< "Author: " << item.author << "\n"
			<< "Title: " << item.title << "\n"
			<< "Year of publication: " << item.year << "\n"
			<< "Status: ";
		if (item.status == 1) {
			os << "Available";
		}
		else {
			os << "Borrowed";
		}
		os << "\n";
		return os;
	}

	
	friend ofstream& operator<<(ofstream& ofs, const MediaItem& item) {
		ofs << static_cast<int>(item.type) << " "
			<< item.author << " "
			<< item.title << " "
			<< item.year << " "
			<< item.status << "\n";
		return ofs;
	}

	
	friend ifstream& operator>>(ifstream& ifs, MediaItem& item) {
		int typeInt;
		ifs >> typeInt >> item.author >> item.title >> item.year >> item.status;
		item.type = static_cast<MediaType>(typeInt);
		return ifs;
	}
};

class Library {
private:
	vector<MediaItem> items;

public:
	void addMediaItem(const MediaItem& item) {
		items.push_back(item);
	}
	
	void addMediaItem(MediaItem&& item) {
		items.push_back(move(item));
	}

	void displayLibrary() const {
		for (const auto& item : items) {
			cout << item << "\n";
		}
	}

	void displayAvailableItems() const {
		for (const auto& item : items) {
			if (item.isAvailable()) cout << item << "\n";
		}
	}

	void displayBorrowedItems() const {
		for (const auto& item : items) {
			if (!item.isAvailable()) cout << item << "\n";
		}
	}

	void saveToFile(const string& filename) const {
		try {
			ofstream file(filename);
			if (!file.is_open()) {
				throw runtime_error("Failed to open the file for saving.");
			}
			for (const MediaItem& item : items) {
				file << item;
			}
			file.close();
			cout << "Data has been successfully saved to the file.\n";
		}
		catch (const exception& e) {
			cerr << "Error saving to file: " << e.what() << "\n";
		}
	}

	void loadFromFile(const string& filename) {
		try {
			ifstream file(filename);
			if (!file.is_open()) {
				throw runtime_error("Failed to open the file for loading.");
			}
			items.clear();
			MediaItem item;
			while (file >> item) {
				items.push_back(item);
			}
			file.close();
			cout << "Data has been successfully loaded from the file.\n";
		}
		catch (const exception& e) {
			cerr << "Error loading from file: " << e.what() << "\n";
		}
	}
};

class Application {
private:
	Library library;
	Application () {}
	void addMedia() {
		try {
			
			int typeNumber;
			MediaType mtype;
			string author, title;
			int year;
			bool status;

			cout << "Media type (1-Book, 2-Magazine, 3-Audio CD, 4-CD-ROM, 5-Cassette, 6-Video Cassette): ";
			cin >> typeNumber;
			
			if (cin.fail() || typeNumber < 1 || typeNumber > 6) {
					cin.clear(); 
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					throw invalid_argument("invalid media type. please enter a number between 1 and 6.");
			}

			switch (typeNumber) {
				case 1: mtype = MediaType::Book; break;
				case 2:	mtype = MediaType::Magazine; break;
				case 3: mtype = MediaType::AudioCD; break;
				case 4: mtype = MediaType::CDROM; break;
				case 5: mtype = MediaType::Cassette; break;
				case 6: mtype = MediaType::VideoCassette; break;
			}
			cout << "Author: ";
			cin >> author;
			cout << "Title: ";
			cin >> title;

			cout << "Year of publication: ";
			cin >> year;
			if (year <= 0 || year >= 2024) {
				throw invalid_argument("Invalid year.");
			}

			cout << "Status (1-Available, 0-Borrowed): ";
			cin >> status;
			if (cin.fail() || status < 0 || status > 1) {
				cin.clear(); 
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				throw invalid_argument("Invalid status. Please enter a 1-Available or 0-Borrowed.");
			}


			library.addMediaItem(MediaItem(mtype, author, title, year, status));
			cout << "The media item has been successfully added.\n";
		}
		catch (const exception& e) {
			cerr << "Error adding media: " << e.what() << "\n";
		}
	}

	void saveToFile() {
		library.saveToFile("library_data.txt");
	}

	void loadFromFile() {
		library.loadFromFile("library_data.txt");
	}

public:
	static Application& getInstance() {
		static Application app;
		return app;
	}
	void run() {
		int choice;
		do {
			try {
				cout << "1. Add new media item\n";
				cout << "2. Display all media items\n";
				cout << "3. Display available media items\n";
				cout << "4. Display borrowed media items\n";
				cout << "5. Save data to file\n";
				cout << "6. Load data from file\n";
				cout << "0. Exit\n";
				cout << "Choice: ";
				cin >> choice;

				if (cin.fail()) {  
					cin.clear();  
					cin.ignore(numeric_limits<streamsize>::max(), '\n');  
					throw invalid_argument("Invalid input. Please enter a valid number.");
				}

				switch (choice) {
				case 1: addMedia(); break;
				case 2: library.displayLibrary(); break;
				case 3: library.displayAvailableItems(); break;
				case 4: library.displayBorrowedItems(); break;
				case 5: saveToFile(); break;
				case 6: loadFromFile(); break;
				case 0: cout << "Exiting the application.\n"; break;
				default: cout << "Invalid choice.\n"; break;
				}
			}
			catch (const exception& e) {
				cerr << "Error: " << e.what() << "\n";
			}
		} while (choice != 0);
	}
};

int main() {
	Application& app = Application::getInstance();
	app.run();
	return 0;
}
