/***
 * 생성자
 * ==========================================================
 * 기본 생성자가 필요하다면 명시적(explicit)으로 구현하고, 필요없다면 못쓰게 만들어라.
 * 값 생성자에서는 필요한 인자를 모두 나열하고 초기화하라.
 * 인자가 1개인 값 생성자(형변환 생성자)는 explicit로 암시적 형변환을 차단하라.
 * 암시적(implied) 복사 생성자가 정상 동작하도록 멤버 변수 정의시 스마트 포인터 (shared_ptr 등)을 사용하고,
 * 생성자에서 가상 함수를 호출하지 마라.
 * 상속 전용 기반 클래스는 protected 생성자로 만들어라.
 * 
 * Modern C++:
 *  C++11~: 이동 연산을 위해 이동 생성자와 이동 대입 연산자가 추가되어 임시 개체 대입시
 *      속도가 향상 되었습니다.
 *  C++11~: 중괄호 초기화를 제공하여 클래스인지, 배열인지, 구조체인지 구분없이 중괄호 ({})를
 *      이용하여 일관성 있게 초기화 할수 있으며, 초기화 파싱 오류도 해결했습니다.
 *  C++11~: default, delete가 추가되어 암시적으로 생성되는 멤버 함수의 사용 여부를 좀더 
 *      명시적으로 정의할수 있습니다.
 *  C++11~: final이 추가되어 강제적으로 상속을 제한할수 있습니다.
 *  C++11~: 생성자 위임이 추가되어 생성자의 초기화 리스트 코드가 좀더 간결해 졌습니다.
 * 
 * 생성자는 개체가 생성될떄 제일 먼저 호출되는 특수 멤버 함수입니다. 개체가 메모리에 할당된
 *  뒤에 호출되고, 초기값을 설정하는 역할을 합니다.
 * 좋은 생성자는 잘못 사용하기엔 어렵게, 바르게 사용하기엔 쉽게 구현해야 하는데요, 그러기 
 *  위해선 명시적 의존성 원칙에 따라 필요한 인자는 모두 나여라는게 좋습니다.
 */

/*  기본 생성자  */
class T {
    public:
        T() {

        }
};

T t;    // (0) 개체 정의 (인스턴스화)
T t();  // (x) T를 리턴하는 함수 t() 선언

// 암시적 기본 생성자
//      컴파일러는 다른 생성자가 정의되지 않으면, 임시적으로 기본 생성자를 정의해 줍니다.
class T1 {

};
T1 t1;  // (0) 암시적 기본 생성자 사용

class T2 {
    public:
        T2() {}     // 사용자 정의 기본 생성자가 있음
};
T2 t2;      // (0) 사용자가 정의한 기본 생성자 사용

class T3 {
    public:
        T3(int, int) {}     // 값 생성자가 있어 암시적 기본 생성자가 정의되지 않음
};
T3 t3;  // (x) 컴파일 오류. 기본 생성자 정의 안됨.

class T4 {
    public: 
        T4(const T4& other) { *this = other; }   // 복사 생성자가 있어 암시적 기본 생성자가 정의되지 않음
};
T4 t4;  // (x) 컴파일 오류. 기본 생성자 정의 안됨

class T {
    int m_Val;  // (~) 암시적으로 기본 생성자가 자동 제로 초기화되거나 안되거나 할수있습니다.
    public:
        int GetVal() const { return m_Val; }
};
T t1;   // (0) 컴파일러가 암시적으로 저으이한 기본 생성자

EXPECT_TRUE(t1.GetVal() == 0 || t1.GetVal() != 0);  // 0으로 자동 초기화 되거나 안될수 있습니다

T t2 = T();     // 자동 제로 초기화 됩니다.
EXPECT_TRUE(t2.GetVale() == 0);

// const 로 선언된 상수 멤버 변숭는 생성시 초기값이 전달되야 하기 떄문에, 암시적 기본 생성자로 초기화 
//  할수 없으며, 명시적으로 생성자를 구현해야 합니다.
class T1 {
    private:
        const int& m_Val;   // 멤버 변수에 참조자가 있어, 암시적으로 생성한 기본 생성자에서 초기화 할수 없음
;};
T1 t1;      // (x) 컴파일 오류. 기본 생성자에서 멤버 변수 초기화 안됨

class T2 {
    const int m_Val;        // 멤버 변수에 상수형 개체가 있어, 암시적으로 생성한 생성자에서 초기화 할수 없음
};
T2 t2;

/*      포인터 멤버 변수의 소유권 분쟁      */
class T {
    int* m_Val;
public:
    // val : new 로 생성된 것을 전달하세요.
    explicit T(int* val) :
        m_Val(val) {}
    // 암시적 복사 생성자의 기본 동작은 멤버별 복사 생성자 호출입니다.
    // T(const T& other) :
    //      m_Val(other.m_Val) {}   // (~) 비권장. 동일한 힙 개체를 참조합니다

    // 힙 개체를 메모리에서 제거 합니다.
    ~T() { delete m_Val; }
};

// (x) 예외 발생. t1이 delete 한 것을 t2도 delete 합니다.
{
    T t1(new int(10));
    T t2(t1);   // 복사 생성의 결과 t1 과 t2가 동일한 힙 개체를 참조합니다.
}

// t1 과 t2 의 유효 범위가 끝나서 각자의 소멸자가 실행되면, 돌일한 힙 개체를 각자 delete 하다가
//      두번 delete 하게 되고, 결국 예외가 발생합니다.
// 이렇게 포인터 멤버 변수의 소유권을 서로 가지고 있고, 서로 소멸시키는 현상을 소유권 분쟁이라 합니다.
/*
    소유권 분쟁을 해결하는 방법은
    1. 소유권 이전을 하거나 (auto_ptr, unique_ptr),
    2. 깊은 복제를 하거나,
    3. 자원을 공유하거나 (shared_ptr),
    4. 유일한 자원으로 대체해서 사용함
*/

/*  깊은 복제   */
class T {
public:
    // val : new 로 생성된 것을 전달하세요.
    explicit T(int* val) :
        m_Val(val) {}

    // (0) NULL 포인터가 아니라면 복제합니다.
    T (const T& other) :
        m_Val(other.mVal != NULL) ? new int(*other.m_Val) : NULL) {}

    // 힙 개체를 메모리에서 제거 합니다.
    ~T() { delete m_Val; }
};

// (0) 힙 개체를 복제하여 소유권 분쟁이 없습니다.
{
    T t1(new int(10));
    T t2(t1);       // 새로운 int 형 개체를 만들고 10을 복제합니다.
}

/*
    복사 생성자만 지원하는 스마트 포인터
힙 개체의 복제본을 만들기 위해 클래스마다 일일이 명시적으로 복사 생성자를 개발하는 것보다는,
암시적 복사 생성자를 그대로 사용할수 있도록 스마트 포인터(shared_ptr 등)를 만들어 사용하는게 
코드도 간결하고 분석하기 좋습니다.
복사 대입 연산까지 지원하는 것은 복사 대입 연산자까지 지원하는 스마트 포인터를 참고하세요.
그리고, 모든 타입을 지원하는 일반화된 스마트 포인터의 구현 예는 auto_ptr을 참고하세요.

1. 스마트 포인터를 클래스 멤버 변수로 정의해 둡니다.
2. 암시적 복사 생성자가 호출되면, 내부적으로 멤버 변수들의 복사 생성자를 호출합니다.
        이때 스마트 포인터의 복사 생성자가 호출됩니다.
3. 스마트 포인터의 복사 생성자에서 포인터 복제를 합니다.
4. 개체의 소멸자 호출뒤 멤버 변수들이 소멸됩니다 (개체 소멸 순서 참고)
5. 멤버 변수 소멸시 스마트 포인터의 소멸자에서 포인터를 delete합니다.
*/

// 복사 생성시 m_Ptr을 복제하고, 소멸시 delete 합니다. (복사 대입 연산은 지원하지 않습니다.)
class IntPtr {
private:
    int* m_Ptr; // new로 생성된 개체입니다.
public:
    explicit IntPtr(int* ptr) :
        m_Ptr(ptr) {}
    
    // (0) NULL 포인터가 아니라면 복제합니다.
    IntPtr(const IntPtr& other) : // #2
        m_Ptr(other.IsValid() ? new int(*other.m_Ptr) : NULL) {}

    // 힙 개체를 메모리에서 제거 합니다.
    ~IntPtr() {
        delete m_Ptr;
    }

    // 포인터 연산자 호출시 m_Ptr에 접근할 수 있게 합니다.
    const int* operator ->() const { return m_Ptr; }
    int* operator ->() { return m_Ptr; }

    const int& operator *() const { return *m_Ptr; }
    int& operator *() { return *m_Ptr; }

    // 유효한지 검사합니다.
    bool IsValid() const { return m_Ptr != NULL ? true : false; }
};

class T {
    // (0) IntPtr로 복사 생성시 포인터의 복제본을 만들고, 
    //      소멸시 IntPtr에서 delete 합니다.
    // (0) 암시적 복사 생성자에서 정상 독작하므로, 명시적으로 복사 생성자를 
    //      구현할 필요가 없습니다.
    IntPtr m_Val;   
public:
    // val : new 로 생성된 것을 전달하세요.
    explicit T(int* val) : 
        m_Val(val) {}
    int GetVal() const { return *m_Val; }
};
// (0) 힙 개체를 복제하여 소유권 분쟁 없이 각자의 힙 개체를 delete 합니다
{
    T t1(new int(10));
    T t2(t1); // 새로운 int형 개체를 만들고 10을 복제합니다.

    EXPECT_TRUE(t2.GetVal() == 10);
} 
// (X) 예외 발생. 2번 delete 합니다. 아직 복사 대입 연산은 지원하지 않습니다.
{
    T t1(new int(10));
    T t2(new int(20));
    t2 = t1; // 아직 복사 대입 연산은 지원하지 않습니다.
}

/*      생성자에서 가상 합수 호출 금지      */
// 부모 클래스의 생성자에서 가상 함수를 호출하면 아직 자식 클래스들이  
//  완전히 생성되지 않은 상태이기에 부모 클래스의 가상함수가 호출됩니다.
//  의도치 않은 동작이므로, 생성자에서는 가상 함수를 호출하지 마세요.
class Base {
protected:
    int m_Val;
public:
    Base() :
        m_Val(0) {
            // (x) 오동작. 가상 함수를 생성자에서 호출합니다.
            // Derived:SetVal()이 호출되길 기대하지만,
            // Base::SetVal()이 호출됨니다
    }
    virtual void SetVal() {
        m_Val = 1;  // Base 에서는 1
    }
    int GetVal() const {return m_Val;}
};

class Derived : public Base {
public:
    Derived() :
        Base() {}   // Base의 기본 생성자를 호출하면서 가상 함수 SetVal()이 호출됩니다.
    virtual void SetVal() {
        m_Val = 2;  // Derived 에서는 2
    }
};

Derived d;

// (x) 오동작. Base 생성자에서 가상 함수인 SetVal()을 호출하면,
// Derived::SetVal() 이 호출되길 기대하나,
// 아직 Derived가 완전히 생성되지 않은 상태이기에,
// Base::SetVal() 이 호출됨
EXPECT_TRUE(d.GetVal() == 1);

/*  기본 생성자, 복사 생성자 사용 제한      */
// 만약 기본 생성자나 복사 생성자가 필요헚다면, 생성자를 사용할수 없게 만드는게 좋습니다.
// 의도하지 않았다면 동작하지 않게 해야 합니다.
// 사용을 제한하는 방법은..
//  * 기본 생성자: 다른 생성자 (값 생성자던, 복사 생성자던)가 정의되면, 암시적 기본 생성자가 
//      정의되지 않으므로 사용이 제한됩니다. 그렇지 않은 경우 명시적으로 기본 생성자를 구현하고,
//      private 도는 protected 로 사용을 제한합니다.
//  * 복사 생성자: private로 정의하면 다른 곳에서는 사용하지 못하고(Uncopyable 참고), protected로
//      정의하면 상속받은 자식 개체에서만 사용할수 있습니다.      
class T {
public:
    T(int, int) {} // (0) 값 생성자를 저의하면 암시적 기본 생성자를 사용할수 없습니다.
private:
    T(const T& other) {}    // (0) private에서 외부에서 복사 생성자 사용할수 없습니다.
};

T t1;       // (x) compile error. 기본 생성자 없음
T t2(0, 0); // (0)
T t3(t1);   // (x) 컴파일 오류. 복사 생성자를 사용할수 없게 private로 하여 단단히 코딩 계약을 했습니다.

/*  상속 전용 기반 클래스   - protected 생성자  */
// 상속해서만 상용할수 있는 클래스는 protected 생성자로 만듭니다. 그러면 
// 개체 정의(인스턴스화)에서는 사용할수 없고, 상속해서만 사용할수 있습니다.
class Base {
protected:  // 개체 정의 (인스턴스화) 에서는 사용할수 없고 상속해서만 사용할수 있습니다.
    Base() {}
public:
    virtual void f() {}
};

class Derived : Base {
    virutal void f() {}
};

Base b;     // (x)
Derived d;  // (0) 상속하면 인스턴스화 가능.
//  C++11~: final이 추가되어 강제적으로 상속을 제한할수 있습니다.

/*  생성자 접근 차단 - private 생성자  */
// 외부에서 생성자 접근을 못하게 하려면 private 생성자로 만듭니다. 이때, 생성을 위한 
// Create() 계열 함수를 별도로 만들수 있습니다. 상속을 제한하거나, 다양한 생성 방식을 
// 개체에서 통제하고 싶을때 사용합니다.
class T {
private:
    T(int a, int b, int c) {}   // 외부에서는 접근 불가
public:
    static T CreateFromA(int a) { return T(a, 0, 0); }  // a값만 가지고 생성
    static T CreateFromB(int b) { return T(0, b, 0); }  // b값만 가지고 생성
    static T CreateFromC(int c) { return T(0, 0, c); }  // c값만 가지고 생성
};   // C++11~: 생성자 위임이 추가되어 생성자의 초기화 리스트 코드가 좀더 간결해 졌습니다.
     // 상기 예는 생성자 위임을 통해서도 구현할수 있습니다
// T t(10, 0, 0);           // (x)
// T* p = new T(10, 0, 0);  // (x)

class U : public T {};
// U u;     // (x) 상속해서 생성할수 없음
// U* p = new u;  // (x)

T t(T::CreateFromA(10));        // (0) T를 복사 생성.

// 혹은 생성자에서 모든 처리를 하기 힘든 개체를 생성할때도 유용합니다.
//  (시스템 종속성이 높다던지, 일단 생성후 환경 정돈을 해야 한다던지 등이요.)
class T {
private:
    T() {}  // 외부에서 접근 불가
public:
    static T* Create() {
        T* result = new T;  // 기본 생성자를 만들고,
        GlobalSetter.f();   // 생성후 산전에 해야할 전역 설정을 하고,
        T->Func(GlobalGetter.f()); // 전역 설정을 참조하여 Func()을 실행하고
        ... // 뭔가를 열심히 더라고 ...

        // 이제 T 개체 생성에 따른 주변 환경도 다 설정했으므로 리턴
        return result;
    }
}
