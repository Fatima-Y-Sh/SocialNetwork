#include <iostream>
#include <string>
#include "DataTypes.h"
using namespace std;

void removeFriendship(Individual*& per1, Individual*& per2);
Individual* doesContainIndividual(World world, Individual* person);

/*
* The following function compares two individuals given as pointers.
* Complexity: O(1)
*/
bool compareIndividuals(Individual* per1, Individual* per2)
{
	if (per1->FirstName == per2->FirstName && per1->LastName == per2->LastName
		&& per1->University == per2->University && per1->Age == per2->Age && per1->gender == per2->gender)
		return true;

	return false;
}
/*
* The following function outputs an individual's info 
* Best Complexity: O(1) (if he has no friends)
* Worst Complexity:O(n) (if he has n friends)
*/
void outputIndividual(Individual* per1)
{
	cout << "Individual Name: " << per1->FirstName << endl;

	cout << "Individual Last Name: " << per1->LastName << endl;

	cout << "Individual University: " << per1->University << endl;

	cout << "Individual Age: " << per1->Age << endl;

	cout << "Individual Gender: " << per1->gender << endl;

	cout << "Friends: " << endl;

	for (Friend* cur = per1->myFriends; cur != NULL; cur = cur->next)
	{
		cout << "\t|" << endl;

		cout << "\t|__ " << cur->self->FirstName << " " << cur->self->LastName << endl;
	}

	cout << endl;

}
/*
* Boolean function that parses a string into an integer. It returns true if it's able to parse and false if it's not
* Complexity: O(1)
*/
bool castAge(string age)
{
	try {
		stoi(age);
		return true;
	}
	catch (exception e) {
		return false;
	}
}

/*
* The following function reads the information of an individual given by the user.
* It dynamically allocates this individual
* Complexity: O(1)
*/
Individual* inputIndividual()
{
	Individual* ind = new Individual;

	cout << "Input individual details: " << endl << endl;

	cout << "\tIndividual Name: ";
	getline(cin, ind->FirstName);

	cout << "\tIndividual Last Name: ";
	getline(cin, ind->LastName);

	cout << "\tIndividual University: ";
	getline(cin, ind->University);

	cout << "\tIndividual Gender: ";
	getline(cin, ind->gender);

	cout << "\tIndividual Age: ";

	string age;
	getline(cin, age);

	while (!castAge(age))
	{
		cout << endl << "\aA casting exception occured whilst reading the age, please try again." << endl << endl;

		cout << "\tIndividual Age: ";
		getline(cin, age);
	}

	ind->Age = stoi(age);

	return ind;
}
/*
* The following function deletes the individual as well as all friendships he has.
* Best Complexity:O(1) (if he has no friends and he's on the head of the world)
* Worst Complexity:O(n^2)
*/
void deleteIndividual(World& world, Individual* person)
{

	if (world.Head == NULL)
		return;

	Individual* cur;

	while (person->myFriends != NULL)
	{
		Individual* friends = person->myFriends->self;
		removeFriendship(person, friends);
	}

	if (world.Head == person)
	{
		cur = world.Head->next;
		delete world.Head;

		cur->previous = NULL;
		world.Head = cur;

		return;
	}

	if (world.Tail == person)
	{
		cur = world.Tail->previous;
		delete world.Tail;

		cur->next = NULL;
		world.Tail = cur;

		return;
	}

	person->previous->next = person->next;
	person->next->previous = person->previous;

	delete person;
}