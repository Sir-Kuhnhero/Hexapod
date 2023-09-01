#include <Arduino.h>
#include <header.h>

// ================================================================
// ===                           math                           ===
// ================================================================
#pragma region Vector3

Vector3::Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

#pragma region operatio overload
// Overload the + operator for vector subtraction
Vector3 Vector3::operator+(const Vector3 &other) const
{
    return Vector3(x + other.x, y + other.y, z + other.z);
}

// Overload the - operator for vector subtraction
Vector3 Vector3::operator-(const Vector3 &other) const
{
    return Vector3(x - other.x, y - other.y, z - other.z);
}

// Overload the * operator for scaling the vector by a scalar (double)
Vector3 Vector3::operator*(const double &scalar) const
{
    return Vector3(x * scalar, y * scalar, z * scalar);
}

// Overload the * operator for scaling the vector by a scalar (float)
Vector3 Vector3::operator*(const float &scalar) const
{
    return Vector3(x * scalar, y * scalar, z * scalar);
}

// Overload the * operator for scaling the vector by a scalar (int)
Vector3 Vector3::operator*(const int &scalar) const
{
    return Vector3(x * scalar, y * scalar, z * scalar);
}

// Overload the assignment operator
Vector3 &Vector3::operator=(const Vector3 &other)
{
    if (this != &other)
    {
        x = other.x;
        y = other.y;
        z = other.z;
    }
    return *this;
}

// due to floating-point precision two floats might not be exactly the same
bool Vector3::operator==(const Vector3 &other) const
{
    return almostEqual(x, other.x) && almostEqual(y, other.y) && almostEqual(z, other.z);
}
#pragma endregion

#pragma region Vector functions
// !! currently not working !! returns a rotated vector (does not modify the origional vector) !! currently not working
Vector3 Vector3::rotate(const float &angle, const char &axis)
{
    Vector3 output;

    float cosTheta = cos(angle);
    float sinTheta = sin(angle);

    switch (axis)
    {
    case 'x':
        output.y = y * cosTheta - z * sinTheta;
        output.z = y * sinTheta + z * cosTheta;
        break;
    case 'y':
        output.x = x * cosTheta + z * sinTheta;
        output.z = -x * sinTheta + z * cosTheta;
        break;
    case 'z':
        output.x = x * cosTheta - y * sinTheta;
        output.y = x * sinTheta + y * cosTheta;
        break;
    default:
        // No rotation for unrecognized axis
        break;
    }

    return output;
}

// returns a normalized vector (Read Only)
Vector3 Vector3::normalized()
{
    float length = sqrt(x * x + y * y + z * z);

    Vector3 output(x, y, z);
    if (length != 0.0)
    {

        output.x /= length;
        output.y /= length;
        output.z /= length;
    }

    return output;
}

// returns the length of a vector (Read Only)
float Vector3::magnitude() const
{
    return sqrt(x * x + y * y + z * z);
}

// returns the inverse of the vector (Read Only)
Vector3 Vector3::inverse() const
{
    Vector3 output(x * -1, y * -1, z * -1);

    return output;
}

// Linearly interpolates between two points.
Vector3 Vector3::Lerp(const Vector3 &start, const Vector3 &end, const float &t)
{
    Vector3 result;
    result.x = (1 - t) * start.x + t * end.x;
    result.y = (1 - t) * start.y + t * end.y;
    result.z = (1 - t) * start.z + t * end.z;
    return result;
}

// normalizes the input vector
Vector3 Vector3::Normalize(Vector3 &vector)
{
    float length = sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);

    if (length != 0.0)
    {

        vector.x /= length;
        vector.y /= length;
        vector.z /= length;
    }

    return vector;
}

float Vector3::Dot(const Vector3 &a, const Vector3 &b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

// !! currently only works with angles bellow 90° !! returns tha ansgle betweem two vectors
float Angle(const Vector3 &from, const Vector3 &to)
{
    // Calculate the cosine of the angle using the dot product and vector magnitudes
    float cosAngle = Vector3::Dot(from, to) / (from.magnitude() * to.magnitude());

    // Calculate the angle in radians adn convert to degree
    return acos(cosAngle) * RAD_TO_DEG;
}

// clamps the length of a vector but keeps the direction
Vector3 Vector3::ClampMagnitude(Vector3 &vector, const float &magnitude)
{
    if (vector.magnitude() > magnitude)
        return Vector3::Normalize(vector) * magnitude;

    return vector;
}
#pragma endregion

#pragma endregion

#pragma region Vector2

Vector2::Vector2(float _x, float _y) : x(_x), y(_y)
{
}

#pragma region operatio overload
// Overload the + operator for vector subtraction
Vector2 Vector2::operator+(const Vector2 &other) const
{
    return Vector2(x + other.x, y + other.y);
}

// Overload the - operator for vector subtraction
Vector2 Vector2::operator-(const Vector2 &other) const
{
    return Vector2(x - other.x, y - other.y);
}

// Overload the * operator for scaling the vector by a scalar (double)
Vector2 Vector2::operator*(const double &scalar) const
{
    return Vector2(x * scalar, y * scalar);
}

// Overload the * operator for scaling the vector by a scalar (float)
Vector2 Vector2::operator*(const float &scalar) const
{
    return Vector2(x * scalar, y * scalar);
}

// Overload the * operator for scaling the vector by a scalar (int)
Vector2 Vector2::operator*(const int &scalar) const
{
    return Vector2(x * scalar, y * scalar);
}

// Overload the assignment operator
Vector2 &Vector2::operator=(const Vector2 &other)
{
    if (this != &other)
    {
        x = other.x;
        y = other.y;
    }
    return *this;
}

// due to floating-point precision two floats might not be exactly the same
bool Vector2::operator==(const Vector2 &other) const
{
    return almostEqual(x, other.x) && almostEqual(y, other.y);
}
#pragma endregion

#pragma region Vector functions
// !! currently not working !! returns a rotated vector (does not modify the origional vector) !! currently not working
Vector2 Vector2::rotate(const float &angle, const char &axis)
{
    Vector2 output;

    return output;
}

// returns a normalized vector (Read Only)
Vector2 Vector2::normalized() const
{
    float length = sqrt(x * x + y * y);

    Vector2 output(x, y);
    if (length != 0.0)
    {

        output.x /= length;
        output.y /= length;
    }

    return output;
}

// returns the length of the vector (Read Only)
float Vector2::magnitude() const
{
    return sqrt(x * x + y * y);
}

// returns the inverse of the vector (Read Only)
Vector2 Vector2::inverse() const
{
    Vector2 output(x * -1, y * -1);

    return output;
}

// Linearly interpolates between two points.
Vector2 Vector2::Lerp(const Vector2 &start, const Vector2 &end, const float &t)
{
    Vector2 result;
    result.x = (1 - t) * start.x + t * end.x;
    result.y = (1 - t) * start.y + t * end.y;
    return result;
}

// normalizes the input vector
Vector2 Vector2::Normalize(Vector2 &vector)
{
    float length = sqrt(vector.x * vector.x + vector.y * vector.y);

    if (length != 0.0)
    {
        vector.x /= length;
        vector.y /= length;
    }

    return vector;
}

float Vector2::Dot(const Vector2 &a, const Vector2 &b)
{
    return a.x * b.x + a.y * b.y;
}

// returns tha ansgle betweem two vectors
float Vector2::Angle(const Vector2 &from, const Vector2 &to)
{
    // Calculate the angle difference in radians
    float angleDiff = atan2(from.y, from.x) - atan2(to.y, to.x);

    // converts randian to degree
    return angleDiff * RAD_TO_DEG;
}

// clamps the length of a vector but keeps the direction
Vector2 Vector2::ClampMagnitude(Vector2 &vector, const float &magnitude)
{
    if (vector.magnitude() > magnitude)
        return Vector2::Normalize(vector) * magnitude;

    return vector;
}

#pragma endregion

#pragma endregion

// due to floating-point precision two floats might not be exactly the same
bool almostEqual(const float &a, const float &b, const float &epsilon)
{
    return fabs(a - b) < epsilon;
}
