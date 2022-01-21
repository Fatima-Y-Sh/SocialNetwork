#include <iostream>

#include <string>
#include <fstream>
#include "sha256.h"
#include "DataTypes.h"

void outputWorld(World world);
void writeWorldToFile(World& world);
void cleanWorld(World& world);
Individual* inputIndividual();
Individual* doesContainIndividual(World world, Individual* person);
void outputIndividual(Individual* per1);
bool hasFriendship(Individual* per1, Individual* per2);
void insertToWorld(World& world, Individual*& per);
void createFriendship(Individual*& per1, Individual*& per2);
void deleteIndividual(World& world, Individual* person);
void removeFriendship(Individual*& per1, Individual*& per2);
Friend* mutualFriendsList(Individual* per1, Individual* per2);
void cleanFriendship(Friend*& friends);
Individual* searchByName(World world);

Friend* suggestedFriends(World world, Individual* per);
void openMainMenu(World& world, string login[][LOGIN_MAX_COLUMNS], int userAmount);

/*
	In this section we define enum types. Enums are easier to read (human readable format), each enum type below is assigned a number (starting from 0)
	SHOW(0)
	CREATE(1)
	DELETE(2)

	When used in switch cases, we listen to case SHOW: (equivalent to case 1:)
 */
enum MAIN_MENU_ITEMS
{
	SHOW, CREATE, DELETE, SEARCH, MANAGE, EXIT_SAVE
};

enum SHOW_MENU_ITEMS
{
	SHOW_WORLD, SHOW_INDIVIDUAL, CHECK_FOR_FRIENDSHIP, EXIT_SHOW_MENU
};

enum CREATION_MENU_ITEMS
{
	CREATE_INDIVIDUAL, CREATE_FRIENDSHIP, EXIT_CREATION_MENU
};

enum DELETE_MENU_ITEMS
{
	DELETE_INDIVIDUAL, DELETE_FRIENDSHIP, EXIT_DELETE_MENU
};

enum SEARCH_MENU_ITEMS
{
	MUTUAL_FRIENDS, SUGGESTED_FRIENDS, EXIT_SEARCH_MENU
};

enum MANAGEMENT_MENU_ITEMS
{
	USER_ADD, EXIT_MANAGEMENT_MENU
};

void openShowMenu(World& world, string login[][LOGIN_MAX_COLUMNS], int userAmount) {

	int choice;

	do {
		cout << "\t***********************************************************" << endl;
		cout << "\a\t  Welcome to the World Friend System's show submenu!" << endl;
		cout << "\t***********************************************************" << endl << endl;

		cout << "0- Show the entire world" << endl;
		cout << "1- Show an individual's profile" << endl;
		cout << "2- Show status between two individuals" << endl;
		cout << "3- Exit to main menu" << endl << endl;

		cout << "Please input your submenu choice: ";
		cin >> choice;

		cout << endl << endl;
	} while (choice < 0 || choice > 3);

	switch (choice)
	{

	case SHOW_WORLD:
		outputWorld(world);
		system("pause");
		openShowMenu(world, login, userAmount);
		break;

	case SHOW_INDIVIDUAL:
	{
		Individual* searchedIndividual = searchByName(world);

		if (searchedIndividual == NULL)
	    {
			cout << endl << "\aThe following individual does not exist, redirecting you to the show submenu." << endl << endl;
			openShowMenu(world, login, userAmount);
			return;
		}

		cout << "\nSearch result:" << endl;
		outputIndividual(searchedIndividual);
		system("pause");
		openShowMenu(world, login, userAmount);

		break;
	}

	case CHECK_FOR_FRIENDSHIP:
	{
		Individual* lSearch = searchByName(world);
		Individual* rSearch = searchByName(world);

		if (lSearch == NULL || rSearch == NULL)
		{
			cout << endl << "\aOne of the inputted individual does not exist, redirecting you to the show submenu." << endl << endl;
			openShowMenu(world, login, userAmount);

			return;
		}

		if (lSearch == rSearch)
		{
			cout << endl << "\aYou must input two distinct individuals, redirecting you to the show submenu." << endl << endl;
			openShowMenu(world, login, userAmount);

			return;
		}

		if (hasFriendship(lSearch, rSearch))
			cout << endl << "The given individuals are friends." << endl;
		else
			cout << endl << "The given individuals are not friends." << endl;

		system("pause");
		openShowMenu(world, login, userAmount);

		break;
	}

	case EXIT_SHOW_MENU:
		openMainMenu(world, login, userAmount);
		break;
	}
}

void openCreationMenu(World& world, string login[][LOGIN_MAX_COLUMNS], int userAmount) {

	int choice;

	do {
		cout << "\t***********************************************************" << endl;
		cout << "\a\t  Welcome to the World Friend System's creation submenu!" << endl;
		cout << "\t***********************************************************" << endl << endl;

		cout << "0- Create a new individual" << endl;
		cout << "1- Create a friendship between two individuals" << endl;
		cout << "2- Exit to main menu" << endl << endl;

		cout << "Please input your submenu choice: ";
		cin >> choice;

		cout << endl << endl;
	} while (choice < 0 || choice > 2);

	switch (choice)
	{

	case CREATE_INDIVIDUAL:
	{
		cin.ignore();

		Individual* ind = inputIndividual();

		ind->myFriends = NULL;
		ind->next = NULL;
		ind->previous = NULL;

		Individual* searchedIndividual = doesContainIndividual(world, ind);

		if (searchedIndividual != NULL)
		{
			cout << endl << "\aThe following individual already exists, please enter another distinct individual. Redirecting you to the creation submenu." << endl << endl;
			delete ind;

			openCreationMenu(world, login, userAmount);
		}

		insertToWorld(world, ind);

		cout << endl << "Succesfully inserted user into the world. Enter show submenu to view changes." << endl << endl;

		system("pause");
		openCreationMenu(world, login, userAmount);
		break;
	}

	case CREATE_FRIENDSHIP:
	{
		Individual* lSearch = searchByName(world);
		Individual* rSearch = searchByName(world);

		if (lSearch == NULL || rSearch == NULL)
		{
			cout << endl << "\aOne of the inputted individual does not exist, redirecting you to the creation submenu." << endl << endl;
			openCreationMenu(world, login, userAmount);

			return;
		}

		if (lSearch == rSearch)
		{
			cout << endl << "\aYou must input two distinct individuals, redirecting you to the creation submenu." << endl << endl;
			openCreationMenu(world, login, userAmount);

			return;
		}

		if (hasFriendship(lSearch, rSearch))
		{
			cout << endl << "\aThe following individuals are already friends, please input individuals that are not friends." << endl << endl;
			openCreationMenu(world, login, userAmount);

			return;
		}

		createFriendship(lSearch, rSearch);

		cout << endl << "Succesfully created user friendship between the following two users." << endl << endl;

		system("pause");

		openCreationMenu(world, login, userAmount);
		break;
	}

	case EXIT_CREATION_MENU:
		openMainMenu(world, login, userAmount);
		break;
	}
}

void openDeletionMenu(World& world, string login[][LOGIN_MAX_COLUMNS], int userAmount) {

	int choice;

	do {
		cout << "\t***********************************************************" << endl;
		cout << "\a\t  Welcome to the World Friend System's deletion submenu!" << endl;
		cout << "\t***********************************************************" << endl << endl;

		cout << "0- Delete an individual" << endl;
		cout << "1- Delete a friendship between two individuals" << endl;
		cout << "2- Exit and return to main menu" << endl << endl;
		cout << "Please input your submenu choice: ";
		cin >> choice;

		cout << endl << endl;
	} while (choice < 0 || choice > 2);

	switch (choice)
	{

	case DELETE_INDIVIDUAL:
	{
		Individual* searchedIndividual = searchByName(world);

		if (searchedIndividual == NULL)
		{
			cout << endl << "\aThe following individual doesn't exist, please enter another distinct individual. Redirecting you to the deletion submenu." << endl << endl;
			openDeletionMenu(world, login, userAmount);

			return;
		}

		deleteIndividual(world, searchedIndividual);

		cout << endl << "Succesfully deleted from the world. Enter show submenu to view changes." << endl << endl;

		system("pause");
		openDeletionMenu(world, login, userAmount);
		break;
	}

	case DELETE_FRIENDSHIP:
	{
		Individual* lSearch = searchByName(world);
		Individual* rSearch = searchByName(world);

		if (lSearch == NULL || rSearch == NULL)
		{
			cout << endl << "\aOne of the inputted individual does not exist, redirecting you to the deletion submenu." << endl << endl;
			openDeletionMenu(world, login, userAmount);

			return;
		}

		if (lSearch == rSearch)
		{
			cout << endl << "\aYou must input two distinct individuals, redirecting you to the deletion submenu." << endl << endl;
			openDeletionMenu(world, login, userAmount);

			return;
		}

		if (!hasFriendship(lSearch, rSearch))
		{
			cout << endl << "\aThe following individuals are not friends, please input individuals that are friends." << endl << endl;
			openDeletionMenu(world, login, userAmount);

			return;
		}

		removeFriendship(lSearch, rSearch);

		cout << endl << "Succesfully deleted friendship between the following two users." << endl << endl;

		system("pause");

		openDeletionMenu(world, login, userAmount);
		break;
	}


	case EXIT_DELETE_MENU:
		openMainMenu(world, login, userAmount);
		break;
	}
}

void openSearchMenu(World& world, string login[][LOGIN_MAX_COLUMNS], int userAmount) {

	int choice;

	do {
		cout << "\t***********************************************************" << endl;
		cout << "\a\t  Welcome to the World Friend System's search submenu!" << endl;
		cout << "\t***********************************************************" << endl << endl;

		cout << "0- Search for mutual friends between two individuals " << endl;
		cout << "1- Find a list of suggested friends" << endl;
		cout << "2- Exit and return to main menu" << endl << endl;
		cout << "Please input your submenu choice: ";
		cin >> choice;

		cout << endl << endl;
	} while (choice < 0 || choice > 2);

	switch (choice)
	{

	case MUTUAL_FRIENDS:
	{
		Individual* lSearch = searchByName(world);
		Individual* rSearch = searchByName(world);

		if (lSearch == NULL || rSearch == NULL)
		{
			cout << endl << "\aOne of the inputted individual does not exist, redirecting you to the search submenu." << endl << endl;
			openSearchMenu(world, login, userAmount);

			return;
		}

		if (lSearch == rSearch)
		{
			cout << endl << "\aYou must input two distinct individuals, redirecting you to the search submenu." << endl << endl;
			openSearchMenu(world, login, userAmount);

			return;
		}

		Friend* mutualList = mutualFriendsList(lSearch, rSearch);
		cout << endl << "The mutual friends of these two individuals are: " << endl;

		for (Friend* cur = mutualList; cur != NULL; cur = cur->next)
		{
			cout << "\t|" << endl;
			cout << "\t|__ " << cur->self->FirstName << " " << cur->self->LastName << endl;
		}

		cout << endl;

		cleanFriendship(mutualList);
		system("pause");

		openSearchMenu(world, login, userAmount);
		break;
	}

	case SUGGESTED_FRIENDS:
	{
		Individual* searchedIndividual = searchByName(world);

		if (searchedIndividual == NULL)
		{
			cout << endl << "\aThe following individual doesn't exist, please enter another distinct individual. Redirecting you to the search submenu." << endl << endl;
			openSearchMenu(world, login, userAmount);
			return;
		}

		Friend* suggestedList = suggestedFriends(world, searchedIndividual);
		cout << endl << "The suggested friends of this individual are: " << endl;

		for (Friend* cur = suggestedList; cur != NULL; cur = cur->next)
		{
			cout << "\t|" << endl;
			cout << "\t|__ " << cur->self->FirstName << " " << cur->self->LastName << " (" << cur->score << ")" << endl;
		}

		cout << endl;

		cleanFriendship(suggestedList); // to free memory

		system("pause");
		openSearchMenu(world, login, userAmount);
		break;
	}

	case EXIT_SEARCH_MENU:
		openMainMenu(world, login, userAmount);
		break;
	}
}

void openManagementMenu(World& world, string login[][LOGIN_MAX_COLUMNS], int userAmount) {

	int choice;

	do {
		cout << "\t***********************************************************" << endl;
		cout << "\a\t  Welcome to the World Friend System's management submenu!" << endl;
		cout << "\t***********************************************************" << endl << endl;

		cout << "0- Create a new user for login" << endl;
		cout << "1- Exit and return to main menu" << endl << endl;
		cout << "Please input your submenu choice: ";
		cin >> choice;

		cout << endl << endl;
	} while (choice < 0 || choice > 1);

	switch (choice)
	{

	case USER_ADD:
	{
		string username, password;

		if (userAmount == LOGIN_MAX_ROWS)
		{
			cout << endl << "\aYou cannot create more than 50 users, delete a user before trying to create an existing one." << endl << endl;
			openManagementMenu(world, login, userAmount);
			break;
		}

		cout << "Please input the username to add: ";
		cin.ignore();
		getline(cin, username);

		cout << "Please input the password to add: ";
		getline(cin, password);

		login[userAmount][0] = sha256(username);
		login[userAmount][1] = sha256(password);

		userAmount++;

		remove("data\\login.txt");
		ofstream outLogin("data\\login.txt");

		for (int i = 0; i < userAmount; i++)
		{
			for (int j = 0; j < LOGIN_MAX_COLUMNS; j++)
			{
				outLogin << login[i][j];

				if (j == 0)
					outLogin << "\t";
			}

			outLogin << endl;
		}

		outLogin.close();

		cout << endl << "Succesfully created new user login. Restart for changes to take effect." << endl << endl;
		openManagementMenu(world, login, userAmount);
		break;
	}

	case EXIT_MANAGEMENT_MENU:
	{
		openMainMenu(world, login, userAmount);
		break;
	}

	}
}

/*
 * Opens the Main menu, used to direct to submenus.
 */
void openMainMenu(World& world, string login[][LOGIN_MAX_COLUMNS], int userAmount)
{

	int choice;

	do {
		cout << "\t***********************************************************" << endl;
		cout << "\a\t  Welcome to the World Friend System's main menu!" << endl;
		cout << "\t***********************************************************" << endl << endl;

		cout << "0- Enter show mode" << endl;
		cout << "1- Enter creation mode" << endl;
		cout << "2- Enter deletion mode" << endl;
		cout << "3- Enter search mode" << endl;
		cout << "4- Enter management mode" << endl;
		cout << "5- Save changes and exit the program" << endl << endl;

		cout << "Please input your submenu choice: ";
		cin >> choice;

		cout << endl << endl;
	} while (choice < 0 || choice > 5);

	switch (choice)
	{
	case SHOW:
		openShowMenu(world, login, userAmount);
		break;
	case CREATE:
		openCreationMenu(world, login, userAmount);
		break;
	case DELETE:
		openDeletionMenu(world, login, userAmount);
		break;
	case SEARCH:
		openSearchMenu(world, login, userAmount);
		break;
	case MANAGE:
		openManagementMenu(world, login, userAmount);
		break;
	case EXIT_SAVE:
		writeWorldToFile(world);
		cleanWorld(world);
		break;
	}

}