#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <iomanip>

using namespace std;

// Structure to represent a movie
struct Movie {
    string name;
    string genre;
    string showTimes[3]; // 3 show times per movie
    int availableSeats[3]; // Available seats for each show time
};

// Structure to represent a user
struct User {
    string name;
    string email;
    string phone;
    set<string> bookedTickets; // Unique booked tickets
};

// Function to load movies from a file
vector<Movie> loadMoviesFromFile(const string& filename) {
    vector<Movie> movies;
    ifstream file(filename);
    
    if (!file.is_open()) {
        cout << "Error: Unable to open movie file.\n";
        return movies;
    }
    
    Movie movie;
    while (getline(file, movie.name) && getline(file, movie.genre)) {
        for (int i = 0; i < 3; ++i) {
            getline(file, movie.showTimes[i]);
        }

        // Read available seats and handle errors properly
        if (!(file >> movie.availableSeats[0] >> movie.availableSeats[1] >> movie.availableSeats[2])) {
            cout << "Error reading seat availability data for movie: " << movie.name << endl;
            file.ignore();  // Skip the rest of the line if reading fails
            continue;
        }

        file.ignore();  // Ignore newline character after reading availableSeats

        movies.push_back(movie);
    }
    file.close();
    return movies;
}

// Function to save movies to a file
void saveMoviesToFile(const string& filename, const vector<Movie>& movies) {
    ofstream file(filename);
    if (file.is_open()) {
        for (const Movie& movie : movies) {
            file << movie.name << endl;
            file << movie.genre << endl;
            for (int i = 0; i < 3; ++i) {
                file << movie.showTimes[i] << endl;
            }
            file << movie.availableSeats[0] << " " << movie.availableSeats[1] << " " << movie.availableSeats[2] << endl;
        }
        file.close();
    } else {
        cout << "Error: Unable to save movie file.\n";
    }
}

// Function to load users from a file
map<string, User> loadUsersFromFile(const string& filename) {
    map<string, User> users;
    ifstream file(filename);
    
    if (!file.is_open()) {
        cout << "Error: Unable to open user file.\n";
        return users;
    }
    
    string name, email, phone, ticket;
    while (getline(file, name) && getline(file, email) && getline(file, phone)) {
        User user;
        user.name = name;
        user.email = email;
        user.phone = phone;
        while (getline(file, ticket) && !ticket.empty()) {
            user.bookedTickets.insert(ticket);
        }
        users[name] = user;
    }
    file.close();
    return users;
}

// Function to save users to a file
void saveUsersToFile(const string& filename, const map<string, User>& users) {
    ofstream file(filename);
    
    if (file.is_open()) {
        for (const auto& pair : users) {
            const User& user = pair.second;
            file << user.name << endl;
            file << user.email << endl;
            file << user.phone << endl;
            for (const string& ticket : user.bookedTickets) {
                file << ticket << endl;
            }
            file << endl; // Separate users
        }
        file.close();
    } else {
        cout << "Error: Unable to save user file.\n";
    }
}

// Function to display available movies
void displayMovies(const vector<Movie>& movies) {
    cout << "\nAvailable Movies:\n";
    cout << setw(30) << left << "Movie Name" << setw(20) << left << "Genre" << endl;
    for (const Movie& movie : movies) {
        cout << setw(30) << left << movie.name << setw(20) << left << movie.genre << endl;
    }
}

// Function to book tickets
void bookTickets(vector<Movie>& movies, map<string, User>& users) {
    string movieName, userName;
    int numTickets, showIndex;

    cout << "\nEnter Movie Name: ";
    cin.ignore();
    getline(cin, movieName);

    bool movieFound = false;
    for (size_t i = 0; i < movies.size(); ++i) {
        if (movies[i].name == movieName) {
            movieFound = true;
            cout << "\nAvailable Show Times for " << movieName << ":\n";
            for (int j = 0; j < 3; ++j) {
                cout << j + 1 << ". " << movies[i].showTimes[j] << " (Seats: " << movies[i].availableSeats[j] << ")\n";
            }

            cout << "Enter Show Time Index (1-3): ";
            cin >> showIndex;

            if (showIndex >= 1 && showIndex <= 3) {
                showIndex--; // Adjust index to 0-based

                cout << "Enter Number of Tickets: ";
                cin >> numTickets;

                if (numTickets > 0 && numTickets <= movies[i].availableSeats[showIndex]) {
                    movies[i].availableSeats[showIndex] -= numTickets;

                    cin.ignore();
                    cout << "Enter User Name: ";
                    getline(cin, userName);

                    if (users.find(userName) == users.end()) {
                        User user;
                        user.name = userName;
                        cout << "Enter Email: ";
                        getline(cin, user.email);
                        cout << "Enter Phone: ";
                        getline(cin, user.phone);
                        users[userName] = user;
                    }

                    string ticket = movieName + " - " + movies[i].showTimes[showIndex];
                    users[userName].bookedTickets.insert(ticket);

                    cout << "\nTickets Booked Successfully!" << endl;
                } else {
                    cout << "Invalid number of tickets or insufficient seats available." << endl;
                }
            } else {
                cout << "Invalid show time index." << endl;
            }
            return;
        }
    }

    if (!movieFound) {
        cout << "Movie not found." << endl;
    }
}

// Function to display user's booked tickets
void displayBookedTickets(const map<string, User>& users) {
    string userName;
    cout << "Enter User Name: ";
    cin.ignore();
    getline(cin, userName);

    if (users.find(userName) != users.end()) {
        const User& user = users.at(userName);
        cout << "\nBooked Tickets for " << userName << ":\n";
        for (const string& ticket : user.bookedTickets) {
            cout << "- " << ticket << endl;
        }
    } else {
        cout << "User not found." << endl;
    }
}

int main() {
    vector<Movie> movies = loadMoviesFromFile("movies.txt");
    map<string, User> users = loadUsersFromFile("users.txt");

    int choice;
    do {
        cout << "\nMovie Ticket Booking System\n";
        cout << "1. Display Movies\n";
        cout << "2. Book Tickets\n";
        cout << "3. Display Booked Tickets\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                displayMovies(movies);
                break;
            case 2:
                bookTickets(movies, users);
                break;
            case 3:
                displayBookedTickets(users);
                break;
            case 4:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice.\n";
        }
    } while (choice != 4);

    saveMoviesToFile("movies.txt", movies);
    saveUsersToFile("users.txt", users);

    return 0;
}

