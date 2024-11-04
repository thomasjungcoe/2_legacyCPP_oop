/*
        상송
==============================================================================
부모 개체의 멤버 함수를 오버로딩 하지 마라. 오버로딩 함수 탐색 규칙에서 제외된다.
자식 개체를 부모 개체에 대입하지 마라. 아무런 오류 없이 복사 손실 된다.
구현 코드가 없는 단위 전략 인터페이스인 경우에만 다중 상속하라.
소멸자에서 가이드한 것같이,
    * 다형 소멸이 필요하면 부모 개체에 가상 소멸자를 사용하라. (가상 소멸자가 아니면 메모리 릭이 발생한다)
    * public Non-Virtual 소멸자인 개체는 상속하지 마라.
    * is-a 관계에서는 public Virtual 소멸자를 사용하라. (가상 소멸자가 아니면 메모리 릭이 발생한다.)
    * has-a 관계에서는 protected Non-Virtual 소멸자를 사용하라.
부모 개체의 기본 구현을 자식 개체에서 재정의해야 한다면, 유틸리티로 제공하라.
상속을 강제하고 싶은 경우, protected 생성자를 사용하거나 순가상 함수를 포함하라. 
    (is-a 관계에서 순가상 함수가 없는 경우, 순가상 소멸자를 사용한다.)
상속을 제한하고 싶은 경우, public Non-Virtual 소멸자 사용으로 규약을 정하고, 준수하라.
    (코팅 계약을 맺기엔 부담이 크다.)
상속 관계에서는 복사 생성자 대신 가상 복사 생성자를 사용하라.
부모 개체의 복사 대입 연산자는 오동작할 소지가 있으니 막아라.

Modern C++:
    C++11~: final이 추가되어 가상 함수를 더이상 오버라이딩 못하게 할수 있고, 
        강제적으로 상속을 제한할수 있습니다.
    C++11~: 생성자 상속이 추가되어 부모 개체의 생성자도 상속받아 사용할수 있어 자식 개체의 
        생성자 재정의 코드가 좀더 간결해 졌습니다.

    개요
상속이란, 기존의 개체를 재활용하여, 새로운 클래스를 만드는 프로그래밍 기법입니다.

상속을 이용하면, 접근 지정자에 따라 부모 개체에 정의된 멤버 변수, 멤버 함수들을 물려받아 
    (상속받아) 자식 개체에서 사용할수 있습니다.
 */

class Base {
private:
    int m_Private;
protected:
    int m_Protected;
public:
    int m_Public;
};

class Derived : public Base {
    void f() {
        m_Private;   // (x) private 접근 불가
        m_Protected; // (0)
        m_Public;    // (0)
    }
};
Derived d;
d.m_Private;         // (x) private 접근 불가
d.m_Protected;       // (x) 외부 노출 불가. 상속한 개체내에서는 가능
d.m_Public;

/*
부모 - 자식 관계는 다음처럼도 불립니다.

부모	        자식	        내용
----------------------------------------------------------------------------
부모	        자식	        일반적인 상속 표현, 부모의 부모인 조상(Ancestor) 표현에 용이
기반(Base)	    파생(Derived)	일반적인 상속 표현
추상(Abstract)	구체(Concrete)	인터페이스와 구체 구현한 상속 관계
슈퍼(Super)	    서브(Sub)	    일반적인 상속 표현, java언어 스타일
상위(Up)    	하위(Down)	    상속 트리 관점에서의 표현
*/

/*  public 상속, protected 상속, private 상속   */
// 상속할때 접근 지정자를 명시하여, 부모 개체의 멤버를 외부에 노출할때 어떻게 노출할지 가시성을 조정할수 있습니다.
class Base {
private:
    int m_Private;
protected:
    int m_Protected;
public:
    int m_Public;
};

// 외부 노출이 안되도록 Base protected->private, Base public->private
class PrivateDerived : private Base {
    void f() {
        m_Private;      // (x) private 접근 불가
        m_Protected;    // (0)
        m_Public;       // (0)
    }
};
PrivateDerived obj1;
obj1.m_Private;     // (x)
obj1.m_Protected;   // (0)
obj1.m_Public;      // (0)


// 외부 노출이 안되지만 송속은 되도록 Base Protected->protected, Base public->protected
class ProtectedDerived : protected Base {
    void f() {
        // m_Private; // (X) 컴파일 오류. private 접근 불가
        m_Protected; // (O)
        m_Public; // (O)
    }
};

class ProtectedDerived2 : public ProtectedDerived { 
    void g() {
        m_Private; // (X) 컴파일 오류. private 접근 불가
        m_Protected; // (O) 자식을 상속한 자손에서 사용할 수 있습니다.
        m_Public; // (O) 자식을 상속한 자손에서 사용할 수 있습니다.
    }
};

class ProtectedDerived2 : public ProtectedDerived { 
    void g() {
        m_Private; // (X) 컴파일 오류. private 접근 불가
        m_Protected; // (O) 자식을 상속한 자손에서 사용할 수 있습니다.
        m_Public; // (O) 자식을 상속한 자손에서 사용할 수 있습니다.
    }
};

ProtectedDerived obj2;
obj2.m_Private; // (X) 컴파일 오류. private 접근 불가
obj2.m_Protected; // (X) 컴파일 오류. protected 접근 불가
obj2.m_Public; 

// 외부 노출과 상속은 되도록 Base protected->protected, Base public->public
class PublicDerived : public Base { 
    void f() {
        // m_Private; // (X) 컴파일 오류. private 접근 불가
        m_Protected; // (O)
        m_Public; // (O)
    }
};

PublicDerived obj3;
obj3.m_Private; // (X) 컴파일 오류. private 접근 불가
obj3.m_Protected; // (X) 컴파일 오류. protected 접근 불가
obj3.m_Public; // (O) 외부에서 사용할 수 있습니다.

/*      부모 개체 항목 이름 출돌과 멤버 접근        */
/*  
만약 부모 개체의 멤버와 자식 개체의 멤버가 이름이 동일하다면, 자식 개체의 것이 부모 
    개체의 이름을 가립니다.
특별히 범위 확인 연산자(::)로 명시적으로 부모 개체의 멤버에 접근할수 있으나, 다형성을 해칠 수
있으니 사용하지 않는게 좋습니다. 가상 함수 오버라이딩 할게 아니라면 다른 이름으로 정의하세요.
*/
public Base {
public:
    int f() { return 10; }
}

class Derived : public Base {
public:
    int f() { return 20; }  // 비권장. 부모 개체의 이름을 가립니다.    
};

Derived d;
Base* b = &d;

EXPECT_TRUE(b->f() == 10);  // 비권장. Base    개체를 이용하면 Base::f()가 호출됨
EXPECT_TRUE(d.f() == 20);   // 비권장. Derived 개체를 이용하면 Derived::f()가 호출됨

EXPECT_TRUE(b->Base::f() == 10); // 부모 개체에 명시적으로 접근합니다.
EXPECT_TRUE(d.Base::f() == 10);  // 부모 개체에 명시적으로 접근합니다.


/* 부모 개체의 멤버 함수 오버로딩   */
/*
부모 개체의 멤버 함수를 오버로딩 (함수명이 동일하고 인자 타임이 다름) 하면 부모 개체의 것을 
    가려 함수 오버로딩의 후보군에 포함되지 않습니다. (오버로딩 함수 탐색 규칙 참고)

다음 예에서 Derived::f(int)는 Base::f() 와 인자가 달라 함수 오버로딩을 한것 같지만, 
    아닙니다. 가려버립니다.
따라서, d.f() 로 접근하면 컴파일 오류가 발생하고, b->Base::f()로 부모 개체에 
    명시적으로 접근해야 합니다.
*/
class Base {
public:
    int f() { return 10; }
};

class Derived : public Base {
public:
    int f(int) { return 20; }       // (~) 함수 오버로딩한게 아닙니다. 그냥 부모 개체의 이름을 가립니다.
};

Derived d;
Base* b = &d;

EXPECT_TRUE(b->f() == 10);  // (△) 비권장. Base 개체를 이용하면 Base::f()가 호출됨 
// EXPECT_TRUE(d.f() == 10); // (X) 컴파일 오류. 오버로딩 함수 탐색 규칙에서 제외됨
EXPECT_TRUE(d.Base::f() == 10); // (△) 비권장. 부모 개체에 명시적으로 접근해야 합니다.
EXPECT_TRUE(d.f(1) == 20);


/*  부모 개체의 가상 함수 오버라이팅    */
/*
부모 개체의 가상 함수를 오버라이딩 (함수명과 인자는 동일하며 자식 개체에서 재구현)하여 다형적으로
    동작하게 할수있습니다.
오버라이딩하려면, 함수명/인자 티입/상수 멤버 함수의 const/동적 예외 사양은 동일해야 하며,
    리턴값은 부모 개체의 것과 같거나 상속 관계 (공변, covariant)이아야 합니다. (가상 함수 참고)
다음 예를 보면 부모 개체로 접근 (b->f()) 하던 자식 개체로 접근 (d.f()) 하던 모두 동일하게 
    자식 개체의 f() 가 호출됩니다.
*/
class Base {
public:
    virtual int f() { reutnr 10; }
};

class Derived : public Base {
public:
    virtual int f() { return 20; }  // (0) 오버라이딩. 함수명/인자 타입/상수 멤버 함수의 const/동적 예외 사양은 동일해야 합니다.
};
Derived d;
Base* b = &d;

EXPECT_TRUE(b->f() == 20);          // (0) 자식 개체의 함수가 호출됩니다.
EXPECT_TRUE(b->Base::f() == 10);    // (~) 비권장. 부모 개체에 명시적으로 접근하면, 부모 개체의 함수가 호출됩니다.
EXPECT_TRUE(d.f() == 20);           

/*  자식 개체의 생성자 재정의   */
// 자식 개체에서 생성자를 재정의하면, 명시적으로 부모 클래스의 생성자를 호출해야 합니다.
class BAse {
    int m_X;
    int m_Y;
public:
    Base(int x, int y) :
        x_X(x),
        m_Y(y) {}
};
class Derived : public Base {
    int m_Z;
public:
    Derived(int x, int y, int z) : 
        Base(x, y),     // 부모 클래스 생성자 호출
        m_Z(z) {}
};

/*      복사 손실       */
class Base {
    int m_X;
    int m_Y;
public:
    Base(int x, int y) :
        m_X(x),
        m_Y(y) {}
};
class Derived : public Base {
    int m_Z;
public:
    Derived(int x, int y, int z) :
        Base(x, y),
        m_Z(z) {}
};

Derived d(1, 2, 3);
Base b = d; // (x) 오동작. 아무런 경고없이 실행됩니다.
d = b;      // (x) 오류. 부모 개체를 자식 개체에 대입할수 없습니다.

/*      다중 상속       */
/*
자식 개체는 다양한 부모를 상속할수 있습니다. 하지만, 구체 구현된 클래스를 다중 상속하면, 
    이름 출돌의 우려가 있기 때문에 잘 사용하지 않습니다. 단위 전략 인터페이스인 경우에만
    다중 상속을 사용하시기 바랍니다.
*/
class Singer {
public:
    int m_Age;      // 동일한 멤버 변수
};
class Dancer {
public:
    int m_Age;      // 동일한 멤버 변수
};
class Idol :
    public Singer,
    public Dancer {
        
};

Idol obj;
obj.m_Age = 10;         // (x) Singer::m_Age 와 Dancer::m_Age 가 모호합니다.
obj.Singer::m_Age = 20; // (~) 비권장. 이름이 동일한 멤버는 ::(범위 확인 연산자)를 이용해 접근 가능합니다.
obj.Dancer::m_Age = 30;

EXPECT_TRUE(obj.Singer::m_Age == 20);
EXPECT_TRUE(obj.Dancer::m_Age == 30);

/*  다이아몬드 상속과 virtual 상속  */
/*
다중 상속은 다이아몬드 상속이 만들어질수 있습니다. 상속한 모양이 다이아몬드 같다고 해서 
    다이아몬드 상속이라 합니다만, 이름 처럼 예쁜녀석은 아닙니다
*/
class Person {
public:
    int m_Age;
};
class Singer : public Person {};
class Dancer : public Person {};
class Idol : 
    public Singer,      // (~) Singer::Person::m_Age 존재
    public Dancer {     // (~) Dancer::Person::m_Age 존재

};

Idol obj;
obj.Singer::m_Age = 20; 
obj.Dancer::m_Age = 30;

EXPECT_TRUE(obj.Singer::m_Age == 20);
EXPECT_TRUE(obj.Dancer::m_Age == 30);   


/*  is-a 관계   */
/*
is-a 관계는 자식 개체를 부모 개체처럼 사용하려고 할때의 상속 관계입니다.
                Shape
                Draw()
/|              ||          |\
Rectangle       Ellipse     Triangle
Draw()          Draw()      Draw()

상기와 같은 상속 구조를 가지고 부모 개체인 Shape의 포인터로 Rectangle, Ellipse, Triangle을
    관리하고 소멸시키는 관계 입니다.

    1. 물려받은 기능은 주로 외부에 그대로 노출되므로, public 상속을 합니다.
    2. 부모 개체의 소멸자는 public Virtual 소멸자로 만듭니다.
        a.  외부에서 부모 개체 포인터로 소멸시키기 때문에 public 소멸자로 정의하고,
        b.  다형 소멸을 해야 하므로 가상 소멸자로 만듭니다. 만약 virtual 로 하지 않으면,
            자식 개체의 소멸자가 호출되지 않아 메모리 릭이 발생할수 있습니다.
*/
class Shape {
public:
    virtual ~Shape() {}     // 2. 다형 소멸을 하려면 public virtual 소멸자를 사용합니다.
    virtual void Draw() const {}
};

class Rectangle : public Shape {        // 1. public 상속합니다.
public:
    virtual void Draw() const {}
};
class Rectangle : public Shape {
public:
    virtual void Draw() const {}
};
class Ellipse : public Shape { // #1. public 상속합니다.
public:
    virtual void Draw() const {}
};
class Triangle : public Shape { // #1. public 상속합니다.
public:
    virtual void Draw() const {}
};

Shape* shapes[3] = {        // 도형들을 Shape* 로 관리합니다.
    new Rectangle(),
    new Ellipse(),
    new Triangle()
};

for(int i = 0; i < 3; ++i) {
    shapes[i]->Draw();      // 다형적으로 그립니다.
}

for(int i = 0; i < 3; ++i) {
    delete shape[i];        // 다형 소멸 합니다. Shape*로 Rectangle, Ellipse, Triangle을 소멸합니다.
}

/*      has-a 관계      */
/*
has-a 관계는 다형 소멸은 하지 않고, 자식 개체가 부모 개체의 멤버 변수, 멤버 함수등을 물려받아       
    포함하고 있는 상송 관계입니다. 주로 공통 코드를 구현하여 항속 하거나 단위 전략 
    인터페이스를  상속 할때 사용합니다.

                    ResizableImpl
                    -m_Width
                    -m_Height
                    #~ResizeableImpl()
                    + GetWidth()
                    + GetHeight()
                    + SetWidth()/Height()
             /                               \
    Rectangle                                Ellipse
    -m_Left                                  -m_CenterX
    -m_Top                                   -m_CenterY

물려받은 기능을
    1. 외부에 노출할 필요가 없다면 private 상속,
    2. 다시 자손들이 상속할수 있게 하고 싶다면 protected 상속,
    3. 외부에서 사용하게 하고 싶다면, public 상속 
을 합니다.

부모 개체의 소멸자는 protected Non-Virtual 소멸자로 만듭니다.
    1. 부모 개체를 상속 받아서만 사용하도록 함과 동시에 부모 개체 포인터로부터 
        다형 소멸을 시도하지 못하도록 protected 소멸자로 정의하고,
    2. 다형 소멸을 안하므로 Non-Virtual 소멸자로 만듭니다.

    공통 코드 구현
여러 개체의 공통 구현을 부모 개체로 만들어 has-a 관계를 맺을때 사용할수 있습니다.

다음 코드는 Rectangle 과 Ellipse 에서 공통으로 사용하는 너비/높이 처리를 
    ResizeableImpl에 공통 구현하고, Retangle과 Ellipse 에서 has-a 관계로 상속한 예입니다.
    1. 다형 소멸을 안하므로 protected Non-Virtual 소멸자로 만듭니다.
    2. 물려받은 기능은 외부에 그대로 노출하기위해, public 상속을 합니다.
*/

// 너비/높이에 대한 공통 구현
class ResizeableImpl {
private:
    int m_Width;
    int m_Height;
protected:
    ResizeableIMpl(int w, int h) :
        m_Width(w),
        m_Height(h) {}
    ~ResizeableImpl() {}     // 1. has-a 관계로 사용되기 때문에, 단독으로 생성되지 않도록 protected 입니다.
public:
    // Get/Set 함수
    int GetWidth()  const { return m_Width; }
    int GetHeight() const { return m_Height; }

    void SetWidth(int val) { m_Width = val; }
    void SetHeight(int val) { m_Height = val; } 
};

class Rectangle : public ResizeableImpl {
    int m_Left;
    int m_Top;
public:
    Rectangle(int l, int t, int w, int h) :
        ResizableImpl(w, h),
        m_Left(l),
        m_Top(t) {}
};

class Ellipse : public ResizeableImpl {
    int m_CenterX;
    int m_CenterY;
public:
    Ellipse(int centerX, int centerY, int w, int h) :
        ResizeableImpl(w, h),
        m_CenterX(centerX),
        m_CenterY(centerY) {}
};
Rectangle r(0, 0, 10, 20);
Ellipse e(5, 10, 10, 20);

/*      단위 전략 인터페이스        */
/*
단위 전략 인터페이스는 개체의 기능 스펙이라 할수있습니다. 작은 단위의 기능 스펙을 
    순가상 함수로 제공하며, 다형 소멸을 지원하지 않습니다.

단위 전략 인터페이스도 has-a 관계의 일종으로 protected Non-Virtual 소멸자를 사용합니다.

1. 다형 소멸을 안하므로 protected Non-Virtual 소멸자로 만듭니다.
    * protected 소멸자 여서 부모 개체를 상속해서만 사용할수 있습니다.
    * Non-Virtual 소멸자여서 다형 소멸을 할수 없습니다.
2. 물려받은 기능은 외부에 그대로 노출하기위해, public 상속을 합니다.
3. 순가상 함수제공합니다.
    순가상 함수 여서 부모 개체를 상속해서만 사용할수 있습니다.
*/

class IEatable {
protected:
    ~IEatable() {}  // 1. 인터페이스여서 protected non-virtual(상속해서 사용하고, 다형 소멸 안함) 입니다.

public:
    virtual void Eat() = 0;     // 3. 기능 스펙을 순가상 함수로 제공합니다. 상속해서만 사용할수 있습니다.
};

class IWalkable {
protected:
    ~Iwalkable() {}             // 1. 인터페이스여서 protected non-virtual(상속해서 사용하고, 다형 소멸 안함) 입니다.

public:
    virtual void Walk() = 0;    // 3. 기능 스펙을 순가상 함수로 제공합니다. 상속해서만 사용할수 있습니다.
};

class Dog : 
    public IEatable,
    public IWalkable {
    public:
        virtual void Eat() {}
        virtual void Walk() {}
    };
    IEatable eatable;       // (x) 소멸자가 protected. 순가상 함수가 있음.
    IWalkable walkable;     // (x) 소멸자가 protected. 순가상 할수가 있음.
    Dog dog;                // (0)

    IEatable* p = new Dog();
    delete* p;              // (x) IEatable 의 소멸자가 protected. 

/*  나쁜 상송 - 부모 개체의 기본 구현  */
// 부모 개체 정의시 마땅히 할게 없으면 순가상 함수로 정의하는게 낫습니다. 
//      괜히 대충 기본 작업을 작성하지 마세요.
class Base {
public:
    virtual void Func() {
        // (~) 비권장 대충 기본 작업을 합니다.

    }
};
class Derived {
public:
    virtual void Func() {
        // 자식 개체에서 제대로 구현합니다.
    }
};


/*      상속 강제       */
/*
부모 개체로만 사용할 것이라면, 직접 인스턴스화 하지 못하고, 상속해서만 사용할수 있도록 강제해야 합니다.
    1. 생성자를 protected 로 만들면 됩니다. (상속 전용 기반 클래스 - protected 생성자 참고)
    2. 순가상 함수가 있으면 됩니다.
    3. has-a 관계이면 protected Non-Virtual 소멸자로 만들면 됩니다.
    4. 1.2.3으로 할수 없는 경우는 is-a 관계에서 다형 소멸이 필요하여 public Virtual 소멸자를 
        써야 하는데, 순가상 함수가 없는 경우입니다. protected 생성자로 할수없고, 
        순가상 함수가 없으므로 다음처럼 인스턴스화 할수 있습니다.
*/
class T {
public:
    virtual ~T() {}
};

T t; // (~) 순가상 함수가 없으면 개체 정의 (인스턴스화)를 할수 있습니다.

// 이럴 경우 순가상 소멸자를 사용합니다. (추상 클래스 참고)
class T {
public:
    virtual ~T() = 0;       // 다형 소멸 하도록 public virtual, 마땅히 할게 없어 순가상
};
T::~T() {}  // 실제 구현 정의가 있어야 합니다.
class U : public T {};

T t;        // (x) 순가상 소멸자가 있어 개체 정의 (인스턴스화) 를 할수 있습니다.
U u;        // (0) 상속하면 개체 정의 (인스턴스화) 가 가능합니다.

/*  상속 제한   */
/*
부모 개체로 사용하지 않을 것이라면, public Non-Virtual 소멸자로 정의합니다. 다만, 아무런
코딩 계약이 되어 있지 않아, 상속할 수도 있습니다. (public Non-Virtual 소멸자 참고)

강제로 상속을 제한하려면, 생성자를 private로 만들면 되는데요, 이 경우 해당 클래스를 생성하려면
    Create() 함수를 별도로 만들어야 합니다. (생성자 접근 차단 - private 생성자 참고)
*/
class T {
private:
    T() {}      // 상속 및 외부에서는 접근 불가
public:
    static T Create() { return T(); }  // 내부 static 함수로 생성
    static T* CreatePtr() { return new T;}
};

class U : public T {};
// U u; // (x) 상속해서 생성할수 없음
// U* p = new u;    // (x)

// T t; // (x)
T t(T::Create());       // (0) T를 복사 생성
T* p = T::CreatePtr();  // (0) T의 포인터 생성
delete p;

// 프로그램 내에서 대부분의 개체는 상속하지 않고 사용하므로, 조금은 심한 코딩 계약이 될 수도 있고, 
//   나주에 상속할수 있는 개체로 리팩토링 했을때 기존에 사용한 Create() 함수들을 모두 
//   수정해야 하기 때문에 리팩토링 부담이 있어 권장하지는 않습니다.

// 그냥 public Non-Virtual 소멸자라면, 상속하지 말라는 뜻이니, 절대 상속하지 마세요.
//  혹시나 상속이 필요하다면, 그때 public Virtual 소멸자나, protected Non-Virtual
//  소멸자로 리팩토링 하시기 바랍니다.

/*      Runtime Type Info (RTTI)와 형변환       */
/*
런타임에 개체의 타임 정보를 얻어내고, dynamic_cast로 형변환 할수 있습니다.

다만, 가상 함수가 있어야 구체적인 자식 개체 (구현 개체)를 알수 있습니다. (명시적 형변환과 typeid 연산자 참고)

다음 코드는 ISinger 와 IDancer의 인터페이스를 상송한 Idol 클래스가 있는 경우,
1. Up casting: 부모 개체 (기반 개체) 로 형변환
2. Down casting: 자식 개체 (구체화된 개체)로 형변환
3. Sibling casting: 상송 트리 계층에서 형제 개체로 형변환

Up casting 과 같이 자식 개체에서 부모 개체로의 변환은 형변환이 필요없지만, 다른 경우에는 
    dynamic_cast로 형변환해야 합니다.
*/
class ISinger {
protected:
    ~ISinger() {}
public:
    virtual void Sing() const = 0;
};
public:
    virtual void Dance() const = 0; 
};
class Idol :
    public ISinger,
    public IDancer {
    public:
        virtual void Sing() const {}
        virtual void Dance() const {}
};

Idol obj;
Idol* idol = &obj;
ISinger *singer = &obj // (0) Up casting. 자식 개체에서 부모 개체로 잘 변환 됩니다.
IDancer *dancer = &obj;

idol = singer; //(x) down casting. 부모에서 자식 개체로는 변환되지 않습니다.
idol = dynamic_cast<Idol*>(singer); // (0) dynamic_cast로 down casting. 자식 개체로 변환됩니다.
dancer = dynamic_cast<IDancer*>(singer) // (0) Sibling casting via dynamic_cast. 형제 개체로 변환됩니다.

EXPECT_TRUE(idol != NULL);
EXPECT_TRUE(dancer != NULL);
EXPECT_TRUE(typeid(obj) == typeid(Idol));
EXPECT_TRUE(typeid(*idol) == typeid(Idol));
EXPECT_TRUE(typeid(*singer) == typeid(Idol));
EXPECT_TRUE(typeid(*dancer) == typeid(Idol));

/*      가상 복사 생성자        */
// 부모 개체의 복사 생성자는 오독작을 할수있습니다.
class Shape {
protected:
    Shape() {}      // (0) 자식 개체에서만 사용할수 있게끔 protected 입니다.
public:
    Shape(const Shape& other) {
        if (typeid(*this) != typeid(other)) {
            cons std::type_info& ti = typeid(other);
            std::cout << ti.name() << std::endl;
        }
    }

    virtual ~Shape() {} // 다형 소멸 하도록 public virtual
};

class Rectangle : public Shape {};
class Ellipse : public Shape {};

Rectangle rect;
Ellipse ellipse;
Shape shape(rect)       // (x) 오동작. shape 은 Rectangle 을 알지 못해 복사 생성할수 없습니다.

/*
따라서,
    1. 복사 생성자를 사용하지 못하도록 private로 막던지,
    2. 복사 생성자를 protected 로 만들고, 자식 개체에서 자기 자신을 복제하는 가상 함수인
        Clone() 함수를 구현하던지, 
해야 합니다.

가상 함수에서 언급했듯, 가상 함수 오버라이딩시 리턴값은 부모 개체의 것과 같거나 상속 관계
    (공변, covariant) 이면 됩니다. 이점을 이용해서, 다음처럼 자식 개체의 타입을 
    리턴하는 Clone()을 만들수 있습니다.
*/
class Shape {
protected:
    Shape() {} // (O) 자식 개체에서만 사용할 수 있게끔 protected 입니다. 
    Shape(const Shape& other) {} // (O) 자식 개체에서만 사용할 수 있게끔 protected 입니다.
public:
    virtual ~Shape() {} // 다형 소멸 하도록 public virtual    
    virtual Shape* Clone() const = 0; // (O) 부모 개체에서는 Shape* 으로 리턴합니다.
};

public Rectangle : public Shape {
public:
    virtual Rectangle* Clone() const {
        return new Rectangle(*this);        // Rectangle의 복사 생성자를 이용하며 본제복을 리턴합니다.
    }
};
class Ellipse : public Shape {
public:
    virtual Ellipse* Clone() const {    // (0) 자식 개체에서는 자식 타입으로 리턴합니다.
        return new Ellipse(*this);      // Ellipse 복사 생성자를 이용하며 복제본을 리턴합니다.
    }
};
Shape* shapes[2] = {
    new Rectangle(),
    new Ellipse()
};
Shape* clones[2];

for(int i = 0; i < 2; ++i) {
    clones[i] = shapes[i]->Clone(); // 복제본을 만듭니다.
}

// (0) 자식 개체의 타입으로 잘 복제 됩니다.
EXPECT_TRUE(typeid(*clones[0]) == typeid(Rectangle));
EXPECT_TRUE(typeid(*clones[1]) == typeid(Ellipse));

for (int i = 0; i < 2; ++i) {
    delete shapes[i];
    delete clones[i];
}

/*      부모 개체의 복사 대입 연산자        */
// 부모 개체의 복사 대입 연산자도 오동작을 할수 있습니다.
class Shape {
protected:
    Shape() {} // (O) 자식 개체에서만 사용할 수 있게끔 protected 입니다.  
    Shape(const Shape& other) {} // (O) 자식 개체에서만 사용할 수 있게끔 protected 입니다.
public:
    virtual ~Shape() {} // 다형 소멸 하도록 public virtual    
    Shape& operator =(const Shape& other) { //  (△) 비권장. 자식 개체가 복사 대입될 수 있습니다.
        if (typeid(*this) != typeid(other)) {
            const std::type_info& ti = typeid(other);
            std::cout << ti.name() << std::endl;  
        } 
        return *this; 
    }
    virtual Shape* Clone() const = 0; 
};

class Rectangle : public Shape {
public:
    virtual Rectangle* Clone() const { 
        return new Rectangle(*this); 
    }
};
class Ellipse : public Shape {
public:
    virtual Ellipse* Clone() const { 
        return new Ellipse(*this); 
    }
};

Rectangle rect1;
Rectangle rect2;
Ellipse ellipse;
Shape* shape = &rect1;

rect1 = rect2; // (0) 메시지 표시 안됨
*shape = ellipse;   // (x) 오동작. 동일한 Shape 타입이어서 실행됩니다. Rectangle에 Ellipse 를 복사 대입합니다.

/*
*shape = other; 를 하면, Rectangle 개체에 부모는 같지만 서로 다른 클래스일 Ellipse 를 
    대입할수 있습니다. 예제에서처럼 typeid(*this) != typeid(other)로 런타임에 검사하여 
    막을 수도 있지만, 기본적으로 부모 클래스의 복사 대입 연산자는 사용하지 못하게 막는게 좋습니다.
*/


class Shape {
protected:    
    Shape() {} // (O) 자식 개체에서만 사용할 수 있게끔 protected 입니다.  
    Shape(const Shape& other) {} // (O) 자식 개체에서만 사용할 수 있게끔 protected 입니다.
    Shape& operator =(const Shape& other) {return *this;} // (O) 부모 개체를 외부에서 직접 사용 못하게 막아 버립니다. 자식 개체에서만 사용할 수 있게끔 protected 입니다.
public:
    virtual ~Shape() {} // 다형 소멸 하도록 public virtual    
    virtual Shape* Clone() const = 0; 
};
class Rectangle : public Shape {
public:
    Rectangle& operator =(const Rectangle& other) { // (O) 자식 개체는 사용할 수 있습니다.
        return *this;
    }   
    virtual Rectangle* Clone() const { 
        return new Rectangle(*this); 
    }
};
class Ellipse : public Shape {
public:
    Ellipse& operator =(const Ellipse& other) { // (O) 자식 개체는 사용할 수 있습니다.
        return *this;
    } 
    virtual Ellipse* Clone() const { 
        return new Ellipse(*this); 
    }
};

Rectangle rect1;
Rectangle rect2;
Ellipse ellipse;
Shape* shape = &rect1;

rect1 = rect2;      // (0) 메시지 표시 안됨
*shape = rect2;     // (x) 복사 대입 연산은 protected 임