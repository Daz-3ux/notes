#include <cstdint>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

// 10进制到62进制
const string chars = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

void reverse(string &s) {
  for(int left = 0, right = s.size()-1; left < right; left++, right--) {
    swap(s[left], s[right]);
  }
}

string encode(int64_t num) {
  string ans;
  while(num > 0) {
    ans += chars[num%62];
    num /= 62;
  } 
  reverse(ans);

  return ans;
}

int64_t decode(string &str) {
  int64_t ans = 0;
  int n = str.size();
  for(int i = 0; i < n; i++) {
    int pos = chars.find(str[i]);
    ans += static_cast<int64_t>(pow(62, n-i-1) * static_cast<int64_t>(pos));
  }

  return ans;
}

int main() {
  int64_t number = 120;
  string enc = encode(number);
  cout << "Encode: " << enc << endl;

  int64_t dec = decode(enc);
  cout << "Decode: " << dec << endl;

  return 0;
}