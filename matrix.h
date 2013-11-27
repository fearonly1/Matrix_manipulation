// matrix.h

#ifndef MATRIX_H
#define MATRIX_H

// nodes to be used in list
struct MatrixElement
{
	int row;
	int column;
	int value;
	MatrixElement* next;
};

class Matrix
{
	public:
		Matrix();										// constructor
		Matrix(const Matrix&);							// copy constructor
		~Matrix();										// destructor
		void push(int, int, int);						// push function
		void push(MatrixElement*);						// overloaded push function
		int returnColumnSize() const;					// returns column size of matrix
		int returnRowSize() const;						// returns row size of matrix
		void printMatrix();								// print matrix
		bool isFull() const;							// verifies if matrix is full
		bool isEmpty()const;							// verifies if matrix is empty()
		Matrix& operator=(const Matrix&);				// assignment operator
		Matrix operator+(const Matrix&);				// addition operator
		Matrix operator*(const Matrix&);				// multiplication operator
		Matrix& operator- (const Matrix& rightMatrix);	// subtraction operator
		int getMaxColSize() const;						// returns maximum column size
		int getMaxRowSize() const;						// returns maximum row size
		MatrixElement* getList() const;					// returns the list of matrix elements
		void clearList();								// deletes the list of matrix elements

	private:
		MatrixElement* matrixElements;	// list of matrix elements
		int maxColSize;					// maximum column size
		int maxRowSize;					// maximum row size
};

#endif
