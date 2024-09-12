#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// Base class for Person
class Person
{
protected:
    string name;
    string contactInfo;

public:
    Person(string name, string contactInfo) : name(name), contactInfo(contactInfo) {}

    string getName() const
    {
        return name;
    }

    string getContactInfo() const
    {
        return contactInfo;
    }
};

// Derived class for Guest
class Guest : public Person
{
public:
    Guest(string name, string contactInfo) : Person(name, contactInfo) {}
};

// Room class
class Room
{
private:
    string roomNumber;
    string roomType;
    bool isAvailable;

public:
    Room(string number, string type) : roomNumber(number), roomType(type), isAvailable(true) {}

    string getRoomNumber() const
    {
        return roomNumber;
    }

    string getRoomType() const
    {
        return roomType;
    }

    bool checkAvailability() const
    {
        return isAvailable;
    }

    void bookRoom()
    {
        isAvailable = false;
    }

    void vacateRoom()
    {
        isAvailable = true;
    }
};

// Reservation class
class Reservation
{
private:
    string reservationID;
    Guest guest;
    Room room;
    string checkInDate;
    string checkOutDate;

public:
    Reservation(string id, Guest g, Room r, string checkIn, string checkOut)
        : reservationID(id), guest(g), room(r), checkInDate(checkIn), checkOutDate(checkOut) {}

    void confirmReservation() const
    {
        cout << "Reservation confirmed for " << guest.getName() << " in room " << room.getRoomNumber()
             << " (" << room.getRoomType() << ") from " << checkInDate << " to " << checkOutDate << endl;
    }

    void cancelReservation() const
    {
        cout << "Reservation cancelled for " << guest.getName() << endl;
    }

    string getReservationID() const
    {
        return reservationID;
    }

    Guest getGuest() const
    {
        return guest;
    }

    Room getRoom() const
    {
        return room;
    }

    bool operator==(const Reservation &other) const
    {
        return reservationID == other.reservationID;
    }
};

// HotelManagementSystem class
class HotelManagementSystem
{
private:
    vector<Room> rooms;
    vector<Reservation> reservations;

public:
    void addRoom(const Room &room)
    {
        rooms.push_back(room);
    }

    void makeReservation(const Reservation &reservation)
    {
        reservations.push_back(reservation);
        reservation.getRoom().bookRoom(); // Book the room
        reservation.confirmReservation(); // Confirm the reservation
    }

    void cancelReservation(const string &reservationID)
    {
        auto it = remove_if(reservations.begin(), reservations.end(),
                            [&reservationID](const Reservation &reservation)
                            {
                                return reservation.getReservationID() == reservationID;
                            });

        if (it != reservations.end())
        {
            (*it).getRoom().vacateRoom(); // Vacate the room
            (*it).cancelReservation();    // Cancel the reservation
            reservations.erase(it, reservations.end());
        }
        else
        {
            cout << "Reservation ID not found." << endl;
        }
    }

    void displayAllReservations() const
    {
        cout << "All Reservations:\n";
        for (const auto &reservation : reservations)
        {
            reservation.confirmReservation(); // Display reservation details
        }
    }

    void displayAvailableRooms() const
    {
        cout << "Available Rooms:\n";
        for (const auto &room : rooms)
        {
            if (room.checkAvailability())
            {
                cout << "Room Number: " << room.getRoomNumber() << " - Type: " << room.getRoomType() << endl;
            }
        }
    }

    void bookRoom()
    {
        string guestName, guestContact, roomNumber, checkInDate, checkOutDate;

        cout << "Enter guest name: ";
        getline(cin, guestName);

        cout << "Enter guest contact info (10 digits only): ";
        getline(cin, guestContact);

        // Check if the contact number is exactly 10 digits and all digits are numbers
        bool isValidContact = (guestContact.length() == 10) && all_of(guestContact.begin(), guestContact.end(), ::isdigit);

        if (!isValidContact)
        {
            cout << "Invalid phone number. It should be exactly 10 digits." << endl;
            return;
        }

        Guest guest(guestName, guestContact);

        cout << "Enter room number: ";
        getline(cin, roomNumber);

        auto roomIt = find_if(rooms.begin(), rooms.end(), [&roomNumber](const Room &room)
                              { return room.getRoomNumber() == roomNumber && room.checkAvailability(); });

        if (roomIt != rooms.end())
        {
            cout << "Enter check-in date (YYYY-MM-DD): ";
            getline(cin, checkInDate);
            cout << "Enter check-out date (YYYY-MM-DD): ";
            getline(cin, checkOutDate);

            string reservationID = "R" + to_string(reservations.size() + 1);
            Reservation reservation(reservationID, guest, *roomIt, checkInDate, checkOutDate);
            makeReservation(reservation);
        }
        else
        {
            cout << "Room is not available or does not exist." << endl;
        }
    }

    void cancelRoomReservation()
    {
        string reservationID;
        cout << "Enter reservation ID to cancel: ";
        getline(cin, reservationID);
        cancelReservation(reservationID);
    }
};

// Main function to demonstrate the system
int main()
{
    HotelManagementSystem hotelSystem;

    // Adding rooms
    Room room1("101", "Single");
    Room room2("102", "Double");
    hotelSystem.addRoom(room1);
    hotelSystem.addRoom(room2);

    int choice;
    do
    {
        cout << "\nHotel Management System Menu:\n";
        cout << "1. Book a Room\n";
        cout << "2. Cancel a Reservation\n";
        cout << "3. Display All Reservations\n";
        cout << "4. Display Available Rooms\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(); // To ignore the newline character after integer input

        switch (choice)
        {
        case 1:
            hotelSystem.bookRoom();
            break;
        case 2:
            hotelSystem.cancelRoomReservation();
            break;
        case 3:
            hotelSystem.displayAllReservations();
            break;
        case 4:
            hotelSystem.displayAvailableRooms();
            break;
        case 5:
            cout << "Exiting the system. Goodbye!" << endl;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 5);

    return 0;
}
