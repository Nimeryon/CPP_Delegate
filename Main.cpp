/*#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "UIElement.h"
#include "GameData.h"
#include "UIEvents.h"

class TestEventReceiver : public IUIEventSubscriber
{
public:
    virtual void OnEvent(const UIEvent* _pEvent)
    {
        if (_pEvent->ID == GameData::eGameDataUpdated)
        {
            std::cout << "Game Data Updated ! "<< std::endl;
        }
        else if (_pEvent->ID == GameData::eDebugModeChanged)
        {
            std::cout << "Debug Changed ! " << std::endl;
        }
    }
};


class OtherEventReceiver : public IUIEventSubscriber
{
public:
    virtual void OnEvent(const UIEvent* _pEvent)
    {
        if (_pEvent->ID == GameData::eGameDataUpdated)
        {
            std::cout << "Game Data Updated ! in the other event receiver" << std::endl;
        }
    }
};

class OtherOtherEventReceiver : public IUIEventSubscriber
{
public:
    virtual void OnEvent(const UIEvent* _pEvent)
    {
    	if (_pEvent->ID == GameData::eGameDataChanged)
        {
            std::cout
                << "Game Data Changed ! / Value: "
                << dynamic_cast<const UIEventParam<int>*>(_pEvent)->Value
                << std::endl;
        }
    }
};

int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");

    GameData g_GameData;
    g_GameData.Init();

    TestEventReceiver g_Test;
    OtherEventReceiver g_Test2;
    OtherOtherEventReceiver g_Test3;
    
    g_GameData.SubscribeToEvent(GameData::eDebugModeChanged, &g_Test);
    g_GameData.SubscribeToEvent(GameData::eGameDataUpdated, &g_Test);

    g_GameData.SubscribeToEvent(GameData::eGameDataUpdated, &g_Test2);

    g_GameData.SubscribeToEvent(GameData::eGameDataChanged, &g_Test3);

    // When the 'd' key 
  
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();
            else
                g_GameData.OnWindowEvent(event);
            
        }
        // Clear screen
        window.clear();
        
        g_GameData.Update();        

        g_GameData.Draw(window);
        // Update the window
        window.display();
    }
    return EXIT_SUCCESS;
}*/












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