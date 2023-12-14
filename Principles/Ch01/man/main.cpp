#include <iostream>
#include <string>

class Man
{
public:
    Man(std::string name = "")
    {
        this->name = name;
        std::cout << "Initialized!" << std::endl;
    }
    ~Man() {}

    void hello()
    {
        std::cout << "Hello " << name << "!" << std::endl;
    }

    void goodbye()
    {
        std::cout << "Good-bye " << name << "!" << std::endl;
    }

private:
        std::string name;
};

int main()
{
    Man m = Man("David");
    m.hello();
    m.goodbye();
}
