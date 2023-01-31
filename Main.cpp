#include <iostream>

#include "Delegate.h"

static void StaticFunction()
{
    std::cout << "FooFunction callback called" << std::endl;
}

class Foo
{
public:
    Foo(int d) : m_d(d) {}

    void MemberFunction()
    {
        std::cout << "MemberFunction called d: " << m_d << std::endl;
    }

    void MemberFunctionWithArgs(float a, int b, std::string c)
    {
        std::cout << "MemberFunctionWithArgs callback called a: " << a << " b: " << b << " c: " << c << " d: " << m_d << std::endl;
    }

private:
    int m_d;
};

int main()
{
    Foo foo1(5);
    Foo foo2(8);

    // Testing EventListener
    Delegate callbackTest;

    callbackTest.Subscribe(StaticFunction);
    callbackTest.Subscribe(&foo1, &Foo::MemberFunction);
    callbackTest();
    callbackTest.Subscribe(StaticFunction);
    callbackTest.Subscribe(&foo1, &Foo::MemberFunction);
    callbackTest();

    callbackTest.UnSubscribe(StaticFunction);
    callbackTest.UnSubscribe(&foo1, &Foo::MemberFunction);
    callbackTest();
    callbackTest.UnSubscribe(StaticFunction);
    callbackTest();

    callbackTest.Clear();
    callbackTest();

    // Testing EventListernerMember
    Delegate<float, int, std::string> callbackWithArgsTest;

    callbackWithArgsTest.Subscribe(&foo1 , &Foo::MemberFunctionWithArgs);
    callbackWithArgsTest(2.0f, 1, "Test1");
    callbackWithArgsTest.Subscribe(&foo1, &Foo::MemberFunctionWithArgs);
    callbackWithArgsTest.Subscribe(&foo2, &Foo::MemberFunctionWithArgs);
    callbackWithArgsTest(2.0f, 2, "Test2");

    callbackWithArgsTest.UnSubscribe(&foo1, & Foo::MemberFunctionWithArgs);
    callbackWithArgsTest(2.0f, 3, "Test3");
    callbackWithArgsTest.UnSubscribe(&foo1, &Foo::MemberFunctionWithArgs);
    callbackWithArgsTest.UnSubscribe(&foo2, &Foo::MemberFunctionWithArgs);
    callbackWithArgsTest(2.0f, 4, "Test4");

    callbackWithArgsTest.Clear();
    callbackWithArgsTest(2.0f, 5, "Test5");

    return EXIT_SUCCESS;
}
