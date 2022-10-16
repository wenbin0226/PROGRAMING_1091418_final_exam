// MakeReservation.cpp
// Member-function definitions for class MakeReservation.
#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

#include "MakeReservation.h" // MakeReservation class definition

extern Date computeCurrentDate();

extern bool leapYear(int year);

extern int inputAnInteger(int begin, int end);

// MakeReservation constructor initializes base-class data members
MakeReservation::MakeReservation(ReservationDatabase& theReservationDatabase,
	AvailableRoomsDatabase& theAvailableRoomsDatabase)
	: reservationDatabase(theReservationDatabase),
	availableRoomsDatabase(theAvailableRoomsDatabase)
{
} // end MakeReservation constructor

// performs transaction
void MakeReservation::execute()
{
	Date currentDate = computeCurrentDate();

	Date availableMonths[7];
	computeAvailableMonths(currentDate, availableMonths);

	int checkInCode;
	int firstDay;
	int lastDay;

	Date checkInDate;
	Date checkOutDate;

	inputCheckInDate(checkInDate, currentDate, availableMonths, checkInCode, firstDay, lastDay);
	inputCheckOutDate(checkOutDate, checkInDate, availableMonths, checkInCode, firstDay, lastDay);

	cout << "\nCheck in date: " << checkInDate.getYear() << '-'
		<< setw(2) << setfill('0') << checkInDate.getMonth() << '-'
		<< setw(2) << checkInDate.getDay() << endl;

	cout << "Check out date: " << checkOutDate.getYear() << '-'
		<< setw(2) << checkOutDate.getMonth() << '-'
		<< setw(2) << checkOutDate.getDay() << endl;

	availableRoomsDatabase.displayAvailableRooms(checkInDate, checkOutDate);

	int roomType;
	do {
		cout << "\nSelect Room Type:\n"
			<< "1. Superior Room\n" << "2. Deluxe Room\n" << "3. Deluxe Twin Room\n"
			<< "4. Superior Suite\n" << "5. Deluxe Suite\n? ";
	} while ((roomType = inputAnInteger(1, 5)) == -1);

	int minNumRooms = availableRoomsDatabase.findMinNumRooms(roomType, checkInDate, checkOutDate);

	if (minNumRooms == 0)
	{
		cout << "\nThere are no rooms available!\n";
		return;
	}

	int numRooms;
	do cout << "\nEnter the number of rooms (1 ~ " << minNumRooms << ", 0 to end): ";
	while ((numRooms = inputAnInteger(0, minNumRooms)) == -1);
	if (numRooms == 0)
		return;

	availableRoomsDatabase.decreaseAvailableRooms(roomType, numRooms, checkInDate, checkOutDate);

	cout << "\nID Number: ";
	string IDNumber;
	getline(cin, IDNumber, '\n');

	cout << "\nName: ";
	string name;
	getline(cin, name, '\n');

	cout << "\nMobile Phone: ";
	string mobile;
	getline(cin, mobile, '\n');

	newReservation.setReservation(roomType, numRooms, checkInDate, checkOutDate, IDNumber, name, mobile);

	cout << "\n    Name   Fare       Mobile          Room type   Number of rooms   Check in date   Check out date\n";
	newReservation.displayReservation();

	reservationDatabase.pushBack(newReservation);

	cout << "\nReservation successfully!" << endl;
}

void MakeReservation::computeAvailableMonths(Date currentDate, Date availableMonths[])
{
	int year = currentDate.getYear();
	int month = currentDate.getMonth();
	for (int i = 1; i <= 6; i++)
	{
		availableMonths[i].setYear(year);
		availableMonths[i].setMonth(month);

		if (month < 12)
			month++;
		else
		{
			year++;
			month = 1;
		}
	}
}

void MakeReservation::inputCheckInDate(Date& checkInDate, Date currentDate, Date availableMonths[],
	int& checkInCode, int& firstDay, int& lastDay)
{
	int days[13] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	cout << "\nPlease Input Check In Date\n";
	cout << "\nMomth :\n";
	for (int i = 1; i <= 6; i++) {
		cout << i << ". " << availableMonths[i].getYear() << "-" << setfill('0') << setw(2) << availableMonths[i].getMonth() << endl;
	}

	do {
		cout << "? ";
		checkInCode = inputAnInteger(1, 6);
	} while (checkInCode == -1);

	if (checkInCode == 1) {
		if (leapYear(availableMonths[checkInCode].getYear()))
			days[2] = 29;
		else
			days[2] = 28;
		firstDay = currentDate.getDay();
		lastDay = days[currentDate.getMonth()];
	}
	else if (checkInCode == 6) {
		if (leapYear(availableMonths[checkInCode].getYear()))
			days[2] = 29;
		else
			days[2] = 28;
		firstDay = 1;
		lastDay = days[availableMonths[checkInCode].getMonth()] - 1;
	}
	else {
		if (leapYear(availableMonths[checkInCode].getYear()))
			days[2] = 29;
		else
			days[2] = 28;
		firstDay = 1;
		lastDay = days[availableMonths[checkInCode].getMonth()];
	}

	int tempday;

	do {
		cout << "\nDay (" << firstDay << " ~ " << lastDay << ") :";
		tempday = inputAnInteger(firstDay, lastDay);
	} while (tempday == -1);
	checkInDate.setYear(availableMonths[checkInCode].getYear());
	checkInDate.setMonth(availableMonths[checkInCode].getMonth());
	checkInDate.setDay(tempday);
}

void MakeReservation::inputCheckOutDate(Date& checkOutDate, Date checkInDate, Date availableMonths[],
	int checkInCode, int firstDay, int lastDay)
{
	int days[13] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	cout << "\nPlease Input Check Out Date\n";
	cout << "\nMonth :\n";

	if (leapYear(checkInDate.getYear()))
		days[2] = 29;
	else
		days[2] = 28;

	int min = 9;
	for (int i = 1; i <= 6; i++) {
		if (checkInCode <= i) {
			if (availableMonths[i].getMonth() == checkInDate.getMonth() && checkInDate.getDay() == days[checkInDate.getMonth()])
				continue;
			cout << i << ". " << availableMonths[i].getYear() << "-" << setfill('0') << setw(2) << availableMonths[i].getMonth() << endl;
			min = (min < i) ? min : i;
		}
	}
	int tempcode;

	do {
		cout << "? ";
		tempcode = inputAnInteger(min, 6);
	} while (tempcode == -1);

	if (tempcode == checkInCode) {
		firstDay = checkInDate.getDay() + 1;
		lastDay = days[checkInDate.getMonth()];
	}
	else {
		if (leapYear(availableMonths[tempcode].getYear()))
			days[2] = 29;
		else
			days[2] = 28;
		firstDay = 1;
		lastDay = days[availableMonths[tempcode].getMonth()];
	}

	int tempday;

	do {
		cout << "\nDay (" << firstDay << " ~ " << lastDay << ") :";
		tempday = inputAnInteger(firstDay, lastDay);
	} while (tempday == -1);
	checkOutDate.setYear(availableMonths[tempcode].getYear());
	checkOutDate.setMonth(availableMonths[tempcode].getMonth());
	checkOutDate.setDay(tempday);

}