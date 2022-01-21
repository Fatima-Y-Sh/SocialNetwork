#include <iostream>
#include <math.h>
#include "DataTypes.h"

bool compareIndividuals(Individual* per1, Individual* per2);
void outputIndividual(Individual* per1);

/*
* The following function checks if two individuals are friends. Takes two individual pointers and loops through their friends list.
* Best Complexity: O(1)
* Worst Complexity: O(n)
*/
bool hasFriendship(Individual* per1, Individual* per2)
{
	for (Friend* cur = per1->myFriends; cur != NULL; cur = cur->next)
		if (cur->self == per2)
			return true;

	return false;
}

/*
 * The following function inserts a friend node in an individual's singly linked friend list. Insertion at head is used to lower complexity.
 * Complexity: O(1)
 */
void createOneWayFriendship(Friend*& friendship, Individual* person)
{
	Friend* tmp = new Friend;
	tmp->self = person;
	tmp->score = -1;
	tmp->next = NULL;

	if (friendship == NULL)
	{
		friendship = tmp;
		return;
	}

	tmp->next = friendship;
	friendship = tmp;
}

/*
* The following function establishes the bi-directional friendship between two individuals.
* Complexity: O(1)
*/
void createFriendship(Individual*& per1, Individual*& per2)
{
	createOneWayFriendship(per1->myFriends, per2);
	createOneWayFriendship(per2->myFriends, per1);
}
/*
*  The following function deletes a friend node in an individual's singly linked friend list. 
*  Best Complexity: O(1)
*  Worst Complexity: O(n)
*/
void removeOneWayFriendship(Friend*& friendship, Individual* person)
{
	Friend* cur = friendship;
	Friend* prev = NULL;

	if (friendship == NULL)
		return;

	if (friendship->self == person)
	{
		cur = cur->next;
		delete friendship;
		friendship = cur;

		return;
	}

	while (cur != NULL && cur->self != person)
	{
		prev = cur;
		cur = cur->next;
	}

	if (cur == NULL) {
		cout << "Not found";
		return;
	}

	prev->next = cur->next;
	delete cur;
}
/*
* The following function establishes deletion of the bi-directional friendship between two individuals.
* Best Complexity:O(1)
* Worst Complexity: O(n)
*/

void removeFriendship(Individual*& per1, Individual*& per2)
{
	removeOneWayFriendship(per1->myFriends, per2);
	removeOneWayFriendship(per2->myFriends, per1);
}
/*
* The following function creates the list of mutual friends between two individuals whose pointers are passed as parameters.
* Complexity: O(n^2)
*/
Friend* mutualFriendsList(Individual* per1, Individual* per2)
{
	Friend* mutualList = new Friend;
	mutualList = NULL;

	for (Friend* cur1 = per1->myFriends; cur1 != NULL; cur1 = cur1->next) {
		for (Friend* cur2 = per2->myFriends; cur2 != NULL; cur2 = cur2->next) {
			if (cur1->self == cur2->self) {
				createOneWayFriendship(mutualList, cur1->self);
				break;
			}
		}
	}

	return mutualList;
}
/*
* The following function counts the number of mutual friends between two individuals whose pointers are passed as parameters.
* Complexity:O(n^2)
*/
int mutualFriendsCount(Individual* per1, Individual* per2)
{
	int count = 0;

	for (Friend* cur1 = per1->myFriends; cur1 != NULL; cur1 = cur1->next) {
		for (Friend* cur2 = per2->myFriends; cur2 != NULL; cur2 = cur2->next) {
			if (cur1->self == cur2->self) {
				count++;
			}
		}
	}

	return count;
}
/*
* The following function inserts suggested friends into the suggested list in a sorted way using a scoring system.
* Best Complexity: O(1)
* Worst Complexity: O(n)
*/
Friend* insertSuggestedFriends(Friend* list, Individual* self, int score)
{
	Friend* tmp = new Friend;
	tmp->self = self;
	tmp->score = score;
	tmp->next = NULL;

	if (list == NULL)
	{
		list = tmp;
		return list;
	}

	if (score >= list->score)
	{
		tmp->next = list;
		list = tmp;
		return list;
	}

	Friend* cur = list;

	while (cur->next != NULL && cur->next->score > score)
		cur = cur->next;

	if (cur->next == NULL)
	{
		cur->next = tmp;
		return list;
	}

	tmp->next = cur->next;
	cur->next = tmp;

	return list;
}
/*
* The following function creates the list of suggested friends of an individual usind multiple criteria.
* Complexity: O(n^4)
*/
Friend* suggestedFriends(World world, Individual* per)
{
	cout << "Generating a list of suggested friends for the following individual, please wait..." << endl;

	int currentScore = 0;

	Friend* suggestedFriends = new Friend;
	suggestedFriends = NULL;

	for (Individual* cur = world.Head; cur != NULL; cur = cur->next)
	{
		if (per != cur && !hasFriendship(per, cur))
		{
			currentScore = 0;

			currentScore += mutualFriendsCount(per, cur);

			if (currentScore > 70)
				currentScore = 70;

			if (per->University == cur->University)
				currentScore += 15;

			if (abs(per->Age - cur->Age) <= 8)
				currentScore += 15;

			suggestedFriends = insertSuggestedFriends(suggestedFriends, cur, currentScore);
		}
	}

	return suggestedFriends;
}
/*
* This function deletes all the friends of a given individual.
* Complexity: O(n)
*/
void cleanFriendship(Friend*& friends)
{
	if (friends == NULL)
		return;

	Friend* cur = friends;

	while (cur != NULL)
	{
		cur = friends->next;
		delete friends;
		friends = cur;
	}
}