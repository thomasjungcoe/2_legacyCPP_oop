/*
    Plmlp 이디엄
================================================
* PImpl 이디엄은 멤버 변수 접슨 부하, 메모리 공간 부하,
    과도한 힙 사용의 부하가 있으니, 상황에 맞게 도입하라.

    개요
PImpl (pointer to implementation, 구현에 대한 포인더)은 구현의 상세 정보를
    은닉하는 프로그래밍 기법으로서, 코드간 종속성이나, 컴파일 종속성을 최소화 해줍니다.

    1. 개체 내부의 멤버 변수를 중첩 클래스에 선언합니다.
    2. 개체 선언부에서 중첩 클래스를 포인터 멤버 변수로 선언합니다.
    3. 개체 정의부에서 중첩 클래스의 실제 선언 및 정의 합니다
    4. 선언부에서 중첩 클래스의 포인터형 변수의 크기만 알면 되기 때문에 전방 
        선언만 하면 됩니다.
*/
// 선언에서, 아마도 헤더 파일
class T {
    class Impl; // #4. 전방 선언
    Impl* m_Impl; // #2. 포인터 멤버 변수로 선언. 전방 선언만 해주면, 클래스의 포인터이므로 8byte크기라고 생각하고, 컴파일을 계속합니다.
};

// 정의에서, 아마도 cpp 파일
class T::Impl { // #3. 실제 선언 및 정의를 합니다.
public:   
    IntPtr m_Val1; // #1 
    IntPtr m_Val2; // #1
}; 

/*
다음 예제는 멤버 변수가 2개 이상인 경우 스마트 포인터와 복사 대입 연산자와의 
    호환성 의 클래스 T를 PImpl 방식으로 리팩토링 한 예입니다.
    (IntPtr은 복사 대입 연산자까지 지원하는 스마트 포인터의 내용을 참고하시기 바랍니다.)

#1 : m_Val1, m_Val2를 T::Impl로 이전하였습니다.
#2 : T에서 class Impl; 전방 선언을 하고, Impl* m_Impl;을 포인터 멤버 
    변수로 선언하였습니다.
#3 : m_Impl 복사 생성을 위해 T의 복사 생성자를 추가했습니다.
#4 : m_Impl 소멸을 위해 T의 소멸자를 추가했습니다.
#5 : m_Impl 복사 대입 연산을 위해 swap을 이용한 복사 대입 연산자를 
    추가했습니다. swap()은 포인터끼리의 바꿔치기이므로 복사 부하가 거의 없습니다.
#6 : T::Impl 정의에서 복사 대입 연산자는 사용하지 않기에 private로 막았습니다. 
    복사 생성자는 암시적 복사 생성자를 그대로 사용합니다
*/
// ----
// 선언에서
// ----
class T {
    class Impl; // #2
    Impl* m_Impl; // #2. T::Impl 타입의 클래스 포인터를 멤버 변수로 사용합니다. 선언 시점에 구체 구현은 은닉되어 있습니다. 
public:
    // val1, val2 : new 로 생성된 것을 전달하세요.
    T(int* val1, int* val2);
    // (△) 비권장 . m_Impl이 포인터 멤버 변수여서, 복사 생성자에서 복제, 소멸자에서 delete, swap을 이용한 복사 대입 연산자를 구현해야 합니다.
    T(const T& other);
    ~T();
    T& operator =(const T& other);
    void Swap(T& other);

    int GetVal1() const;
    int GetVal2() const;
};

// ----
// 정의에서
// ----
/ 복사 생성시 m_Ptr을 복제하고, 소멸시 delete 합니다.
// 복사 대입 연산은 임시 개체 생성 후 swap 합니다.
class IntPtr {
    // 복사 대입 연산자까지 지원하는 스마트 포인터 참고
};
class T::Impl {
public: // T 에서 멤버 변수를 자유롭게 쓰도록 public 입니다.
    // 스마트 포인터를 사용합니다. 암시적 복사 생성자에서 복제본을 만들고, 소멸자에서 잘 소멸합니다.
    IntPtr m_Val1; // #1
    IntPtr m_Val2; // #1
    Impl(int* val1, int* val2) : 
        m_Val1(val1),
        m_Val2(val2) {}
private:        
    // #6. 복사 대입 연산자는 사용하지 않으므로 private로 못쓰게 만듭니다.
    Impl& operator =(const Impl& other) {return *this;}  
};

T::T(int* val1, int* val2) :
    m_Impl(new T::Impl(val1, val2)) {}
    
T::T(const T& other) : 
    m_Impl(new T::Impl(*other.m_Impl)) {} // #3. T::Impl의 복사 생성자를 호출합니다.
T::~T() {delete m_Impl;} // #4. T::Impl을 소멸시킵니다.

// #5. Swap으로 복사 대입 연산자를 구현합니다.
T& T::operator =(const T& other) {
    T temp(other); 
    Swap(temp); 
    return *this;
} 

// #5. 
void T::Swap(T& other) {
    // PImpl 이디엄을 사용하면 포인터 끼리의 swap 이므로 복사 부하가 거의 없습니ㅏㄷ.
    std::swap(this->m_Impl, other.m_Impl);
}

// T:Impl의 멤버 변수를 이용합니다.
int T::GetVal1() const { return *(m_Impl->m_Val1); }
int T::GetVal2() const { return *(m_Impl->m_Val2); }

/*  스마트 포인터를 이용한 PImpl 이디엄 구현    */
/*
멤버 변수가 m_Impl 1개 이므로, 스마트 포인터로 만들려면, 복사 생성자, 복사 대입
    연산자, 소멸자를 별도로 작성하지 않아도 됩니다. (복사 대입 연산자까지 지뭔하는 스마트 폰인터 참조)

선언부에서는,
    1. Impl 을 전방 선언만 합니다.
    2. Impl 을 관리하는 스마트 포인터인 ImplPtr 을 선언합니다. 정의부에서 T의
        암시적 복사 생성자의 암시적 복사 대입 연산자와 암시적 소멸자와 호환되도록
        만들 예정입니다.
    3. 스마트 포인터인 ImplPtr 을 사용하므로 T 의 복사 생성자, 복사 대입 연산자,
        소멸자, Swap() 을 제거했습니다.
*/

// ----
// 선언에서
// ----

// ----    
// #3. T 선언 : 복사 생성자, 소멸자, swap을 이용한 복사 대입 연산자, Swap 불필요
// ----
class T {
    class Impl; // 전방 선언
    // ----    
    // #2. ImplPtr 선언
    // ----
    class ImplPtr {
    private:
        Impl* m_Ptr; // 포인터형 변수로 사용합니다.
    public: 
        explicit ImplPtr(Impl* ptr);
        ImplPtr(const ImplPtr& other);
        ~ImplPtr();

        ImplPtr& operator =(const ImplPtr& other);
        void Swap(ImplPtr& other);

        const Impl* operator ->() const;
        Impl* operator ->();
    
        const Imp& operator *() const;
        Impl& operator *();

        bool IsValid() const;
    }    ;

    // (O) 스마트 포인터를 사용하여, 복사 생성자, 소멸자를 구현할 필요가 없고, 
    // (O) 멤버 변수도 1개여서 Swap으로 복사 대입 연산자를 구현할 필요가 없습니다.
    ImplPtr m_Impl;
public:
    // val1, val2 : new 로 생성된 것을 전달하세요.
    T(int* val1, int* val2);

    int GetVal1() const;
    int GetVal2() const;
};

/*
정의부에서는,
    1. IntPtr 을 사용한 Impl 을 구현합니다.
    2. ImplPtr을 구현합니다. 이때, 복사 생성자, 복사 대입 연산자, 소멸자를
        구현하여 T의 암시적 복사 생성자, 암시적 복사 대입 연산자, 암시적 소멸자와
        호환되게 합니다.
*/
// ----
// 정의에서
// ----

// 복사 생성시 m_Ptr을 복제하고, 소멸시 delete 합니다.
// 복사 대입 연산은 임시 개체 생성 후 swap 합니다.
class IntPtr {
private:
    int* m_Ptr;
public:
    explicit IntPtr(int* ptr) : 
        m_Ptr(ptr) {}
    IntPtr(const IntPtr& other) :
        m_Ptr(other.IsValid() ? new int(*other.m_Ptr) : NULL) {}
    ~IntPtr() { delete m_Ptr; }

    IntPtr& operator =(const IntPtr& other) {
        IntPtr temp(other);
            Swap(temp);
        return *this;
    }
    void Swap(IntPtr& other) {
        std::swap(this->m_Ptr, other.m_Ptr);
    }

    const int* operator ->() const { return m_Ptr; }
    int* operator ->() { return m_Ptr; }

    const int& operator *() const {return *m_Ptr;}
    int& operator *() {return *m_Ptr;}

    bool IsValid() const { return m_Ptr != NULL ? true : false }
};

// ----
// Impl 정의
// ----
class T::Impl {
public: // T 에서 멤버 변수를 자유롭게 쓰도록 public 입니다.
    // 스마트 포인터를 사용합니다. 알시적 복사 생성자에서 복제본을 만들고,
    //  소멸자에서 잘 소멸합니다.
    IntPtr m_Val1;
    IntPtr m_Val2;
    Impl(int* val1, int* val2) : 
        m_Val1(val1),
        m_Val2(val2) {}
private:
    // 복사 대입 연산자는 사용하지 않으므로 private로 못쓰게 만듭니다.
    Impl& operator =(const Impl& other) { return *this; }
};

// ----
// ImplPtr 정의
// ----
T:ImplPtr::ImplPtr(T::Impl* ptr) : m_Ptr(ptr) {}
T::ImplPtr::ImplPtr(const T::ImplPtr& other) :
    m_Ptr(other.IsValid() ? new T::Impl(*other.m_Ptr) : NULL) {}    // Impl의 복사 생성자를 호출합니다.
T::ImplPtr::~ImplPtr() { delete m_Ptr; }    // Impl을 소멸시킵니다.
T::ImplPtr& T::Impltr::operator =(const T::ImplPtr& other) {
    ImplPtr temp(other);
    Swap(temp);
    return *this;
}

void T::ImpltPtr::Swap(T::ImplPtr& other) {
    std::swap(this->m_Ptr, other.m_Ptr);
}

const T::Impl* T::ImplPtr::operator ->() const { return m_Ptr; }
T:Impl*        T::ImplPtr::operator ->()       { return m_Ptr; }

bool T::ImplPtr::IsValid() const { return m_Ptr != NULL? true : false; }

// ----
// T 정의
// ----
T::T(int* val1, int* val2) :
    m_Impl(new T::Impl(val1, val2)) {}

// TImpl의 멤버 변수를 이용합니다.
int T::GetVal1() const { return *(m_Impl->m_Val1); }
int T::GetVal2() const { return *(m_Impl->m_Val2); }
// STL을 이용하면 좀더 간단하게 구현할수 있습니다. 자세한 내용은 "unique_ptr을 이용한 PImpl 구현"을 참고

/*  PImpl 이디엄 오버헤드   */
/*
PImpl 이디엄은
    1. 구현 코드를 은닉하여 고드간 종속성이나, 컴파일 종송성을 최소화 하고,
    2. swap을 이용한 복사 대입 연산자 구현 편의성을 제공합니다.
다음 오버헤드가 있으니, 상황에 맞게 도입하셔야 합니다.
    1. 멤버 변수 접근 오버헤드 : m_Impl 을 통해 간접적으로 접근합니다.
    2. 메모리 골간 오버헤드 : m_Impl 포인터 메모리 공간이 추가로 필요합니다.
    3. 힙 공간 오버해드 : m_Impl 과 멤버 변수들이 모두 힙 공간에만 배치됩니다.
*/