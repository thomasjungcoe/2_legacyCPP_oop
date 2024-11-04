/**
 *      추상 클래스, 인터페이스
 * ================================================================================
 * 기능 스펙을 정의하여 코딩 계약을 맺으려면 인터페이스로 정의하라.
 * 기능 스펙과 어느 정도의 공통 기늘을 제공하려면 추상 클래스로 정의하라.
 * 
 * Modern C++:
 *  C++11~: default, delete가 추가되어 암시적으로 생성되는 멤버 함수의 사용 여부를 좀더
 *      명시적으로 정의할수 있습니다.
 *  C++20~: 컨셉 설계를 활용하여 마치 인터페이스처럼 컨셉에 의한 코딩 계약을 만들수 있습니다.
 * 
 *  추상 클래스
 * 인스턴스화 할수 없는 부모 클래스를 추상 클래스라고 합니다. 추상 클래스는 구체 구현한
 *  자식 클래스들의 공통적인 일반 개념(기능 스펙)을 정의하는데 사용합니다. 공통적인 일반
 *  개념을 잘 설계하면, 재활용성이 높아져 활용도가 높아지고, 특정 문제들을 해결하는데 
 *  있어서 공통된 접근을 하기 때문에 고민의 가지수가 적어집니다.
 * 
 * 1. 공통적인 일반 개년 (기능 스펙)을 정의합니다.
 * 2. 추상 클래스는 1개 이상의 순가상 함수가 있어야 합니다.
 * 3. 상속 강제를 해야 하므로 protected 생성자를 사용합니다.
 * 4. 추상 클래스는 복사 생성자를 사용하지 못하도록 private로 막던지, 상속해서만 사용할수 
 *  있도록 protected로 만들지, 자식 개체에서 가상 복사 생성자를 구현합니다. (가상 복사 생성자 참고)
 * 5. 추상 클래스는 복사 대입 연산자를 private 로 막습니다. (부모 개체의 복사 대입 연산자 참고)
 * 6. 다형 소멸을 하려면 public Virtual 소멸자를 사용합니다.
 */

#include <iostream>
class Shape {
    // 1. 모든 도형은 왼쪽 상단 좌표와 크기를 가집니다.
    int m_Left;
    int m_Top;
    int m_Width;
    int m_Height;
private:
    Shape(const Shape& other) {}    // #4. 추상 클래스여서 외부에서 사용 못하게 복사 생성자 막음
    Shape& operator =(const Shape& other) { return *this; } // 5. 추상 클래스여서 외부에서 사용 못하게 복사 대입 연산자 막음
protected:
    Shape() {}      // 3. 추상 클래스여서 상속한 개체에서만 생성할수 있게함
public:
    virtual ~Shape() {}     // 6. 다형 소멸 하도록 public virtual
    virtual void Draw() const = 0;  // 2. 순가상 합수입니다. 자식 클래스에서 구체화 해야 합니다.
};

// Shape을 구체화한 클래스 입니다. Draw()에서 사각형을 그립니다.
class Rectangle : public Shape {
public:
    virtual void Draw() const {
        std::cout << "Rectangle::Draw()" << std::endl;
    }
};

// SHape 을 구체화한 클래스 입니다. Draw() 에서 타원을 그립니다.
class Ellipse : public Shape {
public:
    virtual void Draw() const {
        std::cout << "Ellipse::Draw()" << std::endl;
    }
};

Shape shape;        // (x) 추상 클래스는 순가상 함수가 있어 구체화 할수 없습니다.
Shape* p = new Shape // (x) ""

Shape* shapes[2] = {    // 도형들을 Shape* 로 관리합니다.
    new Rectangle(),
    new Ellipse()
};

for (int i = 0; i < 2; ++i) {
    shapes[i]->Draw();  // 다형적으로 그립니다.
}

for (int i = 0; i < 2; ++i) {
    delete shapes[i];   // 다형 소멸 합니다. Shape*로 Rectangle, Ellipse을 소멸합니다.
}


class Abstract {
public:
    virtual ~Abstract() = 0;    // 다형 소멸 하도록 public virtual, 마땅히 할게 없어 순가상
};
Abstract::~Abstract() {}        // 실제 구현 정의가 있어야 함

class Concrete : public Abstract {};

Abstract abstract;  // (x) 정상 코딩 계약. 추상 클래스는 순가상 함수가 있어 인스턴스화 할수 없습니다.
Concrete concrete;  // (0) 상속하면 인스턴스화 가능

/*      인터페이스      */
/*
추상 클래스중 구체 구현없이 모두 추상화된 기능 스펙으로 구성된 클래스를 특별히 
    인터페이스라고 합니다.
    * 인터페이스는 모두 순가상 합수로 구성됩니다.
    * 상속 강제를 해야 하므로 protected 생성자를 사용합니다.
    * 일반적으로 인터페이스는 구체 구현이 없으므로 복사 생성자, 복사 대입 연산자를 
    *   private로 막습니다. (가상 복사 생성자와 부모 개체의 복사 대입 연산자 참고)
    * 인반적으로 인터페이스는 기능 스펙만 제공하고, 다형 소멸을 제공하지 않습니다. 다형 
    *   소멸을 제공하는건 그냥 추상 클래스라고 보시는게 좋습니다.
    * 다형 소멸을 사지 않으므로 protected Non-Virtual 소멸자를 사용합니다.

다음은 IDrawable 인터페이스로 Shape 클래스를 구현한 예입니다.
1. 인터페이스는 1개 이상의 순가상 함수가 있어야 합니다. IDrawable에 Draw() 순가상 함수를 선언합니다.
2. IDrawable은 상송 강제를 해야 하므로 protected 기본 생성자를 사용합니다.
3. IDrawable 은 인터페이스여서 복사 생성자, 복사 대입 연산자를 외부에서 사용하지 못하도록 
    private 을 사용합니다.
4. IDrawable은 인터페이스여서 protected Non-Virtual 소멸자를 사용합니다.
5. Shape 은 IDrawable 을 상속하고, 다형 소멸을 할것이므로 public Virtual 소멸자를 사용합니다.
    (상속한 IDrawable 의 Draw() 함수가 구체화 되지 않았으므로 여전히 추상 클래스입니다.)
*/

// 인터페이스
class IDrawable {
private:
    IDrawable(const IDrawable& other) {}    // 3. 인터페이스여서 외부에서 사용 못하게 복사 생성자 막음
    IDrawable& operator =(const IDrawable& other) { return &this; } // 3
protected:
    IDrawable() {}  // 2. 인터페이스여서 상속한 개체에서만 생성할수 있게함
    ~IDrawable() {} // 4. 인터페이스여서 protected non-Virtual (상속해서 사용하고, 다형 소멸 안함) 입니다.
public:
    virtual void Draw() const = 0;  // 1. 순가상 합수입니다. 자식 클래스에서 구체화 해야합니다.
};

// 추상 클래스
class Shape : public IDrawable {    // 5. Shape 은 IDrawable 인터페이스를 제공합니다.
    // 모든 도형은 왼쪽 상단 좌표와 크기를 가집니다.
    int m_Left;
    int m_Top;
    int m_Width;
    int m_Height;
public:
    virtual ~Shape() {} // 5. 다형 소멸 하도록 public virtual
}

// Shape을 구체화한 클래스 입니다. Draw() 에서 사각형을 그립니다.
class Rectangle : public Shape {
public:
    virtual void Draw() const {
        std::cout << "Rectangle::Draw()" << std::endl;
    }
};

// Shape을 구체화한 클래스 입니다. Draw()에서 타원을 그립니다.
class Ellipse : public Shape {
public:
    virtual void Draw() const {
        std::cout << "Ellipse::Draw()" << std::endl;
    }
}

hape shape; // (X) 컴파일 오류. 추상 클래스는 순가상 함수가 있어 구체화 할 수 없습니다.
Shape* p = new Shape; // (X) 컴파일 오류. 추상 클래스는 순가상 함수가 있어 구체화 할 수 없습니다.

IDrawable drawable; // (x) 인터페이스는 순가상 함수가 있어 구체화 할수 없습니다.
IDrawable* p = new IDrawable; // (x)

// (0) Shape으로 다형 소멸합니다.
Shape* shapes[2] = {
    new Rectangle(),
    new Ellipse()
};

// (0) Shape이 IDrawable을 상속했으므로 Draw() 할수 있습니다.
for (int i = 0; i < 2; ++i) {
    shapes[i]->Draw();  // 다형적으로 그립니다.
}

for (int i =0; i < 2; ++i) {
    delete shapes[i];   // 다형 소멸 합니다. Shape* 로 Rectangle, Ellipse을 소멸합니다.
}

// C++11~: default, delete가 추가되어 암시적으로 생성되는 멤버 함수의 사용 여부를 좀더 명시적으로 정의할수 있습니다.

class DrawUtil {
public:
    static void Draw(const IDrawable& drawable) {
        drawable.Draw();
    }    
};

// (O) IDrawable 인터페이스로 Draw() 할 수 있습니다.
Shape* shapes[2] = { // 도형들을 Shape* 로 관리합니다.
    new Rectangle(), 
    new Ellipse()
};

// (O) IDrawable 인터페이스로 다형적으로 그립니다. 
for(int i = 0; i < 2; ++i) {
    DrawUtil::Draw(*shapes[i]); 
}
for(int i = 0; i < 2; ++i) {
    delete shapes[i]; // 다형 소멸 합니다. Shape*로 Rectangle, Ellipse을 소멸합니다.
} 
// 인터페이스는 다형 소멸을 지원하지 않으므로, 다음과 같이 IDrawable 포인터로 delete 할수 없습니다.

// (x) protecetd 소멸자 이므로, IDrawable로 다형 소멸 되지 않습니다.
IDrawable* drawables[2] = {
    new Rectangle(), 
    new Ellipse()
};

for (int i = 0; i < 3; ++i) {
    drawable[i]->Draw();        // 다형적으로 그립니다.
}

for (int i = 0; i < 2; ++i) {
    delete drawables[i]; // (x) 인터페이스는 protected Non-Virtual 소멸자이기 때문에 다형 소멸을 제공하지 않습니다.
}
// C++20~: 컨셉 설계를 활용하여 마치 인터페이스처럼 컨셉에 의한 코딩 계약을 만들수 있습니다.