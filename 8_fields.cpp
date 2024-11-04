/*
    멤버 함수, 상수 멤버 함수, 가상 함수, Getter, Setter
    ==============================================================

    멤버 변수를 수정하지 않는다면, 상수 멤버 함수로 작성하라.
    정적 멤버 함수는 obj.f() 가 아닌 T::f() 와 같이 호출하라.
    자식 개체에서 부모 개체의 비 가상 함수를 재저의 하지 마라.
    가상 함수를 정의하면 가상 함수 테이블을 위한 추가 공간이 필요하니 
        꼭 필요한 경우만 사용하라.
    Getter 함수의 리턴값은 기본 자료형인 경우 값 복사로, 클래스 티입인 경우 
        참조자로 작성하라.
    Setter 함수의 인자는 기본 타입인 경우 값 복사로, 클래스 타입인 경우
        참조자로 작성하라.

    Modern C++:
        C++11~: override가 추가되어 가상 함수 오버라이딩의 코딩 규약이 좀더 단단해졌습니다.
        C++11~: final이 추가되어 가상 함수를 더이상 오버라이딩 못하게 할수있습니다.
        C++11~: 멤버 함수 참조 지정자가 추가되어 멤버 함수에 &, && 로 좌측값에서
            호출될때와 우측값에서 호출될 때를 구분하여 함수 오버로딩을 할수 있습니다.
*/

/*
    멤버 함수
클래스는 캡슐화를 위해 데이터(멤버 변수)와 이를 처리하는 함수(멤버 함수)로 응집합니다.
*/
class Date {
    int m_Year;
    int m_Month;
    int m_Day;
public: 
    Date(int year, int month, int day) :
        m_Year(year),
        m_Month(month), 
        m_Day(day) {}

    // Getter/Setter
    int GetYear() const {return m_Year;} // 상수 멤버 함수
    int GetMonth() const {return m_Month;}
    int GetDay() const {return m_Day;}

    void SetYear(int val) {m_Year = val;} // 멤버 함수
    void SetMonth(int val) {m_Month = val;}
    void SetDay(int val) {m_Day = val;}

    // 내부적으로 전체 개월수를 계산하기 위해,
    // 데이터와 처리하는 함수를 응집하였습니다. 
    int CalcTotalMonth() const { // 상수 멤버 함수
        return m_Year * 12 + m_Month; 
    }
};
Date date(20, 2, 10); // 20년 2월 10일
EXPECT_TRUE(date.CalcTotalMonth() == 20 * 12 + 2); 

/*  상수 멤버 함수  */
// 멤버 함수의 뒤에 const를 붙여 상수 멤버 함수를 만들수 있습니다. 상수 멤버
// 함수는 상수성 계약에 따라 다음을 준수합니다.
/*
1. 멤버 변수를 수정하지 않습니다.
class T {
    int m_Val;
public:
    void Func() const { m_Val = 10; } // (x) const 함수는 멤버 변수를 수정할수 없습니다.
}

2. 멤버 변수를 몰래 수정할수 있는 포인터나 참조자를 리턴하지 않습니다.
class T {
    int m_Val;
public:
    int* Func() const { return &m_Val } // (x) int* 리턴. const 함수는 const int*를 리턴해야 합니다.
}

3. 내부 구현시 상수 멤버 함수만을 호출합니다.
class T {
    int m_Val;
public:
    void Func() const { NonConstFunc(); } // (x) const 함수는 비상수 멤버 함수 호출할수 없습니다.
    void NonConstFunc() {}
}

4. 메모리를 수정하지 않기 때문에 예외에 예외가 발생하지 않습니다.
*/

/*  비상수 멤버 함수의 비 상수성 전파   */
// 상수 멤버 함수가 될수 있음에도 비 상수 멤버 함수로 작성한다면, 이를 사용하는
//  모든 함수나 개체들이 비 상수로 만들어져야 합니다. 비 상수성은 바이러스처럼 
//  전파되니 상수 멤버 함수가 될수있다면 꼭 상수 멤버 함수로 만드세요.
class T {
    int m_Val;
public:
    int GetVal() {return m_Val;} // (△) 비권장. 상수 멤버 함수인데 비 상수로 정의했습니다.
};

class U {
    T m_T;
public:
    // m_T.GetVal()은 비 상수 멤버 함수여서 상수 멤버 함수인 GetInnerVal()에서 호출할 수 없습니다.
    // 어쩔 수 없이 GetInnerVal()을 비 상수로 만들어야 컴파일 할 수 있습니다.
    int GetInnerVal() const {return m_T.GetVal();} // (X) 컴파일 오류. 
};

/*  정적 멤버 함수  */
// 멥버 함수에 static을 사용하여 개체에 속하지 않는 정적 멤버 함수를 정의할수 있습니다.
class T {
public:
    static int f() { return 10; }   
};

EXPECT_TRUE(T::f() == 10);  // (0) T의 정적 멤버 함수 호출
T obj;
EXPECT_TRUE(obj.f() == 10); // (~) T의 정적 멤버 함수 호출. 되기는 합니다만 일반 멤버 함수 호출과 구분이 안되어 가독성이 떨어집니다.

/*  가상 함수   */
/*  
virtual 을 붙이면 가상 함수가되며, 부모 개체의 포인터나 참조자로 자식 
    개체에서 재구현한 함수 (overriding 이라 합니다.)에 접근할수 있습니다.
    * Base 포인터로 일반 함수인 f() 를 호출하면, Base::f() 가 호출되고,
    * 자식 개체인 Derived 포인터로 호출하면, Derived::f()가 호출됩니다.
일관성이 없으므로 사용하지 말아야 합니다.

가상 함수인 경우 Base 포인터 이던 Derived 포인터 이던, 
    Derived::v()가 정상적으로 호출됩니다.
*/
class Base {
public: 
    int f() { return 10; }
    virtual int v() { return 10; }  // 가상 합수
};

class Derived : public Base {
public:
    int f() { return 20; }          // (~) 비권장. Base 의 동일한 이름의 비 가상 함수를 가림
    virtual int v() { return 20; }  // (0) Base의 가상 함수 재구현 (오버라이딩) 굳이 virtual을 붙일 필요는 없습니다.
};

Derived d;
Base* b = &d;

EXPECT_TRUE(b->f() == 10);  // (~) 버권장. Base 개체를 이용하면 Base::f()가 호출됨
EXPECT_TRUE(d.f() == 20);   // (~) 비권장. Derived 개체를 이용하면 Derived::f()가 호출됨
EXPECT_TRUE(static_cast<Base&>(d).f() == 10); // (~) 가려진 Base::f() 함수를 호출

EXPECT_TRUE(b->v() == 20); // (0) 가상 함수여서 Derived::v()가 호출됨
EXPECT_TRUE(d.v() == 20);  // (0) 가상 함수여서 Derived::v()가 호출됨

/*
자식 개체에서 부모 개체의 함수를 오버라이딩 하려면,
1. 부모 개체에서 virtual 함수로 선언해야 합니다.
2. 자식 개체에서 함수명/인자 타입/상수 멤버 함수의 const/동적 예외 사양이 동일해야 합니다.
만일 다르다면, 오버라이딩 되지 않습니다. 컴파일러가 오류를 감지하지 못하니 주의하시기 바랍니다.
    C++11~: override 가 추가되어 가상 함수 오버라이딩의 코딩 규약이 좀더 단단해졌습니다. 또한,
        final이 추가되어 가상 함수를 더이상 오버라이딩 못하게 할수있습니다.
*/

/*  순가상 함수     */
// 순가상 함수는 실제 구현없이 함수 규약만 정의할때 사용합니다. 순가상 함수가 있는 클래스는
// 인스턴스화 할수없으며, 반드시 상송해서 자식 개체에서 구현해야 합니다.
class IEatable {
public:
    virtual void Eat() = 0;     // 순가상 함수
};

class Dog : public IEatable {
public:
    virtual void Eat() {}   // 순가상 함수는 자식 개체에서 실제 구현을 해야 합니다.
};
IEatable eatable;   // (x) 컴파일 오류. 순가상 함수가 있기 때문에 인스턴스화 할수있습니다.
Dog dog;            // (0)

/*      Getter 함수     */
/* 
개체의 멤버 변수를 리턴하는 함수를 특별히 Getter 함수라고 합니다. 
  다음 규칙에 맞춰 리턴 타입을 작성하는게 좋습니다.
    1. int 등 기본 자료형의 경우는 복사 부하가 참조 부하보다 적기 때문에 값을 리턴하는게 
        좋습니다.
    2. 클래스등 복사 부하가 참조 부하보다 큰 개체는 참조자를 리턴하는게 좋습니다.
    3. 멤버 변수는 널 값이 되는 경우가 없기 때문에 포인터보다는 참조가로 리턴하는게 좋습니다.
    4. 멤버 변수를 수정하지 않는다면 상수 멤버 함수로 작성합니다.
    5. 값 타입을 리턴하는 경우는 어짜피 리턴값이 복제되므로, 리턴값에 const 를 굳이 붙일 
        필요가 없습니다.
*/
/*  Setter 함수     */
/*
개체의 멤버 변수를 성절하는 함수를 특별히 Setter 함수라고 합니다.
1. int 등 기본 자료형의 경우는 복사 부하가 참조 부하보다 적기 때문에 값을 전달하는게 좋습니다. 
2. 클래스 등 복사 부하가 참조 부하보다 큰 개체는 참조자를 전달하는게 좋습니다.
    함수 인자를 수정하는게 아니기 때문에 함수 인자는 상수여야 합니다.
3. 널검사가 최소화 될 수 있도록, 널 값이 되지 않는 경우라면 포인터보다는 참조자를 전달하는게 좋습니다.
4. 값 타입을 전달하는 경우는 어짜피 인자에 복제되므로, 굳이 인자에 const를 붙일 필요가 없습니다.
*/