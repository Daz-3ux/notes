# intro
- 本文是对level的用法的简单举例
- 只涉及最基础的接口用法

# 基础接口
## CODE
```cpp
#include <iostream>
#include <assert.h>
#include <leveldb/options.h>
#include "leveldb/db.h"
#include "leveldb/write_batch.h"

int main(int argc, char **argv)
{
  // open a database
  leveldb::DB *db;                // 用来返回数据库实例
  leveldb::Options opts;          // 数据库控制参数
  opts.create_if_missing = true;  // 数据库存在就直接打开，不存在就创建一个
  leveldb::Status status = leveldb::DB::Open(opts, "./testdb", &db);
  assert(status.ok());

  // write data
  status = db->Put(leveldb::WriteOptions(), "name", "daz");
  assert(status.ok());

  // read data
  std::string val;
  status = db->Get(leveldb::ReadOptions(), "name", &val);
  assert(status.ok());
  std::cout << val << std::endl;
  
  // batch atomic write 批量原子写
  leveldb::WriteBatch batch;
  batch.Delete("name");
  batch.Put("name0","daz0");
  batch.Put("name1","daz1");
  batch.Put("name2","daz2");
  batch.Put("name3","daz3");
  batch.Put("name4","daz4");
  batch.Put("name5","daz5");
  batch.Put("name6","daz6");
  status = db->Write(leveldb::WriteOptions(), &batch);
  assert(status.ok());

  // Scan database
  leveldb::Iterator *it = db->NewIterator(leveldb::ReadOptions());
  for(it->SeekToFirst(); it->Valid(); it->Next()) {
    std::cout << it->key().ToString() << ": " << it->value().ToString() << std::endl;
  }
  assert(it->status().ok());
  // range Scan
  for(it->Seek("name3"); it->Valid() && it->key().ToString() < "name7"; it->Next()) {
    std::cout << it->key().ToString() << ": " << it->value().ToString() << std::endl;
  }

  // close a database
  delete it; // 不删除的话可能会造成内存泄漏,从而产生报错
  delete db;
}
```

## 常见错误
```
sl: /build/leveldb/src/leveldb-1.23/db/version_set.cc:755: leveldb::VersionSet::~VersionSet(): Assertion `dummy_versions_.next_ == &dummy_versions_' failed.
```
- 错误原因:忽略了迭代器的删除,可能会造成内存泄漏,所以程序产生报错
- 解决方法:
```cpp
delete it;
```

# 包装为一个简易的类
## code
```cpp
#include <iostream>
#include <assert.h>
#include "leveldb/options.h"
#include "leveldb/db.h"
#include "leveldb/write_batch.h"

class myLevelDb 
{
private:
  leveldb::DB *db;
  leveldb::Options opts;
  leveldb::Status status;

public:
  myLevelDb () { opts.create_if_missing = false; }
  ~myLevelDb () { delete db;}

  // open a database
  void openDb(bool a, std::string dbName) {
    opts.create_if_missing = a;
    status = leveldb::DB::Open(opts, dbName, &db);
    assert(status.ok());
  }

  // write database
  void writeDb(std::string name, std::string value) {
    status = db->Put(leveldb::WriteOptions(), name, value);
    assert(status.ok());
  }
  // read database
  void getValue(std::string name) {
    std::string val;
    status = db->Get(leveldb::ReadOptions(), name, &val);
    assert(status.ok());
    std::cout << val << std::endl;
  }

  // delete value
  void deleteValue (std::string name) {
    status = db->Delete(leveldb::WriteOptions(), name);
    assert(status.ok());
  }

  // scan database
  void scanDb () {
    leveldb::Iterator *it = db->NewIterator(leveldb::ReadOptions());
    for(it->SeekToFirst(); it->Valid(); it->Next()) {
      std::cout << it->key().ToString() << ": " << it->value().ToString() << std::endl;
    }
    assert(it->status().ok());
    delete it;
  }

  void scanDbRange(std::string name_first, std::string name_last) {
    leveldb::Iterator *it = db->NewIterator(leveldb::ReadOptions());
    for (it->Seek(name_first); it->Valid() && it->key().ToString() <= name_last; it->Next())
    {
      std::cout << it->key().ToString() << ": " << it->value().ToString() << std::endl;
    }
    delete  it;
  }
};

int main(int argc, char **argv)
{
  myLevelDb myDB;
  myDB.openDb(true, "./test2");
  myDB.writeDb(std::string("daz"), "180");
  myDB.getValue("daz");
  myDB.scanDb();
  myDB.deleteValue("daz");
  myDB.scanDb();

  return 0;
}
```

