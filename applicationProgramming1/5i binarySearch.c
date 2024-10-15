#include <stdio.h>

#pragma warning(disable:4996)

int* binary_search(int arr[], int n, int target) {
    int left = 0;
    int right = n - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2; 

        if (arr[mid] == target) {
            return &arr[mid]; 
        }
        else if (arr[mid] < target) {
            left = mid + 1; 
        }
        else {
            right = mid - 1; 
        }
    }

    return NULL; 
}

int main() {
    int arr[] = { 2, 5, 7, 8, 11, 12 };
    int n = sizeof(arr) / sizeof(arr[0]); 
    int target;

    printf("Enter integer value: ");
    scanf("%d", &target);
    printf("\n");

    int* result = binary_search(arr, n, target);

    if (result) {
        printf("The value was found =)\n");
    }
    else {
        printf("The value wasnt found =(\n");
    }

    return 0;
}