#ifndef TSquareMatrix_h
#define TSquareMatrix_h
#define OK 0
#define BAD 1

class TSquareMatrix
{
	private:
		unsigned _size; //matrix is _size*_size
		double** _values;
		double* _tmp;

		//private methods
		void deleteArray(double** ptr, unsigned size);
		void createArray(double*** ptr, unsigned size);
		double determinant(double** matrix, unsigned size);
		void setValues(double** source, unsigned srcSize, double** destination, unsigned destSize);
		void initTmp();
		void deleteTmp();
		unsigned myCat(char** s, char* tmp, unsigned* strLen);


	public:
		//constructors and destructors
		TSquareMatrix();
		TSquareMatrix(double** values, unsigned size);
		TSquareMatrix(const TSquareMatrix& matrix);
		~TSquareMatrix();
		//accessors
		void setSize(unsigned size);
		void setSizeWithSavingValues(unsigned size);
		void fill(double** source, unsigned srcSize);
		void setValueByIndex(double value, unsigned i, unsigned j);
		unsigned getSize();
		void copyValuesTo(double*** emptyPtr); //allocates new memory!!!
		double getValueByIndex(unsigned i, unsigned j);
		//other methods
		TSquareMatrix addTo(const TSquareMatrix& other); //result = this + other
		TSquareMatrix subtractFrom(const TSquareMatrix& other); //result = other - this
		TSquareMatrix multiplyBy(const TSquareMatrix& other); //result = this * other
		TSquareMatrix divideBy(const TSquareMatrix& other); //result = this / other
		TSquareMatrix getTransposed();
		TSquareMatrix getMinorMatrix(unsigned i, unsigned j);
		double getMinor(unsigned i, unsigned j);
		double getAlgebraicComplement(unsigned i, unsigned j);
		TSquareMatrix getInversed();
		double getDeterminant();
		TSquareMatrix multiplyBy(double number);
		TSquareMatrix divideBy(double number);
		unsigned findElementByValue(double value, unsigned* cnt, unsigned*** indexes); //allocates new memory!!! return OK or BAD
		unsigned printValuesToStr(char** str, unsigned precisionDigitsInFractionalPart); //allocates new memory with malloc!!! returns OK or BAD
		void printValues(unsigned precisionDigitsInFractionalPart); 
		bool isTheSameAs(const TSquareMatrix& other);
		double maxValue();
		double minValue();
		//Override operators
		TSquareMatrix& operator = (const TSquareMatrix& other);
		TSquareMatrix& operator += (const TSquareMatrix& other);
		TSquareMatrix& operator -= (const TSquareMatrix& other);
		TSquareMatrix& operator *= (const TSquareMatrix& other);
		TSquareMatrix& operator /= (const TSquareMatrix& other);
		TSquareMatrix& operator *= (double number);
		TSquareMatrix& operator /= (double number);
		TSquareMatrix operator + ();
		TSquareMatrix operator - ();
		TSquareMatrix operator + (const TSquareMatrix& other);
		TSquareMatrix operator - (const TSquareMatrix& other);
		TSquareMatrix operator * (const TSquareMatrix& other);
		TSquareMatrix operator / (const TSquareMatrix& other);
		TSquareMatrix operator * (double number);
		TSquareMatrix operator / (double number);
		double*& operator [] (unsigned i);
		bool operator == (const TSquareMatrix& other);
		bool operator != (const TSquareMatrix& other);
		friend TSquareMatrix operator * (double number, TSquareMatrix& matrix);
		friend TSquareMatrix operator / (double number, TSquareMatrix& matrix);
};

#endif //TSquareMatrix_h
