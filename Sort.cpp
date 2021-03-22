#include <iostream>
#include <iterator>
#include <vector>

int partition(std::vector<int> &nums, int left, int right)
{
    int flag = nums[right];
    int i = left;
    for (int j = left; j < right; ++j)
    {
        if (nums[j] < flag)
        {
            // swap i, j;
            int temp = nums[i];
            nums[i] = nums[j];
            nums[j] = temp;
            ++i;
        }
    }
    int temp = nums[i];
    nums[i] = nums[right];
    nums[right] = temp;
    return i;
}

void _quickSort(std::vector<int> &nums, int left, int right)
{
    if (left >= right)
    {
        return;
    }
    int flag = partition(nums, left, right);
    _quickSort(nums, left, flag - 1);
    _quickSort(nums, flag + 1, right);
}

void quickSort(std::vector<int> &nums)
{
    _quickSort(nums, 0, nums.size() - 1);
}

void merge(std::vector<int> &nums, int left, int mid, int right)
{
    int p1 = 0;
    int p2 = mid - left + 1;
    int len1 = mid - left + 1;
    int len2 = right - left + 1;
    std::vector<int> copyV{nums.begin() + left, nums.begin() + right + 1};
    int count = left;
    while (p1 < len1 && p2 < len2)
    {
        if (copyV[p1] < copyV[p2])
        {
            nums[count++] = copyV[p1++];
        }
        else
        {
            nums[count++] = copyV[p2++];
        }
    }
    while (p1 < len1)
    {
        nums[count++] = copyV[p1++];
    }
    while (p2 < len2)
    {
        nums[count++] = copyV[p2++];
    }
}

void _mergeSort(std::vector<int> &nums, int left, int right)
{
    if (left >= right)
    {
        return;
    }
    int mid = (left + right) / 2;
    _mergeSort(nums, left, mid);
    _mergeSort(nums, mid + 1, right);
    merge(nums, left, mid, right);
}

void mergeSort(std::vector<int> &nums)
{
    _mergeSort(nums, 0, nums.size() - 1);
}

int main()
{
    std::vector<int> nums{1, 6, 3, 3, 5, 4, 8, 2, 0, 6};
    quickSort(nums);
    std::copy(nums.begin(), nums.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;

    std::vector<int> nums1{1, 6, 3, 5, 4, 8, 2};
    mergeSort(nums1);
    std::copy(nums1.begin(), nums1.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;
}