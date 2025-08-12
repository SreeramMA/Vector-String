#include <assert.h>
#include <cstring>
#include <iostream>

class C { // Needed for testing
private:
  int value;

public:
  C(int v = 0) : value(v) {}
  C(const C &other) : value(other.value) {}
  C(C &&other) noexcept : value(other.value) { other.value = 0; }
  C &operator=(const C &other) {
    if (this != &other) {
      value = other.value;
    }
    return *this;
  }
  C &operator=(C &&other) noexcept {
    if (this != &other) {
      value = other.value;
      other.value = 0;
    }
    return *this;
  }
  ~C() {}
  int get_val() { return value; }
};

///----------- DO NOT MODIFY ABOVE THIS LINE -----------///
///------------------ PLEASE DON'T :D ------------------///

// Please declare functions inside class and define them outside

template <class T> class Vector {
private:
  T *start = nullptr;
  size_t alloc_len = 0; // Total length allocated
  size_t real_len = 0;  // Total length used


  // Declare member functions - a lot of them :p
public:
  // Constructor without parameters
  Vector() : start(nullptr), alloc_len(0), real_len(0) {}

  //Constructor with parameters
  Vector(size_t n, T& value) : alloc_len(n), real_len(n), start(new T[n]) {
    for (size_t i = 0; i < n; i++){
        start[i] = value;
    }
  }

  Vector(const char* str);

  // Copy constructor
  Vector(const Vector<T>& other) : alloc_len(other.alloc_len), real_len(other.real_len),
                                      start(new T[other.alloc_len]) {
    for (size_t i = 0; i < other.real_len; i++) {
      start[i] = other.start[i];
    }
  }

  // Move constructor
  Vector(Vector<T>&& other) noexcept : alloc_len(other.alloc_len), real_len(other.real_len),
                                        start(other.start){
    other.start = nullptr;
    other.alloc_len = 0;
    other.real_len = 0;
  }

  // Destructor
  ~Vector(){
    delete[] start;
  }

  // Resizing the vector if required
  void resize();

  //Length of the vector
  size_t length();

  //Push back a copy
  void push_back_copy(const T& value);

  //Push back by moving
  void push_back_move(T&& value);

  //pop back
  void pop_back();

  // Overloading assignment operator
  Vector<T>& operator=(const Vector<T>& other);
  Vector<T>& operator=(Vector<T>&& other) noexcept;

  //Equality operator
  bool operator==(const Vector<T>& other) const;

  //Indexing operator
  T& operator[](size_t index);

  Vector<T>& operator=(const char* str);


  Vector<T>& operator+=(const char c);

  Vector<T>& operator+=(const char* str);

  Vector<T>& operator+=(const Vector<T>& other);

  Vector<T> operator+(const Vector<T>& other);

  friend std::ostream& operator<<(std::ostream& os, const Vector<char>& str);

  friend std::istream& operator>>(std::istream& is, Vector<char>& str); 
};

typedef Vector<char> String;



// Use partial template specialization


// Define all member functions here
template <class T> void Vector<T>::resize(){
    if (alloc_len == 0){
        alloc_len = 1;
    }
    else{
        alloc_len *= 2;
    }
    T *new_start = new T[alloc_len];
    for (size_t i = 0;i < real_len; i++){
        new_start[i] = start[i];
    }
    delete[] start;
    start = new_start; 
}


template <class T> size_t Vector<T>::length(){
    return real_len;
}

template <class T> void Vector<T>::push_back_copy(const T& value){
    if (real_len == alloc_len) resize();
    start[real_len] = value;
    real_len++;
}

template <class T> void Vector<T>::push_back_move(T&& value){
    if (real_len == alloc_len) resize();
    start[real_len] = std::move(value);
    real_len++;

}

template <class T> void Vector<T>::pop_back(){
    if (real_len > 0){
        real_len--;
    }
}

template <class T> Vector<T>& Vector<T>::operator=(const Vector<T>& other){
    if (this != &other){
        delete[] start;
        alloc_len = other.alloc_len;
        real_len = other.real_len;
        start = new T[alloc_len];
        for (size_t i = 0; i < real_len; i++){
            start[i] = other.start[i];
        }
    }
    return *this; 
}

template <class T> Vector<T>& Vector<T>::operator=(Vector<T>&& other) noexcept{
    if (this != &other){
        delete[] start;
        alloc_len = other.alloc_len;
        real_len = other.real_len;
        start = other.start;
        other.start = nullptr;
        other.alloc_len = 0;
        other.real_len = 0;
    }
    return *this;
}

template <class T> bool Vector<T>::operator==(const Vector<T>& other) const{
    if (real_len != other.real_len) return false;
    for (size_t i = 0; i < real_len; i++){
        if (start[i] != other.start[i]) return false;
    }
    return true;
}

template <class T> T& Vector<T>::operator[](size_t index){
    return start[index];
}

template<> String::Vector(const char* str){
    alloc_len = strlen(str);
    real_len = alloc_len;
    start = new char[alloc_len];
    for (size_t i = 0; i < real_len; i++){
        start[i] = str[i];
    }
}

template<> String& String::operator=(const char* str){
    delete[] start;
    alloc_len = strlen(str);
    real_len = alloc_len;
    start = new char[alloc_len];
    for (size_t i = 0; i < real_len; i++){
        start[i] = str[i];
    }
    return *this;
}

template<> String& String::operator+=(const char c){
    if (real_len == alloc_len){
        resize();
    }
    start[real_len] = c;
    real_len++;
    return *this;
}

template<> String& String::operator+=(const char* str){
    size_t new_len = real_len + strlen(str);
    while (new_len > alloc_len){
        resize();
    }
    for (size_t i = 0; i < strlen(str); i++){
        start[real_len + i] = str[i];
    }
    real_len = new_len;
    return *this;
}

template<> String& String::operator+=(const String& other){
    if (this == &other){
        String temp(other);
        for (size_t i = 0; i < temp.real_len; i++){
                this->push_back_copy(temp.start[i]);
        }
        return *this;
    }
    for(size_t i = 0; i < other.real_len ; i++){
        this->push_back_copy(other.start[i]);
    }
    return *this;
}

template<> String String::operator+(const String& other){
    String result;
    result.alloc_len = real_len + other.real_len;
    result.real_len = result.alloc_len;
    result.start = new char[result.alloc_len];
    for (size_t i = 0; i < real_len; i++){
        result.start[i] = start[i];
    }
    for (size_t i = 0; i < other.real_len; i++){
        result.start[real_len + i] = other.start[i];
    }
    return result;
}

std::ostream& operator<<(std::ostream& os, const String& str) {
    for (size_t i = 0; i < str.real_len; i++){
        os << str.start[i];
    }
    return os;
}

std::istream& operator>>(std::istream& is, Vector<char>& str) {
    char buffer[1000];
    is >> buffer;
    str = String(buffer);
    return is;
}
///----------- DEFINE TO SWITCH ON THE QUERY -----------///

// For switching on testing of a query, just do the following
// #define QUERY_<number of query> . For example, #define QUERY_22
// Uncomment below to switch queries ON

// #define QUERY_1
// #define QUERY_2
// #define QUERY_3
// and so on....

#define QUERY_1
#define QUERY_2
#define QUERY_3
#define QUERY_4
#define QUERY_5
#define QUERY_6
#define QUERY_7
#define QUERY_8
#define QUERY_9
#define QUERY_10
#define QUERY_11
#define QUERY_12
#define QUERY_13
#define QUERY_14
#define QUERY_15
#define QUERY_16
#define QUERY_17
#define QUERY_18
#define QUERY_19
#define QUERY_20
#define QUERY_21
#define QUERY_22
#define QUERY_23
#define QUERY_24
#define QUERY_25
#define QUERY_26
#define QUERY_27
#define QUERY_28

///----------- DO NOT MODIFY BELOW THIS LINE -----------///
///------------------ PLEASE DON'T :D ------------------///

int main() {

  Vector<Vector<int>> vect; // Empty vector of vectors
  Vector<String> sr_vect;   // Empty vector of strings
  Vector<C> obj_vect;       // Empty vector of objects of class C

  int num_of_queries;
  std::cin >> num_of_queries;
  while (num_of_queries--) {
    char type[50];
    char query[50];
    std::cin >> type >> query;
    if (strcmp(type, "Vector") == 0) {
      if (strcmp(query, "output") == 0) {
        int x;
        std::cin >> x;
#ifdef QUERY_1
        for (int i = 0; i < vect[x].length(); i++) {
          std::cout << vect[x][i] << ' ';
        }
        std::cout << std::endl;
#endif
      } else if (strcmp(query, "create_empty") == 0) {
#ifdef QUERY_2
        Vector<int> v;
        vect.push_back_copy(v);
#endif
      } else if (strcmp(query, "create_value") == 0) {
        int n, k;
        std::cin >> n >> k;
#ifdef QUERY_3
        Vector<int> v(n, k);
        vect.push_back_copy(v);
#endif
      } else if (strcmp(query, "push") == 0) {
        int x, k;
        std::cin >> x >> k;
#ifdef QUERY_4
        vect[x].push_back_copy(k);
#endif
      } else if (strcmp(query, "pop") == 0) {
        int x;
        std::cin >> x;
#ifdef QUERY_5
        vect[x].pop_back();
#endif
      } else if (strcmp(query, "assign_copy") == 0) {
        int x, y;
        std::cin >> x >> y;
#ifdef QUERY_6
        vect[x] = vect[y];
#endif
      } else if (strcmp(query, "assign_move") == 0) {
        int x, y;
        std::cin >> x >> y;
#ifdef QUERY_7
        vect[x] = std::move(vect[y]);
#endif
      } else if (strcmp(query, "equal") == 0) {
        int x, y;
        std::cin >> x >> y;
#ifdef QUERY_8
        std::cout << (vect[x] == vect[y] ? "YES" : "NO") << std::endl;
#endif
      } else if (strcmp(query, "modify") == 0) {
        int x, i, k;
        std::cin >> x >> i >> k;
#ifdef QUERY_9
        vect[x][i] = k;
#endif
      } else if (strcmp(query, "element") == 0) {
        int x, i;
        std::cin >> x >> i;
#ifdef QUERY_10
        std::cout << vect[x][i] << std::endl;
#endif
      } else {
        std::cout << "Oh no! Should not reach here!!" << std::endl;
        std::cout
            << "Either you modified the code or your input queries are wrong"
            << std::endl;
        assert(0);
      }
    } else if (strcmp(type, "String") == 0) {
      if (strcmp(query, "output") == 0) {
        int x;
        std::cin >> x;
#ifdef QUERY_11
        std::cout << sr_vect[x] << std::endl;
#endif
      } else if (strcmp(query, "create_empty") == 0) {
#ifdef QUERY_12
        String s;
        sr_vect.push_back_copy(s);
#endif
      } else if (strcmp(query, "create_value") == 0) {
        int n;
        char c;
        std::cin >> n >> c;
#ifdef QUERY_13
        String s(n, c);
        sr_vect.push_back_copy(s);
#endif
      } else if (strcmp(query, "create_special") == 0) {
#ifdef QUERY_14
        String s("Hello World!");
        sr_vect.push_back_copy(s);
#endif
      } else if (strcmp(query, "push") == 0) {
        int x;
        char c;
        std::cin >> x >> c;
#ifdef QUERY_15
        sr_vect[x].push_back_copy(c);
#endif
      } else if (strcmp(query, "pop") == 0) {
        int x;
        std::cin >> x;
#ifdef QUERY_16
        sr_vect[x].pop_back();
#endif
      } else if (strcmp(query, "assign_copy") == 0) {
        int x, y;
        std::cin >> x >> y;
#ifdef QUERY_17
        sr_vect[x] = sr_vect[y];
#endif
      } else if (strcmp(query, "assign_move") == 0) {
        int x, y;
        std::cin >> x >> y;
#ifdef QUERY_18
        sr_vect[x] = std::move(sr_vect[y]);
#endif
      } else if (strcmp(query, "assign_special") == 0) {
        int x;
        std::cin >> x;
#ifdef QUERY_19
        sr_vect[x] = "Hello World!";
#endif
      } else if (strcmp(query, "equal") == 0) {
        int x, y;
        std::cin >> x >> y;
#ifdef QUERY_20
        std::cout << (sr_vect[x] == sr_vect[y] ? "YES" : "NO") << std::endl;
#endif
      } else if (strcmp(query, "modify") == 0) {
        int x, i;
        char c;
        std::cin >> x >> i >> c;
#ifdef QUERY_21
        sr_vect[x][i] = c;
#endif
      } else if (strcmp(query, "element") == 0) {
        int x, i;
        std::cin >> x >> i;
#ifdef QUERY_22
        std::cout << sr_vect[x][i] << std::endl;
#endif
      } else if (strcmp(query, "append_character") == 0) {
        int x;
        char c;
        std::cin >> x >> c;
#ifdef QUERY_23
        sr_vect[x] += c;
#endif
      } else if (strcmp(query, "append_string") == 0) {
        int x, y;
        std::cin >> x >> y;
#ifdef QUERY_24
        sr_vect[x] += sr_vect[y];
#endif
      } else if (strcmp(query, "append_special") == 0) {
        int x;
        std::cin >> x;
#ifdef QUERY_25
        sr_vect[x] += "Hello World!";
#endif
      } else if (strcmp(query, "concatenate") == 0) {
        int x, y, z;
        std::cin >> x >> y >> z;
#ifdef QUERY_26
        sr_vect[x] = sr_vect[y] + sr_vect[z];
#endif
      } else {
        std::cout << "Oh no! Should not reach here!!" << std::endl;
        std::cout
            << "Either you modified the code or your input queries are wrong"
            << std::endl;
        assert(0);
      }
    } else if (strcmp(type, "Object") == 0) {
      if (strcmp(query, "move") == 0) {
        int x;
        std::cin >> x;
        C obj(x);
#ifdef QUERY_27
        obj_vect.push_back_move(std::move(obj));
        std::cout << obj.get_val() << std::endl;
#endif
      } else if (strcmp(query, "copy") == 0) {
        int x;
        std::cin >> x;
        C obj(x);
#ifdef QUERY_28
        obj_vect.push_back_copy(obj);
        std::cout << obj.get_val() << std::endl;
#endif
      } else {
        std::cout << "Oh no! Should not reach here!!" << std::endl;
        std::cout
            << "Either you modified the code or your input queries are wrong"
            << std::endl;
        assert(0);
      }
    } else {
      std::cout << "Oh no! Should not reach here!!" << std::endl;
      std::cout
          << "Either you modified the code or your input queries are wrong"
          << std::endl;
      assert(0);
    }
  }
}