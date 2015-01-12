#include <functional>
#include <iostream>
#include <cstring>

class MyClass
{
public:

    bool Allowed ();

    template <typename T>
    inline int applyFunc (T const &func)
    {
        if (Allowed ()) {
            return func (i);
        }
        return 0;
    }

    MyClass (int i, bool allowed) :
        i (i),
        allowed (allowed)
    {
    }

private:

    int i;
    bool allowed;
};

bool
MyClass::Allowed ()
{
    return allowed;
}

int main (int argc, char **argv)
{
    MyClass mc (10, strcmp (argv[1], "allowed") == 0);
    int i = mc.applyFunc ([](int i) -> int {
        return i + 10;
    });
    return i;
}

