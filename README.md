# CPP_Delegate
A quick and lightweight C++ Delegate implementation.

## Usage
Here's a quick explanation of how to use:
  - First create a new Delegate with you wanted arguments
  - Subscribe you desired functions or lambda functions
  - Don't forget to Unsubscribe your functions when not needed or Clear your Delegate

```C++
#include <iostream>
#include "Delegate.h"

static void StaticFunction() { std::cout << "FooFunction callback called" << std::endl; }

class Foo
{
public:
    void MemberFunction() { std::cout << "MemberFunction called" << std::endl; }
    void MemberFunctionWithArgs(float a) { std::cout << "MemberFunctionWithArgs callback called a: " << a << std::endl; }
};

int main()
{
    auto lambdaFunction = []{ std::cout << "LambadaFunctionWithArgs callback called" << std::endl; };

    Foo foo1;
    Foo foo2;

    // Testing EventListener
    Delegate callbackTest;

    callbackTest.Subscribe(lambdaFunction);
    callbackTest.Subscribe(StaticFunction);
    callbackTest.Subscribe(&foo1, &Foo::MemberFunction);
    callbackTest();
    callbackTest.Subscribe(StaticFunction);
    callbackTest.Subscribe(&foo1, &Foo::MemberFunction);
    callbackTest();

    callbackTest.UnSubscribe(lambdaFunction);
    callbackTest.UnSubscribe(StaticFunction);
    callbackTest.UnSubscribe(&foo1, &Foo::MemberFunction);
    callbackTest();
    callbackTest.UnSubscribe(StaticFunction);
    callbackTest();

    callbackTest.Clear();
    callbackTest();

    // Testing EventListernerMember
    Delegate<float> callbackWithArgsTest;

    callbackWithArgsTest.Subscribe(&foo1 , &Foo::MemberFunctionWithArgs);
    callbackWithArgsTest(1.0f);
    callbackWithArgsTest.Subscribe(&foo1, &Foo::MemberFunctionWithArgs);
    callbackWithArgsTest.Subscribe(&foo2, &Foo::MemberFunctionWithArgs);
    callbackWithArgsTest(2.0f);

    callbackWithArgsTest.UnSubscribe(&foo1, & Foo::MemberFunctionWithArgs);
    callbackWithArgsTest(3.0f);
    callbackWithArgsTest.UnSubscribe(&foo1, &Foo::MemberFunctionWithArgs);
    callbackWithArgsTest.UnSubscribe(&foo2, &Foo::MemberFunctionWithArgs);
    callbackWithArgsTest(4.0f);

    callbackWithArgsTest.Clear();
    callbackWithArgsTest(5.0f);

    return EXIT_SUCCESS;
}
```
