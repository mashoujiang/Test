#include <iostream>

template <typename T>
class SharedPtr
{
public:
    SharedPtr(T *p) : m_ptr(p), m_countPtr(new int(1))
    {
    }
    SharedPtr(SharedPtr &sp)
    {
        ++(*sp.m_countPtr);
        m_countPtr = sp.m_countPtr;
        m_ptr = sp.m_ptr;
    }
    ~SharedPtr()
    {
        decrease();
    }
    SharedPtr &operator=(SharedPtr &sPtr)
    {
        if (m_ptr == sPtr.m_ptr)
        {
            return *this;
        }
        decrease();
        m_ptr = sPtr.m_ptr;
        ++(*sPtr.m_countPtr);
        m_countPtr = sPtr.m_countPtr;
        return *this;
    }
    void decrease()
    {
        if (--*m_countPtr == 0)
        {
            delete m_ptr;
            m_ptr = nullptr;
            delete m_countPtr;
            m_countPtr = nullptr;
            std::cout << "deconstructed\n";
        }
    }

    int getCount()
    {
        return *m_countPtr;
    }
    T *data()
    {
        return m_ptr;
    }

private:
    T *m_ptr;
    int *m_countPtr;
};

int main()
{
    SharedPtr<int> iSp(new int(10));
    {
        SharedPtr<int> temp(new int(22));
        temp = iSp;
        SharedPtr<int> one = iSp;
        std::cout << temp.getCount() << std::endl;
        std::cout << iSp.getCount() << std::endl;

        SharedPtr<int> two(iSp);
        std::cout << temp.getCount() << std::endl;
        std::cout << iSp.getCount() << std::endl;
        std::cout << *temp.data() << std::endl;
        std::cout << *iSp.data() << std::endl;
    }
    std::cout << std::endl;
    SharedPtr<int> one = iSp;
    std::cout << iSp.getCount() << std::endl;
    SharedPtr<int> two(iSp);
    std::cout << iSp.getCount() << std::endl;
    std::cout << *iSp.data() << std::endl;
}