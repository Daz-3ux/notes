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
  delete db;

}