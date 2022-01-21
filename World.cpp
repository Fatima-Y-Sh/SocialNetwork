#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "DataTypes.h"
using namespace std;

const string WORLD_PATH = "data\\world.txt";

bool compareIndividuals(Individual* per1, Individual* per2);
void createFriendship(Individual*& per1, Individual*& per2);
void outputIndividual(Individual* per1);
void cleanFriendship(Friend*& friends);
void outputIndividual(Individual* per1);
void removeFriendship(Individual*& per1, Individual*& per2);

/*
* The following function searches for an individual who is passed as a parameter, and returns a pointer to his position in the world.
* Complexity:O(n)
*/
Individual* doesContainIndividual(World world, Individual* person)
{
	if (world.Head == NULL)
		return NULL;

	for (Individual* cur = world.Head; cur != NULL; cur = cur->next)
		if (compareIndividuals(cur, person))
			return cur;

	return NULL;
}
/*
* The following function searches for an individual by his/her name and returns options when multiple individuals are found.
* Complexity: O(n)
*/
Individual* searchByName(World world)
{
	Individual* T[1000];
	int n = -1;

	string firstName, lastName;
	cin.ignore();

	cout << "Input individual details: " << endl << endl;

	cout << "\tIndividual Name: ";
	getline(cin, firstName);

	cout << "\tIndividual Last Name: ";
	getline(cin, lastName);

	for (Individual* cur = world.Head; cur != NULL; cur = cur->next)
	{
		if (cur->FirstName == firstName && cur->LastName == lastName)
		{
			n++;

			T[n] = cur;
		}
	}

	if (n == -1)
		return NULL;

	if (n == 0)
		return T[0];

	cout << endl << "Found multiple individuals, displaying..." << endl << endl;

	for (int i = 0; i <= n; i++)
	{
		cout << "<--------------> " << i << " <-------------->" << endl << endl;
		outputIndividual(T[i]);
	}

	cout << endl << "Enter a number from 0 to " << n << " in order to choose a single individual: " << endl;

	int choice;
	cin >> choice;

	while (choice < 0 || choice > n)
	{
		cout << "Incorrect! Please enter a number from 0 to " << n << " in order to choose a single individual: " << endl;

		cin >> choice;
	}

	return T[choice];
}
/*
* The following function outputs the whole world.
* Complexity:O(kn) (where k is the number of the individual's friends)
*/
void outputWorld(World world)
{
	for (Individual* cur = world.Head; cur != NULL; cur = cur->next)
		outputIndividual(cur);

}
/*
* The following function inserts the given individual at the head of the world ( to lower complexity).
* Complexity: O(1)
*/
void insertToWorld(World& world, Individual*& per)
{
	if (world.Head == NULL)
	{
		world.Head = per;
		world.Tail = per;
		return;
	}

	per->next = world.Head;
	world.Head->previous = per;

	world.Head = per;
}

/*
* The following function reads the data from the text file and stores it into the doubly linked list.
* Best Complexity: O(n)
* Worst Complexity: O(n^2)
*/
void loadWorldFromFile(World& world)
{
	ifstream worldFile;
	string line;

	worldFile.open(WORLD_PATH.c_str()); // Opens world file from path.


	if (!worldFile.is_open()) // Checks if world file actually exists.
	{
		cout << endl << "\aThe world file has not been found! Generating a new one..." << endl << endl;
		ofstream generateFile(WORLD_PATH.c_str());

		if (generateFile.is_open()) // If the world file does not exist, generate one.
			generateFile.close();

		return;
	}

	while (getline(worldFile, line)) // Loop through each line of the world.txt file.
	{

		stringstream ss(line); // Convert each line into a string stream for tokenization

		Individual* lIndividual = new Individual, * rIndividual = new Individual;

		string age;

		getline(ss, lIndividual->FirstName, '#'); // Saves stringstream tokens into the individuals.

		getline(ss, lIndividual->LastName, '#');

		getline(ss, age, '#');
		getline(ss, lIndividual->gender, '#');
		getline(ss, lIndividual->University, ',');

		lIndividual->Age = stoi(age); // Parses string into an integer.

		lIndividual->myFriends = NULL;
		lIndividual->next = NULL;
		lIndividual->previous = NULL;

		getline(ss, rIndividual->FirstName, '#');

		if (rIndividual->FirstName == "") // If right hand does not exist, adds to world if he doesn't already belong there.
		{
			Individual* doesLExist = doesContainIndividual(world, lIndividual);

			if (doesLExist == NULL)
				insertToWorld(world, lIndividual);

			continue; // Switches over to line after it.
		}

		getline(ss, rIndividual->LastName, '#'); // Reading right hand individual information.
		getline(ss, age, '#');
		getline(ss, rIndividual->gender, '#');
		getline(ss, rIndividual->University, '#');

		rIndividual->Age = stoi(age);

		rIndividual->myFriends = NULL;
		rIndividual->next = NULL;
		rIndividual->previous = NULL;

		Individual* doesLExist = doesContainIndividual(world, lIndividual); // Checking if the individuals already exist in the world.
		Individual* doesRExist = doesContainIndividual(world, rIndividual);

		if (doesLExist == NULL)
			insertToWorld(world, lIndividual); // Insert to world.
		else
		{
			delete lIndividual;
			lIndividual = doesLExist; // Delete dynamically allocated individual and use searched individual (memory).
		}

		if (doesRExist == NULL)
			insertToWorld(world, rIndividual);
		else
		{
			delete rIndividual;
			rIndividual = doesRExist;
		}

		createFriendship(lIndividual, rIndividual); // Create bi-friendship.
	}

	worldFile.close(); // Close file when done.
}

/*
* The following function gives the format of the individual with no friends in the output file.
* Complexity: O(1)
*/
string getSingleStringLayout(Individual* lIndividual)
{
	stringstream format;

	format << lIndividual->FirstName << "#" << lIndividual->LastName << "#" << lIndividual->Age << "#" << lIndividual->gender << "#" << lIndividual->University << ",";

	return format.str();
}

/*
* The following function gives the format of the individual with another in the output file.
* Complexity: O(1)
*/
string getDoubleStringLayout(Individual* lIndividual, Individual* rIndividual)
{
	stringstream format;

	format << lIndividual->FirstName << "#" << lIndividual->LastName << "#" << lIndividual->Age << "#" << lIndividual->gender << "#" << lIndividual->University << "," <<
		rIndividual->FirstName << "#" << rIndividual->LastName << "#" << rIndividual->Age << "#" << rIndividual->gender << "#" << rIndividual->University;

	return format.str();
}

/*
* The following function saves the changes into the output file.
* Best Complexity:O(n)
* Worst Complexity:O(nk^2) (where k is the maximum number of friends)
*/
void writeWorldToFile(World& world)
{
	remove(WORLD_PATH.c_str());

	string currentLine;

	ofstream worldFile(WORLD_PATH.c_str());

	if (worldFile.is_open())
	{

		for (Individual* cur = world.Head; cur != NULL; cur = cur->next)
			if (cur->myFriends == NULL) //single person with no friends
				worldFile << getSingleStringLayout(cur) << endl;

		for (Individual* cur = world.Head; cur != NULL; cur = cur->next)
		{
			while (cur->myFriends != NULL) //person with one or more friends 
			{
				worldFile << getDoubleStringLayout(cur, cur->myFriends->self) << endl;

				Individual* individual = cur->myFriends->self;

				removeFriendship(cur, individual); 
			}

		}
	}

	worldFile.close();
}

/*
* The following function clears all the individuals from the world as well as their friendships.
* Complexity:O(n^2)
*/
void cleanWorld(World& world)
{
	if (world.Head == NULL || world.Tail == NULL)
		return;

	Individual* cur = world.Head;

	while (cur != NULL)
	{
		cur = world.Head->next;

		cleanFriendship(world.Head->myFriends);
		delete world.Head;
		world.Head = cur;
	}

}