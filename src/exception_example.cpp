// runtime_error.cpp
// compile with: /EHsc /GR
#include <iostream>
#include <typeinfo>
#include <string>
#include "include/table.h"

using namespace std;
class PositiveNumber
{
    int positiveNumber;
public:
    PositiveNumber(int num) :positiveNumber(num)
    {
        if (num < 0)
            throw exception("negative number");
    }
};
int main()
{
  Table<char, int> A(5);
  try
  {
    A.Add('k', 11);
    A.Add('t', 54);
    A.Add('l', 22);
    A.Add('h', 91);
    A.Add('j', 84);
    A.Add('b', 32);
    std::cout << A.Find('t') << std::endl;
    SortTable<char, int> C(A);
    cout << C['k'] << std::endl;
    SortTable<int, int> B(5);
    B.Add(5, 2);
    B.Add(10, 5);
    B.Add(7, 3);
    B.Add(8, 4);
    B.Add(0, 0);
    B.Add(1, 1);
    B.Delete(7);
  }
  catch(std::runtime_error)
  {
    cout << "Not found\n";
  }
    //string a = "1";
    int a = -1;
    auto& ti1 = typeid(a);
    cout << ti1.name() << endl;
    // runtime_error
    try
    {
        PositiveNumber n(a);
    }
    catch (exception &e)
    {
        cerr << "Caught " << e.what() << endl;
        cerr << "Type " << typeid(e).name() << endl;
    };
}
/* Output:
Caught bad locale name
Type class std::runtime_error
*/