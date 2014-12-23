#include <iostream>
#include <iomanip>
#include <sstream>
#include "onut.h"
using namespace std;

#ifdef WIN32
HANDLE hcon;
#endif

struct IOSetColor {
    int color;
};
ostream& operator <<(ostream& os, const IOSetColor& color)
{
    #ifdef WIN32
        SetConsoleTextAttribute(hcon, color.color);
    #endif
    return os;
}

IOSetColor setColor(int color) { 
    return{ color };
}

int errCount = 0;

int majorTestCount = 0;
int subTestCount = 0;
int testCount = 0;

template<typename TtextType>
void majorTest(TtextType testName) {
    ++majorTestCount;
    subTestCount = 0;
    testCount = 0;
    cout << setColor(7) << setw(2) << majorTestCount << " - " << testName << endl << endl;
}

template<typename TtextType>
void subTest(TtextType testName) {
    ++subTestCount;
    testCount = 0;
    stringstream ss;
    ss << majorTestCount << "." << subTestCount;
    cout << setColor(7) << setw(6) << ss.str() << " - " << testName << endl << endl;
}

template<typename TtextType>
void checkTest(bool cond, TtextType testName) {
    ++testCount;
    stringstream ss;
    ss << majorTestCount << "." << subTestCount << "." << testCount;
    if (cond) {
        cout << setColor(7) << setw(10) << ss.str() << " - " << setColor(10) << "PASSED " << setColor(7) << testName << endl;
    }
    else {
        ++errCount;
        cout << setColor(7) << setw(10) << ss.str() << " - " << setColor(12) << "FAILED " << setColor(7) << testName << endl;
    }
}

template<uintptr_t TloopCount>
bool testRandomPool() {
    onut::Pool<9, 7, 11, false> pool;

    class CObj {
    public:
        CObj() : a(1), b(2.5f) {}
        CObj(int _a) : a(_a), b(2.5f) {}
        CObj(int _a, float _b) : a(_a), b(_b) {}
        int a;
        float b;
    };

    int allocCount = 0;
    decltype(TloopCount) i = 0;
    CObj* objs[7] = { nullptr };
    for (; i < TloopCount; ++i) {
        auto rnd = rand() % 2;
        if (rnd == 0) {
            auto index = rand() % 7;
            objs[index] = pool.alloc<CObj>();
            if (allocCount < 7) {
                if (objs[index] == nullptr) {
                    checkTest(false, "allocCount < 7, so alloc should not be nullptr");
                    return false;
                }
                auto aligned = (reinterpret_cast<uintptr_t>(objs[2]) % 11) == 0;
                if (!aligned) {
                    checkTest(aligned, "Alloc is not aligned to 11 bytes");
                    return false;
                }
                ++allocCount;
            }
            else {
                if (objs[index] != nullptr) {
                    checkTest(false, "allocCount is max, so alloc should be nullptr");
                    return false;
                }
            }
        }
        else if (rnd == 1) {
            auto index = rand() % 7;
            if (objs[index] == nullptr) {
                // Dealloc should fail
                if (pool.dealloc(objs[index])) {
                    checkTest(false, "Dealloc nullptr");
                    return false;
                }
            }
            else {
                // Dealloc should succeed
                if (!pool.dealloc(objs[index])) {
                    stringstream ss;
                    ss << "Dealloc objs[" << index << "]";
                    checkTest(false, ss.str());
                    return false;
                }
                objs[index] = nullptr;
                --allocCount;
            }
        }
        if (pool.getAllocCount() != allocCount) {
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

void foo() {
}

bool foodb() {
    return true;
}

void foo(int& a, float b) {
    a = 5;
    b = 12.75f;
}

int main() {
#ifdef WIN32
    hcon = GetStdHandle(STD_OUTPUT_HANDLE);
#endif

    majorTest("onut::Pool");
    {
        subTest("Basic tests using onut::Pool<16, 10, 5, false>");
        {
            onut::Pool<16, 10, 5, false> pool;

            class CObj {
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

            class CTooBigObj {
            public:
                float tooBigMember[5];
            };
            auto pObjToBig = pool.alloc<CTooBigObj>();
            checkTest(pObjToBig == nullptr, "Trying alloc obj too big");

            CObj* objs[10] = { nullptr };
            for (int i = 0; i < 10; ++i) {
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

        subTest("Stress test with onut::Pool<9, 7, 10, false> and random alloc/dealloc");
        {
            srand(static_cast<unsigned int>(time(0)));

            srand(0);
            checkTest(testRandomPool<10000>(), "seed = 0");

            srand(5632);
            checkTest(testRandomPool<10000>(), "seed = 5632");

            for (int i = 0; i < 5; ++i) {
                srand(static_cast<unsigned int>(time(0)));
                checkTest(testRandomPool<10000>(), "seed = random");
            }

            cout << setColor(7) << endl;
        }
        cout << setColor(7) << endl;
    }

    majorTest("onut::Synchronous");
    {
        subTest("Basic Tests");
        {
            onut::Synchronous<> synchronous;

            int a = 1;
            float b = 2.5f;

            synchronous.sync([&a, &b](){
                a = 3;
                b = 4.25f;
            });
            checkTest(synchronous.size() == 1, "Lambda call added. Queue size = 1");

            synchronous.processQueue();
            checkTest(a == 3 && b == 4.25f, "processQueue(). Values are 3 and 4.25f");

            checkTest(synchronous.size() == 0, "Queue size = 0");

            cout << setColor(7) << endl;
        }
        cout << setColor(7) << endl;
    }

    system("pause");
    return errCount;
}
