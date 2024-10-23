/***
 * 소멸자 destructor
 * =================================================================
 * 획득된 자원은 꼭 소멸시켜라.
 * 암시적 소멸자가 정상 작동하도록 멤버 변수 정의 스마트 포인터 
 *  (auto_ptr, unique_ptr, shared_ptr등)를 사용하라
 * 다형 소멸이 필요하면 부모 개체에 가상 소멸자를 사용하라. (가상 소멸자가 아니면 메로리 릭이 발생한다.)
 * public Non-Virtual 소멸자인 개체는 상속하지 마라.
 * is-a 관계에서는 public Virtual 소면자를 사용하라. (가상 소멸자가 아니면 메모리 릭이 발생한다.)
 * has-a 관계에서는 protected Non-Virtual 소멸자를 사용하라.
 * 생성자처럼 소멸자에서도 가상 함수를 호출하지 마라.
 * 소멸자에서 예외를 방출하지 마라. (필요하다면 Release() 함수를 구현하라.)
 * 
 * Modern C++:
 *  C++11~: default, delete가 추가되어 암시적으로 생성도는 멤버 함수의 사용 
 *      여부를 좀더 명시적으로 정의 할수있습니다.
 *  C++11~: final이 추가되어 강제적으로 상속을 제한할수 있습니다.
 *  C++11~: noexept가 추가되어 함수의 예외 방출 여부를 보증하며, 소멸자는 
 *      기본적으로 noexcept로 동작합니다.
 * 
 * new로 생성한 포인터 멤버 변수를 사용후 delete로 소멸시키지 않으면, 
 *      메모리에 그대로 남아 있어 (메모리릭), 메모리 부족으로 프로그램이 
 *      중단됩니다.
 * 따라서 획득된 자원은 꼭 소멸(RAII(Resource Acquisition Is Initialization)
 *      시커야 하는데요, 소멸자는 개체가 소멸될때 호출되므로, 
 *      보통 소멸자에서 이런한 자원을 소멸 시킵니다)
 */
class T {
    int* m_Ptr;
public:
    T() : m_Ptr(new int(10)) {} // new로 생성한 것은
    ~T() { delete m_Ptr; }      // (0) delete로 소멸
};

/*  소멸자 호출 시점    */
/*
소멸자는 하기 상황에서 자동으로 호출됩니다.
 1. 전역 변수, 정적 변수 (정적 전역 변수, 정적 멤버 변수, 
        함수내 정적 지역 변수)인 경우 프로그램 종료시
 2. 스택에 생성한 지역 변수인 경우 블록 유효 범위의 끝
 3. new로 생성한 힙 개체인 경우 delete 시 
 4. 임시 개체인 경우 표현식의 끝
 5. 예외 발생에 따른 스택 풀기시
*/

/*  개체 소멸 순서  */
/*
개체가 소멸되면, 소멸자 본문이 실행되고, 본문내의 지역 변수를 소멸시키고, 
    멤버 변수를 소멸시킵니다.
 그뒤 부모 클래스의 소멸자를 호출합니다. 모든 소멸자 호출이 끝나면, 
    메모리를 해제합니다.

    1. 소멸자 본문 실행
    2. 본문 내의 지역 변수 소멸 (선언의 역순)
    3. 멤버 변수 소멸(선언의 역순)
    4. 부모 클래스 소멸자 본문 호출
    5. 부모 클래스 소멸자 본문내의 지역 변수 소멸 (선언의 역순)
    6. 부모 클래스 멤버 변수 소멸 (선언의 역순)
    7. 메모리 해제
*/
class BaseMemberObj {
public:
    BaseMemberObj()  { std::cout << "1. BaseMemberObj::BaseMemberObj()"   << std::endl; }
    ~BaseMemberObj() { std::cout << "10. BaseMemberObj::~BaseMemberObj()" << std::endl; }
};
class BaseLocalObj {
public:
    ~BaseLocalObj() { std::cout << "9. BaseLocalObj::~BaseLocalObj()"}
};
class Base {
    BaseMemberObj m_BaseMemberObj;
public:
    Base() { std::cout << "2. Base::Base()" << std::endl; }
    ~Base() {
        BaseLocalObj baseLocalObj;
        std::cout << "8. Base::~Base()" << std::endl;
    }
};
class DerivedMemberObj
public:
    DerivedMemberObj()  { std::cout << "3. DerivedMemberObj::DerivedMemberObj()"  << std::endl; }
    ~DerivedMemberObj() { std::cout << "7. DerivedMemberObj::~DerivedMemberObj()" << std::endl; }
};
class DerivedLocalObj {
public:
    ~DerivedLocalObj() {std::cout << "6. DerivedLocalObj::~DerivedLocalObj()" << std::endl;}
};        
class Derived : public Base {
    DerivedMemberObj m_DerivedMemberObj;
public:
    Derived() {std::cout << "4. Derived::Derived()" << std::endl;}
    ~Derived() {
        DerivedLocalObj derivedLocalObj;
        std::cout << "5. Derived::~Derived()" << std::endl;
    }
};

Derived d;

/*
1. BaseMemberObj::BaseMemberObj()
2. Base::Base()
3. DerivedMemberObj::DerivedMemberObj()
4. Derived::Derived()
5. Derived::~Derived() // 소멸자 호출
6. DerivedLocalObj::~DerivedLocalObj() // 소멸자 지역 변수 소멸(선언의 역순)
7. DerivedMemberObj::~DerivedMemberObj() // 멤버 변수 소멸(선언의 역순)
8. Base::~Base() // 부모 클래스 소멸자 호출
9. BaseLocalObj::~BaseLocalObj()
10. BaseMemberObj::~BaseMemberObj()
*/

/* 다형 소멸 */
class Base {}
class Derived1 : public Base {};
class Derived2 : public Base {};

Base* ptr1 = new Derived1();
Base* ptr2 = new Derived2();
... // ptr1, ptr2를 사용하고,

delete ptr1;    // Derived1을 소멸시킵니다.
delete ptr2;    // Derived2을 소멸시킵니다.

/* 
Derived 에서 소멸자 호출하면 Base 까지 소멸시키지만 Base만 생성해서 소멸시키면
    Derived 소멸자가 호출되지 않아 Derived 에서 사용한 자원의 메모리 릭이 
    발생할수 있습니다.
이렇게 부모 개체 포인터로부터 다형 소멸을 해야 하는 경우에는 자식 개체의 소멸자가 
    호출되도록 꼭 virtual 소멸자로 작성하여야 합니다.
*/
class Base {
    public:
        virtual ~Base() { std::cout << "2. Base::~Base()" << std::endl; }   // (0) 다형 소멸을 지원함
};

class Derived : public Base {
public:
    ~Derived() { std::cout << "1. Derived::~Derived()" << std::endl; }
};

Derived* d = new Derived;
Base* b = d;
delete b;   // (0) 1, 2 호출됨. 다형 소멸 지원.

