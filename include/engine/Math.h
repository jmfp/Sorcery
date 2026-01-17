#pragma once

class Math
{
    public:
        template<typename T, typename U>
        static auto Add(T* a, U* b)
        {
            return *a + *b;
        }

        template<typename T, typename U>
        static auto Subtract(T* a, U* b)
        {
            return *a - *b;
        }
        float static Pi()
        {
            return 3.14159265358979323846f;
        }
        float static DegToRad(float degrees)
        {
            return degrees * (Pi() / 180.0f);
        }
        float static RadToDeg(float radians)
        {
            return radians * (180.0f / Pi());
        }
        template<typename T>
        static auto Square(const T& value)
        {
            return value * value;
        }
        float static GravitationalForce(float mass1, float mass2, float distance, float G = 6.67430e-11f)
        {
            return G * ((mass1 * mass2) / Square(distance));
        }
};

struct Vector3
{
    float x, y, z;

    Vector3(float x, float y, float z){ this->x = x; this->y = y; this->z = z; }
    Vector3 operator+(const Vector3& other) const
    {
        return Vector3(x + other.x, y + other.y, z + other.z);
    }
    Vector3 operator-(const Vector3& other) const
    {
        return Vector3(x - other.x, y - other.y, z - other.z);
    }
    Vector3 dotProduct(const Vector3& other) const
    {
        return Vector3(x * other.x, y * other.y, z * other.z);
    }
};

struct Vector2
{
    float x, y;

    Vector2(float x, float y){ this->x = x; this->y = y; }

    Vector2 operator+(const Vector2& other) const
    {
        return Vector2(x + other.x, y + other.y);
    }

    Vector2 operator-(const Vector2& other) const
    {
        return Vector2(x - other.x, y - other.y);
    }
    Vector2 dotProduct(const Vector2& other) const
    {
        return Vector2(x * other.x, y * other.y);
    }
};
