#include<iostream>
#include<vector>

using namespace std;


class Solution
{
public:
	vector<int> exchange(vector<int>&nums)
	{
		int i = 0;
		int j = nums.size()-1;
		while (i < j)
		{
			while (i < j && nums[i]%2==1)
			{
				i++;
			}
			while (j > i && nums[j]%2==0)
			{
				j--;
			}
			int temp = nums[i];
			nums[i] = nums[j];
			nums[j] = temp;
		}
		return nums;
	}
};

int main()
{
	int n;
	vector<int> nums;
	while (cin >> n)
	{
		nums.push_back(n);
		cout << n <<" ";
	}
	Solution s;
	s.exchange(nums);
	return 0;
}

