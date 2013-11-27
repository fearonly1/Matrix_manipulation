// matrix.cpp

#include "matrix.h"
#include <iostream>
#include <string>
using namespace std;

// post: A Matrix object with a linked list pointing to NULL
Matrix::Matrix()
{
	matrixElements = NULL;
	maxColSize = 0;
	maxRowSize = 0;
}

// pre: At least one matrix object that is already initialized
// post: The first matrix object is identical to the second
Matrix::Matrix(const Matrix& SecondMatrix)
{
	matrixElements = new MatrixElement;

	MatrixElement* ptrToLocalList = matrixElements;
	MatrixElement* ptrToForiegnList = SecondMatrix.matrixElements;
	maxColSize = SecondMatrix.maxColSize;
	maxRowSize = SecondMatrix.maxRowSize;

	if (SecondMatrix.matrixElements == NULL)
		matrixElements = NULL;
	else
	{
		ptrToForiegnList = new MatrixElement;
		for (ptrToForiegnList = SecondMatrix.matrixElements; ptrToForiegnList != NULL; ptrToForiegnList = ptrToForiegnList->next)
		{
			ptrToLocalList->column = ptrToForiegnList->column;
			ptrToLocalList->row = ptrToForiegnList->row;
			ptrToLocalList->value = ptrToForiegnList->value;
			if (ptrToForiegnList->next != NULL)
			{
				ptrToLocalList->next = new MatrixElement;
				ptrToLocalList = ptrToLocalList->next;
			}
			else
			{
				ptrToLocalList->next = NULL;
				break;
			}
		}
	}
}

// Pre: Either a NULL matrix or an initialized one
// Post: All memory used by matrix has been deallocated
Matrix::~Matrix()
{
	if (matrixElements != NULL)
	{
		MatrixElement* temp = matrixElements;	// pointer to the local linked list
		while(matrixElements != NULL)	// traverse list and delete all content
		{
			temp = matrixElements;
			matrixElements = matrixElements->next;
			delete temp;
		}

		maxColSize = 0;
		maxRowSize = 0;

	}
}

// Pre: The row, column, and value numbers of a new element represented in a list of triples
// Post: Matrix object has new value added to it's linked list
void Matrix::push(int row, int column, int value)
{
	if (!isFull)
	{
		MatrixElement* temp = new MatrixElement;
		if (maxColSize < column) maxColSize = column;
		if (maxRowSize < row) maxRowSize = row;
		temp->row = --row;			
		temp->column = --column;
		temp->value = value;
		temp->next = NULL;
		push(temp);
	}
	else
		cout << "Matrix is full, cannot push more values" << endl;
}

// Pre: The row, column, and value numbers of a new element represented in a list of triples
// Post: Matrix object has new value added to it's linked list
void Matrix::push(MatrixElement* listHeadIn)
{
	if (!isFull)
	{
		MatrixElement* temp = new MatrixElement;
		temp = listHeadIn;
	
		// if the linked list is empty
		if (matrixElements == NULL)
		{
			matrixElements = temp;
			return;
		}
	
		// traverse this Matrix and add values of linked list appropriately
		MatrixElement* prev = NULL;
		MatrixElement* currentPtr = matrixElements;
		while (currentPtr != NULL) // examining the elements of the operand list
		{
			// case 1: listHeadIn is pointing to an element that belongs in the beginning of the list
			if (prev == NULL && ((currentPtr->row >= temp->row) && (temp->column < currentPtr->column)))
			{
				temp->next = matrixElements;
				matrixElements = temp;
				break;
			}
	
			// case 2: listHeadIn is pointing to an element that has an equal position in the local linked list
			else if (currentPtr->column == temp->column && currentPtr->row == temp->row)
			{
				currentPtr->value = currentPtr->value + temp->value;
				break;
			}
	
			// case 3: listHeadIn is pointing to an element of which there is no equal in the linked list of the right matrix
			// This would imply that the either whole local linked list has already been traversed and an equal value hasn't been found,
			// or that a spot for it's insertion is found by calculating the positions of the surrounding nodes
			else if (((((temp->row < currentPtr->row) || (temp->row == currentPtr->row)) && 
				((temp->column < currentPtr->column))) && (((prev->row < temp->row) || prev->row == temp->row) &&
				(prev->column < temp->column))))
			{
				prev->next = temp;
				temp->next = currentPtr;
				break;
			}
	
			// case 4: listHeadIn is pointing to an element that belongs in the end of the list
			else if (currentPtr->next == NULL && ((((currentPtr->row <= temp->row) && (temp->column > currentPtr->column))) ||
				(currentPtr->row < temp->row)))
			{
				currentPtr->next = new MatrixElement;
				currentPtr->next->row = temp->row;
				currentPtr->next->column = temp->column;
				currentPtr->next->value = temp->value;
				currentPtr->next->next = NULL;
				break;
			}
	
			prev = currentPtr;
			currentPtr = currentPtr->next;
		}
	}
	else
		cout << "Matrix is full, cannot push more values" << endl;
}
	
//post: A integer representing the column size of the matrix
int Matrix::returnColumnSize() const
{
	return maxColSize;
}

// Post: An integer representing the row size of the matrix
int Matrix::returnRowSize() const
{
	return maxRowSize;
}

// Pre: A matrix object with either an empty or initialized linked list
// Post: A sparse matrix printed to console
void Matrix::printMatrix()
{
	MatrixElement* ptrToList = matrixElements;
	int numOfElements = maxColSize * maxRowSize;	// the total number of matrix elements
	int columnIndex = 0;
	int rowIndex = 0;
	cout << endl;
	while(ptrToList != NULL)
	{
		// if the node is a value but the previous nodes are not then continue filling the
		// previous spots until the position of the next value is reached
		while (ptrToList->column > columnIndex && ptrToList->row >= rowIndex)
		{
			if (columnIndex == 0) cout << "0";
			else cout << " 0";
			columnIndex++;
			if (columnIndex == getMaxColSize() && rowIndex != getMaxRowSize()) // if the columnIndex has reached the maximum column size then increase the row index and reset the columnIndex
			{
				rowIndex++;
				columnIndex = 0;
				cout << endl;
			}
		}
		
		// if the element being pointed to is in the same position as the current column and row indexes then place it's value in the matrix position
		if (ptrToList->column == columnIndex && ptrToList->row == rowIndex)
		{
			if (columnIndex != 0) cout << " ";
			cout << ptrToList->value;
			columnIndex++;
			if (columnIndex == maxColSize && rowIndex != maxRowSize) // if the columnIndex has reached the maximum column size then increase the row index and reset the columnIndex
			{
				rowIndex++;
				columnIndex = 0;
				cout << endl;
			}
		}

		// if the end of the list has been reached but the array hasn't been filled print out a zero and increment indexes
		if (ptrToList->next == NULL && rowIndex != maxRowSize)
		{
			while (columnIndex != maxColSize)
			{
				if (columnIndex != 0) cout << " ";
				cout << "0";
				columnIndex++;
			}
		}

		// traverse to next position in list
		ptrToList = ptrToList->next;
	}
	cout << endl << endl;
}

// Post: A boolean value indicating whether or not the matrix is full
bool Matrix::isFull() const
{
	Matrix* temp = new Matrix;
	if (temp == NULL)
		return true;
	else
		return false;
}

// Post: A boolean value indicating whether or not the matrix is empty
bool Matrix::isEmpty() const
{
	if (matrixElements != NULL)
		return false;
	else 
		return true;
}

// pre: two matrices
// post: the left operand matrix becomes equal to the right operand matrix
Matrix& Matrix::operator= (const Matrix& SecondMatrix)
{
	MatrixElement* ptr1;
	MatrixElement* ptr2;
	this->maxRowSize = SecondMatrix.maxRowSize;
	this->maxColSize = SecondMatrix.maxColSize;

	if (SecondMatrix.matrixElements == NULL)
	{
		this->matrixElements = NULL;
		return *this;
	}
	else
	{
		ptr1 = SecondMatrix.matrixElements;
		this->matrixElements = new MatrixElement;
		ptr2 = this->matrixElements;
		while(ptr1 != NULL)
		{
			ptr2->column = ptr1->column;
			ptr2->row = ptr1->row;
			ptr2->value = ptr1->value;
			ptr2->next = new MatrixElement;
			ptr2 = ptr2->next;
			ptr1 = ptr1->next;
		}

		ptr2->next = NULL; // set the last element to NULL
	}

	return *this;
}

// pre: two matrix objects of the same dimensions
// post: the result of two matrices added together
Matrix Matrix::operator+ (const Matrix& rightMatrix)
{
	if (rightMatrix.matrixElements == NULL)
		return *this;
	else
	{
		// make a negated copy of the right matrix operand
		MatrixElement* ptr3;
		MatrixElement* ptr4;
		Matrix negatedForeignMatrix;
		negatedForeignMatrix.maxRowSize = maxRowSize;
		negatedForeignMatrix.maxColSize = maxColSize;
		
		negatedForeignMatrix.matrixElements = new MatrixElement;
		ptr4 = negatedForeignMatrix.matrixElements;
		for (MatrixElement* ptr3 = rightMatrix.matrixElements; ptr3 != NULL; ptr3 = ptr3->next)
		{
			ptr4->column = ptr3->column;
			ptr4->row = ptr3->row;
			ptr4->value = ptr3->value;
			if (ptr3->next != NULL)
			{
				ptr4->next = new MatrixElement;
				ptr4 = ptr4->next;
			}
			else
			{
				ptr4->next = NULL;
				break;
			}
		}
	
		ptr4->next = NULL; // set the last element to NULL
		
		// the code here adds the matrix represented by the array copy and the negated rightMatrix
		// traverse this Matrix and add values of linked list appropriately
		MatrixElement* ptrToForeignList = negatedForeignMatrix.matrixElements;
		while (ptrToForeignList != NULL)
		{
	
			MatrixElement* prev = NULL;
			MatrixElement* currentPtr = matrixElements;
			while (currentPtr != NULL) // examining the elements of the operand list
			{
				// case 1: listHeadIn is pointing to an element that belongs in the beginning of the list
				if (prev == NULL && ((currentPtr->row >= ptrToForeignList->row) && (ptrToForeignList->column < currentPtr->column)))
				{
					ptrToForeignList->next = negatedForeignMatrix.matrixElements;
					matrixElements = ptrToForeignList;
					break;
				}
		
				// case 2: listHeadIn is pointing to an element that has an equal position in the local linked list
				else if (currentPtr->column == ptrToForeignList->column && currentPtr->row == ptrToForeignList->row)
				{
					currentPtr->value = currentPtr->value + ptrToForeignList->value;
					break;
				}
		
				// case 3: listHeadIn is pointing to an element of which there is no equal in the linked list of the right matrix
				// This would imply that the either whole local linked list has already been traversed and an equal value hasn't been found,
				// or that a spot for it's insertion is found by calculating the positions of the surrounding nodes
				else if (((((ptrToForeignList->row < currentPtr->row) || (ptrToForeignList->row == currentPtr->row)) && 
					((ptrToForeignList->column < currentPtr->column))) && (((prev->row < ptrToForeignList->row) || prev->row == ptrToForeignList->row) &&
					(prev->column < ptrToForeignList->column))))
				{
					prev->next = ptrToForeignList;
					ptrToForeignList->next = currentPtr;
					break;
				}
		
				// case 4: listHeadIn is pointing to an element that belongs in the end of the list
				else if (currentPtr->next == NULL && ((((currentPtr->row <= ptrToForeignList->row) && (ptrToForeignList->column > currentPtr->column))) ||
					(currentPtr->row < ptrToForeignList->row)))
				{
					currentPtr->next = new MatrixElement;
					currentPtr->next->row = ptrToForeignList->row;
					currentPtr->next->column = ptrToForeignList->column;
					currentPtr->next->value = ptrToForeignList->value;
					currentPtr->next->next = NULL;
					break;
				}
		
				prev = currentPtr;
				currentPtr = currentPtr->next;
			}

			ptrToForeignList = ptrToForeignList->next;
		}
	}
	return *this;
}

// pre: two matrix objects with dimensions appropriate for multiplication
// post: the result of two matrices multiplied together
Matrix Matrix::operator* (const Matrix& rightMatrix)
{
	// create a third matrix that will contain the product of the first two matrices
	Matrix productMatrix;
	productMatrix.maxRowSize = this->maxRowSize;
	productMatrix.maxColSize = rightMatrix.maxColSize;

	if (rightMatrix.matrixElements == NULL)
		return *this;
	else
	{
		// make a copy of the right matrix operand
		MatrixElement* ptr3;
		MatrixElement* ptr4;
		Matrix copyOfForeignMatrix;
		copyOfForeignMatrix.maxRowSize = maxRowSize;
		copyOfForeignMatrix.maxColSize = maxColSize;
		
		copyOfForeignMatrix.matrixElements = new MatrixElement;
		ptr4 = copyOfForeignMatrix.matrixElements;
		for (MatrixElement* ptr3 = rightMatrix.matrixElements; ptr3 != NULL; ptr3 = ptr3->next)
		{
			ptr4->column = ptr3->column;
			ptr4->row = ptr3->row;
			ptr4->value = ptr3->value;
			if (ptr3->next != NULL)
			{
				ptr4->next = new MatrixElement;
				ptr4 = ptr4->next;
			}
			else
			{
				ptr4->next = NULL;
				break;
			}
		}
	
		ptr4->next = NULL; // set the last element to NULL

		MatrixElement* ptrToFirstList = this->matrixElements;	// pointer for traversing first matrixElement list
		while (ptrToFirstList != NULL)	// loop for traversing position of first matrixElement list
		{
			MatrixElement* ptrToSecondList = rightMatrix.matrixElements;	// pointer for traversing second matrixElement list
				while (ptrToSecondList != NULL)	// loop for traversing position of second matrixElement list
				{
					// if the current element of the first matrixElement list has a column number equal to the row number of the element in the second matrixElement list
					// then multiply the two values together and set them to the element of the product matrix with a row index equal to the row index of the element currently
					// being pointed to in the first list, and a column index equal to the column index of the element currently being pointed to in the second matrixElement list
					if (ptrToFirstList->column == ptrToSecondList->row)
					{
						MatrixElement* product = new MatrixElement;	// new matrixElement object to hold the product of the two matrix elements
						product->row = ptrToFirstList->row;
						product->column = ptrToSecondList->column;
						product->value = ptrToFirstList->value * ptrToSecondList->value;
							
						// traverse the matrixElement list of the third matrix to check and see if any nodes already exist with the column and row numbers of the new
						// matrix element node
						productMatrix.push(product);
					}
			
					ptrToSecondList = ptrToSecondList->next;	// point to next element in list
				}
	
			ptrToFirstList = ptrToFirstList->next; // pont to the next element in list
		}

		return productMatrix;
	}
}

// pre: two matrix objects of the same dimensions
// post: the result of two matrices after subtraction
Matrix& Matrix::operator- (const Matrix& rightMatrix)
{
	if (rightMatrix.matrixElements == NULL)
		return *this;
	else
	{
		// make a negated copy of the right matrix operand
		MatrixElement* ptr3;
		MatrixElement* ptr4;
		Matrix negatedForeignMatrix;
		negatedForeignMatrix.maxRowSize = maxRowSize;
		negatedForeignMatrix.maxColSize = maxColSize;
		
		negatedForeignMatrix.matrixElements = new MatrixElement;
		ptr4 = negatedForeignMatrix.matrixElements;
		for (MatrixElement* ptr3 = rightMatrix.matrixElements; ptr3 != NULL; ptr3 = ptr3->next)
		{
			ptr4->column = ptr3->column;
			ptr4->row = ptr3->row;
			ptr4->value = -1 * ptr3->value;
			if (ptr3->next != NULL)
			{
				ptr4->next = new MatrixElement;
				ptr4 = ptr4->next;
			}
			else
			{
				ptr4->next = NULL;
				break;
			}
		}
	
		ptr4->next = NULL; // set the last element to NULL
		

		// the code here adds the matrix represented by the array copy and the negated rightMatrix
		// traverse this Matrix and add values of linked list appropriately
		MatrixElement* ptrToForeignList = negatedForeignMatrix.matrixElements;
		while (ptrToForeignList != NULL)
		{
	
			MatrixElement* prev = NULL;
			MatrixElement* currentPtr = matrixElements;
			while (currentPtr != NULL) // examining the elements of the operand list
			{
				// case 1: listHeadIn is pointing to an element that belongs in the beginning of the list
				if (prev == NULL && ((currentPtr->row >= ptrToForeignList->row) && (ptrToForeignList->column < currentPtr->column)))
				{
					ptrToForeignList->next = negatedForeignMatrix.matrixElements;
					matrixElements = ptrToForeignList;
					break;
				}
		
				// case 2: listHeadIn is pointing to an element that has an equal position in the local linked list
				else if (currentPtr->column == ptrToForeignList->column && currentPtr->row == ptrToForeignList->row)
				{
					currentPtr->value = currentPtr->value + ptrToForeignList->value;
					break;
				}
		
				// case 3: listHeadIn is pointing to an element of which there is no equal in the linked list of the right matrix
				// This would imply that the either whole local linked list has already been traversed and an equal value hasn't been found,
				// or that a spot for it's insertion is found by calculating the positions of the surrounding nodes
				else if (((((ptrToForeignList->row < currentPtr->row) || (ptrToForeignList->row == currentPtr->row)) && 
					((ptrToForeignList->column < currentPtr->column))) && (((prev->row < ptrToForeignList->row) || prev->row == ptrToForeignList->row) &&
					(prev->column < ptrToForeignList->column))))
				{
					prev->next = ptrToForeignList;
					ptrToForeignList->next = currentPtr;
					break;
				}
		
				// case 4: listHeadIn is pointing to an element that belongs in the end of the list
				else if (currentPtr->next == NULL && ((((currentPtr->row <= ptrToForeignList->row) && (ptrToForeignList->column > currentPtr->column))) ||
					(currentPtr->row < ptrToForeignList->row)))
				{
					currentPtr->next = new MatrixElement;
					currentPtr->next->row = ptrToForeignList->row;
					currentPtr->next->column = ptrToForeignList->column;
					currentPtr->next->value = ptrToForeignList->value;
					currentPtr->next->next = NULL;
					break;
				}
		
				prev = currentPtr;
				currentPtr = currentPtr->next;
			}

			ptrToForeignList = ptrToForeignList->next;
		}
	}

	return *this;
}

// post: an integer representing the column dimension of this matrix
int Matrix::getMaxColSize() const
{
	return maxColSize;
}

// post: an integer representing the row dimension of this matrix
int Matrix::getMaxRowSize() const
{
	return maxRowSize;
}

// post: the head of the linked list containing the matrix elements
MatrixElement* Matrix::getList() const
{
	return matrixElements;
}

// pre: a linked list containing nodes representing elements in a matrix
// post: the linked list has all memory deallocated and the dimensions of the matrix are set to 0
void Matrix::clearList()
{
	while(matrixElements != NULL)
	{
		MatrixElement* temp = matrixElements;
		matrixElements = matrixElements->next;
		delete temp;
	}
	maxColSize = 0;
	maxRowSize = 0;
}
