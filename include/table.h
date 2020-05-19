
#pragma once
#include <exception>
#include <iostream>
#include <algorithm>
#include <ctime>
#include <functional>
#include <string>
#include <stdlib.h>
#include <vector>
#include <list>
#include <optional>
template<class Key, class Data>
class Record {
public:
    Key key;
    Data data;
    Record() {};
    Record(Key _key, Data _data) :key(_key), data(_data) {};
    bool operator ==(const Key&);
    bool operator !=(const Key&);
   // friend std::ostream& operator<<(std::ostream& os, const Record<Key, Data>& d);
};

template<class Key, class Data>
inline bool Record<Key, Data>::operator!=(const Key& _key)
{
    return key != _key;
}

template<class Key, class Data>
inline bool Record<Key, Data>::operator==(const Key& _key)
{
    return key == _key;
}

template<class Key, class Data>
class Table {
protected:
    int size;
    int count;
    Record<Key, Data>* dataArray;

public:
  Table(int _size) : size(_size), count(0)
  {
      dataArray = new Record<Key, Data>[size];
  }
  void Add(Key key, Data data) {
      dataArray[count].key = key;
      dataArray[count].data = data;
      ++count;
  }
  Data Find(Key key) {
    for (int i = 0; i < count; ++i)
        if (dataArray[i].key == key)
            return dataArray[i].data;
    throw std::exception("Not found");
  }
  template<class _Key, class _Data>
  friend class SortTable;
  friend std::ostream& operator<<(std::ostream& os, const Record<Key, Data>& d);
};
template<class Key, class Data>
void InsertSort(Record<Key, Data>* data,size_t size)
{
  for (int startIndex = 0; startIndex < size - 1; ++startIndex)
  {
    int smallestIndex = startIndex;

    for (int currentIndex = startIndex + 1; currentIndex < size; ++currentIndex)
    {
      if (data[currentIndex].key < data[smallestIndex].key)
        smallestIndex = currentIndex;
    }
    std::swap(data[startIndex], data[smallestIndex]);
  }
}
template<class Key, class Data>
class SortTable : public Table<Key, Data >
{
public:
    SortTable(size_t _size);
    SortTable(const SortTable<Key, Data>& table);
    SortTable(const Table<Key, Data>& table):Table<Key,Data>::Table(table.size)
    {
      this->count = table.count;
      for (int i = 0; i < this->count; ++i)
      {
        this->dataArray[i] = table.dataArray[i];
      }
      size_t start_time = clock();
      //std::sort(this->dataArray, this->dataArray + this->count, [](Record<Key, Data>a, Record<Key, Data>b)->bool {return a.key < b.key; });
      InsertSort(this->dataArray, this->count);
      size_t end_time = clock();
      size_t search_time = end_time - start_time;
      std::cout << "Time:" << search_time;
    }
    void Add(Key key, Data data);
    Data Find(Key key);
    void Delete(const Key& key);
    Data operator[](const Key& key);
};

template<class Key, class Data>
SortTable<Key, Data>::SortTable(size_t _size) : Table<Key, Data>::Table(_size) { };

template<class Key, class Data>
SortTable<Key, Data>::SortTable(const SortTable<Key, Data>& table) {
    this->size = table.size;
    this->count = table.count;
    this->dataArray = new Record<Key, Data>[this->size];
    for (int i = 0; i < this->count; ++i) 
    {
      this->dataArray[i] = table.dataArray[i];
    }
}

template<class Key, class Data>
void SortTable<Key, Data>::Add(Key key, Data data) {
  int l = - 1, r = this->count;
  while (l  < r - 1) {
    int mid = (l + r) / 2;
    if (this->dataArray[mid].key < key) l = mid;
    else r = mid;
  }
  for (int j = this->count; j > r; j--)
  {
    this->dataArray[j].key = this->dataArray[j - 1].key;
    this->dataArray[j].data = this->dataArray[j - 1].data;
  }
  this->dataArray[r].key = key;
  this->dataArray[r].data = data;
  ++this->count;
}

template<class Key, class Data>
Data SortTable<Key, Data>::Find(Key key) {
  int l = -1, r = this->count;
  while (l < r - 1) {
    int mid = (l + r) / 2;
    if (this->dataArray[mid].key < key) l = mid;
    else r = mid;
  }
  if (this->dataArray[r].key == key) return this->dataArray[r].data;
  throw std::runtime_error("Not Found");
}
template<class Key, class Data>
inline void SortTable<Key, Data>::Delete(const Key& key)
{
  int l = -1, r = this->count;
  while (l < r - 1) {
    int mid = (l + r) / 2;
    if (this->dataArray[mid].key < key) l = mid;
    else r = mid;
  }
  if(l>r)
    throw std::runtime_error("Not Found");
  for (int j = r; j < this->count-1; j++)
  {
    this->dataArray[j].key = this->dataArray[j + 1].key;
    this->dataArray[j].data = this->dataArray[j + 1].data;
  }
  this->count--;
}
template<class Key, class Data>
inline Data SortTable<Key, Data>::operator[](const Key& key)
{
  return this->Find(key);
}

template<class Key, class Data>
std::ostream& operator<<(std::ostream& os, const Record<Key, Data>& d)
{
  os << "|" << d.key << "| " << d.data << "|\n";
  return os;
}
//#################################################################################################

template<class Key, class Data>
class Hash_Table
{
private:
  using HashFunc = std::function<size_t(Key)>;
  int size;
  int count;
  HashFunc myHashFunc;
  std::list<Record<Key, Data>>* dataArray;
public:
  Hash_Table(int _size = 5);
  Hash_Table(HashFunc  hFunc,int _size = 5) :size(_size), myHashFunc(hFunc) {}
  void Add(Key key, Data data);
  Data Find(Key key);
  virtual void Delete(const Key& key);
  virtual Data operator[](const Key& key);
};

template<class Key, class Data>
inline Hash_Table<Key, Data>::Hash_Table(int _size) : size(_size)
{
  this->dataArray = new std::list < Record<Key, Data>>[_size];
    this->myHashFunc = [&](Key _key)
    {
      return ((15 * _key + 8) % (this->size + 6));
    };
}

template<>
inline Hash_Table<std::string, std::string>::Hash_Table(int _size) : size(_size)
{
  this->dataArray = new std::list < Record<std::string, std::string>>[_size];
    this->myHashFunc = [&](std::string _key)
    {
      size_t hash = 5381;
      for (int i = 0; i < _key.length(); i++)
        hash = ((hash << 5) + hash) + _key[i];
      return hash;
    };
}

template<>
inline Hash_Table<std::string, int>::Hash_Table(int _size) : size(_size)
{
  this->dataArray = new std::list < Record<std::string, int>>[_size];
  this->myHashFunc = [&](std::string _key)
  {
    size_t hash = 5381;
    for (int i = 0; i < _key.length(); i++)
      hash = ((hash << 5) + hash) + _key[i];
    return hash;
  };
}

template<>
inline Hash_Table<std::string, double>::Hash_Table(int _size) : size(_size)
{
  this->dataArray = new std::list < Record<std::string, double>>[_size];
  this->myHashFunc = [&](std::string _key)
  {
    size_t hash = 5381;
    for (int i = 0; i < _key.length(); i++)
      hash = ((hash << 5) + hash) + _key[i];
    return hash;
  };
}

template<class Key, class Data>
inline void Hash_Table<Key, Data>::Add(Key key, Data data)
{
  Record<Key, Data> rec;
  rec.data = data; rec.key = key;
  size_t _key =  this->myHashFunc(key)%this->size;
  size_t  counter = 0;
  this->dataArray[_key].push_back(rec);
}

template<class Key, class Data>
inline Data Hash_Table<Key, Data>::Find(Key key)
{
  size_t  _key = this->myHashFunc(key) % this->size;
  Data data;
  bool isFound = false;
  for (const auto& ent : this->dataArray[_key])
  {
    if (ent.key == key)
    {
      isFound = true;
      data = ent.data;
    }
  }
  if (!isFound)
  {
    throw "Find_Error";
  }
  return data;
}

template<class Key, class Data>
inline void Hash_Table<Key, Data>::Delete(const Key& key)
{
  size_t _key = this->myHashFunc(key) % this->size;
  bool isFound = false;
  for (auto i=this->dataArray[_key].begin();i != this->dataArray[_key].end();i++)
  {
    if (i->key == key)
    {
      this->dataArray[_key].erase(i);
      isFound = true;
      break;
    }
  }
  if (!isFound)
  {
    throw "Find_Error";
  }
}

template<class Key, class Data>
inline Data Hash_Table<Key, Data>::operator[](const Key& key)
{
  return this->Find(key);
}
