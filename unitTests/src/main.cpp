#include <direct.h>
#include <future>
#include <iomanip>
#include <iostream>
#include <sstream>

#ifdef WIN32
#include <Windows.h>
#endif

#include <onut/ContentManager.h>
#include <onut/Dispatcher.h>
#include <onut/Files.h>
#include <onut/Pool.h>
#include <onut/Resource.h>
#include <onut/Settings.h>
#include <onut/Strings.h>

using namespace std;

#ifdef WIN32
HANDLE hcon;
#endif

struct IOSetColor
{
    int color;
};
ostream& operator <<(ostream& os, const IOSetColor& color)
{
#ifdef WIN32
    SetConsoleTextAttribute(hcon, color.color);
#endif
    return os;
}

IOSetColor setColor(int color)
{
    return{color};
}

int errCount = 0;

int majorTestCount = 0;
int subTestCount = 0;
int testCount = 0;

template<typename TtextType>
void majorTest(TtextType testName)
{
    ++majorTestCount;
    subTestCount = 0;
    testCount = 0;
    cout << setColor(7) << setw(2) << majorTestCount << " - " << testName << endl << endl;
}

template<typename TtextType>
void subTest(TtextType testName)
{
    ++subTestCount;
    testCount = 0;
    stringstream ss;
    ss << majorTestCount << "." << subTestCount;
    cout << setColor(7) << setw(6) << ss.str() << " - " << testName << endl << endl;
}

template<typename TtextType>
void checkTest(bool cond, TtextType testName)
{
    ++testCount;
    stringstream ss;
    ss << majorTestCount << "." << subTestCount << "." << testCount;
    if (cond)
    {
        cout << setColor(7) << setw(10) << ss.str() << " - " << setColor(10) << "Ok     " << setColor(7) << testName << endl;
    }
    else
    {
        ++errCount;
        cout << setColor(7) << setw(10) << ss.str() << " - " << setColor(12) << "FAILED " << setColor(7) << testName << endl;
    }
}

template<uintptr_t TloopCount>
bool testRandomPool()
{
    auto pPool = OPool::create(9, 7);

    class CObj
    {
    public:
        CObj() : a(1), b(2.5f) {}
        CObj(int _a) : a(_a), b(2.5f) {}
        CObj(int _a, float _b) : a(_a), b(_b) {}
        int a;
        float b;
    };

    int allocCount = 0;
    decltype(TloopCount) i = 0;
    CObj* objs[7] = {nullptr};
    for (; i < TloopCount; ++i)
    {
        auto rnd = rand() % 101;
        if (rnd < 50)
        {
            auto index = rand() % 7;
            if (objs[index])
            {
                // Dealloc should succeed
                if (!pPool->dealloc(objs[index]))
                {
                    stringstream ss;
                    ss << "Dealloc objs[" << index << "]";
                    checkTest(false, ss.str());
                    return false;
                }
                objs[index] = nullptr;
                --allocCount;
            }
            objs[index] = pPool->alloc<CObj>();
            if (allocCount < 7)
            {
                if (objs[index] == nullptr)
                {
                    checkTest(false, "allocCount < 7, so alloc should not be nullptr");
                    return false;
                }
                ++allocCount;
            }
            else
            {
                if (objs[index] != nullptr)
                {
                    checkTest(false, "allocCount is max, so alloc should be nullptr");
                    return false;
                }
            }
        }
        else if (rnd < 100)
        {
            auto index = rand() % 7;
            if (objs[index] == nullptr)
            {
                // Dealloc should fail
                if (pPool->dealloc(objs[index]))
                {
                    checkTest(false, "Dealloc nullptr");
                    return false;
                }
            }
            else
            {
                // Dealloc should succeed
                if (!pPool->dealloc(objs[index]))
                {
                    stringstream ss;
                    ss << "Dealloc objs[" << index << "]";
                    checkTest(false, ss.str());
                    return false;
                }
                objs[index] = nullptr;
                --allocCount;
            }
        }
        else
        {
            for (int j = 0; j < 7; ++j)
            {
                objs[j] = nullptr;
            }
            pPool->clear();
            allocCount = 0;
        }
        if (pPool->getAllocCount() != allocCount)
        {
            stringstream ss;
            ss << "allocCount " << allocCount << " == pool.getAllocCount() " << pPool->getAllocCount();
            checkTest(false, ss.str());
            return false;
        }
    }
    if (pPool->getAllocCount() != allocCount)
    {
        stringstream ss;
        ss << "allocCount " << allocCount << " == pool.getAllocCount() " << pPool->getAllocCount();
        checkTest(pPool->getAllocCount() == allocCount, ss.str());
    }
    return i == TloopCount;
}

void foo() {}

bool foob()
{
    return true;
}

void fooab(int* pA, float b)
{
    *pA = 5;
    b = 12.75f;
}

bool foo1Called = false;
bool foo2Called = false;
bool foo3Called = false;
bool foo4Called = false;
bool foo5Called = false;

void foo1()
{
    checkTest(!foo1Called && !foo2Called && !foo3Called && !foo4Called && !foo5Called,
              "foo1 called in order");
    foo1Called = true;
}

void foo2()
{
    checkTest(foo1Called && !foo2Called && !foo3Called && !foo4Called && !foo5Called,
              "foo2 called in order");
    foo2Called = true;
}

void foo3()
{
    checkTest(foo1Called && foo2Called && !foo3Called && !foo4Called && !foo5Called,
              "foo3 called in order");
    foo3Called = true;
}

void foo4()
{
    checkTest(foo1Called && foo2Called && foo3Called && !foo4Called && !foo5Called,
              "foo4 called in order");
    foo4Called = true;
}

void foo5()
{
    checkTest(foo1Called && foo2Called && foo3Called && foo4Called && !foo5Called,
              "foo5 called in order");
    foo5Called = true;
}

void runSynchronousTests()
{
    subTest("Basic Tests");
    {
        auto pDispatcher = ODispatcher::create();

        int a = 1;
        float b = 2.5f;

        pDispatcher->dispatch([&a, &b]()
        {
            a = 3;
            b = 4.25f;
        });
        checkTest(pDispatcher->size() == 1, "Lambda call added. Queue size = 1");

        pDispatcher->processQueue();
        checkTest(a == 3 && b == 4.25f, "processQueue(). Values are 3 and 4.25f");

        checkTest(pDispatcher->size() == 0, "Queue size = 0");

        pDispatcher->dispatch(foo);
        pDispatcher->dispatch(foob);
        pDispatcher->dispatch(fooab, &a, b);
        checkTest(pDispatcher->size() == 3, "3 foo functions added. Queue size = 3");

        pDispatcher->processQueue();
        checkTest(a == 5 && b == 4.25f, "processQueue(). Values are 5 and 4.25f");

        checkTest(pDispatcher->size() == 0, "Queue size = 0");

        cout << setColor(7) << endl;
    }

    subTest("Test call order");
    {
        auto pDispatcher = ODispatcher::create();
        foo1Called = false;
        foo2Called = false;
        foo3Called = false;
        foo4Called = false;
        foo5Called = false;
        pDispatcher->dispatch(foo1);
        pDispatcher->dispatch(foo2);
        pDispatcher->dispatch(foo3);
        pDispatcher->dispatch(foo4);
        pDispatcher->dispatch(foo5);
        checkTest(pDispatcher->size() == 5, "5 foo functions added. Queue size = 5");

        pDispatcher->processQueue();

        checkTest(pDispatcher->size() == 0, "Queue size = 0");

        cout << setColor(7) << endl;
    }

    subTest("Threaded test");
    {
        auto pDispatcher = ODispatcher::create();
        foo1Called = false;
        foo2Called = false;
        foo3Called = false;
        foo4Called = false;
        foo5Called = false;

        auto ret = std::async(std::launch::async, [&pDispatcher]
        {
            pDispatcher->dispatch(foo1);
            //std::this_thread::sleep_for(std::chrono::milliseconds(20));
            pDispatcher->dispatch(foo2);
            pDispatcher->dispatch(foo3);
            //std::this_thread::sleep_for(std::chrono::milliseconds(200));
            pDispatcher->dispatch(foo4);
            //std::this_thread::sleep_for(std::chrono::milliseconds(50));
            pDispatcher->dispatch(foo5);
            //std::this_thread::sleep_for(std::chrono::milliseconds(450));
        });

        auto startTime = std::chrono::steady_clock::now();
        while (ret.wait_for(std::chrono::seconds(0)) != std::future_status::ready &&
               std::chrono::steady_clock::now() - startTime < std::chrono::seconds(5))
        {
            pDispatcher->processQueue();
        }
        pDispatcher->processQueue();

        checkTest(pDispatcher->size() == 0, "Synchronous didn't stall (< 5seconds)");

        cout << setColor(7) << endl;
    }
}

class TestResource1 : public onut::Resource
{
public:
    static std::shared_ptr<TestResource1> createFromFile(const std::string& filename, const OContentManagerRef& pContentManager = nullptr)
    {
        if (filename == "not found") return nullptr;
        return OMake<TestResource1>();
    }
    int a = 5;
    float b = 3.25f;
};
class TestResource2 : public onut::Resource
{
public:
    static std::shared_ptr<TestResource2> createFromFile(const std::string& filename, const OContentManagerRef& pContentManager = nullptr)
    {
        if (filename == "not found") return nullptr;
        return OMake<TestResource2>();
    }
    int a = 7;
    float b = 10.75f;
};

int main(int argc, char** args)
{
#ifdef WIN32
    hcon = GetStdHandle(STD_OUTPUT_HANDLE);
#endif

    if (argc > 1)
    {
        _chdir(args[1]);
    }

    majorTest("onut::Pool");
    {
        subTest("Basic tests using onut::Pool(16, 10)");
        {
            auto pPool = OPool::create(16, 10);

            class CObj
            {
            public:
                CObj() : a(1), b(2.5f) {}
                CObj(int _a) : a(_a), b(2.5f) {}
                CObj(int _a, float _b) : a(_a), b(_b) {}
                int a;
                float b;
            };
            auto pObj = pPool->alloc<CObj>();
            checkTest(pObj != nullptr, "Alloc CObj()");

            checkTest(pPool->getAllocCount() == 1, "Alloc count is 1");

            checkTest(pObj->a == 1 && pObj->b == 2.5f, "CObj values are 1 and 2.5f");

            checkTest(!pPool->dealloc(static_cast<CObj*>(nullptr)), "Dealloc nullptr");

            checkTest(pPool->dealloc(pObj), "Dealloc");

            checkTest(pPool->getAllocCount() == 0, "Alloc count is 0");

            class CTooBigObj
            {
            public:
                float tooBigMember[5];
            };
            auto pObjToBig = pPool->alloc<CTooBigObj>();
            checkTest(pObjToBig == nullptr, "Trying alloc obj too big");

            CObj* objs[10] = {nullptr};
            for (int i = 0; i < 10; ++i)
            {
                objs[i] = pPool->alloc<CObj>();
            }
            checkTest(
                objs[0] != nullptr && objs[1] != nullptr && objs[2] != nullptr && objs[3] != nullptr && objs[4] != nullptr &&
                objs[5] != nullptr && objs[6] != nullptr && objs[7] != nullptr && objs[8] != nullptr && objs[9] != nullptr,
                "Alloc objs[10]");

            pObj = pPool->alloc<CObj>();
            checkTest(pObj == nullptr, "Trying alloc over the max obj");

            checkTest(pPool->getAllocCount() == 10, "Alloc count is 10");

            checkTest(pPool->dealloc(objs[0]), "Dealloc objs[0]");

            checkTest(pPool->getAllocCount() == 9, "Alloc count is 9");

            pObj = pPool->alloc<CObj>();
            checkTest(pObj != nullptr, "Alloc CObj()");

            pObj = pPool->alloc<CObj>();
            checkTest(pObj == nullptr, "Trying alloc over the max obj again");

            pPool->clear();
            checkTest(pPool->getAllocCount() == 0, "clear(). Alloc count is 0");

            pObj = pPool->alloc<CObj>();
            checkTest(pObj != nullptr, "Alloc CObj()");

            checkTest(pPool->getAllocCount() == 1, "Alloc count is 1");

            pObj = pPool->alloc<CObj>(7);
            checkTest(pObj != nullptr, "Alloc CObj(7)");

            checkTest(pObj->a == 7 && pObj->b == 2.5f, "CObj values are 7 and 2.5f");

            pObj = pPool->alloc<CObj>(4, 3.25f);
            checkTest(pObj != nullptr, "Alloc CObj(4, 3.25f)");

            checkTest(pObj->a == 4 && pObj->b == 3.25f, "CObj values are 4 and 3.25f");

            cout << setColor(7) << endl;
        }

        subTest("Stress test with onut::Pool(9, 7) and random alloc/dealloc");
        {
            srand(static_cast<unsigned int>(time(0)));

            srand(0);
            checkTest(testRandomPool<10000>(), "seed = 0");

            srand(5632);
            checkTest(testRandomPool<10000>(), "seed = 5632");

            for (int i = 0; i < 5; ++i)
            {
                srand(static_cast<unsigned int>(time(0)));
                checkTest(testRandomPool<10000>(), "seed = random");
            }

            cout << setColor(7) << endl;
        }
        cout << setColor(7) << endl;
    }
    
    majorTest("onut::Synchronous");
    {
        runSynchronousTests();
        cout << setColor(7) << endl;
    }
    
    majorTest("String utilities");
    {
        {
            auto ret = onut::utf16ToUtf8(L"Hello World");
            checkTest(ret == "Hello World", "UTF16 -> UTF8 \"Hello World\"");
        }
        {
            auto ret = onut::utf8ToUtf16("Hello World");
            checkTest(ret == L"Hello World", "UTF8 -> UTF16 \"Hello World\"");
        }
        {
            auto retUtf8 = onut::utf16ToUtf8(L"你好");
            auto ret = onut::utf8ToUtf16(retUtf8);
            checkTest(ret == L"你好", "UTF16 -> UTF8 -> UTF16 \"Ni Hao\"");
        }
        {
            auto split = onut::splitString("Hello,World", ',');
            checkTest(split.size() == 2, "\"Hello,World\" contains 2 split");
            if (split.size() >= 1) checkTest(split[0] == "Hello", "split[0] = \"Hello\"");
            if (split.size() >= 2) checkTest(split[1] == "World", "split[1] = \"World\"");
        }
        {
            auto split = onut::splitString("Hello, ,World", ',');
            checkTest(split.size() == 3, "\"Hello, ,World\" contains 3 split");
            if (split.size() >= 1) checkTest(split[0] == "Hello", "split[0] = \"Hello\"");
            if (split.size() >= 2) checkTest(split[1] == " ", "split[1] = \" \"");
            if (split.size() >= 3) checkTest(split[2] == "World", "split[2] = \"World\"");
        }
        {
            auto split = onut::splitString("Hello,,World", ',');
            checkTest(split.size() == 2, "\"Hello,,World\" contains 2 split");
            if (split.size() >= 1) checkTest(split[0] == "Hello", "split[0] = \"Hello\"");
            if (split.size() >= 2) checkTest(split[1] == "World", "split[1] = \"World\"");
        }
        {
            auto split = onut::splitString(",Hello,World", ',');
            checkTest(split.size() == 2, "\",Hello,World\" contains 2 split");
            if (split.size() >= 1) checkTest(split[0] == "Hello", "split[0] = \"Hello\"");
            if (split.size() >= 2) checkTest(split[1] == "World", "split[1] = \"World\"");
        }
        {
            auto split = onut::splitString(",Hello,World,", ',');
            checkTest(split.size() == 2, "\",Hello,World,\" contains 2 split");
            if (split.size() >= 1) checkTest(split[0] == "Hello", "split[0] = \"Hello\"");
            if (split.size() >= 2) checkTest(split[1] == "World", "split[1] = \"World\"");
        }
        {
            auto split = onut::splitString(",Hello,World,", ',', false);
            checkTest(split.size() == 4, "\",Hello,World,\" contains 4 split");
            if (split.size() >= 1) checkTest(split[0] == "", "split[0] = \"\"");
            if (split.size() >= 2) checkTest(split[1] == "Hello", "split[4] = \"Hello\"");
            if (split.size() >= 3) checkTest(split[2] == "World", "split[2] = \"World\"");
            if (split.size() >= 4) checkTest(split[3] == "", "split[3] = \"\"");
        }
        {
            auto split = onut::splitString("Hello,World,", ',');
            checkTest(split.size() == 2, "\"Hello,World,\" contains 2 split");
            if (split.size() >= 1) checkTest(split[0] == "Hello", "split[0] = \"Hello\"");
            if (split.size() >= 2) checkTest(split[1] == "World", "split[1] = \"World\"");
        }
        {
            auto split = onut::splitString("Hello,World, ", ',');
            checkTest(split.size() == 3, "\"Hello,World, \" contains 3 split");
            if (split.size() >= 1) checkTest(split[0] == "Hello", "split[0] = \"Hello\"");
            if (split.size() >= 2) checkTest(split[1] == "World", "split[1] = \"World\"");
            if (split.size() >= 3) checkTest(split[2] == " ", "split[1] = \" \"");
        }
        {
            auto split = onut::splitString("Hello,World, ,", ',');
            checkTest(split.size() == 3, "\"Hello,World, ,\" contains 3 split");
            if (split.size() >= 1) checkTest(split[0] == "Hello", "split[0] = \"Hello\"");
            if (split.size() >= 2) checkTest(split[1] == "World", "split[1] = \"World\"");
            if (split.size() >= 3) checkTest(split[2] == " ", "split[1] = \" \"");
        }
        cout << setColor(7) << endl;
    }
    
    majorTest("File search");
    {
        subTest("Basic seaches");
        {
            auto ret = onut::findFile("res1.txt", "../..");
            checkTest(ret == "../../assets/textures/res1.txt", "Search existing file res1.txt");
        }
        {
            auto ret = onut::findFile("someFileThatDoesntExist.txt", "../..");
            checkTest(ret == "", "Search missing file someFileThatDoesntExist.txt");
        }
        {
            auto ret = onut::findFile("res1.txt", "../../assets/textures");
            checkTest(ret == "../../assets/textures/res1.txt", "Search existing from withing res1.txt folder");
        }
        {
            auto ret = onut::findFile("res1.txt", "../..", false);
            checkTest(ret == "", "Search res1.txt without deep search");
        }
        cout << setColor(7) << endl;
    }
    
    majorTest("onut::ContentManager");
    {
        subTest("Basic tests");
        {
            auto pContentManager = OContentManager::create();

            auto pRes1 = pContentManager->getResourceAs<TestResource1>("res1.txt");
            checkTest(pRes1 != nullptr, "Create res1");

            checkTest(pContentManager->size() == 1, "Res count = 1");

            auto pRes2 = pContentManager->getResourceAs<TestResource2>("res2.txt");
            checkTest(pRes2 != nullptr, "Create res2");

            checkTest(pContentManager->size() == 2, "Res count = 2");

            pContentManager->clear();
            checkTest(pContentManager->size() == 0, "Cleared. Res count = 0");

            pRes1 = pContentManager->getResourceAs<TestResource1>("res1.txt");
            pRes2 = pContentManager->getResourceAs<TestResource2>("res2.txt");
            checkTest(pRes1 != nullptr && pRes2 != nullptr, "Create res1 and res2");

            checkTest(pContentManager->size() == 2, "Res count = 2");

            auto pRes3 = pContentManager->getResourceAs<TestResource1>("res1.txt");
            checkTest(pRes3 != nullptr, "Create res1 again");

            checkTest(pContentManager->size() == 2, "Res count = 2");

            auto pRes4 = pContentManager->getResourceAs<TestResource1>("someFileThatDoesntExist.txt");
            checkTest(pRes4 == nullptr, "Create missing \"someFileThatDoesntExist.txt\" asset");

            cout << setColor(7) << endl;
        }
        
        subTest("Async tests");
        {
            auto pContentManager = OContentManager::create();

            bool testRet[6] = {false};
            std::atomic<int> completed = 0;

            auto future1 = std::async(std::launch::async, [&]
            {
                testRet[0] = pContentManager->getResourceAs<TestResource1>("res1.txt") != nullptr;
                testRet[1] = pContentManager->getResourceAs<TestResource2>("someFileThatDoesntExist.txt") == nullptr;
                testRet[2] = pContentManager->getResourceAs<TestResource2>("res2.txt") != nullptr;
                ++completed;
            });
            auto future2 = std::async(std::launch::async, [&]
            {
                testRet[3] = pContentManager->getResourceAs<TestResource1>("res1.txt") != nullptr;
                testRet[4] = pContentManager->getResourceAs<TestResource2>("res2.txt") != nullptr;
                testRet[5] = pContentManager->getResourceAs<TestResource2>("someFileThatDoesntExist.txt") == nullptr;
                ++completed;
            });

            auto pDispatcher = ODispatcher::create();
            while (completed < 2)
            {
                pDispatcher->processQueue();
            }

            future1.wait();
            future2.wait();

            checkTest(testRet[0], "Create res1.txt from thread 1");
            checkTest(testRet[1], "Create missing someFileThatDoesntExist.txt from thread 1");
            checkTest(testRet[2], "Create res2.txt from thread 1");
            checkTest(testRet[3], "Create res1.txt from thread 2");
            checkTest(testRet[4], "Create res2.txt from thread 2");
            checkTest(testRet[5], "Create missing someFileThatDoesntExist.txt from thread 2");
            checkTest(pContentManager->size() == 2, "Res count = 2");

            cout << setColor(7) << endl;
        }

        cout << setColor(7) << endl;
    }

    oSettings = nullptr;

    system("pause");
    return errCount;
}
