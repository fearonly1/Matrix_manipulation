// Programmer: Bader Aljishi
// CS3240 - Data Structures
// Assignment #5 - Matrices
#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <string>
#include "matrix.h"	// import matrix class
using namespace std;

string userMatrixOptionsMenu();	// prompts user for data to initialize matrix objects
void addMatrices(string, Matrix[]);	// adds two matrices with the same dimensions
void subtractMatrices(string, Matrix[]);
void multiplyMatrices(string, Matrix[]);
string getToken(string, int);	// for finding tokens in strings
void readFile(string, Matrix[]);	// for reading a file with matrix input
void writeFile(string, Matrix[]);	// for writing a matrix to an output file
void printMatrix(string, Matrix[]);	// for printing the requested matrix
string IntToString(int);	// function for converting Int values to be used for a string

int main()
{
	Matrix matrixArr[10];										// array of 10 Matrix objects
	string userChoice = "";
	while (true)
	{
		while (true)
		{
			userChoice = userMatrixOptionsMenu();					// prompt user for operation request
			if (userChoice != "Invalid") break;
			else
				cout << "Input invalid, please try again: \n" << endl;
		}

		if (userChoice[0] == 'A') addMatrices(userChoice, matrixArr);		// if addition is chosen
		if (userChoice[0] == 'S') subtractMatrices(userChoice, matrixArr);	// if subtraction is chosen
		if (userChoice[0] == 'M') multiplyMatrices(userChoice, matrixArr);	// if multiplication is chosen
		if (userChoice[0] == 'P') printMatrix(userChoice, matrixArr);	// if print is chosen
		if (userChoice[0] == 'R') readFile(userChoice, matrixArr);	// if read from file is chosen
		if (userChoice[0] == 'W') writeFile(userChoice, matrixArr); // if write to file is chosen
		if (userChoice[0] == 'Q') break;	// if quit is chosen
	}

	return 0;
}

// post: A valid string that can be used to select Matrix functions
string userMatrixOptionsMenu()
{
	string userChoice = "";
	cout << "For all of the operations defined below, <Matrix#> is an integer\n";
	cout << "between 1 and 10 corresponding to one of the matrices in your current list:\n\n";
	cout << "A <Matrix1> <Matrix2> <Matrix3> --- Adds <Matrix1> to <Matrix2> and stores the result in <Matrix3>\n";
	cout << "P <Matrix1> --- Prints <Matrix1> to the screen\n";
	cout << "M <Matrix1> <Matrix2> <Matrix3> --- Multiplies <Matrix1> and <Matrix2> and stores the result in <Matrix3>\n";
	cout << "Q --- Exits the program";
	cout << "R <Matrix1> <file-name> --- Reads <Matrix1> from the file <file-name>\n";
	cout << "S <Matrix1> <Matrix2> <Matrix3> --- Subtracts <Matrix2> from <Matrix1> and stores the result in <Matrix3>\n";
	cout << "W <Matrix1> <file-name> --- Creates a new file called <file-name>, and saves the contents of <Matrix1>\n\n";
	cout << "What would you like to do with your current list of Matrices? Please request an operation (case sensitive): ";
	bool validString = true;
	// prompt user for operation and check for valid response
	while (true)
	{
		//bool validString = true;
		getline(cin, userChoice);
		int userChoiceIndex2 = userChoice[2] - '0';
		// if the first character is not one of the option letters and/or the second character is not a blank space
		// the operation request is invalid
		if (userChoice == "Q") break;	// If the request is to quit then break out of the loop and return the string 'Q'
		if ((userChoice[0] == 'A' || userChoice[0] == 'M' || userChoice[0] == 'S') &&
			// if the second token is not a number the operation request is invalid
			(getToken(userChoice, 2) != "1" && getToken(userChoice, 2) != "2" &&
			 getToken(userChoice, 2) != "3" && getToken(userChoice, 2) != "4" &&
			 getToken(userChoice, 2) != "5" && getToken(userChoice, 2) != "6" &&
			 getToken(userChoice, 2) != "7" && getToken(userChoice, 2) != "8" &&
			 getToken(userChoice, 2) != "9" && getToken(userChoice, 2) != "10") ||
			 count(userChoice.begin(), userChoice.end(), ' ') > 3)
			// if the matrix number for the first matrix requested is not between 1 and 10 the operation request is invalid
			// requesting matrix 10 adds another character to the string and appropriate adjustments in logic must be made
		{
			validString = false;
			break;
		}

		// if the operation requested is an addition, multiplication, or subtraction operation the user input must be
		// at most 10 characters, at least 7 characters, and contain two other matrix choices
		if ((userChoice[0] == 'A' || userChoice[0] == 'M' || userChoice[0] == 'S') &&
			(count(userChoice.begin(), userChoice.end(), ' ') != 3) &&
			((userChoice.length() < 7 || userChoice.length() > 10)))
		{
			validString = false;
			break;
		}

		// if this string has tokens which are not valid numbers, then it is invalid
		if (userChoice[0] == 'A' || userChoice[0] == 'M' || userChoice[0] == 'S')
		{
			for (int i = 2; i < 4; i++)
			{
				if (getToken(userChoice, i) != "1" && getToken(userChoice, i) != "2" &&
					getToken(userChoice, i) != "3" && getToken(userChoice, i) != "4" &&
					getToken(userChoice, i) != "5" && getToken(userChoice, i) != "6" &&
					getToken(userChoice, i) != "7" && getToken(userChoice, i) != "8" &&
					getToken(userChoice, i) != "9" && getToken(userChoice, i) != "10")
				{
					validString = false;
					break;
				}
			}
		}

		// if the operation requested is a read to file or write to file operation then the user input must be
		// at least 10 characters long with one token at the end representing the file name
		if ((userChoice[0] == 'R' || userChoice[0] == 'W') && count(userChoice.begin(), userChoice.end(), ' ') != 2)
		{
			validString = false;
			break;
		}

		// if a read or write request has an invalid filename then it isn't valid
		if (userChoice[0] == 'R' || userChoice[0] == 'W')
		{
			string filename = getToken(userChoice, 3);
			for (int i = 0; i < filename.length(); i++)
			{
				// filename must only contain letters, numbers, or a '.'
				if (!isalnum(filename[i]) && filename[i] != '.')
				{
					validString = false;
					break;
				}
			}
		}

		// if print request is longer than 4 characters or less than 3 characters then request is invalid
		if (userChoice[0] == 'P' && ((userChoice.length() < 3 || userChoice.length() > 4) || 
			count(userChoice.begin(), userChoice.end(), ' ') != 1))
		{
			validString = false;
			break;
		}

		break;
	}
	if (validString)
		return userChoice;
	else
		return "Invalid";
}

// pre: variables including the user request as a valid string, the matrix within the request we want
// post: a string holding the token variable we want
string getToken(string userRequest, int tokenNumber)
{
	int index = 0;
	int numOfTokens = 0;
	while(true)
	{
		// traverse the string, if the character is a space continue traversal and increment index
		// if the character at index is not a space increment numOfTokens and skip the rest of the 
		// token by setting a seperate loop to traverse through it
		if (userRequest[index] == ' ')
			index++;
		else if (userRequest[index] != ' ')
		{
			int lengthOfToken = 1;
			numOfTokens++;
			while (userRequest[index + lengthOfToken] != ' ')
				lengthOfToken++;
			if (numOfTokens == tokenNumber)
				return userRequest.substr(index, lengthOfToken);

			index += lengthOfToken;

			if (index >= userRequest.length()) break;
		}
	}
	return userRequest;
}

// pre: a string describing the user requesting an addition of matrices
// post: the result of two matrices added together and placed in the matrix of the user's choice
void addMatrices(string userChoice, Matrix matrixArr[])
{
	// get the three matrices that will be required for the user request
	int M1 = atoi(getToken(userChoice, 2).c_str());
	int M2 = atoi(getToken(userChoice, 3).c_str());
	int M3 = atoi(getToken(userChoice, 4).c_str());

	if (matrixArr[M1 - 1].getMaxRowSize() == matrixArr[M2 - 1].getMaxRowSize() &&
		matrixArr[M1 - 1].getMaxColSize() == matrixArr[M2 - 1].getMaxColSize())
	matrixArr[M3 - 1] = matrixArr[M1 - 1] + matrixArr[M2 - 1];	// add the two matrices and assign them to the third matrix specified
																// by the user
																// indeces must be decremented for proper assignment
	else	// if the dimensions of the matrix are not identical then addition is not possible
		cout << "Error: Dimensions of matrix not equal, addition is not possible" << endl;
}

void subtractMatrices(string userChoice, Matrix matrixArr[])
{
	// get the three matrices that will be required for the user request
	int M1 = atoi(getToken(userChoice, 2).c_str());
	int M2 = atoi(getToken(userChoice, 3).c_str());
	int M3 = atoi(getToken(userChoice, 4).c_str());

	if (matrixArr[M1 - 1].getMaxRowSize() == matrixArr[M2 - 1].getMaxRowSize() &&
		matrixArr[M1 - 1].getMaxColSize() == matrixArr[M2 - 1].getMaxColSize())
	matrixArr[M3 - 1] = matrixArr[M1 - 1] - matrixArr[M2 - 1];	// subtract the two matrices and assign them to the third matrix specified
																// by the user
																// indeces must be decremented for proper assignment
	else	// if the dimensions of the matrix are not identical then addition is not possible
		cout << "Error: Dimensions of matrix not equal, addition is not possible" << endl;
}

void multiplyMatrices(string userChoice, Matrix matrixArr[])
{
	// get the three matrices that will be required for the user request
	int M1 = atoi(getToken(userChoice, 2).c_str());
	int M2 = atoi(getToken(userChoice, 3).c_str());
	int M3 = atoi(getToken(userChoice, 4).c_str());

	if (matrixArr[M1 - 1].getMaxRowSize() == matrixArr[M2 - 1].getMaxColSize() &&
		matrixArr[M1 - 1].getMaxColSize() == matrixArr[M2 - 1].getMaxRowSize())
	matrixArr[M3 - 1] = matrixArr[M1 - 1] * matrixArr[M2 - 1];	// multiply the two matrices and assign them to the third matrix specified
																// by the user
																// indeces must be decremented for proper assignment
	else	// if the dimensions of the matrix are not valid for multiplication then return and error message
		cout << "Error: Dimensions of matrix not appropriate for multiplication\n" << endl;
}

// pre: user input specifying file to be opened and matrix number to copy the data
// post: input containing a sorted list of tiples is read from a file and stored in the array specified in the user input as a 
// linked list
void readFile(string userChoice, Matrix matrixArr[])
{
	// if the matrix which the information to be assigned will go is not empty then clear out it's contents
	matrixArr[atoi(getToken(userChoice, 2).c_str()) - 1].clearList();

	string filename = getToken(userChoice, 3);
	string listOfTriples = "";
	ifstream myfile(filename);
	if (myfile.is_open())
	{
		while (getline(myfile, listOfTriples)) 
		{
			matrixArr[atoi(getToken(userChoice, 2).c_str()) - 1].push(atoi(getToken(listOfTriples, 1).c_str()), atoi(getToken(listOfTriples, 2).c_str()), atoi(getToken(listOfTriples, 3).c_str()));
		}
		myfile.close();
	}
	else
		cout << "Unable to open file\n" << endl;
}


void writeFile(string userChoice, Matrix matrixArr[])
{
	string filename = getToken(userChoice, 3);
	string listOfTriples = "";
	ofstream myfile(filename);
	if (myfile.is_open())
	{
		// assign the matrix data into a string
		string matrixInfo = "";
		MatrixElement* ptrToOutputMatrix = matrixArr[atoi(getToken(userChoice, 2).c_str()) - 1].getList();
		string row = ""; // to temporarily hold the row of a line
		string column = ""; // to temporarily hold the column index of a line
		string value = ""; // to temporarily hold the value of a line
		while (ptrToOutputMatrix != NULL)
		{
			matrixInfo += IntToString(ptrToOutputMatrix->row + 1);
			matrixInfo += " ";
			matrixInfo += IntToString(ptrToOutputMatrix->column + 1);
			matrixInfo += " ";
			matrixInfo += IntToString(ptrToOutputMatrix->value);
			matrixInfo += '\n';

			ptrToOutputMatrix = ptrToOutputMatrix->next;
		}

		myfile.write(matrixInfo.c_str(), matrixInfo.length());
		myfile.close();
	}
	else
		cout << "Unable to open file\n" << endl;
}

// pre: user input specifying matrix number to be printed and a matrix in the array with data
// post: the matrix represented by the list of triples printed to console
void printMatrix(string userChoice, Matrix matrixArr[])
{
	matrixArr[atoi(getToken(userChoice, 2).c_str()) - 1].printMatrix();
}

// pre: an int value
// post: the int value converted to a string
string IntToString(int numIn)
{
    string str;
    ostringstream temp;
    temp << numIn;
    return temp.str();
}