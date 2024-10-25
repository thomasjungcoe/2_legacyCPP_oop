/***
 * 멤버 변수, 초기화 리스트
 * =================================================================
 * 멤버 변수 초기화시, 생성후 대입하지 말고 초기화 리스트를 사용하라.
 *  (초기화 리스트의 순서는 멤버 변수 선언 순서에 맞춰라.)
 * 생성자에서 필요한 인자를 모두 나열하고 초기화하라.
 * 메모리 패딩을 고려하여 멤버 변수 선언 순서를 정하라.
 * 암시적 복사 생성자, 암시적 복사 대입 연산자, 암시적 소멸자가 정상 
 *  동작하도록 스마트 포인터 (auto_ptr, unique_ptr, shared_ptr 등)를
 *  멤버변수로 사용하라
 * 
 * Modern C++:
 *  C++11~: 생성자 위임이 추가되어 생성자의 초기화 리스트 코드가 좀더 간결해 졌습니다.
 *  C++11~: 멤버 선언부 초기화가 추가되어 비정적 멤버 변수의 초기화가 쉬워졌습니다.
 *  C++11~: alignas() 화 alignof()가 추가되어 메모리 정렬 방식을 표준화 됐습니다.
 *  C++11~: align()은 메모리 정렬된 포인터를 구합니다.
 *  C++17~: 인라인 변수가 추가되어 헤더 파일에 정의된 변수를 여러개의 cpp에서
 *      #include 하더라도 중복 정의 없이 사용할수 있습니다. 또한, 클래스 정적 
 *      멤버 변수를 선언부에서 초기화 할수있습니다.
 *  C++20~: 비트 필드 선언부 초기화가 추가되었습니다
 *  C++20~: [[no_unique_address]]가 추가되어 아무 멤버 변수가 없는 개체의 크기를 최적화합니다.
 */ 

/* 
멤버 변수는 개체의 데이터를 저장 및 관리합니다. 주로 private로 은닉하고,
    Getter 함수와 Setter 함수를 통해 외부에 노출합니다.

참조자 멤버 변수와 상수 멤버 변수는 초기화가 필수입니다. 반드시 값 생성자를 만든뒤 초기화
    리스트를 이용해서 초기화해야 합니다.
*/
class T {
    int& m_Val2;      // 참조자 멤버 변수. 참조자는 초기화 되어야 함
    const int m_Val4; // 상수 멤버 변수. 초기값이 세팅되어야 함
    ```

public:
    // 참조자 멤버 변수와 상수 멤버 변수는 초기화 필수
    T(int& val2, int val4) :
        m_Val2(val2),       // 참조자는 반드시 초기화 리스트에서 세팅되어야 함
        m_Val4(val4) {}     // 상수 멤버 변수는 초기값이 세팅되어야 함
};

// 상수 멤버 변수와 정적 상수 멤버 변수는 선언부에서 초기화 할수 있습니다.
const int m_Val4 = 0;   // 선언부 초기화 지원
static const int s_c_m_Val6 = 0;    // (0)

// 정적 멤버 변수는 선언과 정의를 분리해서 작성해야 합니다. (정적 멤버 변수 보다는 
    // 생성 시점을 명시적으로 알수 있는 함수내 정적 지역 변수 사용을 권장합니다)
class T {
public:
    static int s_m_Val5;    // 정적 멤버 변수. 별도 정의 필요
};
int T::s_m_Val5;            // 별도 정의 필요

// 전체적인 선언과 정의 방법은 다음과 같습니다.
class T {
    int m_Val1; // 멤버 변수
    int& m_Val2; // 참조자 멤버 변수. 참조자는 초기화 되어야 함
    int* m_Val3; // 포인터 멤버 변수
    const int m_Val4; // 상수 멤버 변수. 초기값이 세팅되어야 함
    // const int m_Val4 = 0; // (O) 선언부 초기화 지원
public:    
    static int s_m_Val5; // 정적 멤버 변수. 선언만 했기에 별도 정의 필요
    static const int s_c_m_Val6; // 정적 상수 멤버 변수
    // static const int s_c_m_Val6 = 0; // (O) 선언부 초기화 지원
public: 
    // 참조자 멤버 변수와 상수 멤버 변수는 초기화 필수
    T(int& val2, int val4) :
        m_Val2(val2), // 참조자는 반드시 초기화 리스트에서 세팅되어야 함
        m_Val4(val4) {} // 상수 멤버 변수는 초기값이 세팅되어야 함
};

int T::s_m_Val5; // 선언만 했기에 별도 정의 필요
const int T::s_c_m_Val6 = 0; // 선언만 했다면 정의 필요. 선언부 초기화를 했다면 별도 정의 불필요

/*
    멤버 변수명과 인자명이 같은 경우
생성자의 초기화 리스트에서는 멤버 변수명과 인자명이 같더라도 합께 사용할수 있습니다.
1. a(a) 로 사용한 경우, 멤버 변수 a의 복사 생성자에 인자 a를 전달합니다.
2. 함수 본문에서는 인자가 멤버 변수를 가리므로 this->멤버 변수명으로 사용해야 합니다.
m_ or xxxx_ 와같이 뒤에 _ 를 사용하는 방법도 있습니다. 앞에 _를 사용하진 마세요.
*/
class T {
public:
    int a;
    int b;
    int c;
    T (int a, int b, int c) :   // 멤버 변수명과 인자명이 같더라도 초기화 리스트에서 사용 가능합니다.
        a(a),
        b(b),
        c(c) {
            // 함수 본문에서 멤버 변수명과 인자명이 같으면, 멤버 변수는 this를 써서 접근합니다.
            this->a += 1;   // 멤버 변수 a를 수정합
            a += 2;         // 인자 a를 수정함
    }
;}
T t(10, 20, 30);
EXPECT_TRUE(t.a == 11 && t.b == 20 && t.c == 30);


/* 개체 크기와 메모리 정령 */
/* 
일반적으로 개체의 크기는 멤버 변수의 합입니다.
class T1 {
    int m_X;
    int m_Y;
};
EXPECT_TRUE(sizeof(T1) == sizeof(int) * 2); // 8

하지만 다음의 경우를 보면, char(1byte) + int (4byte) 여서 5byte 일것 닽지만, 사실은 8byte 입니다.
class T2 { // 멤버 변수중 가장 큰 int 에 맞춰 정렬
    char m_X; // 1byte. 3 byte 패딩
    int m_Y;
};
EXPECT_TRUE(sizeof(T2) == sizeof(int) * 2); // 8
이는 메모리에서 멤버 변수의 데이터를 좀더 빠른 속도로 읽기 위해, 멤버 변수 중 가장 크기가 큰 값으로
메모리 정렬을하기 때문입니다. 메모리 정렬은 1byte이거나 2의 배수 byte(2, 4, 6, 8...) 일수 있습니다.

CPU는 메모리(RAM)에 접근하여 처리할 데이터를 읽어오는데, 이 접근 횟수가 많을 수록 속도가 느려집니다.

CPU가 한번에 데이터를 가져올수 있는 크기가 4byte나 8byte로 정해저 있는데요,
4byte로 가져온다고 가정하고, 메모리를 정렬하지 않고 1byte 단위로 배치된 멤버 변수를
읽는다면, 상기 T2의 경우 int 값(m_Y)을 읽으려면 2번 접근해야 합니다.

메모리 정렬이 1byte 단위인 경우
0x0000 0000     char int int int
0x0000 0004     int
0x0000 0008

메모리 정렬이 4byte 단위인 경우
0x0000 0000     char [padding]
0x0000 0004     int int int int
0x0000 0008
이러한 이점 때문에 메로리 정렬을 수행하며, 메모리 정렬을 위해 추가된 byte를
    voeld(Padding) 이라 합니다.
*/
class T3 {      // 멤버 변수중 가장 큰 double 에 맞춰 정렬
    char   m_X; // 1byte. 7byte 패딩
    double m_Y;
};
EXPECT_TRUE(sizeof(T3) == sizeof(double) * 2);  // 16

struct T4 {
    char   m_X;
    int    m_Y;
    double m_Z;
};
EXPECT_TRUE(sizeof(T3) == sizeof(double) * 2);  // 16


/*  빈 클래스와 자식 개체의 크기    */
/*
멤버 변수가 없는 빈 클래스도 자료형이므로 최소 1byte의 크기를 가집니다.

class Empty {};     // 빈 클래스는 강제로 1byte 
EXPECT_TRUE(sizeof(Empty) == 1);
*/
class Empty() {};   // 빈 클래스는 강제로 1byte
EXPECT_TRUE(sizeof(Empty) == 1);
class Composite {
    int m_X;
    Empty m_Empty;  // 빈 클래스는 강제로 1byte
};
EXPECT_TRUE(sizeof(Composite) == sizeof(int) + sizeof(int));
// C++20~: [[no_unique_address]]가 추가되어 아무 멤버 변수가 없는 개체의 크기를 최적화합니다.

// 하지만, 이를 상속하고, 상속한 개체에서 멤버 변수가 구현되었다면, 강제로 
//  추가되 1byte는 빼고 크기가 설절됩니다.
class Empty {}; // 빈 클래스는 강제로 1byte
EXPECT_TRUE(sizeof(Empty) == 1);

class EmptyDerived : public Empty { // 빈 클래스를 상속받으면, 강제 1byte는 빼고 크기가 설정됨
    int m_X;
};
EXPECT_TRUE(sizeof(EmptyDerived) == sizeof(int));

/*  
빈 클래스라도 가상 함수가 있다면 가상 함수 테이블이 생성됩니다. 컴파일러에 
    따라 다를 수도 있으나 대부분 8byte입니다.
*/
class Base {    // 멤버 변수는 없지만 virtual이 있어 가상 함수 테이블이 생성됨
public:
    virtual ~Base() {}
};
EXPECT_TRUE(sizeof(Base) == 8);

class Derived : public Base {
    // 가상 함수 테이블 크기로 정렬됨
    char m_X;
};
EXPECT_TRUE(sizeof(Derived) == 8 + 8);
std::cout << sizeof(Derived) << std::endl;

//   메모리 할당에 따른 멤버 변수 선언 순서
// 멤버 변수를 선언할때에는 메모리 정렬을 고려하여 선언하는게 좋습니다.
//  특히, char 와 같이 4byte 이하인 멤버 변수들은 몰아서 선언하는게 좋습니다.

// 다음 코드는 패딩 잡업에 의해 빈공간이 생겨 16byte 크기이지만,
class T {
    char m_Char1; // 1byte, 3byte 패딩
    int m_Int1;   // 4byte
    char m_Char2; // 1byte, 3byte 패딩
    int m_Int2;   // 4byte
};
EXPECT_TRUE(sizeof(T) == 16);

// 다음 코드는 패딩이 최소화되어 12byte입니다.
class T {
    char m_Char1;       // 1 byte.
    char m_Char2;       // 1 byte, 2 byte 패딩.
    int m_Int1;         // 4 byte.
    int m_Int2;         // 4 byte.
};
EXPECT_TRUE(sizeof(T) == 12);

/*  포인터 멤버 변수    */
/*
포인터 멤버 변수는 복사 생성이나 복사 대입 연산시 복사 되면서 소유권 분쟁을 하게 됩니다.
    이럴떄 어떤 것을 delete 해야 할지 상당히 고민되죠.

다음 코드에서 T는 new로 생성한 포인터도 전달 받고, 지역 변수 val 의 주소도 전달 받습니다.
    T 가 알아서 이를 판단하기 어려워 호출하는 쪽에서 delete하는데요,
*/
class T {
    int* m_Ptr;
public:
    T(int* ptr) :
        m_Ptr(ptr) {}
};
int* ptr = new int;
int val = 10;

T t1(ptr);  // (~) 비권장. ptr는 new 한 것이기 때문에 m_Ptr은 delete 되어야 합니다.
delete ptr; // (~) 비권장. 그냥 밖에서 지워버렸습니다.

T t2(&val); // 요것은 delete하면 안됩니다 

// 이렇게 외부에서 일일이 포인터를 delete하면, 다음처럼 이미 delete한 t1 을 t2 에 대입하는
//  실수도 빈번해 집니다. 아무리 꼼꼼히 검토하더라도요.
T t1(ptr);
delete ptr; // delete 했습니다.

T t2(&val);
t2 = t1;    // (~) 비권장. 이미 지워버린 ptr을 가진 t1을 t2에 복사했습니다.
            //      이런 실수 많이 하게 됩니다.
