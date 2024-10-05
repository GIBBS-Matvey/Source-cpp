#include <iostream>
#include <vector>

using namespace std;


/// size_t ~ unsigned long long int

/// Процесс создания - конструкторы
/// Процесс копирования - кон-ор копирования + operator=(copy) + operator=(move)
/// Процесс удаления - деструктор

/*class A {};

class B {
public:
*//*B() = default;*//*
    B() {}
    B(const B& other) = default;
    B& operator=(const B& other) = default;
    ///
    ~B() = default;

};*/



/// m * n || n * k --> m * k


template <typename T>
class Vector {
private:
    T* array;
    size_t size;
    size_t capacity;

public:
    Vector() : array(nullptr), size(0), capacity(0) {}

    ///other -> [array][size]
    /// *this ->[array][size]
    /// Vector v;  --> def.ctor
    /// Vector v_2(v);  --> copy ctor
    /// Vector v_3(v_2); --> copy ctor

    Vector(const Vector& other) : size(other.size), capacity(other.capacity) {                    /// Глубокое копирование
        array = reinterpret_cast<T*>(new int8_t[other.capacity * sizeof(T)]);
        for (int i = 0; i < size; ++i) {
            new (array + i) T(other.array[i]);
        }
    }

    Vector(Vector&& other) : size(other.size), array(other.array), capacity(other.capacity) {
        other.array = nullptr;
        other.size = 0;
    }
    
    Vector& operator=(Vector&& other) {
        /// v()l_value = (v)r_value   /// никогда такого не будет
        for (size_t i = 0; i < size; ++i) {
            (array + i)->~T();
        }
        delete[] reinterpret_cast<int8_t*>(array);
        
        size = other.size;
        capacity = other.capacity;
        
        array = other.array;
        other.array = nullptr;
        return *this;
    }
    
    
    
    
    
    Vector& operator=(const Vector& other) {
        
        /// v = v;

        if (this == &other) {
            return *this;
        }

        for (size_t i = 0; i < size; ++i) {
            (array + i)->~T();
        }
        delete reinterpret_cast<int8_t*>(array);

        size = other.size;
        capacity = other.capacity;

        array = new int8_t[capacity * sizeof(T)];
        for (int i = 0; i < size; ++i) {
            new (array + i) T(other.array[i]);
        }

        return *this;
    }


    /// [1][2][3][4][][][]

    void reserve(size_t n) {
        if (capacity >= n) {
            return;
        }

        T* newArray = reinterpret_cast<T*>(new int8_t[n * sizeof(T)]);

        size_t i = 0;
        try {
            for (; i < size; ++i) {
                new(newArray + i) T(array[i]); /// placement new
            }
        } catch (...) {
            for (size_t j = 0; j < i; ++j) {
                (newArray + i)->~T();
            }
            delete[] reinterpret_cast<int8_t*>(array);
            throw;
        }

        for (size_t i = 0; i < size; ++i) {
            (array + i)->~T();
        }

        delete[] reinterpret_cast<int8_t*>(array);
        array = newArray;
        capacity = n;
    }

    /// malloc + T()
    /// [T()][T()][T()][T()][T()][T()]
    /// T a;
    /// G b;
    /// a = reinterpret_cast<T>(b)
    /// a.f();
    /// b.g();

    /// int a = 5.0; CE
    /// int a = reinterpret_cast<int>("dfigdfg")    EXEP(bad_cast) __save_safety_idiom

    /// static_cast<>()
    /// dynamic_cast<>()
    /// const_cast()

    void resize(size_t n, const T& value = T()) {
        if (n > capacity) {
            reserve(n);
        }
        for (size_t i = size; i < n; ++i) {
            new (array + i) T(value);
        }
        size = n;
    }

    void push_back(const T& value) {
        if (capacity == size) {
            reserve(2 * size);
        }
        new (array + size) T(value);
    }

    void pop_back() {
        --size;
    }

    ///[1][2][3][4][5][][255]
    /// int a = 5;
    /// resize(3)

    void shrink_to_fit() {

    }

    /// [1][2][3][4][][][][][]
    ///--size

    int get_capacity() {
        return capacity;
    }

    int get_size() {
        return size;
    }

    ~Vector() {
        for (size_t i = 0; i < size; ++i) {
            (array + i)->~T();
        }
        delete[] reinterpret_cast<int8_t*>(array);
    }
    
    /// vector.reserve(10);
    /// 
    
    /// [a][b][c][d][e][][][][]
};

void Square(int a, int b) {
    int s = a * b;
}

int Square_return(int a, int b) {
    return a * b;
}


/// exeption


class DivisionOnZero {
public:
    DivisionOnZero() {
        cout << "DivisionOnZero" << endl;
    }
};

class Integer {
private:
    int a;
public:
    explicit Integer(int a) : a(a) {}

    Integer operator/(const Integer& other) const;

    Integer& operator=(const Integer& other) = default;

    Integer& operator/=(const Integer& other);

    friend Integer Division(const Integer& first, const Integer& second);

    Integer Divide(const Integer& other) const;

    friend ostream& operator<<(ostream& os, const Integer& integer) {
        os << integer.a << endl;
        return os;
    }

    /// cout << c << d;
    /// endl VS '\n'
    /// [a][b][c][d][][][][][]
    /// cout << a << b << c << d << endl;
    /// cout <<
    /// printf("%d", a, b, c);

};


/// int b = 3;
/// int& a = b;     /// & является частью записи типа данных (int&)
/// &a;             /// & является унарным оператором взятия адреса объекта



/*template <typename T>
void PrintF(const T& value, ...) {
    T* tmp = &value;
    while (tmp) {
        printf(tmp);
        ++tmp;
    }
}*/


/// tmp[0xAFD...] + i
/// 0xAFD... + i * sizeof(T)


/// PrintF(a, b ,c);
/// PrintF(a);

/// c /= b
Integer& Integer::operator/=(const Integer& other) {
    *this = (*this / other);
    return *this;
}

Integer Integer::operator/(const Integer& other) const {
    if (other.a == 0) {
        throw DivisionOnZero();
    }
    return Integer(this->a / other.a);
}


Integer Division(const Integer& first, const Integer& second) {   /// friend - делает метод СВОБОДНОЙ функцией
    return Integer(first.a / second.a);
}


Integer Integer::Divide(const Integer& other) const {   /// friend - делает метод СВОБОДНОЙ функцией
    return Integer(a / other.a);
}



int main() {


    /*const Integer c_a(6);
    Integer a(5);
    Integer b(3);
    Integer c(6);
    cout << c << a << b;

    try {
        c = a / b;
    } catch (...) {
        cout << "Error has just caught";
    }

    c /= b;     /// c = c / b
    c / b;*/
    std::vector<int> v(2, 3);
    
    std::vector<int> v_2(std::vector<int>(10, 10));
    
    std::vector<int> v_3 = std::vector<int>(10, 10);
    
    
    v = std::vector<int>(10, 10);
    
    
    /*Vector<int> v_2(v);*/
    /*Vector<int> v_3(Vector<int>(v));*/


    /*Vector<int> v;
    *//*int* a;*//*

    int s = Square_return(2, 3);    ///rvalue и lvalue объекты*/

    /*int n = int(); /// 0

    int k;  ///будет лежать мусор

    int K = int();  /// будет лежать (0)*/



    /*int a;          /// []
    a = 5;*/

    /*int b = 5;*/



    /*int c;
    c = 5;*/

    /*const int a;    /// процесс выделения памяти /// не сработет // CE
    a = 0;          /// процесс инициализации

    int& ref;
    ref = a;*/

    /*Vector<int> v;
    v.reserve(16);
    for (int i = 0; i < 16; ++i) {
        int value;
        cin >> value;
        v.push_back(value);
    }*/
    
    


    /// [1]
    /// [1][2]
    /// [1][2][3][4]
    /// [1][2][3][4][5][][][] ---> O(n) bad
    /// size = 1024 = 2^10
    /// 1014 ---> O(1) super
    

    /// push_back() --> O(1)(amort)


    return 0;
}


