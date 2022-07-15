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
  //myDB.scanDb();

  return 0;
}