#include <vector>
#include <algorithm>
#include <iostream>

// Function to get the maximum value in the array
int getMax(const std::vector<int>& arr) {
    int max_val = arr[0];
    for (size_t i = 1; i < arr.size(); ++i) {
        if (arr[i] > max_val) {
            max_val = arr[i];
        }
    }
    return max_val;
}

// Counting Sort subroutine for Radix Sort
void countingSort(std::vector<int>& arr, int exp) {
    std::vector<int> output(arr.size());
    std::vector<int> count(10, 0);

    // Count occurrences of each digit (C++98 compatible)
    for (size_t i = 0; i < arr.size(); ++i) {
        int digit = (arr[i] / exp) % 10;
        count[digit]++;
    }

    // Convert to cumulative counts
    for (int i = 1; i < 10; ++i) {
        count[i] += count[i - 1];
    }

    // Build output array (reverse order for stability)
    for (int i = static_cast<int>(arr.size()) - 1; i >= 0; --i) {
        int digit = (arr[i] / exp) % 10;
        output[count[digit] - 1] = arr[i];
        count[digit]--;
    }

    // Copy back to original array
    for (size_t i = 0; i < arr.size(); ++i) {
        arr[i] = output[i];
    }
}

// Radix Sort implementation
void radixSort(std::vector<int>& arr) {
    if (arr.empty()) return;

    int max_val = getMax(arr);
    for (int exp = 1; max_val / exp > 0; exp *= 10) {
        countingSort(arr, exp);
    }
}

// Test function
int main() {
    std::vector<int> arr = {170, 45, 75, 90, 802, 24, 2, 66};
    
    radixSort(arr);
    
    std::cout << "Sorted array: ";
    for (size_t i = 0; i < arr.size(); ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << "\n";
    
    return 0;
}

