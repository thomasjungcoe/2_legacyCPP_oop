/**
 *  다형성 polymorphism(?)
 * =============================================================================
 * 다형성은 개체의 속성이나 기능(함수)이 상황에 따라 여러가지 형태로 변할수있는 성질을 말합니다.
 *  * 함수 오버로딩을 통해 전달된 인자에 따라 다른 기능을 실행하고,
 *  * 상속 관계를 통해 자식 개체들이 다른 동작을 수행하게 됩니다.
 */

/*
C++에서 다형성을 위해 제공하는 기능들은 다음과 같습니다.

항목	                내용
함수 오버로딩	        전달된 인자에 따라 다른 함수가 호출됩니다.
                        * 연산자 오버로딩 참고
                        * 오버로딩 함수 결정 규칙 참고
                        * 오버로딩 함수 탐색 규칙 참고
상속	                부모 개체를 재활용하여, 새로운 자식 클래스를 만들어 확장합니다.
                        * is-a 관계 참고
                        * has-a 관계 참고
                        * 상속 강제 참고
                        * 상속 제한 참고
추상 클래스	            자식 개체에게 일반 개념을 제공합니다.
                        * 추상 클래스 참고
                        * public Virtual 소멸자 참고
인터페이스	            기능 스펙을 제공합니다.
                        * 인터페이스 참고
                        * protected Non-Virtual 소멸자 참고
소멸자	                상속 관계에 따라 다르게 사용합니다.
                        * 다형 소멸 참고
                        * public Non-Virtual 소멸자 참고
                        * public Virtual 소멸자 참고
                        * protected Non-Virtual 소멸자 참고
가상 함수 오버라이딩	부모 개체의 가상 함수를 재구현 합니다.
                        * 가상 함수 참고
                        * 가상 함수 테이블 참고
                        * 순가상 함수 참고
                        * 순가상 소멸자 참고
가상 복사 생성자	    개체 타입에 따라 자기 자신을 복제 생성하는 함수입니다.
                        * 가상 복사 생성자 참고
템플릿	                개체 타입에 따라 다르게 동작하는 함수와 클래스
dynamic_cast        	상속 관계가 있는 개체간의 변환을 제공합니다.
                        * 명시적 형변환 참고
                        * Runtime Type Info(RTTI)와 형변환 참고
Runtime Type Info(RTTI)	개체의 타입 정보를 제공합니다.
* Runtime Type Info(RTTI)와 형변환 참고
*/