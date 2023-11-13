#include <iostream>
#include <vector>

using namespace std;

vector<int> productExceptSelf(vector<int>& nums) {
  int size = nums.size();
  vector<int> ans;

  for(int i = 0; i < size; i++) {
    int left = i-1, right = i+1;
    int sum = 1;
    while(left >= 0) {
      sum *= nums[left--];
    }
    while(right < size) {
      sum *= nums[right++];
    }
    ans.emplace_back(sum);
  }

  return ans;
}

int main() {
  vector<int> nums{1,2,3,4};
  vector<int> ans = productExceptSelf(nums);
  for(auto num : ans) {
    cout << num << endl;
  }
}