#include "StrBlob.h"

int main(int argc, char **argv)
{
    StrBlob sb{"hello", "world"};
    const StrBlob csb{"hello", "world", "world", "sj"};

    std::cout << "sb : " << sb.front() << " " << sb.back() <<  std::endl;
    std::cout << "csb : " << csb.front() << " " << csb.back() << std::endl;

    return 0;
}

