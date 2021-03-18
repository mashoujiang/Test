#include <stdio.h>
#include <string.h>
class MyStr
{
public:
    MyStr() = default;
    MyStr(char *data)
    {
        auto size = strlen(data);
        if (size == 0)
        {
            m_size = 1;
            m_data = new char[1];
            *m_data = '\0';
        }
        else
        {
            m_size = size;
            m_data = new char[size + 1];
            strcpy(m_data, data);
        }
    }
    MyStr &operator=(const MyStr &str)
    {
        if (this == &str)
        {
            return *this;
        }
        delete[] m_data;
        m_size = str.m_size;
        strcpy(m_data, str.m_data);
        return *this;
    }

    MyStr &operator=(const char *str)
    {
        if (m_data == str)
        {
            return *this;
        }
        delete[] m_data;
        m_size = strlen(str);
        m_data = new char[m_size + 1];
        strcpy(m_data, str);
        return *this;
    }

    MyStr &operator+(const MyStr &str)
    {
        m_size += str.m_size;
        strcat(m_data, str.m_data);
        return *this;
    }

    MyStr &operator+(const char *str)
    {
        m_size += strlen(str);
        strcat(m_data, str);
        return *this;
    }
    void info()
    {
        printf("size: %d, data: %s\n", m_size, m_data);
    }

private:
    char *m_data;
    size_t m_size;
};

int main()
{
    return 0;
}
