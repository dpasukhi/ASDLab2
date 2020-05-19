// runtime_error.cpp
// compile with: /EHsc /GR
#include <iostream>
#include <typeinfo>
#include <string>
#include "include/table.h"

#define SIZE 10

using namespace std;

//template<class Key=string,class Data>
//unsigned StrHash(string _key)
//{
//  unsigned hash = 5381;
//  for (int i = 0; i < _key.length(); i++)
//    hash = ((hash << 5) + hash) + _key[i];
//    return hash;
//}
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
    //#############
    Hash_Table<int, int> G(10);
    G.Add(1, 3);
    G.Add(2, 4);
    cout << "\n" << G.Find(2) << endl;
    G.Delete(1);
    //############
    function<size_t(string)> h = [&](string _key)->size_t
      {
        size_t hash = 5381;
        for (int i = 0; i < _key.length(); i++)
          hash = ((hash << 5) + hash) + _key[i];
        return hash;
      };
    Hash_Table<string, int> S(10);
    S.Add("234", 5);
    S.Add("256", 6);
    cout << "\n" << S.Find("234") << endl;
    S.Delete("256");
    cout << "\n" << S.Find("256") << endl;
    //##############
  }
  catch(std::runtime_error)
  {
    cout << "Not found\n";
  }
  catch (char*)
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