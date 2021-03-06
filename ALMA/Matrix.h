#pragma once
#include<stdexcept>
#include<vector>
#include"LUDecomposition.h"
#include"QRDecomposition.h"
/**
Jama = Java Matrix class.
<P>
The Java Matrix Class provides the fundamental operations of numerical
linear algebra.  Various constructors create Matrices from two dimensional
arrays of double precision floating point numbers.  Various "gets" and
"sets" provide access to submatrices and matrix elements.  Several methods
implement basic matrix arithmetic, including matrix addition and
multiplication, matrix norms, and element-by-element array operations.
Methods for reading and printing matrices are also included.  All the
operations in this version of the Matrix Class involve real matrices.
Complex matrices may be handled in a future version.
<P>
Five fundamental matrix decompositions, which consist of pairs or triples
of matrices, permutation vectors, and the like, produce results in five
decomposition classes.  These decompositions are accessed by the Matrix
class to compute solutions of simultaneous linear equations, determinants,
inverses and other matrix functions.  The five decompositions are:
<P><UL>
<LI>Cholesky Decomposition of symmetric, positive definite matrices.
<LI>LU Decomposition of rectangular matrices.
<LI>QR Decomposition of rectangular matrices.
<LI>Singular Value Decomposition of rectangular matrices.
<LI>Eigenvalue Decomposition of both symmetric and nonsymmetric square matrices.
</UL>
<DL>
<DT><B>Example of use:</B></DT>
<P>
<DD>Solve a linear system A x = b and compute the residual norm, ||b - A x||.
<P><PRE>
double[][] vals = {{1.,2.,3},{4.,5.,6.},{7.,8.,10.}};
Matrix A = new Matrix(vals);
Matrix b = Matrix.random(3,1);
Matrix x = A.solve(b);
Matrix r = A.times(x).minus(b);
double rnorm = r.normInf();
</PRE></DD>
</DL>
@author The MathWorks, Inc. and the National Institute of Standards and Technology.
@version 5 August 1998
*/
#include <exception>

 class Matrix {

	/* ------------------------
	Class variables
	* ------------------------ */

	/** Array for internal storage of elements.
	@serial internal array storage.
	*/
 private:
	// double A[][] ;
	 std::vector< std::vector< double > > A;

/** Row and column dimensions.
@serial row dimension.
@serial column dimension.
*/
	 int m, n;
	 void checkMatrixDimensions(Matrix B) {
		 if (B.m != m || B.n != n) {
			 throw std::invalid_argument("Matrix dimensions must agree.");
		 }
	 }
/* ------------------------
Constructors
* ------------------------ */

/** Construct an m-by-n matrix of zeros.
@param m    Number of rows.
@param n    Number of colums.
*/

 public:
	 Matrix(int M, int N) {
		 m = M;
		 n = N;
		 A.resize(m, std::vector<double>(n));
	 }

/** Construct an m-by-n constant matrix.
@param m    Number of rows.
@param n    Number of colums.
@param s    Fill the matrix with this scalar value.
*/

	Matrix(int M, int N, double s) {
	m = M;
	n = N;
	A.resize(m, std::vector<double>(n));
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			A[i][j] = s;
		}
	}
}

/** Construct a matrix from a 2-D array.
@param A    Two-dimensional array of doubles.
@exception  IllegalArgumentException All rows must have the same size()
@see        #constructWithCopy
*/

	Matrix(std::vector< std::vector<double>> a) {
	m = a.size();
	n = a[0].size();
	for (int i = 0; i < m; i++) {
		if (a[i].size() != n) {
			throw std::invalid_argument("All rows must have the same size().");
		}
	}
	A = a;
}

	Matrix(const std::vector<std::array<double, 3>>&a) {
		m = a.size();
		n = a[0].size();
		for (int i = 0; i < m; i++) {
			if (a[i].size() != n) {
				throw std::invalid_argument("All rows must have the same size().");
			}
		}
		for (auto& v : a)
		{
			A.push_back(std::vector<double>(v.begin(), v.end()));
		}
	//	A = a;
	}
/** Construct a matrix quickly without checking arguments.
@param A    Two-dimensional array of doubles.
@param m    Number of rows.
@param n    Number of colums.
*/

	 Matrix(std::vector< std::vector<double>> a, int M, int N) {
	A = a;
	m = M;
	n = N;
}

/** Construct a matrix from a one-dimensional packed array
@param vals One-dimensional array of doubles, packed by columns (ala Fortran).
@param m    Number of rows.
@exception  IllegalArgumentException Array size() must be a multiple of m.
*/

	 Matrix(std::vector<double> vals, int M) {
	m = M;
	n = (m != 0 ? vals.size() / m : 0);
	if (m*n != vals.size()) {
		throw std::invalid_argument("Array size() must be a multiple of m.");
	}
	A [m][n];
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			A[i][j] = vals[i + j*m];
		}
	}
}

/* --		----------------------
Public Methods
* ------------------------ */

/** Construct a matrix from a copy of a 2-D array.
@param A    Two-dimensional array of doubles.
@exception  IllegalArgumentException All rows must have the same size()
*/

 static Matrix constructWithCopy(std::vector< std::vector<double>> a) {
	int m = a.size();
	int n = a[0].size();
	Matrix X =  Matrix(m,n);
	std::vector< std::vector<double>> C = X.getArray();
	for (int i = 0; i < m; i++) {
		if (a[i].size() != n) {
			throw std::invalid_argument("All rows must have the same size().");
		}
		for (int j = 0; j < n; j++) {
			C[i][j] = a[i][j];
		}
	}
	return X;
}

/** Make a deep copy of a matrix
*/

 Matrix copy() {
	Matrix X =  Matrix(m,n);
	std::vector< std::vector<double>> C = X.getArray();
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			C[i][j] = A[i][j];
		}
	}
	return X;
}

/** Clone the Matrix object.
*/

 /*Object clone() {
	return this.copy();
}*/

/** Access the internal two-dimensional array.
@return     Pointer to the two-dimensional array of matrix elements.
*/

 std::vector< std::vector<double>>  getArray() {
	return A;
}

 /*std::vector< std::vector<double>>  &getArray() {
	 return A;
 }*/

/** Copy the internal two-dimensional array.
@return     Two-dimensional array copy of matrix elements.
*/

 std::vector< std::vector<double>>  getArrayCopy() {
	std::vector< std::vector<double>> C;
	C.resize(m, std::vector<double>(n));
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			C[i][j] = A[i][j];
		}
	}
	return C;
}

/** Make a one-dimensional column packed copy of the internal array.
@return     Matrix elements packed in a one-dimensional array by columns.
*/

  std::vector<double> getColumnPackedCopy() {
	std::vector<double> vals ;
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			vals[i + j*m] = A[i][j];
		}
	}
	return vals;
}

/** Make a one-dimensional row packed copy of the internal array.
@return     Matrix elements packed in a one-dimensional array by rows.
*/

 std::vector<double> getRowPackedCopy() {
	std::vector<double> vals;
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			vals[i*n + j] = A[i][j];
		}
	}
	return vals;
}

/** Get row dimension.
@return     m, the number of rows.
*/

 int getRowDimension() {
	return m;
}

/** Get column dimension.
@return     n, the number of columns.
*/

 int getColumnDimension() {
	return n;
}

/** Get a single element.
@param i    Row index.
@param j    Column index.
@return     A(i,j)
@exception  int
*/

 double get(int i, int j) {
	return A[i][j];
}

/** Get a submatrix.
@param i0   Initial row index
@param i1   Final row index
@param j0   Initial column index
@param j1   Final column index
@return     A(i0:i1,j0:j1)
@exception  int Submatrix indices
*/

 Matrix getMatrix(int i0, int i1, int j0, int j1) {
	Matrix X =  Matrix(i1 - i0 + 1,j1 - j0 + 1);
	std::vector< std::vector<double>> B = X.getArray();
	try {
		for (int i = i0; i <= i1; i++) {
			for (int j = j0; j <= j1; j++) {
				B[i - i0][j - j0] = A[i][j];
			}
		}
	}
	catch (int e) {
		throw ("Submatrix indices");
	}
	return X;
}

/** Get a submatrix.
@param r    Array of row indices.
@param c    Array of column indices.
@return     A(r(:),c(:))
@exception  int Submatrix indices
*/

 Matrix getMatrix(std::vector<int> r , std::vector<int> c ) {
	Matrix X =  Matrix(r.size(),c.size());
	std::vector< std::vector<double>> B = X.getArray();
	try {
		for (int i = 0; i < r.size(); i++) {
			for (int j = 0; j < c.size(); j++) {
				B[i][j] = A[r[i]][c[j]];
			}
		}
	}
	catch (int e) {
		throw ("Submatrix indices");
	}
	return X;
}

/** Get a submatrix.
@param i0   Initial row index
@param i1   Final row index
@param c    Array of column indices.
@return     A(i0:i1,c(:))
@exception  int Submatrix indices
*/

 Matrix getMatrix(int i0, int i1, std::vector<int> c ) {
	Matrix X =  Matrix(i1 - i0 + 1,c.size());
	std::vector< std::vector<double>> B = X.getArray();
	try {
		for (int i = i0; i <= i1; i++) {
			for (int j = 0; j < c.size(); j++) {
				B[i - i0][j] = A[i][c[j]];
			}
		}
	}
	catch (int e) {
		throw ("Submatrix indices");
	}
	return X;
}

/** Get a submatrix.
@param r    Array of row indices.
@param j0   Initial column index
@param j1   Final column index
@return     A(r(:),j0:j1)
@exception  int Submatrix indices
*/
 void setA(std::vector< std::vector<double>> B)
 {
	 A = B;
 }
 Matrix getMatrix(std::vector<int> r , int j0, int j1) {
	Matrix X =  Matrix(r.size(),j1 - j0 + 1);
	std::vector< std::vector<double>> B = X.getArray();
	try {
		for (int i = 0; i < r.size(); i++) {
			for (int j = j0; j <= j1; j++) {
				B[i][j - j0] = A[r[i]][j];
			}
		}
	}
	catch (int e) {
		throw ("Submatrix indices");
	}
	X.setA(B);
	return X;
}

/** Set a single element.
@param i    Row index.
@param j    Column index.
@param s    A(i,j).
@exception  int
*/

 void set(int i, int j, double s) {
	A[i][j] = s;
}

/** Set a submatrix.
@param i0   Initial row index
@param i1   Final row index
@param j0   Initial column index
@param j1   Final column index
@param X    A(i0:i1,j0:j1)
@exception  int Submatrix indices
*/

 void setMatrix(int i0, int i1, int j0, int j1, Matrix X) {
	try {
		for (int i = i0; i <= i1; i++) {
			for (int j = j0; j <= j1; j++) {
				A[i][j] = X.get(i - i0,j - j0);
			}
		}
	}
	catch (int e) {
		throw ("Submatrix indices");
	}
}

/** Set a submatrix.
@param r    Array of row indices.
@param c    Array of column indices.
@param X    A(r(:),c(:))
@exception  int Submatrix indices
*/

 void setMatrix(std::vector<int> r , std::vector<int> c, Matrix X) {
	try {
		for (int i = 0; i < r.size(); i++) {
			for (int j = 0; j < c.size(); j++) {
				A[r[i]][c[j]] = X.get(i,j);
			}
		}
	}
	catch (int e) {
		throw ("Submatrix indices");
	}
}

/** Set a submatrix.
@param r    Array of row indices.
@param j0   Initial column index
@param j1   Final column index
@param X    A(r(:),j0:j1)
@exception  int Submatrix indices
*/

 void setMatrix(std::vector<int> r , int j0, int j1, Matrix X) {
	try {
		for (int i = 0; i < r.size(); i++) {
			for (int j = j0; j <= j1; j++) {
				A[r[i]][j] = X.get(i,j - j0);
			}
		}
	}
	catch (int e) {
		throw ("Submatrix indices");
	}
}

/** Set a submatrix.
@param i0   Initial row index
@param i1   Final row index
@param c    Array of column indices.
@param X    A(i0:i1,c(:))
@exception  int Submatrix indices
*/

 void setMatrix(int i0, int i1, std::vector<int> c , Matrix X) {
	try {
		for (int i = i0; i <= i1; i++) {
			for (int j = 0; j < c.size(); j++) {
				A[i][c[j]] = X.get(i - i0,j);
			}
		}
	}
	catch (int e) {
		throw ("Submatrix indices");
	}
}

/** Matrix transpose.
@return    A'
*/

 Matrix transpose() {
	Matrix X =  Matrix(n,m);
	std::vector< std::vector<double>> C = X.getArray();
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			C[j][i] = A[i][j];
		}
	}
	return X;
}

/** One norm
@return    maximum column sum.
*/
 double max(double f, double s){
	 if (f > s)
		 return f;
	 return s;
 }
 double norm1() {
	double f = 0;
	for (int j = 0; j < n; j++) {
		double s = 0;
		for (int i = 0; i < m; i++) {
			s +=abs(A[i][j]);
		}
		f = max(f,s);
	}
	return f;
}

/** Two norm
@return    maximum singular value.


 double norm2() {
	return (new SingularValueDecomposition(this).norm2());
}*/

/** Infinity norm
@return    maximum row sum.
*/

 double normInf() {
	double f = 0;
	for (int i = 0; i < m; i++) {
		double s = 0;
		for (int j = 0; j < n; j++) {
			s += abs(A[i][j]);
		}
		f = max(f,s);
	}
	return f;
}

/** Frobenius norm
@return    sqrt of sum of squares of all elements.
*/

 double normF() {
	double f = 0;
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			f = hypot(f,A[i][j]);
		}
	}
	return f;
}

/**  Unary minus
@return    -A
*/

 Matrix uminus() {
	Matrix X =  Matrix(m,n);
	std::vector< std::vector<double>> C = X.getArray();
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			C[i][j] = -A[i][j];
		}
	}
	return X;
}

/** C = A + B
@param B    another matrix
@return     A + B
*/
 Matrix plus(Matrix B) {
	checkMatrixDimensions(B);
	Matrix X =  Matrix(m,n);
	std::vector< std::vector<double>> C = X.getArray();
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			C[i][j] = A[i][j] + B.A[i][j];
		}
	}
	return X;
}

/** A = A + B
@param B    another matrix
@return     A + B
*/

 Matrix* plusEquals(Matrix B) {
	checkMatrixDimensions(B);
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			A[i][j] = A[i][j] + B.A[i][j];
		}
	}
	return this;
}

/** C = A - B
@param B    another matrix
@return     A - B
*/
 Matrix minus(Matrix B) {
	checkMatrixDimensions(B);
	Matrix X =  Matrix(m,n);
	std::vector< std::vector<double>> C = X.getArray();
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			C[i][j] = A[i][j] - B.A[i][j];
		}
	}
	return X;
}

/** A = A - B
@param B    another matrix
@return     A - B
*/

 Matrix* minusEquals(Matrix B) {
	checkMatrixDimensions(B);
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			A[i][j] = A[i][j] - B.A[i][j];
		}
	}
	return this;
}

/** Element-by-element multiplication, C = A.*B
@param B    another matrix
@return     A.*B
*/

 Matrix arrayTimes(Matrix B) {
	checkMatrixDimensions(B);
	Matrix X =  Matrix(m,n);
	std::vector< std::vector<double>> C = X.getArray();
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			C[i][j] = A[i][j] * B.A[i][j];
		}
	}
	return X;
}

/** Element-by-element multiplication in place, A = A.*B
@param B    another matrix
@return     A.*B
*/

 Matrix* arrayTimesEquals(Matrix B) {
	checkMatrixDimensions(B);
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			A[i][j] = A[i][j] * B.A[i][j];
		}
	}
	return this;
}

/** Element-by-element right division, C = A./B
@param B    another matrix
@return     A./B
*/
 Matrix arrayRightDivide(Matrix B) {
	checkMatrixDimensions(B);
	Matrix X =  Matrix(m,n);
	std::vector< std::vector<double>> C = X.getArray();
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			C[i][j] = A[i][j] / B.A[i][j];
		}
	}
	return X;
}

/** Element-by-element right division in place, A = A./B
@param B    another matrix
@return     A./B
*/

 Matrix* arrayRightDivideEquals(Matrix B) {
	checkMatrixDimensions(B);
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			A[i][j] = A[i][j] / B.A[i][j];
		}
	}
	return this;
}

/** Element-by-element left division, C = A.\B
@param B    another matrix
@return     A.\B
*/

 Matrix arrayLeftDivide(Matrix B) {
	checkMatrixDimensions(B);
	Matrix X =  Matrix(m,n);
	std::vector< std::vector<double>> C = X.getArray();
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			C[i][j] = B.A[i][j] / A[i][j];
		}
	}
	return X;
}

/** Element-by-element left division in place, A = A.\B
@param B    another matrix
@return     A.\B
*/

 Matrix* arrayLeftDivideEquals(Matrix B) {
	checkMatrixDimensions(B);
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			A[i][j] = B.A[i][j] / A[i][j];
		}
	}
	return this;
}

/** Multiply a matrix by a scalar, C = s*A
@param s    scalar
@return     s*A
*/

 Matrix times(double s) {
	Matrix X =  Matrix(m,n);
	std::vector< std::vector<double>> C = X.getArray();
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			C[i][j] = s*A[i][j];
		}
	}
	return X;
}

/** Multiply a matrix by a scalar in place, A = s*A
@param s    scalar
@return     replace A by s*A
*/

	Matrix* timesEquals(double s) {
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			A[i][j] = s*A[i][j];
		}
	}
	return this;
}

/** Linear algebraic matrix multiplication, A * B
@param B    another matrix
@return     Matrix product, A * B
@exception  IllegalArgumentException Matrix inner dimensions must agree.
*/

 Matrix times(Matrix B) {
	if (B.m != n) {
		throw std::invalid_argument("Matrix inner dimensions must agree.");
	}
	Matrix X =  Matrix(m,B.n);
	std::vector< std::vector<double>> C = X.getArray();
	std::vector<double>  Bcolj;
	for (int j = 0; j < B.n; j++) {
		for (int k = 0; k < n; k++) {
			Bcolj[k] = B.A[k][j];
		}
		for (int i = 0; i < m; i++) {
			std::vector<double>	Arowi  = A[i];
			double s = 0;
			for (int k = 0; k < n; k++) {
				s += Arowi[k] * Bcolj[k];
			}
			C[i][j] = s;
		}
	}
	return X;
}

/** LU Decomposition
@return     LUDecomposition
@see LUDecomposition
*/

 LUDecomposition<Matrix> lu() {
	return  LUDecomposition<Matrix>(this);
}

/** QR Decomposition
@return     QRDecomposition
@see QRDecomposition
*/
 QRDecomposition<Matrix> qr() {
	return  QRDecomposition<Matrix>(this);
}

/** Cholesky Decomposition
@return     CholeskyDecomposition
@see CholeskyDecomposition


 CholeskyDecomposition chol() {
	return new CholeskyDecomposition(this);
}*/

/** Singular Value Decomposition
@return     SingularValueDecomposition
@see SingularValueDecomposition


 SingularValueDecomposition svd() {
	return new SingularValueDecomposition(this);
}*/

/** Eigenvalue Decomposition
@return     EigenvalueDecomposition
@see EigenvalueDecomposition


 EigenvalueDecomposition eig() {
	return new EigenvalueDecomposition(this);
}*/

/** Solve A*X = B
@param B    right hand side
@return     solution if A is square, least squares solution otherwise
*/

 Matrix solve(Matrix B) {
	return (m == n ? ( LUDecomposition<Matrix>(this)).solve(B) :
		( QRDecomposition<Matrix>(this)).solve(B));
}

/** Solve X*A = B, which is also A'*X' = B'
@param B    right hand side
@return     solution if A is square, least squares solution otherwise.
*/

 Matrix solveTranspose(Matrix B) {
	return transpose().solve(B.transpose());
}

/** Matrix inverse or pseudoinverse
@return     inverse(A) if A is square, pseudoinverse otherwise.
*/

 Matrix inverse() {
	return solve(identity(m,m));
}

/** Matrix determinant
@return     determinant
*/

 double det() {
	return  LUDecomposition<Matrix>(this).det();
}

/** Matrix rank
@return     effective numerical rank, obtained from SVD.


 int rank() {
	return new SingularValueDecomposition(this).rank();
}*/

/** Matrix condition (2 norm)
@return     ratio of largest to smallest singular value.


 double cond() {
	return new SingularValueDecomposition(this).cond();
}*/

/** Matrix trace.
@return     sum of the diagonal elements.
*/

 int min(int x, int y)
 {
	 if (x > y)
		 return y;
	 return x;
 }
 double trace() {
	double t = 0;
	for (int i = 0; i < min(m,n); i++) {
		t += A[i][i];
	}
	return t;
}

/** Generate matrix with random elements
@param m    Number of rows.
@param n    Number of colums.
@return     An m-by-n matrix with uniformly distributed random elements.
*/
 static float random() { return static_cast <float> (rand()) / static_cast <float> (RAND_MAX); }
 static Matrix random(int m, int n) {
	Matrix A =  Matrix(m,n);
	std::vector< std::vector<double>> X  = A.getArray();
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			X[i][j] = random();
		}
	}
	return A;
}

/** Generate identity matrix
@param m    Number of rows.
@param n    Number of colums.
@return     An m-by-n matrix with ones on the diagonal and zeros elsewhere.
*/

 static Matrix identity(int m, int n) {
	Matrix A =  Matrix(m,n);
	std::vector< std::vector<double>> X  = A.getArray();
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			X[i][j] = (i == j ? 1.0 : 0.0);
		}
	}
	return A;
}


/** Print the matrix to stdout.   Line the elements up in columns
* with a Fortran-like 'Fw.d' style format.
@param w    Column width.
@param d    Number of digits after the decimal.


 void print(int w, int d) {
	print(new PrintWriter(System.out,true),w,d);
}

/** Print the matrix to the output stream.   Line the elements up in
* columns with a Fortran-like 'Fw.d' style format.
@param output Output stream.
@param w      Column width.
@param d      Number of digits after the decimal.


 void print(PrintWriter output, int w, int d) {
	DecimalFormat format = new DecimalFormat();
	format.setDecimalFormatSymbols(new DecimalFormatSymbols(Locale.US));
	format.setMinimumIntegerDigits(1);
	format.setMaximumFractionDigits(d);
	format.setMinimumFractionDigits(d);
	format.setGroupingUsed(false);
	print(output,format,w + 2);
}

/** Print the matrix to stdout.  Line the elements up in columns.
* Use the format object, and right justify within columns of width
* characters.
* Note that is the matrix is to be read back in, you probably will want
* to use a NumberFormat that is set to US Locale.
@param format A  Formatting object for individual elements.
@param width     Field width for each column.
@see java.text.DecimalFormat#setDecimalFormatSymbols


 void print(NumberFormat format, int width) {
	print(new PrintWriter(System.out,true),format,width);
}

// DecimalFormat is a little disappointing coming from Fortran or C's printf.
// Since it doesn't pad on the left, the elements will come out different
// widths.  Consequently, we'll pass the desired column width in as an
// argument and do the extra padding ourselves.

/** Print the matrix to the output stream.  Line the elements up in columns.
* Use the format object, and right justify within columns of width
* characters.
* Note that is the matrix is to be read back in, you probably will want
* to use a NumberFormat that is set to US Locale.
@param output the output stream.
@param format A formatting object to format the matrix elements
@param width  Column width.
@see java.text.DecimalFormat#setDecimalFormatSymbols


 void print(PrintWriter output, NumberFormat format, int width) {
	output.println();  // start on new line.
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			String s = format.format(A[i][j]); // format the number
			int padding = Math.max(1,width - s.size()()); // At _least_ 1 space
			for (int k = 0; k < padding; k++)
				output.print(' ');
			output.print(s);
		}
		output.println();
	}
	output.println();   // end with blank line.
}

/** Read a matrix from a stream.  The format is the same the print method,
* so printed matrices can be read back in (provided they were printed using
* US Locale).  Elements are separated by
* whitespace, all the elements for each row appear on a single line,
* the last row is followed by a blank line.
@param input the input stream.
*/
 /*
 static Matrix read(BufferedReader input) throws java.io.IOException{
	StreamTokenizer tokenizer = new StreamTokenizer(input);

// Although StreamTokenizer will parse numbers, it doesn't recognize
// scientific notation (E or D); however, Double.valueOf does.
// The strategy here is to disable StreamTokenizer's number parsing.
// We'll only get whitespace delimited words, EOL's and EOF's.
// These words should all be numbers, for Double.valueOf to parse.

tokenizer.resetSyntax();
tokenizer.wordChars(0,255);
tokenizer.whitespaceChars(0, ' ');
tokenizer.eolIsSignificant(true);
java.util.Vector v = new java.util.Vector();

// Ignore initial empty lines
while (tokenizer.nextToken() == StreamTokenizer.TT_EOL);
if (tokenizer.ttype == StreamTokenizer.TT_EOF)
throw new java.io.IOException("Unexpected EOF on matrix read.");
do {
	v.addElement(Double.valueOf(tokenizer.sval)); // Read & store 1st row.
} while (tokenizer.nextToken() == StreamTokenizer.TT_WORD);

int n = v.size();  // Now we've got the number of columns!
double row[n] ;
for (int j = 0; j<n; j++)  // extract the elements of the 1st row.
	row[j] = ((Double)v.elementAt(j)).doubleValue();
v.removeAllElements();
v.addElement(row);  // Start storing rows instead of columns.
while (tokenizer.nextToken() == StreamTokenizer.TT_WORD) {
	// While non-empty lines
	v.addElement(row = new double[n]);
	int j = 0;
	do {
		if (j >= n) throw new java.io.IOException
		("Row " + v.size() + " is too long.");
		row[j++] = Double.valueOf(tokenizer.sval).doubleValue();
	} while (tokenizer.nextToken() == StreamTokenizer.TT_WORD);
	if (j < n) throw new java.io.IOException
	("Row " + v.size() + " is too short.");
}
int m = v.size();  // Now we've got the number of rows.
double A[m][] ;
v.copyInto(A);  // copy the rows out of the vector
return new Matrix(A);
}


/* ------------------------
Private Methods
* ------------------------ */

/** Check if size(A) == size(B) **/




};