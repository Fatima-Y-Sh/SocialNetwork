#include <iostream>
using namespace std;

#ifndef DATATYPES_H
#define DATATYPES_H

const int LOGIN_MAX_ROWS = 50, LOGIN_MAX_COLUMNS = 2;

struct Individual;

struct Friend {
	Individual* self;
	int score;
	Friend* next;
};

struct Individual {
	string FirstName, LastName, University;
	int Age;
	string gender;
	Friend* myFriends;
	Individual* next;
	Individual* previous;
};

struct World
{
	Individual* Head, * Tail;
};

#endif
