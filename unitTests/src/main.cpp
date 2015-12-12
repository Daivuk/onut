#include <direct.h>
#include <future>
#include <iomanip>
#include <iostream>
#include <sstream>
#include "onut.h"
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
    onut::StaticPool<9, 7, 11, false> pool;

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
                if (!pool.dealloc(objs[index]))
                {
                    stringstream ss;
                    ss << "Dealloc objs[" << index << "]";
                    checkTest(false, ss.str());
                    return false;
                }
                objs[index] = nullptr;
                --allocCount;
            }
            objs[index] = pool.alloc<CObj>();
            if (allocCount < 7)
            {
                if (objs[index] == nullptr)
                {
                    checkTest(false, "allocCount < 7, so alloc should not be nullptr");
                    return false;
                }
                auto aligned = (reinterpret_cast<uintptr_t>(objs[index]) % 11) == 0;
                if (!aligned)
                {
                    checkTest(aligned, "Alloc is not aligned to 11 bytes");
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
                if (pool.dealloc(objs[index]))
                {
                    checkTest(false, "Dealloc nullptr");
                    return false;
                }
            }
            else
            {
                // Dealloc should succeed
                if (!pool.dealloc(objs[index]))
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
            pool.clear();
            allocCount = 0;
        }
        if (pool.getAllocCount() != allocCount)
        {
            stringstream ss;
            ss << "allocCount " << allocCount << " == pool.getAllocCount() " << pool.getAllocCount();
            checkTest(false, ss.str());
            return false;
        }
    }
    if (pool.getAllocCount() != allocCount)
    {
        stringstream ss;
        ss << "allocCount " << allocCount << " == pool.getAllocCount() " << pool.getAllocCount();
        checkTest(pool.getAllocCount() == allocCount, ss.str());
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

template <typename TsynchronousType>
void runSynchronousTests()
{
    subTest("Basic Tests");
    {
        TsynchronousType synchronous;

        int a = 1;
        float b = 2.5f;

        synchronous.sync([&a, &b]()
        {
            a = 3;
            b = 4.25f;
        });
        checkTest(synchronous.size() == 1, "Lambda call added. Queue size = 1");

        synchronous.processQueue();
        checkTest(a == 3 && b == 4.25f, "processQueue(). Values are 3 and 4.25f");

        checkTest(synchronous.size() == 0, "Queue size = 0");

        synchronous.sync(foo);
        synchronous.sync(foob);
        synchronous.sync(fooab, &a, b);
        checkTest(synchronous.size() == 3, "3 foo functions added. Queue size = 3");

        synchronous.processQueue();
        checkTest(a == 5 && b == 4.25f, "processQueue(). Values are 5 and 4.25f");

        checkTest(synchronous.size() == 0, "Queue size = 0");

        cout << setColor(7) << endl;
    }

    subTest("Test call order");
    {
        TsynchronousType synchronous;
        foo1Called = false;
        foo2Called = false;
        foo3Called = false;
        foo4Called = false;
        foo5Called = false;
        synchronous.sync(foo1);
        synchronous.sync(foo2);
        synchronous.sync(foo3);
        synchronous.sync(foo4);
        synchronous.sync(foo5);
        checkTest(synchronous.size() == 5, "5 foo functions added. Queue size = 5");

        synchronous.processQueue();

        checkTest(synchronous.size() == 0, "Queue size = 0");

        cout << setColor(7) << endl;
    }

    subTest("Threaded test");
    {
        TsynchronousType synchronous;
        foo1Called = false;
        foo2Called = false;
        foo3Called = false;
        foo4Called = false;
        foo5Called = false;

        auto ret = std::async(std::launch::async, [&synchronous]
        {
            synchronous.sync(foo1);
            //std::this_thread::sleep_for(std::chrono::milliseconds(20));
            synchronous.sync(foo2);
            synchronous.sync(foo3);
            //std::this_thread::sleep_for(std::chrono::milliseconds(200));
            synchronous.sync(foo4);
            //std::this_thread::sleep_for(std::chrono::milliseconds(50));
            synchronous.sync(foo5);
            //std::this_thread::sleep_for(std::chrono::milliseconds(450));
        });

        auto startTime = std::chrono::steady_clock::now();
        while (ret.wait_for(std::chrono::seconds(0)) != std::future_status::ready &&
               std::chrono::steady_clock::now() - startTime < std::chrono::seconds(5))
        {
            synchronous.processQueue();
        }
        synchronous.processQueue();

        checkTest(synchronous.size() == 0, "Synchronous didn't stall (< 5seconds)");

        cout << setColor(7) << endl;
    }
}

class TestResource1
{
public:
    template<typename TcontentManagerType>
    static TestResource1* createFromFile(const std::string& filename, TcontentManagerType* pContentManager = nullptr)
    {
        if (filename == "not found") return nullptr;
        return new TestResource1();
    }
    int a = 5;
    float b = 3.25f;
};
class TestResource2
{
public:
    template<typename TcontentManagerType>
    static TestResource2* createFromFile(const std::string& filename, TcontentManagerType* pContentManager = nullptr)
    {
        if (filename == "not found") return nullptr;
        return new TestResource2();
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
        subTest("Basic tests using onut::Pool<16, 10, 5, false>");
        {
            onut::StaticPool<16, 10, 5, false> pool;

            class CObj
            {
            public:
                CObj() : a(1), b(2.5f) {}
                CObj(int _a) : a(_a), b(2.5f) {}
                CObj(int _a, float _b) : a(_a), b(_b) {}
                int a;
                float b;
            };
            auto pObj = pool.alloc<CObj>();
            checkTest(pObj != nullptr, "Alloc CObj()");

            checkTest(pool.getAllocCount() == 1, "Alloc count is 1");

            checkTest(pObj->a == 1 && pObj->b == 2.5f, "CObj values are 1 and 2.5f");

            checkTest(!pool.dealloc(static_cast<CObj*>(nullptr)), "Dealloc nullptr");

            checkTest(pool.dealloc(pObj), "Dealloc");

            checkTest(pool.getAllocCount() == 0, "Alloc count is 0");

            class CTooBigObj
            {
            public:
                float tooBigMember[5];
            };
            auto pObjToBig = pool.alloc<CTooBigObj>();
            checkTest(pObjToBig == nullptr, "Trying alloc obj too big");

            CObj* objs[10] = {nullptr};
            for (int i = 0; i < 10; ++i)
            {
                objs[i] = pool.alloc<CObj>();
            }
            checkTest(
                objs[0] != nullptr && objs[1] != nullptr && objs[2] != nullptr && objs[3] != nullptr && objs[4] != nullptr &&
                objs[5] != nullptr && objs[6] != nullptr && objs[7] != nullptr && objs[8] != nullptr && objs[9] != nullptr,
                "Alloc objs[10]");

            checkTest((reinterpret_cast<uintptr_t>(objs[2]) % 5) == 0, "Checking that objs[2] is aligned to 5 bytes");
            checkTest((reinterpret_cast<uintptr_t>(objs[4]) % 5) == 0, "Checking that objs[4] is aligned to 5 bytes");
            checkTest((reinterpret_cast<uintptr_t>(objs[9]) % 5) == 0, "Checking that objs[9] is aligned to 5 bytes");

            pObj = pool.alloc<CObj>();
            checkTest(pObj == nullptr, "Trying alloc over the max obj");

            checkTest(pool.getAllocCount() == 10, "Alloc count is 10");

            checkTest(pool.dealloc(objs[0]), "Dealloc objs[0]");

            checkTest(pool.getAllocCount() == 9, "Alloc count is 9");

            pObj = pool.alloc<CObj>();
            checkTest(pObj != nullptr, "Alloc CObj()");

            pObj = pool.alloc<CObj>();
            checkTest(pObj == nullptr, "Trying alloc over the max obj again");

            pool.clear();
            checkTest(pool.getAllocCount() == 0, "clear(). Alloc count is 0");

            pObj = pool.alloc<CObj>();
            checkTest(pObj != nullptr, "Alloc CObj()");

            checkTest(pool.getAllocCount() == 1, "Alloc count is 1");

            pObj = pool.alloc<CObj>(7);
            checkTest(pObj != nullptr, "Alloc CObj(7)");

            checkTest(pObj->a == 7 && pObj->b == 2.5f, "CObj values are 7 and 2.5f");

            pObj = pool.alloc<CObj>(4, 3.25f);
            checkTest(pObj != nullptr, "Alloc CObj(4, 3.25f)");

            checkTest(pObj->a == 4 && pObj->b == 3.25f, "CObj values are 4 and 3.25f");

            cout << setColor(7) << endl;
        }

        subTest("Stress test with onut::Pool<9, 7, 11, false> and random alloc/dealloc");
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
        runSynchronousTests<onut::Synchronous<>>();
        cout << setColor(7) << endl;
    }

    majorTest("onut::Synchronous using onut::Pool");
    {
        runSynchronousTests<onut::Synchronous<onut::Pool<>>>();
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
            auto ret = onut::findFile<false>("res1.txt", "../..");
            checkTest(ret == "../../assets/textures/res1.txt", "Search existing file res1.txt");
        }
        {
            auto ret = onut::findFile<false>("someFileThatDoesntExist.txt", "../..");
            checkTest(ret == "", "Search missing file someFileThatDoesntExist.txt");
        }
        {
            auto ret = onut::findFile<false>("res1.txt", "../../assets/textures");
            checkTest(ret == "../../assets/textures/res1.txt", "Search existing from withing res1.txt folder");
        }
        {
            auto ret = onut::findFile<false>("res1.txt", "../..", false);
            checkTest(ret == "", "Search res1.txt without deep search");
        }
        cout << setColor(7) << endl;
    }

    majorTest("onut::ContentManager");
    {

        subTest("Basic tests");
        {
            onut::ContentManager<false> contentManager;

            auto pRes1 = contentManager.getResource<TestResource1>("res1.txt");
            checkTest(pRes1 != nullptr, "Create res1");

            checkTest(contentManager.size() == 1, "Res count = 1");

            auto pRes2 = contentManager.getResource<TestResource2>("res2.txt");
            checkTest(pRes2 != nullptr, "Create res2");

            checkTest(contentManager.size() == 2, "Res count = 2");

            contentManager.clear();
            checkTest(contentManager.size() == 0, "Cleared. Res count = 0");

            pRes1 = contentManager.getResource<TestResource1>("res1.txt");
            pRes2 = contentManager.getResource<TestResource2>("res2.txt");
            checkTest(pRes1 != nullptr && pRes2 != nullptr, "Create res1 and res2");

            checkTest(contentManager.size() == 2, "Res count = 2");

            auto pRes3 = contentManager.getResource<TestResource1>("res1.txt");
            checkTest(pRes3 != nullptr, "Create res1 again");

            checkTest(contentManager.size() == 2, "Res count = 2");

            auto pRes4 = contentManager.getResource<TestResource1>("someFileThatDoesntExist.txt");
            checkTest(pRes4 == nullptr, "Create missing \"someFileThatDoesntExist.txt\" asset");

            cout << setColor(7) << endl;
        }

        subTest("Async tests");
        {
            onut::ContentManager<false> contentManager;

            bool testRet[6] = {false};
            std::atomic<int> completed = 0;

            auto future1 = std::async(std::launch::async, [&]
            {
                testRet[0] = contentManager.getResource<TestResource1>("res1.txt") != nullptr;
                testRet[1] = contentManager.getResource<TestResource2>("someFileThatDoesntExist.txt") == nullptr;
                testRet[2] = contentManager.getResource<TestResource2>("res2.txt") != nullptr;
                ++completed;
            });
            auto future2 = std::async(std::launch::async, [&]
            {
                testRet[3] = contentManager.getResource<TestResource1>("res1.txt") != nullptr;
                testRet[4] = contentManager.getResource<TestResource2>("res2.txt") != nullptr;
                testRet[5] = contentManager.getResource<TestResource2>("someFileThatDoesntExist.txt") == nullptr;
                ++completed;
            });

            extern onut::Synchronous<onut::Pool<>> g_mainSync;
            while (completed < 2)
            {
                g_mainSync.processQueue();
            }

            future1.wait();
            future2.wait();

            checkTest(testRet[0], "Create res1.txt from thread 1");
            checkTest(testRet[1], "Create missing someFileThatDoesntExist.txt from thread 1");
            checkTest(testRet[2], "Create res2.txt from thread 1");
            checkTest(testRet[3], "Create res1.txt from thread 2");
            checkTest(testRet[4], "Create res2.txt from thread 2");
            checkTest(testRet[5], "Create missing someFileThatDoesntExist.txt from thread 2");
            checkTest(contentManager.size() == 2, "Res count = 2");

            cout << setColor(7) << endl;
        }

        cout << setColor(7) << endl;
    }

    majorTest("onut::UINodeNav");
    {
        subTest("Simple directional tests");
        {
            struct V2f
            {
                float x;
                float y;
            };
            onut::UINodeNav<V2f> nav({{0, 0}, {0, -10}, {10, 0}, {0, 10}, {-10, 0}}, .3f, std::chrono::milliseconds(0));

            checkTest(nav.size() == 5, "Has 5 nodes");

            checkTest(nav.getSelectedIndex() == 0, "Default selected index = 0");

            nav.navigate<V2f>({0, -1});
            checkTest(nav.getSelectedIndex() == 1, "Nav up. Selected = 1");

            nav.navigate<V2f>({1.41f, 1.41f});
            checkTest(nav.getSelectedIndex() == 2, "Nav down right. Selected = 2");

            nav.navigate<V2f>({-1.41f, 1.41f});
            checkTest(nav.getSelectedIndex() == 3, "Nav down left. Selected = 3");

            nav.navigate<V2f>({-1.41f, -1.41f});
            checkTest(nav.getSelectedIndex() == 4, "Nav up left. Selected = 4");

            nav.navigate<V2f>({1, 0});
            checkTest(nav.getSelectedIndex() == 0, "Nav right. Selected = 0");

            nav.navigate<V2f>({1, 0});
            checkTest(nav.getSelectedIndex() == 2, "Nav right again. Selected = 2");

            cout << setColor(7) << endl;
        }
        subTest("Test best pick for angle");
        {
            struct V2f
            {
                float x;
                float y;
            };
            onut::UINodeNav<V2f> nav({{0, 0}, {0, -10}, {2, -10}, {-1, -10}}, .3f, std::chrono::milliseconds(0));

            checkTest(nav.size() == 4, "Has 4 nodes");

            nav.navigate<V2f>({1, 0});
            checkTest(nav.getSelectedIndex() == 0, "Nav right. Selected unchanged");

            nav.navigate<V2f>({-1, 0});
            checkTest(nav.getSelectedIndex() == 0, "Nav left. Selected unchanged");

            nav.navigate<V2f>({0, 1});
            checkTest(nav.getSelectedIndex() == 0, "Nav down. Selected unchanged");

            nav.navigate<V2f>({0, -1});
            checkTest(nav.getSelectedIndex() == 1, "Nav up. Selected = 1");

            nav.setSelected(0);
            checkTest(nav.getSelectedIndex() == 0, "Select 0. Selected = 0");

            nav.navigate<V2f>({1.41f, -1.41f});
            checkTest(nav.getSelectedIndex() == 2, "Nav up right. Selected = 2");

            nav.setSelected(0);
            checkTest(nav.getSelectedIndex() == 0, "Select 0. Selected = 0");

            nav.navigate<V2f>({-1.41f, -1.41f});
            checkTest(nav.getSelectedIndex() == 3, "Nav up left. Selected = 3");

            cout << setColor(7) << endl;
        }
        subTest("Test insertion and removal of nodes");
        {
            struct V2f
            {
                float x;
                float y;

                bool operator==(const V2f& other) const
                {
                    return x == other.x && y == other.y;
                }
            };
            onut::UINodeNav<V2f> nav({{0, 0}, {0, -30}, {20, 0}, {0, 10}, {-40, 0}}, .3f, std::chrono::milliseconds(0));

            checkTest(nav.size() == 5, "Has 5 nodes");

            nav.removeNode(0);
            checkTest(nav.getSelectedIndex() == 2, "Remove index 0. Selected = 2");

            nav.setSelected({20, 0});
            checkTest(nav.getSelectedIndex() == 1, "Select by value. Selected = 1");

            nav.removeNode({-40, 0});
            checkTest(nav.getSelectedIndex() == 1, "Removed by value. Selected = 1");

            nav.removeNode(0);
            checkTest(nav.getSelectedIndex() == 0, "Remove index 0. Selected = 0");

            checkTest(nav.size() == 2, "Has 2 nodes");

            nav.removeNode(0);
            checkTest(nav.getSelectedIndex() == 0, "Remove index 0. Selected = 0");

            nav.addNode({0, 0});
            checkTest(nav.size() == 2, "Node added. Has 2 nodes");

            nav.navigate<V2f>({0, -1});
            checkTest(nav.getSelectedIndex() == 1, "Nav up. Selected = 1");

            nav.clear();
            checkTest(nav.size() == 0, "Nodes cleared. Has 0 nodes");

            nav.setNodes({{0, 0}, {-10, 0}});
            checkTest(nav.size() == 2, "Set nodes. Has 2 nodes");

            nav.setSelected(1);
            checkTest(nav.getSelectedIndex() == 1, "Select 1. Selected = 0");

            nav.setNodes({{0, 0}, {0, -10}, {10, 0}, {0, 10}, {-10, 0}});
            checkTest(nav.size() == 5, "Set nodes. Has 5 nodes");

            checkTest(nav.getSelectedIndex() == 0, "Default selected index = 0");

            nav.navigate<V2f>({0, -1});
            checkTest(nav.getSelectedIndex() == 1, "Nav up. Selected = 1");

            nav.navigate<V2f>({1.41f, 1.41f});
            checkTest(nav.getSelected() == V2f{10, 0}, "Nav down right. Selected = {10, 0}");

            nav.navigate<V2f>({-1.41f, 1.41f});
            checkTest(nav.getSelectedIndex() == 3, "Nav down left. Selected = 3");

            nav.navigate<V2f>({-1.41f, -1.41f});
            checkTest(nav.getSelectedIndex() == 4, "Nav up left. Selected = 4");

            nav.navigate<V2f>({1, 0});
            checkTest(nav.getSelectedIndex() == 0, "Nav right. Selected = 0");

            nav.navigate<V2f>({1, 0});
            checkTest(nav.getSelectedIndex() == 2, "Nav right again. Selected = 2");

            cout << setColor(7) << endl;
        }
        cout << setColor(7) << endl;
    }

    majorTest("onut::EventManager");
    {
        subTest("Basic add/remove of events");
        {
            onut::EventManager evMgr;
            bool event1_called = false;
            bool event2_called = false;
            bool condition1 = true;
            bool condition2 = true;

            evMgr.addEvent("Event1", [&condition1, &event1_called] { event1_called = true; return condition1; });
            checkTest(evMgr.size() == 1, "Event1 added. size = 1");

            evMgr.addEvent("Event2", [&condition2, &event2_called] { event2_called = true; return condition2; });
            checkTest(evMgr.size() == 2, "Event2 added. size = 2");

            evMgr.removeEvent("Event1");
            checkTest(evMgr.size() == 1, "Event1 removed. size = 1");

            evMgr.processEvents();
            checkTest(!event1_called && event2_called, "processEvents(). Event1 not called, Event2 called");

            evMgr.clear();
            checkTest(evMgr.size() == 0, "Clearted. size = 0");

            cout << setColor(7) << endl;
        }

        subTest("Single observer");
        {
            onut::EventManager evMgr;
            bool event1_callback = false;
            bool event2_callback = false;
            bool condition1 = true;
            bool condition2 = false;
            {
                onut::EventObserver observer(&evMgr);

                evMgr.addEvent("Event1", [&condition1] { return condition1; });
                checkTest(evMgr.size() == 1, "Event1 added. size = 1");

                observer.observe("Event1", [&event1_callback] { event1_callback = true; });
                evMgr.processEvents();
                checkTest(event1_callback, "Observing Event1. processEvents. Observer called.");
                event1_callback = false;

                evMgr.addEvent("Event2", [&condition2] { return condition2; });
                checkTest(evMgr.size() == 2, "false Event2 added. size = 2");

                observer.observe("Event2", [&event2_callback] { event2_callback = true; });
                evMgr.processEvents();
                checkTest(event1_callback && !event2_callback, "Observing Event2. Event1 called, Event2 not called");
                event1_callback = false;
                event2_callback = false;

                condition2 = true;
                evMgr.processEvents();
                checkTest(event1_callback && event2_callback, "Event2 now true. Both called");
                event1_callback = false;
                event2_callback = false;

                evMgr.removeEvent("Event1");
                checkTest(evMgr.size() == 1, "Event1 removed. size = 1");

                evMgr.processEvents();
                checkTest(!event1_callback && event2_callback, "processEvents. Event1 not called, Event2 called");
                event1_callback = false;
                event2_callback = false;

                evMgr.addEvent("Event1", [&condition1] { return condition1; });
                checkTest(evMgr.size() == 2, "Event1 added. size = 2");

                evMgr.processEvents();
                checkTest(event1_callback && event2_callback, "processEvents. Both called");
                event1_callback = false;
                event2_callback = false;
            }

            evMgr.processEvents();
            checkTest(!event1_callback && !event2_callback, "Observer out of scope. processEvents. None called");

            cout << setColor(7) << endl;
        }

        subTest("Multiple observer");
        {
            onut::EventManager evMgr;
            int event1_callback = 0;
            int event2_callback = 0;
            int event3_callback = 0;
            bool condition1 = true;
            bool condition2 = true;
            bool condition3 = true;
            {
                onut::EventObserver observer1(&evMgr);
                onut::EventObserver observer2(&evMgr);

                evMgr.addEvent("Event1", [&condition1] { return condition1; });
                evMgr.addEvent("Event2", [&condition2] { return condition2; });
                evMgr.addEvent("Event3", [&condition3] { return condition3; });
                checkTest(evMgr.size() == 3, "3 events added. size = 3");

                observer1.observe("Event1", [&event1_callback] { ++event1_callback; });
                observer1.observe("Event2", [&event2_callback] { ++event2_callback; });
                observer2.observe("Event2", [&event2_callback] { ++event2_callback; });
                observer2.observe("Event3", [&event3_callback] { ++event3_callback; });

                evMgr.processEvents();
                checkTest(event1_callback == 1 && event2_callback == 2 && event3_callback == 1, "processEvents. Counts: 1, 2, 1");
                event1_callback = event2_callback = event3_callback = 0;

                observer2.stopObserving("Event2");
                evMgr.processEvents();
                checkTest(event1_callback == 1 && event2_callback == 1 && event3_callback == 1, "observer2 stopObserving \"Event2\". Counts: 1, 1, 1");
                event1_callback = event2_callback = event3_callback = 0;

                observer2.observe("Event3", [&event3_callback] { ++event3_callback; });
                evMgr.processEvents();
                checkTest(event1_callback == 1 && event2_callback == 1 && event3_callback == 2, "observer2 observe \"Event3\". Counts: 1, 1, 2");
                event1_callback = event2_callback = event3_callback = 0;

                observer2.stopObserving("Event3");
                evMgr.processEvents();
                checkTest(event1_callback == 1 && event2_callback == 1 && event3_callback == 0, "observer2 stopObserving \"Event3\". Counts: 1, 1, 0");
                event1_callback = event2_callback = event3_callback = 0;

                observer2.observe("Event2", [&event2_callback] { ++event2_callback; });
                observer2.observe("Event3", [&event3_callback] { ++event3_callback; });
                evMgr.processEvents();
                checkTest(event1_callback == 1 && event2_callback == 2 && event3_callback == 1, "Added back \"Event2-3\". Counts: 1, 2, 1");
                event1_callback = event2_callback = event3_callback = 0;

                condition2 = false;
                evMgr.processEvents();
                checkTest(event1_callback == 1 && event2_callback == 0 && event3_callback == 1, "Event2 is false: Counts: 1, 0, 1");
                event1_callback = event2_callback = event3_callback = 0;

                observer1.stopObservingAll();
                evMgr.processEvents();
                checkTest(event1_callback == 0 && event2_callback == 0 && event3_callback == 1, "Event1 stop all: Counts: 0, 0, 1");
                event1_callback = event2_callback = event3_callback = 0;

                condition2 = true;
                evMgr.processEvents();
                checkTest(event1_callback == 0 && event2_callback == 1 && event3_callback == 1, "Event2 is true again: Counts: 0, 1, 1");
                event1_callback = event2_callback = event3_callback = 0;
            }

            evMgr.processEvents();
            checkTest(!event1_callback && !event2_callback && !event3_callback, "Observers out of scope. processEvents. None called");

            cout << setColor(7) << endl;
        }
        cout << setColor(7) << endl;
    }

    system("pause");
    return errCount;
}
