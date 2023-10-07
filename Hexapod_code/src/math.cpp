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

// Overload the * operator for scaling the vector by a scalar (float)
Vector3 Vector3::operator*(const float &scalar) const
{
    return Vector3(x * scalar, y * scalar, z * scalar);
}

// Overload the / operator for scaling the vector by a scalar (float)
Vector3 Vector3::operator/(const float &scalar) const
{
    return Vector3(x / scalar, y / scalar, z / scalar);
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

// due to floating-point precision two floats might not be exactly the same
bool Vector3::operator!=(const Vector3 &other) const
{
    return !(almostEqual(x, other.x) && almostEqual(y, other.y) && almostEqual(z, other.z));
}
#pragma endregion

#pragma region Vector functions
// !! currently not working !! return a rotated vector (Read Only)
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

// return a normalized vector (Read Only)
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

// return the length of a vector (Read Only)
float Vector3::magnitude() const
{
    return sqrt(x * x + y * y + z * z);
}

// return the inverse of the vector (Read Only)
Vector3 Vector3::inverse() const
{
    Vector3 output(x * -1, y * -1, z * -1);

    return output;
}

// return the x & y components of a Vector3 as Vector2 (Read Only)
// Vector2 Vector3::toVector2() const
//{
//    Vector2 output(x, y);
//
//    return output;
//}

// return the vector but with z = 0 (Read Only)
Vector3 Vector3::xyPlane() const
{
    Vector3 output(x, y, 0);

    return output;
}

// Linearly interpolate between two points (Read Only)
Vector3 Vector3::Lerp(const Vector3 &start, const Vector3 &end, const float &t)
{
    Vector3 result;
    result.x = (1 - t) * start.x + t * end.x;
    result.y = (1 - t) * start.y + t * end.y;
    result.z = (1 - t) * start.z + t * end.z;
    return result;
}

// normalize the input vector
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

// return dot product of two vectors
float Vector3::Dot(const Vector3 &a, const Vector3 &b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

// !! currently only works with angles bellow 90° !! return the angle between two vectors (Read Only)
float Angle(const Vector3 &from, const Vector3 &to)
{
    // Calculate the cosine of the angle using the dot product and vector magnitudes
    float cosAngle = Vector3::Dot(from, to) / (from.magnitude() * to.magnitude());

    // Calculate the angle in radians adn convert to degree
    return acos(cosAngle) * RAD_TO_DEG;
}

// clamp the length of a vector but keeps the direction
Vector3 Vector3::ClampMagnitude(Vector3 &vector, const float &magnitude)
{
    if (vector.magnitude() > magnitude)
        return Vector3::Normalize(vector) * magnitude;

    return vector;
}

#pragma endregion

Vector3 Vector3::forward(1, 0, 0);
Vector3 Vector3::back(-1, 0, 0);
Vector3 Vector3::right(0, 1, 0);
Vector3 Vector3::left(0, -1, 0);
Vector3 Vector3::up(0, 0, 1);
Vector3 Vector3::down(0, 0, -1);
Vector3 Vector3::zero(0, 0, 0);

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

// Overload the * operator for scaling the vector by a scalar (float)
Vector2 Vector2::operator*(const float &scalar) const
{
    return Vector2(x * scalar, y * scalar);
}

// Overload the / operator for scaling the vector by a scalar (float)
Vector2 Vector2::operator/(const float &scalar) const
{
    return Vector2(x / scalar, y / scalar);
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

// due to floating-point precision two floats might not be exactly the same
bool Vector2::operator!=(const Vector2 &other) const
{
    return !(almostEqual(x, other.x) && almostEqual(y, other.y));
}
#pragma endregion

#pragma region Vector functions
// !! currently not working !! return a rotated vector (Read Only)
Vector2 Vector2::rotate(const float &angle)
{
    Vector2 output;
    float sinOfAngle = sin(angle * DEG_TO_RAD);
    float cosOfAngle = cos(angle * DEG_TO_RAD);
    output.x = x * cosOfAngle - y * sinOfAngle;
    output.y = x * sinOfAngle + y * cosOfAngle;

    return output;
}

// return a normalized vector (Read Only)
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

// return the length of the vector (Read Only)
float Vector2::magnitude() const
{
    return sqrt(x * x + y * y);
}

// return the inverse of the vector (Read Only)
Vector2 Vector2::inverse() const
{
    Vector2 output(x * -1, y * -1);

    return output;
}

// return the x & y components of a Vector2 as Vector3 with z = 0 (Read Only)
Vector3 Vector2::toVector3() const
{
    Vector3 output(x, y, 0);

    return output;
}

// Linearly interpolate between two points (Read Only)
Vector2 Vector2::Lerp(const Vector2 &start, const Vector2 &end, const float &t)
{
    Vector2 result;
    result.x = (1 - t) * start.x + t * end.x;
    result.y = (1 - t) * start.y + t * end.y;
    return result;
}

// normalize the input vector
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

// return dot product of two vectors
float Vector2::Dot(const Vector2 &a, const Vector2 &b)
{
    return a.x * b.x + a.y * b.y;
}

// return the angle between two vectors (Read Only)
float Vector2::Angle(const Vector2 &from, const Vector2 &to)
{
    // Calculate the angle difference in radians
    float angleDiff = atan2(from.y, from.x) - atan2(to.y, to.x);

    // converts randian to degree
    return angleDiff * RAD_TO_DEG;
}

// clamp the length of a vector but keeps the direction
Vector2 Vector2::ClampMagnitude(Vector2 &vector, const float &magnitude)
{
    if (vector.magnitude() > magnitude)
        return Vector2::Normalize(vector) * magnitude;

    return vector;
}

#pragma endregion

Vector2 Vector2::forward(1, 0);
Vector2 Vector2::back(-1, 0);
Vector2 Vector2::right(0, 1);
Vector2 Vector2::left(0, -1);
Vector2 Vector2::zero(0, 0);

#pragma endregion

// due to floating-point precision two floats might not be exactly the same
bool almostEqual(const float &a, const float &b, const float &epsilon)
{
    return fabs(a - b) < epsilon;
}

// Function to calculate the length of a path defined by a dynamic array of Vector3
float calculatePathLength(const std::vector<Vector3> &path)
{
    float length = 0.0;

    for (size_t i = 1; i < path.size(); ++i)
    {
        // Calculate the length of the vector between consecutive points
        Vector3 segment = path[i] - path[i - 1];
        length += segment.magnitude();
    }

    return length;
}

// Function to find the index of the longest path in an array of path lengths
size_t findLongestPath(const std::vector<float> &pathLengths)
{
    if (pathLengths.empty())
    {
        return 0; // Handle empty array gracefully
    }

    float maxLength = pathLengths[0];
    size_t longestId = 0;

    for (size_t i = 1; i < pathLengths.size(); ++i)
    {
        if (pathLengths[i] > maxLength)
        {
            maxLength = pathLengths[i];
            longestId = i;
        }
    }

    return longestId;
}

// Function to interpolate points along a path by a specified length
Vector3 interpolatePathByLength(const std::vector<Vector3> &path, float targetLength)
{
    Vector3 interpolatedPath = path[0];
    float totalLength = calculatePathLength(path);
    targetLength = constrain(targetLength, 0, totalLength);

    // Handle invalid input gracefully
    if (totalLength <= 0 || targetLength <= 0)
    {
        return interpolatedPath;
    }

    // can't go beond path limits
    if (targetLength >= totalLength)
    {
        for (size_t i = 1; i < path.size(); i++)
        {
            interpolatedPath = interpolatedPath + (path[i] - path[i - 1]);
        }

        return interpolatedPath;
    }

    // interpolate
    float currentLength = 0;

    // add the first point as a starting value
    interpolatedPath;

    for (size_t i = 1; i < path.size(); ++i)
    {
        Vector3 segment = path[i] - path[i - 1];
        float segmentLength = segment.magnitude();

        if (currentLength + segmentLength >= targetLength)
        {
            // Calculate the interpolation ratio
            float t = (targetLength - currentLength) / segmentLength;

            // Interpolate the point and add it to the path
            interpolatedPath = interpolatedPath + segment * t;

            // Exit the loop since we've reached the target length
            return interpolatedPath;
        }

        // add segment to final output
        interpolatedPath = interpolatedPath + segment;

        currentLength += segmentLength;
    }
    return interpolatedPath;
}

// returns a point on a circle based on a point inside the circle that is projected by a vector
Vector2 projectPointToCircle(const float &radius, const Vector2 &point, Vector2 direction)
{
    // no direction for projection -> no calculation
    if (direction.magnitude() == 0)
        return point;
    // start being (0, 0) or having the same direction as directionInput makes the calculation very simple
    if (point.magnitude() == 0 || point.normalized() == direction.normalized() || point.normalized() * -1 == direction.normalized())
        return direction.normalized() * radius;

    // if point is outside the circle reposition it to be inside
    if (direction.magnitude() > radius - 0.005)
        Vector2::ClampMagnitude(direction, radius - 0.005);

    float lengthC = point.magnitude();

    float angleBeta = 180 - Vector2::Angle(direction, point); // calculate angle beta on unequal triangle

    // calculate missing Angles
    float sinGamma = (lengthC * sin(angleBeta * DEG_TO_RAD)) / radius;

    float angleGamma = asin(sinGamma) * RAD_TO_DEG;
    float angleAlpha = 180 - angleBeta - angleGamma;

    float projectionLength = (radius * sin(angleAlpha * DEG_TO_RAD)) / sin(angleBeta * DEG_TO_RAD);

    Vector2 output = point + direction.normalized() * projectionLength;

    return output;
}

// return index of smalles value in the array
size_t findSmallestValue(const std::vector<float> &array)
{
    if (array.size() <= 0)
    {
        // Handle the case of an empty array or invalid length
        return 0;
    }

    int indexOfSmalles = 0; // Assume the first element is the smallest

    for (int i = 1; i < array.size(); i++)
    {
        if (array[i] < array[indexOfSmalles])
        {
            indexOfSmalles = i; // Update the smallest value
        }
    }

    return indexOfSmalles;
}