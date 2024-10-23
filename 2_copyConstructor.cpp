/***
 * 복사 대입 연산자와 nothrow Swap
 * ===============================================================
 * 멤버 변수가 1개라면, 암시적 복사 대입 연산자가 정상 동작하도록 멤버 변수
 *  정의시 스마트 포인터 (shared_ptr등)를 사용하고, 필요없다면 못쓰게 만들어라
 * 멤버 변수가 2개 이상이라면, 복사 대입 연사자를 예외에 안전하도록 swap()으로
 *  구현하고, 필요없다면 못쓰게 만들어라.
 * 
 * Modern C++:
 *  C++11~: 이동 연산을 위해 이동 생성자와 이동 대입 연산자가 추가되어 임시
 *  개체 대입시 속도가 향상되었습니다
 *  C++11~: default, delete가 추가되어 암시적으로 생성되는 멤버 함수의 사용
 *  여부를 좀더 명시적으로 정의할수 있습니다.
 */

/*  복사 대입 연산자    */
// 복사 대입 연산자는 개체의 내용을 복제하는 역할을 합니다.
//      T& operator =(const T& other) 복사 대입 연산자
/*  암시적 복사 대입 연산자 */
// 복사 생성자와 마찬가지로, 복사 대입 연산자를 정의하지 않으면, 컴파일러가 임시적으로
//  복사 대입 연산자를 정의해 줍니다. 기본 동작은 복사 생성자와 유사하게 멤버별 복사 대입입니다.
class T {
    int m_X;
    int m_Y;
public:
    T(int x, int y) :
        m_X(x),
        m_Y(y) {}
    // 암시적 복사 생성자의 기본 동작은 멤버별 복사 생성자 호출입니다.
    // T(const T& other):
    //   m_X(other.m_X),
    //   m_Y(other.m_Y) {} 

    // 암시적 복사 대입 연산자의 기본 동작은 멤버별 복사 대입입니다.
    // T& operator =(const T& other) {
    //   m_X = other.m_X;
    //   m_Y = other.m_Y;
    // }

    int GetX() const { return m_X; }
    int GetY() const { return m_Y; }
};
T t1(10, 20);
T t2(1, 2);
t2 = t1;    // (0) 암시적 복사 대입 연산자 호출

EXPECT_TRUE(t2.GetX() == 10 && t2.GetY() == 20);

/*  swap을 이용한 예외 보증 복사 대입 연산자    */
// 예외가 방생하면, 스택 풀기에 언급된 것처럼 예외가 발생하기 전의 
//  상태를 복원해야 합니다.(이를 예외 보증이라 합니다.)
// 암시적 복사 대입 연산자는 각 멤버 변수별로 복사 대입을 하는데요, 
//   그러다 보니 중간에 예외가 발생했을 경우, 이전에 이미 수정한 개체를 
//   복원할수 없어 예외 보증이 안됩니다.   
T& operator =(const T& other) {
    m_X = other.m_X;
    m_Y = other.m_Y;    // 여기서 예외가 발생했다면 m_X를 되돌려야 합니다.
}
// 이러한 문제를 해결하기 위해,
//  1. 임시 개체를 만든 뒤,
//  2. swap() 을 이용해 this 와 임시 개체를 바꿔치기하여,
// 예외를 보증하는 복사 대입 연산자를 구현할수 있습니다.
class T {
    int m_X;
    int m_Y;
public:
    T(int x, int y) : 
        m_X(x), 
        m_Y(y) {} 
    // 암시적 복사 생성자의 기본 동작은 멤버별 복사 생성자 호출입니다.    
    // T(const T& other) :
    //     m_X(other.m_X),
    //     m_Y(other.m_Y) {}
    
    T& operator =(const T& other) {
        // other를 복제한 임시 개체를 만듭니다.
        T temp(other);  // (0) 생성시 예외가 발생하더라도 this는 그대로 입니다.

        // this의 내용과 임시 개체의 내용을 바꿔치기 합니다.
        // this는 이제 other를 복제한 값을 가집니다.
        Swap(temp);

        return *this;
    }   // temp는 지역 변수여서 자동으로 소멸됩니다.

    // 멤버 변수들의 값을 바꿔치기 합니다.
    void Swap(T& other) {
        // (~) 비권장. int 형이라 복사 부하가 크지는 않습니다만,
        // 조금 큰 개체라면 복사 부하가 있고 예외를 발생할수 있습니다.
        std::swap(this->m_X, other.m_X);
        std::swap(this->m_Y, other.m_Y);
    }

    int GetX() const { return m_X; }
    int GetY() const { return m_Y; }
};
T t1(10, 20);
T t2(1, 2);
t2 = t1;    // (0) swap 버전 복사 대입 연산자 호출

EXPECT_TRUE(t2.GetX() == 10 && t2.GetY == 20);

/*  swap의 복사 부하    */
// swap을 이용한 예외 보증 복사 대입 연산자는 예외 보증이 되어 좋습니다만, 
//  심각한 복사 부하가 있습니다.
t1 = t2             // 복사 대입 1회
std::swap(t1, t2);  // 복사 생성 1회 복사 대입 2회
// 1. 멤버 변수별 복사 대입 방식을 사용하면, 복사 대입 연산이 1회 일어나지만,
// 2. swap() 을 이용하면, 복사 생성 1회와 복사 대입 연산 2회가 방생하는 걸 알수 있습니다.

// 보통 swap()은 다음과 같이 임시 개체를 만들고, 각각 값을 복사 대입하기 때문에 복사 대입 
//  연산에서 복사 부하가 있을수 밖에 없습니다. 또한 복사 대입 과정에서 또다른 예외가 발생할 수도있죠.
swap(T& left, T& right) {
    T temp(right);      // 복사 생성 1회, 멤버별 복사 대입 연산과 거의 동등한 부하
    right = left;       // 복사 대입 연산 1회 - swap에 따른 추가 복사 부하
    left = temp;        // 복사 대입 연산 1회 - swap에 따른 추가 복사 부하
}
// 따라서, nothrow swap 기법을 이용하여, swap()은 복사 부하도 없고, 예외도 발생시키지 않도록 최적화해야 합니다.

/*  nothrow swap - 포인터 멤버 변수를 이용한 swap 최적화    */
/*
1. Big은 임의의 큰 데이터를 처리하는 클래스로 가정합니다.
2. Big의 복사 생성자와 복사 대입 연산자에 메시지를 출력해서 복사 부하를 확인합니다.
3. T는 Big 을 포인터 멤버 변수로 관리합니다.
4. 포인터 멤버 변수의 소유권 분쟁이 없도록 T의 복사 생성자에서 Big을 복제하고 소멸자에서
    delete 합니다.
5. 복사 대입 연산자를 만들고 Swap() 으로 구현합니다.
6. Swap()은 포인터 멤버 변수들끼리 바꿔치기합니다. 실제 Big을 복사하는 것이 아니라 포인터만 복사하여
    복사 부하를 줄입니다.
*/
class Big {
    int m_Val;  // 1. 실제로는 복사 부하가 큰 데이터라고 생각해 주세요.
public:
    explicit Big(int val) :
        m_Val(val) {}
    Big(const Big& other) : 
        m_Val(other.m_Val) {
            std::cout << "Big::Big(const Big& other)" << std::endl; // #2
    }
    Big& operator =(const Big& other) {
        m_Val = other.m_Val;
        std::cout << "Big::operator =(const Big& other)" << std::endl;  // #2
        return *this;
    }
    int GetVal() const { return m_Val; }
    void SetVal(int val) { m_Val = val; }
};
class T {
    Big* m_Big; // #3 복사 부하가 큰 데이터는 포인더로 관리합니다.
public:
    explicit T(Big* big) : 
        m_Big(big) {}
    // NULL 포인터가 아니라면 복제합니다.
    T(const T& other) :
        m_Big(other.m_Big != NULL ? new Big(*other.m_Big) : NULL) { // #4
    }
    // 힙 개체를 메모리에서 제거 합니다.
    ~T() {
        delete m_Big;   // #4
    }

    T& operator =(const T& other) { // #5 
    
        // other를 복제한 임시 개체를 만듭니다.
        T temp(other);  // (0) 생성시 예외가 발생하더라도 this는 그대로 입니다.

        // this의 내용과 임시 개체의 내용을 바꿔치기 합니다.
        // this는 이제 other를 복제한 값을 가집니다.
        Swap(temp);

        return *this;

    }   // temp는 지역 변수여서 자동으로 소멸됩니다.

    // 멤버 변수들의 값을 바꿔치기 합니다.
    void Swap(T& other) { // #6
        // (0) 포인터 변수끼리의 복사/대입이라 복사 부하가 크지 않습니다.
        // 예외가 발생할 확률도 낮습니다.
        std::swap(this->m_Big, other.m_Big);
    }

    const Big* GetBig() const { return m_Big; }
};
T t1(new Big(10));
T t2(new Big(1));
t2 = t1;    // (0) swap 버전 복사 대입 연산자 호출

EXPECT_TRUE(t2.GetBig()->GetVal() == 10);

// 복사 대입 연산시 임시 개체 (temp) 를 생성하느라 복사 생성사 (T(const T& other))에서 
//  Big 개체 1개를 복사 생성한 것(멤버별 복사 대입에서와 거의 동등한 부하입니다.)
//  외에는 다른 복사 부하가 없습니다.

// 즉, 포인터 멤버 변수로 정의한 개체의 복사 대입 연산자를 swap() 으로 구현하면,
//  1. 예외에 안전하고,
//  2. 복사 부하는 멤버 변수별 복사 대입과 거의 동등합니다.

/*  복사 대입 연산자까지 지원하는 스마트 포인터     */
// 복사 대입 연산자 지원을 위해 복사 생성자만 지원하는 스마트 포인터에 swap을 
// 이용한 복사 대입 연산자 지원 기능을 추가하면,
// 1. 복사 생성시 스마트 포인터에서 복제를 해주고,
// 2. 소멸시 스마트 포인터에서 delete 해주고,
// 3. 복사 대입 연산시 스마트 포인터에서 swap() 해 주므로,
// 암시적 복사 생성자, 암시적 소명자, 암시적 복사 대입 연산자와 호완되어
//  별도로 구현할 필요가 없어집니다. 따라서 다음처럼 복사 생성자, 소멸자, 복사 대입
//  연산자 정의 없이 간소하게 클래스를 작성할수 있습니다.
class T {
    IntPtr m_Val;
public:
    explicit T(int* val) :
        m_Val(val) {}
    int GetVal() const { return *m_Val; }
};

// 복사 생성시 m_Ptr을 복제하고, 소멸시 delete 힙니다.
// 복사 대입 연산은 임시 개체 생성후 swap 합니다.
class IntPtr {
private:
    int* m_Ptr; // new로 생성된 개체이니다.
public:
    explicit IntPtr(int* ptr) :
        m_Ptr(ptr) {}
    
    // (0) NULL 포인터가 아니라면 복제합니다.
    IntPtr(const IntPtr& other) :
        m_Ptr(other.IsValid() ? new int(*other.m_Ptr) : NULL) {}
    
    // 힙 개체를 메모리에서 제거 합니다.
    ~IntPtr() { delete m_Ptr; }

    IntPtr& operator =(const IntPtr& other) {

        // other의 힙 개체를 복제한 임시 개체를 만듭니다.
        IntPtr temp(other); // (0) 생성시 예외가 방생하더라도 this는 그대로 입니다.

        // this의 내용과 임시 개체의 내용을 바꿔치기 합니다.
        // this는 이제 other의 힘 개체를 복제한 값을 가집니다.
        Swap(temp); // (0) 포인터 끼리의 값 변경이므로 복사 부하가 없고, 예외가 발생하지 않습니다.

        return *this;
        // temp는 지역 변수여서 자동으로 소멸됩니다.
        // 소멸되면서 this가 이전에 가졌던 힙 개체를 소멸합니다.
    }
    // 멤버 변수들의 값을 바꿔치기 합니다.
    void Swap(IntPtr& other) {
        std::swap(this->m_Ptr, other.m_Ptr);    // (0) 포인더 끼리의 값 번경이므로 복사 부하도 없고, 예외가 발생하지 않습니다.
    }

    // 포인터 연산자 호출시 m_Ptr에 접근할수 있게 합니다.
    const int* operator ->() const { return m_Ptr; }
    int* operator ->() { return m_Ptr; }

    const int& operator *() const { return *m_Ptr; }
    int& operator *() { return *m_Ptr;}

    // 유효한지 검사합니다.
    bool isValid() const {return m_Ptr != NULL ? true : false; }
};

class T {
    // (0) IntPtr로 복사 생성과 복사 대입시 포인터의 복제본을 만들고, 
    //      소멸시 IntPtr에서 delete 합니다.
    // (0) 암시적 복사 생성자에서 정상 동작하므로, 명시적으로 복사 생성자를
    //      구현할 필요가 없습니다.
    // (0) 포인터 멤버 변수가 1개 있고, 내부적으로 복사 대입 연산시 swap하므로
    //      복사 대입 연산자를 구현할 필요가 없습니다.
    IntPtr m_Val;
public:
    // val : new 로 생성된 것을 전달하세요.
    explicit T(int* val) :
        m_Val(val) {}
    int GetVal() const { return *m_Val; }
};

// (0) 힙 개체를 복제하여 소유권 분쟁 없이 각자의 힙 개체를 delete 합니다.
{
    T t1(new int(10));
    T t2(t1);   // 새로운 int형 개체를 만들고 10을 복제합니다.

    EXPECT_TRUE(t2.GetVal() == 10);
}
// (0) 복사 대입 연산 시에도 소유권 분쟁 없이 각자의 힙 개체를 delete 합니다.
{
    T t1(new int(10));
    T t2(new int(20));
    t2 = t1;    // (0) swap 버전 복사 대입 연산자 호출
    EXPECT_TRUE(t2.GetVal() == 10);
}

/*  멤버 변수가 2개 이상인 경우 스마트 포인터와 복사 대입 연산자와의 호환성 */
class T {
    // (0) IntPtr로 복사 생성과 복사 대입시 포인터의 복제본을 만들고, 소멸시 IntPtr에서 delete 합니다.
    // (0) 암시적 복사 생성자에서 정상 동작하므로, 명시적으로 복사 생성자를 구현할 필요가 없습니다.
    // (0) 포인터 멤버 변수가 2개 있어, 예외에 안전하지 않으므로 swap으로 복사 대입 연산자를 구현합니다.
    IntPtr m_Val1;
    IntPtr m_Val2;
public:
    // val1, val2 : new 로 생성된 것을 전달하세요.
    T(int* val1, int* val2) :
        m_Val1(val1),
        m_Val2(val2) {}
    T& operator =(const T& other) {
        T temp(other);  // (0) 생성시 예외가 발생하더라도 this는 그대로 입니다.
        Swap(temp);     // (0) 포인터 끼리의 값 변경이므로 복사 부하가 없고, 예외가 발생하지 않습니다.
        return *this;
    }
    void Swap(T& other) {
        m_Val1.Swap(other.m_Val1);  // 포인터 끼리의 값 변경이므로 복사 부하도 없고, 예외가 발생하지 않습니다.
        m_Val2.Swap(other.m_Val2);  
    }

    int GetVal1() const { return *m_Val1; }
    int GetVal2() const { return *m_Val2; }
};

// (0) 힙 개체를 복제하여 소유권 분쟁 없이 각자의 힙 개체를 delete 합니다.
{
    T t1(new int(10), new int(20));
    T t2(t1);   // 새로운 int 형 개체를 만들고 10, 20 를 복제합니다.

    EXPECT_TRUE(t2.GetVal1() == 10 && t2.GetVal2() == 20);
}
// (0) 복사 대입 연산 시에도 소유권 분쟁 없이 각자의 힙 개체를 delete 합니다.
{
    T t1(new int(10), new int(20));
    T t2(new int(1), new int (2));
    t2 = t1; // (O) swap 버전 복사 대입 연산자 호출
    EXPECT_TRUE(t2.GetVal1() == 10 && t2.GetVal2() == 20);
}
// 혹은 멤버 변수를 무조건 1개로 유지하는 방법도 있습니다. (Plmpl 이디엄 참조)

