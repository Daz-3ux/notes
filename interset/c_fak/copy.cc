#include <iostream>
#include <string>
#include <cstring>

class String
{
private:
    char *m_Buffer;
    unsigned int m_size;
public:
    String(const char *string){
        m_size = strlen(string);
        m_Buffer = new char[m_size+1];
        memcpy(m_Buffer, string, m_size);
        m_Buffer[m_size] = '\0';
    }

    ~String() { delete[] m_Buffer; }

    //String(const String &other) = delete;
    String(const String &other) : m_size(other.m_size)
    {
        std::cout << "Copied String!" << std::endl;
        m_Buffer = new char[m_size+1];
        memcpy(m_Buffer, other.m_Buffer, m_size+1);
    }

    char& operator[](unsigned int index)
    {
        return m_Buffer[index];
    }

    friend std::ostream& operator<<(std::ostream& stream, const String& string);
};

std::ostream& operator<<(std::ostream& stream, const String& string)
{
    stream << string.m_Buffer;
    return stream;
}

void PrintString(const String& string)
{//通过const引用来传递对象
    std::cout << string << std::endl;
}

int main(int argc, char* argv[])
{
    String string = "Daz";
    String second = string;

    second[2] = 'a';

    std::cout << string << std::endl;
    std::cout << second << std::endl;

    std::cin.get();
}