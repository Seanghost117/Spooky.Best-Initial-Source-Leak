#pragma once


class vector
{
public:
	// Members
	float x{}, y{}, z{};

	// Construction/destruction:
	vector()
	{
		//Invalidate();
		x = y = z = 0.f;
	}
	vector(float X, float Y, float Z)
	{
		x = X;
		y = Y;
		z = Z;
	}

	vector(const float* clr)
	{
		x = clr[0];
		y = clr[1];
		z = clr[2];
	}

	// Initialization
	void Init(float ix = 0.0f, float iy = 0.0f, float iz = 0.0f)
	{
		x = ix; y = iy; z = iz;
	}

	// Got any nasty NAN's?
	bool IsValid() const
	{
		return !isinf(x) && !isinf(y) && !isinf(z) && !IsZero();
	}
	void Invalidate()
	{
		x = y = z = std::numeric_limits<float>::infinity();
	}

	// array access...
	float operator[](int i) const
	{
		return ((float*)this)[i];
	}
	float& operator[](int i)
	{
		return reinterpret_cast<float*>(this)[i];
	}

	// Base address...
	float* Base()
	{
		return reinterpret_cast<float*>(this);
	}
	float const* Base() const
	{
		return reinterpret_cast<float const*>(this);
	}

	// Initialization methods
	void RandomV(float minVal, float maxVal)
	{
		x = minVal + static_cast<float>(rand()) / RAND_MAX * (maxVal - minVal);
		y = minVal + static_cast<float>(rand()) / RAND_MAX * (maxVal - minVal);
		z = minVal + static_cast<float>(rand()) / RAND_MAX * (maxVal - minVal);
	}
	void Zero() ///< zero out a vector
	{
		x = y = z = 0.0f;
	}

	// equality
	bool operator==(const vector& v) const
	{
		return v.x == x && v.y == y && v.z == z;
	}
	bool operator!=(const vector& v) const
	{
		return (v.x != x) || (v.y != y) || (v.z != z);
	}

	// arithmetic operations
	vector& operator+=(const vector& v)
	{
		x += v.x; y += v.y; z += v.z;
		return *this;
	}

	vector& operator-=(const vector& v)
	{
		x -= v.x; y -= v.y; z -= v.z;
		return *this;
	}

	vector& operator*=(float fl)
	{
		x *= fl;
		y *= fl;
		z *= fl;
		return *this;
	}

	vector& operator*=(const vector& v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		return *this;
	}

	vector& operator/=(const vector& v)
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;
		return *this;
	}

	// this ought to be an opcode.
	vector&	operator+=(float fl)
	{
		x += fl;
		y += fl;
		z += fl;
		return *this;
	}

	// this ought to be an opcode.
	vector&	operator/=(float fl)
	{
		x /= fl;
		y /= fl;
		z /= fl;
		return *this;
	}
	vector&	operator-=(float fl)
	{
		x -= fl;
		y -= fl;
		z -= fl;
		return *this;
	}

	// negate the vector components
	void	Negate()
	{
		x = -x; y = -y; z = -z;
	}

	// Get the vector's magnitude.
	float	Length() const
	{
		return sqrt(x*x + y * y + z * z);
	}

	// Get the vector's magnitude squared.
	float LengthSqr() const
	{
		return x * x + y * y + z * z;
	}

	// return true if this vector is (0,0,0) within tolerance
	bool IsZero(float tolerance = 0.01f) const
	{
		return x > -tolerance && x < tolerance &&
			y > -tolerance && y < tolerance &&
			z > -tolerance && z < tolerance;
	}

	static float Normalizevector(vector& v)
	{
		const float l = v.Length();
		if (l != 0.0f) {
			v /= l;
		}
		else {
			v.x = v.y = v.z = 0.0f;
		}
		return l;
	}
	float	NormalizeInPlace()
	{
		return Normalizevector(*this);
	}
	vector	Normalized() const
	{
		vector res = *this;
		const float l = res.Length();
		if (l != 0.0f) {
			res /= l;
		}
		else {
			res.x = res.y = res.z = 0.0f;
		}
		return res;
	}

	// check if a vector is within the box defined by two other vectors
	bool WithinAABox(vector const &boxmin, vector const &boxmax) const
	{
		return (
			(x >= boxmin.x) && (x <= boxmax.x) &&
			(y >= boxmin.y) && (y <= boxmax.y) &&
			(z >= boxmin.z) && (z <= boxmax.z)
			);
	}

	// Get the distance from this vector to the other one.
	float	DistTo(const vector &vOther) const
	{
		vector delta = *this - vOther;
		return delta.Length();
	}

	// Get the distance from this vector to the other one squared.
	// NJS: note, VC wasn't inlining it correctly in several deeply nested inlines due to being an 'out of line' .  
	// may be able to tidy this up after switching to VC7
	float DistToSqr(const vector &vOther) const
	{
		vector delta;

		delta.x = x - vOther.x;
		delta.y = y - vOther.y;
		delta.z = z - vOther.z;

		return delta.LengthSqr();
	}

	// Copy
	void	CopyToArray(float* rgfl) const
	{
		rgfl[0] = x, rgfl[1] = y, rgfl[2] = z;
	}

	void Mul(float scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
	}

	// Multiply, add, and assign to this (ie: *this = a + b * scalar). This
	// is about 12% faster than the actual vector equation (because it's done per-component
	// rather than per-vector).
	void	MulAdd(const vector& a, const vector& b, float scalar)
	{
		x = a.x + b.x * scalar;
		y = a.y + b.y * scalar;
		z = a.z + b.z * scalar;
	}

	vector CrossProduct(const vector& a, const vector& b)
	{
		return vector(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
	}
	vector Cross(const vector& vOther)
	{
		return CrossProduct(*this, vOther);
	}

	// Dot product.
	float	Dot(const vector& b) const
	{
		return (x*b.x + y * b.y + z * b.z);
	}

	// assignment
	vector& operator=(const vector &vOther)
		= default;

	// 2d
	float	Length2D() const
	{
		return sqrt(x * x + y * y);
	}

	/// get the component of this vector parallel to some other given vector
	vector  ProjectOnto(const vector& onto) const
	{
		return onto * (this->Dot(onto) / (onto.LengthSqr()));
	}

	// copy constructors
	//	vector(const vector &vOther);

	// arithmetic operations
	vector	operator-() const
	{
		return vector(-x, -y, -z);
	}

	vector	operator+(const int& v) const
	{
		return vector(x + v, y + v, z + v);
	}
	vector	operator+(const vector& v) const
	{
		return vector(x + v.x, y + v.y, z + v.z);
	}
	vector	operator-(const vector& v) const
	{
		return vector(x - v.x, y - v.y, z - v.z);
	}
	vector	operator*(const vector& v) const
	{
		return vector(x * v.x, y * v.y, z * v.z);
	}
	vector	operator/(const vector& v) const
	{
		return vector(x / v.x, y / v.y, z / v.z);
	}
	vector	operator*(float fl) const
	{
		return vector(x * fl, y * fl, z * fl);
	}
	vector	operator/(float fl) const
	{
		return vector(x / fl, y / fl, z / fl);
	}
};

typedef vector qangle;

class __declspec(align(16)) vectorAligned : public vector
{
public:
	vectorAligned() : w(0)
	{
	};
	vectorAligned(float X, float Y, float Z) : w(0)
	{
		Init(X, Y, Z);
	}

	explicit vectorAligned(const vector &vOther) : w(0)
	{
		Init(vOther.x, vOther.y, vOther.z);
	}

	vectorAligned& operator=(const vector &vOther)
	{
		Init(vOther.x, vOther.y, vOther.z);
		return *this;
	}

	vectorAligned& operator=(const vectorAligned &vOther)
	{
		_mm_store_ps(Base(), _mm_load_ps(vOther.Base()));
		return *this;
	}

	float w;
};

static void vectorAdd(const vector& a, const vector& b, vector& c)
{
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	c.z = a.z + b.z;
}

static void vectorCopy(const vector& src, vector& dst)
{
	dst.x = src.x;
	dst.y = src.y;
	dst.z = src.z;
}

static void vectorSubtract(const vector& a, const vector& b, vector& c)
{
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;
}

static void vectorClear(vector& a)
{
	a.x = a.y = a.z = 0.0f;
}