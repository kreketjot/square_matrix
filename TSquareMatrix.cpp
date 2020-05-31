#include "TSquareMatrix.h"
#include <cstdlib>
#include <cstdio>
#include <cstring>

#define DEFAULT_STR_LEN 1024

//constructors and destructors
TSquareMatrix::TSquareMatrix() : _size(0), _values(nullptr), _tmp(nullptr) {}

TSquareMatrix::TSquareMatrix(double** values, unsigned size)
{
	_size = size;
	createArray(&_values, size);
	setValues(values, size, _values, size);
	initTmp();
}

TSquareMatrix::TSquareMatrix(const TSquareMatrix& matrix) : TSquareMatrix(matrix._values, matrix._size) {}
	
TSquareMatrix::~TSquareMatrix()
{
	deleteArray(_values, _size);
	deleteTmp();
}

//accessors
void TSquareMatrix::setSize(unsigned size)
{
	if(size == _size)
		return;
	deleteArray(_values, _size);
	_size = size;
	createArray(&_values, _size);
	deleteTmp();
	initTmp();
}

void TSquareMatrix::setSizeWithSavingValues(unsigned size)
{
	double** oldValues = _values;

	if(size == _size || !_size)
		return;
	createArray(&_values, size);
	setValues(oldValues, _size, _values, size);
	deleteArray(oldValues, _size);
	_size = size;
	deleteTmp();
	initTmp();
}

void TSquareMatrix::setValues(double** source, unsigned srcSize, double** destination, unsigned destSize)
{
	if(!srcSize || !destSize || source == nullptr || destination == nullptr)
		return;
	for(unsigned i=0; i<destSize; i++)
		for(unsigned j=0; j<destSize; j++)
			if(i<srcSize && j<srcSize)
				destination[i][j] = source[i][j];
			else
				destination[i][j] = 0;
}

void TSquareMatrix::fill(double** source, unsigned srcSize)
{
	setValues(source, srcSize, _values, _size);
}

void TSquareMatrix::setValueByIndex(double value, unsigned i, unsigned j)
{
	i--;
	j--;
	if(i<_size && j<_size)
		_values[i][j] = value;
}

unsigned TSquareMatrix::getSize()
{
	return _size;
}

void TSquareMatrix::copyValuesTo(double*** emptyPtr) //allocates new memory!!!
{
	createArray(emptyPtr, _size);
	setValues(_values, _size, *emptyPtr, _size);
}

double TSquareMatrix::getValueByIndex(unsigned i, unsigned j)
{
	i--;
	j--;
	if(i<_size && j<_size)
		return _values[i][j];
	else
		return 0;
}

//other methods
TSquareMatrix TSquareMatrix::addTo(const TSquareMatrix& other) //result = this + other
{
	TSquareMatrix matrix;

	if(_size == other._size && _size)
	{
		matrix.setSize(_size);
		for(unsigned i=0; i<_size; i++)
			for(unsigned j=0; j<_size; j++)
				matrix._values[i][j] = _values[i][j] + other._values[i][j];
	}
	return matrix;
}

TSquareMatrix TSquareMatrix::subtractFrom(const TSquareMatrix& other) //result = other - this
{
	TSquareMatrix matrix;

	if(_size == other._size && _size)
	{
		matrix.setSize(_size);
		for(unsigned i=0; i<_size; i++)
			for(unsigned j=0; j<_size; j++)
				matrix._values[i][j] = other._values[i][j] - _values[i][j];
	}
	return matrix;
}

TSquareMatrix TSquareMatrix::multiplyBy(const TSquareMatrix& other) //result = this * other
{
	TSquareMatrix matrix;
	double tmp;

	if(_size == other._size && _size)
	{
		matrix.setSize(_size);
		for(unsigned i=0; i<_size; i++)
			for(unsigned j=0; j<_size; j++)
			{
				tmp = 0;
				for(unsigned k=0; k<_size; k++)
					tmp += _values[i][k] * other._values[k][j];
				matrix._values[i][j] = tmp;
			}
	}
	return matrix;
}

TSquareMatrix TSquareMatrix::divideBy(const TSquareMatrix& other) //result = this / other
{
	TSquareMatrix matrix(other);
	return multiplyBy(matrix.getInversed());
}

TSquareMatrix TSquareMatrix::getTransposed()
{
	TSquareMatrix matrix;

	if(_size)
	{
		matrix.setSize(_size);
		for(unsigned i=0; i<_size; i++)
			for(unsigned j=0; j<_size; j++)
				matrix._values[i][j] = _values[j][i];
	}
	return matrix;
}

TSquareMatrix TSquareMatrix::getMinorMatrix(unsigned i, unsigned j)
{
	TSquareMatrix matrix;
	unsigned size = _size - 1;

	if(size)
	{
		i--;
		j--;
		matrix.setSize(size);
		for(unsigned l=0; l<size; l++)
			for(unsigned k=0; k<size; k++)
				if(l < i && k < j)
					matrix._values[l][k] = _values[l][k];
				else if(l < i && k >= j)
					matrix._values[l][k] = _values[l][k+1];
				else if(l >= i && k < j)
					matrix._values[l][k] = _values[l+1][k];
				else
					matrix._values[l][k] = _values[l+1][k+1];
	}
	return matrix;
}

double TSquareMatrix::getMinor(unsigned i, unsigned j)
{
	return getMinorMatrix(i, j).getDeterminant();
}

double TSquareMatrix::getAlgebraicComplement(unsigned i, unsigned j)
{
	double res = getMinor(i, j);
	if((i+j) % 2)
		res = -res;
	return res;
}

TSquareMatrix TSquareMatrix::getInversed()
{
	TSquareMatrix tmpMatrix, resMatrix;
	double det = getDeterminant();

	if(_size && det)
	{
		tmpMatrix = getTransposed();
		resMatrix.setSize(_size);
		for(unsigned i=0; i<_size; i++)
			for(unsigned j=0; j<_size; j++)
				resMatrix._values[i][j] = tmpMatrix.getAlgebraicComplement(i+1, j+1) / det;
	}
	return resMatrix;
}

double TSquareMatrix::getDeterminant()
{
	double** ptr, det = 0;

	if(_size)
	{
		createArray(&ptr, _size);
		setValues(_values, _size, ptr, _size);
		det = determinant(ptr, _size);
		deleteArray(ptr, _size);
	}
	return det;
}

TSquareMatrix TSquareMatrix::multiplyBy(double number)
{
	TSquareMatrix matrix;

	if(_size)
	{
		matrix.setSize(_size);
		for(unsigned i=0; i<_size; i++)
			for(unsigned j=0; j<_size; j++)
				matrix._values[i][j] = _values[i][j] * number;
	}
	return matrix;
}

unsigned TSquareMatrix::findElementByValue(double value, unsigned* cnt, unsigned*** indexes)
{
	unsigned* ind, **indices;
	unsigned cnt_ = 0, indLen = 2;

	if(!_size)
		return BAD;
	ind = (unsigned*) malloc(2*indLen*sizeof(unsigned));
	if(ind == nullptr)
		return BAD;
	for(unsigned i=0; i<_size; i++)
		for(unsigned j=0; j<_size; j++)
			if(_values[i][j] == value)
			{
				cnt_++;
				if(indLen < cnt_)
				{
					indLen *= 2;
					ind = (unsigned*) realloc(ind, 2*indLen*sizeof(unsigned));
					if(ind == nullptr)
						return BAD;
				}
				*(ind + (cnt_-1)*2) = i;
				*(ind + (cnt_-1)*2 + 1) = j;
			}
	if(cnt_)
	{
		indices = new unsigned*[cnt_];
		for(unsigned i=0; i<cnt_; i++)
		{
			indices[i] = new unsigned[2];
			indices[i][0] = *(ind + i*2);
			indices[i][1] = *(ind + i*2 + 1);
		}
	}
	else
		indices = nullptr;
	free(ind);
	*indexes = indices;
	*cnt = cnt_;
	return OK;
}

unsigned TSquareMatrix::printValuesToStr(char** str, unsigned precisionDigitsInFractionalPart)
{
	char* s, tmp[DEFAULT_STR_LEN];
	unsigned strLen = DEFAULT_STR_LEN, indexLen2 = 0, indexLen1, tmp_ = _size-1, maxLen=0;
	double max;

	s = (char*) malloc(strLen);
	if(s == nullptr)
		return BAD;
	if(_size)
	{
		strcpy(s, "  ");
		do
		{
			tmp_ /= 10;
			indexLen2++;
			strcat(s, " ");
		}
		while(tmp_);
		//find maxLen
		max = minValue();
		if(max < 0)
			max = -max;
		if(maxValue() > max)
			max = maxValue();
		do
		{
			max /= 10;
			maxLen++;
		}
		while(max >= 1);
		tmp_ = maxLen + precisionDigitsInFractionalPart;
		indexLen1 = indexLen2;
		if(indexLen1 < tmp_)
			indexLen1 = tmp_;
		//end of findinf maxLen
		for(unsigned i=0; i<_size;)
		{
			snprintf(tmp, DEFAULT_STR_LEN, "  j=%-*u", indexLen1, ++i);
			if(myCat(&s, tmp, &strLen) == BAD)
				return BAD;
		}
		for(unsigned i=0; i<_size; i++)
		{
			snprintf(tmp, DEFAULT_STR_LEN, "\ni=%-*u", indexLen2, i+1);
			if(myCat(&s, tmp, &strLen) == BAD)
				return BAD;
			for(unsigned j=0; j<_size; j++)
			{
				snprintf(tmp, DEFAULT_STR_LEN, "  %-+#*.*f", maxLen, precisionDigitsInFractionalPart, _values[i][j]);
				if(myCat(&s, tmp, &strLen) == BAD)
					return BAD;
			}
		}
	}
	else
		strcpy(s, "This is empty matrix. It wasn't initialized or this matrix is result of incorrect action (e.g. A/0, A*B where A and B are different sizes, etc).");
	*str = s;
	return OK;
}

void TSquareMatrix::printValues(unsigned precisionDigitsInFractionalPart)
{
	char* str;

	if(printValuesToStr(&str, precisionDigitsInFractionalPart) != OK)
		perror("The following error occurred");
	else
	{
		fputs(str, stdout);
		free(str);
	}
	puts("");
}

unsigned TSquareMatrix::myCat(char** s, char* tmp, unsigned* strLen)
{
	if(strlen(tmp) + strlen(*s) >= *strLen)
	{
		*strLen = *strLen * 2;
		*s = (char*) realloc(s, *strLen);
		if(*s == nullptr)
			return BAD;
	}
	strcat(*s, tmp);
	return OK;
}

bool TSquareMatrix::isTheSameAs(const TSquareMatrix& other)
{
	if(_size != other._size)
		return false;
	if(!_size)
		return true;
	for(unsigned i=0; i<_size; i++)
		for(unsigned j=0; j<_size; j++)
			if(_values[i][j] != other._values[i][j])
				return false;
	return true;
}

double TSquareMatrix::maxValue()
{
	double max = 0;

	if(_size)
	{
		max = **_values;
		for(unsigned i=0; i<_size; i++)
			for(unsigned j=0; j<_size; j++)
				if(_values[i][j] > max)
					max = _values[i][j];
	}
	return max;	
}

double TSquareMatrix::minValue()
{
	double min = 0;

	if(_size)
	{
		min = **_values;
		for(unsigned i=0; i<_size; i++)
			for(unsigned j=0; j<_size; j++)
				if(_values[i][j] < min)
					min = _values[i][j];
	}
	return min;	
}

void TSquareMatrix::deleteArray(double** ptr, unsigned size)
{
	if(ptr == nullptr || !size)
		return;
	for(unsigned i=0; i<size; i++)
		delete[] ptr[i];
	delete[] ptr;
}

void TSquareMatrix::createArray(double*** ptr, unsigned size)
{
	double** arr;

	if(!size)
	{
		ptr = nullptr;
		return;
	}
	arr = new double*[size];
	for(unsigned i=0; i<size; i++)
		arr[i] = new double[size];
	*ptr = arr;
}

double TSquareMatrix::determinant(double** matrix, unsigned size)
{
	double** oldMatrix = matrix;
	double res = 0;
	char sign = 1;

	switch(size)
	{
		case 0:
			break;
		case 1:
			res = **oldMatrix;
			break;
		case 2:
			res = oldMatrix[0][0]*oldMatrix[1][1] - oldMatrix[0][1]*oldMatrix[1][0];
			break;
		default:
			double** newMatrix;

			createArray(&newMatrix, size-1);
			for(unsigned i=0; i<size; i++)
			{
				size--;
				for(unsigned j=0; j<size; j++)
					for(unsigned k=0; k<size; k++)
						if(k < i)
							newMatrix[j][k] = oldMatrix[j+1][k];
						else
							newMatrix[j][k] = oldMatrix[j+1][k+1];
				res += sign*oldMatrix[0][i]*determinant(newMatrix, size);
				sign = -sign;
				size++;
			}
			deleteArray(newMatrix, size-1);
			break;
	}
	return res;
}

void TSquareMatrix::initTmp()
{
	if(_size)
	{
		_tmp = new double[_size];
		for(unsigned i=0; i<_size; i++)
			_tmp[i] = 0;
	}
	else
		_tmp = nullptr;
}

void TSquareMatrix::deleteTmp()
{
	if(_tmp != nullptr)
		delete[] _tmp;
}

TSquareMatrix& TSquareMatrix::operator = (const TSquareMatrix& other)
{
	if(this != &other)
	{
		setSize(other._size);
		fill(other._values, _size);
	}
	return *this;
}

TSquareMatrix& TSquareMatrix::operator += (const TSquareMatrix& other)
{
	*this = addTo(other);
	return *this;
}

TSquareMatrix& TSquareMatrix::operator -= (const TSquareMatrix& other)
{
	*this = *this - other;
	return *this;
}

TSquareMatrix& TSquareMatrix::operator *= (const TSquareMatrix& other)
{
	*this = multiplyBy(other);
	return *this;
}

TSquareMatrix& TSquareMatrix::operator /= (const TSquareMatrix& other)
{
	*this = divideBy(other);
	return *this;
}

TSquareMatrix& TSquareMatrix::operator *= (double number)
{
	*this = multiplyBy(number);
	return *this;
}

TSquareMatrix& TSquareMatrix::operator /= (double number)
{
	*this = *this / number;
	return *this;
}

TSquareMatrix TSquareMatrix::operator + ()
{
	return *this;
}

TSquareMatrix TSquareMatrix::operator - ()
{
	return multiplyBy(-1);
}

TSquareMatrix TSquareMatrix::operator + (const TSquareMatrix& other)
{
	return addTo(other);
}

TSquareMatrix TSquareMatrix::operator - (const TSquareMatrix& other)
{
	TSquareMatrix matrix(other);
	return matrix.subtractFrom(*this);
}

TSquareMatrix TSquareMatrix::operator * (const TSquareMatrix& other)
{
	return multiplyBy(other);
}

TSquareMatrix TSquareMatrix::operator / (const TSquareMatrix& other)
{
	return divideBy(other);
}

TSquareMatrix TSquareMatrix::operator * (double number)
{
	return multiplyBy(number);
}

TSquareMatrix TSquareMatrix::operator / (double number)
{
	if(number)
		return multiplyBy(1 / number);
	TSquareMatrix matrix;
	return matrix;
}

double*& TSquareMatrix::operator [] (unsigned i)
{
	i--;
	if(i < _size)
		return _values[i];
	return _tmp;
}

bool TSquareMatrix::operator == (const TSquareMatrix& other)
{
	return isTheSameAs(other);
}

bool TSquareMatrix::operator != (const TSquareMatrix& other)
{
	return !(isTheSameAs(other));
}

TSquareMatrix operator * (double number, TSquareMatrix& matrix)
{
	return matrix.multiplyBy(number);
}

TSquareMatrix operator / (double number, TSquareMatrix& matrix)
{
	return matrix.getInversed() * number;
}
