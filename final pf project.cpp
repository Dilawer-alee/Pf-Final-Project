#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

struct User {
    string username, password;
};

struct Ticket {
    string movieName, username, showTime;
    int seatNumber, price;
};

vector<User> users;
vector<Ticket> tickets;
vector<string> movies = {"Avatar", "Avengers", "Inception", "Titanic", "Joker"};
vector<string> showTimes = {"12:00 PM", "3:00 PM", "6:00 PM", "9:00 PM"};

void saveUsers() {
    ofstream file("users.txt");
    for (const auto &user : users) {
        file << user.username << " " << user.password << endl;
    }
    file.close();
}

void loadUsers() {
    ifstream file("users.txt");
    if (file) {
        User user;
        while (file >> user.username >> user.password) {
            users.push_back(user);
        }
    }
    file.close();
}

void saveTickets() {
    ofstream file("tickets.txt");
    for (const auto &ticket : tickets) {
        file << ticket.username << " " << ticket.movieName << " " << ticket.showTime << " "
             << ticket.seatNumber << " " << ticket.price << endl;
    }
    file.close();
}

void loadTickets() {
    ifstream file("tickets.txt");
    if (file) {
        Ticket ticket;
        while (file >> ticket.username >> ticket.movieName >> ticket.showTime >> ticket.seatNumber >> ticket.price) {
            tickets.push_back(ticket);
        }
    }
    file.close();
}

void registerUser() {
    User newUser;
    cout << "Enter Username: ";
    cin >> newUser.username;
    cout << "Enter Password: ";
    cin >> newUser.password;
    users.push_back(newUser);
    saveUsers();
    cout << "Registration Successful!\n";
}

bool loginUser(string &currentUser) {
    string username, password;
    cout << "Enter Username: ";
    cin >> username;
    cout << "Enter Password: ";
    cin >> password;

    for (auto &user : users) {
        if (user.username == username && user.password == password) {
            cout << "Login Successful!\n";
            currentUser = username;
            return true;
        }
    }
    cout << "Invalid Credentials!\n";
    return false;
}

void displayMovies() {
    cout << "\nAvailable Movies:\n";
    for (int i = 0; i < movies.size(); i++) {
        cout << i + 1 << ". " << movies[i] << endl;
    }
    cout << "\nAvailable Show Times:\n";
    for (int i = 0; i < showTimes.size(); i++) {
        cout << i + 1 << ". " << showTimes[i] << endl;
    }
}

void bookTicket(string username) {
    displayMovies();

    int movieChoice, showTimeChoice, seatNumber, price;
    cout << "\nSelect Movie (1-" << movies.size() << "): ";
    cin >> movieChoice;
    if (movieChoice < 1 || movieChoice > movies.size()) {
        cout << "Invalid Movie Choice!\n";
        return;
    }

    cout << "Select Show Time (1-" << showTimes.size() << "): ";
    cin >> showTimeChoice;
    if (showTimeChoice < 1 || showTimeChoice > showTimes.size()) {
        cout << "Invalid Show Time!\n";
        return;
    }

    cout << "Enter Seat Number (1-100): ";
    cin >> seatNumber;
    if (seatNumber < 1 || seatNumber > 100) {
        cout << "Invalid Seat Number!\n";
        return;
    }

    cout << "Enter Ticket Price: ";
    cin >> price;

    Ticket newTicket = {movies[movieChoice - 1], username, showTimes[showTimeChoice - 1], seatNumber, price};
    tickets.push_back(newTicket);
    saveTickets();

    cout << "\nTicket Booked Successfully!\n";
    cout << "Movie: " << movies[movieChoice - 1] << " | Time: " << showTimes[showTimeChoice - 1]
         << " | Seat: " << seatNumber << " | Price: $" << price << endl;
}

void viewTickets(string username) {
    bool found = false;
    cout << "\nYour Booked Tickets:\n";
    cout << left << setw(15) << "Movie" << setw(15) << "Show Time" << setw(10) << "Seat" << "Price\n";
    for (auto &ticket : tickets) {
        if (ticket.username == username) {
            found = true;
            cout << left << setw(15) << ticket.movieName << setw(15) << ticket.showTime << setw(10)
                 << ticket.seatNumber << "$" << ticket.price << endl;
        }
    }
    if (!found) {
        cout << "No Tickets Found!\n";
    }
}

void cancelTicket(string username) {
    string movieName, showTime;
    int seatNumber;
    bool found = false;

    cout << "Enter Movie Name: ";
    cin.ignore();
    getline(cin, movieName);
    cout << "Enter Show Time: ";
    getline(cin, showTime);
    cout << "Enter Seat Number: ";
    cin >> seatNumber;

    for (auto it = tickets.begin(); it != tickets.end(); ++it) {
        if (it->username == username && it->movieName == movieName && it->showTime == showTime && it->seatNumber == seatNumber) {
            tickets.erase(it);
            saveTickets();
            found = true;
            cout << "\nTicket Canceled Successfully!\n";
            break;
        }
    }

    if (!found) {
        cout << "No Matching Ticket Found!\n";
    }
}

void manageMovies() {
    cout << "\n=== Admin Panel ===\n";
    cout << "1. Add Movie\n2. View Movies\nEnter Choice: ";
    int choice;
    cin >> choice;

    if (choice == 1) {
        string newMovie;
        cout << "Enter New Movie Name: ";
        cin.ignore();
        getline(cin, newMovie);
        movies.push_back(newMovie);
        cout << "Movie Added Successfully!\n";
    } else if (choice == 2) {
        displayMovies();
    } else {
        cout << "Invalid Choice!\n";
    }
}

int main() {
    loadUsers();
    loadTickets();
    int choice;
    string currentUser;

    while (true) {
        cout << "\n=== Movie Ticket Booking System ===\n";
        cout << "1. Register\n2. Login\n3. Book Ticket\n4. View Tickets\n5. Cancel Ticket\n6. Admin Panel\n7. Exit\n";
        cout << "Enter Your Choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                registerUser();
                break;
            case 2:
                loginUser(currentUser);
                break;
            case 3:
                if (!currentUser.empty()) {
                    bookTicket(currentUser);
                } else {
                    cout << "Please log in first!\n";
                }
                break;
            case 4:
                if (!currentUser.empty()) {
                    viewTickets(currentUser);
                } else {
                    cout << "Please log in first!\n";
                }
                break;
            case 5:
                if (!currentUser.empty()) {
                    cancelTicket(currentUser);
                } else {
                    cout << "Please log in first!\n";
                }
                break;
            case 6:
                manageMovies();
                break;
            case 7:
                exit(0);
            default:
                cout << "Invalid Choice! Try Again.\n";
        }
    }

    return 0;
}
