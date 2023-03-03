#include <iostream>
//#include <map>
#include <unordered_map>
#include <vector>

using namespace std;

int main(int argc, char **argv)
{
  vector<int> nums{1,2,3,4,5,6};
  unordered_map<int, int> counter;
  for(int num: nums) {
    counter[num]++;
  }

  for(auto &it : counter) {
    int key = it.first;
    int value = it.second;
    cout << key << ": " << value << endl;
  }

  cout << "non-exist key: " << counter[7] << endl;
  

}