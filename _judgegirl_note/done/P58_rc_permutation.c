# define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>


void bbsort(int arr[], int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (arr[j] > arr[j + 1]) {
                int tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
}

void f(int index, int arr[], int used[], int tmp[], int n) {
    if (index == n) {
        for (int i = 0; i < n; i++)
            printf("%d%c", tmp[i], (i == n - 1) ? ' ' : '\n');
    }
    else {
        for (int i = 0; i < n; i++) {
            if (used[i] == 0) {
                used[i] = 1;
                tmp[index] = arr[i];
                f(index + 1, arr, used, tmp, n);
                used[i] = 0;
            }
        }
    }
}

// 先將一層層f中的arr設置為used = 1, 然後在最後一個(index = n - 1) 的f設置完後, 便會print一次
// , 然後將最後一個used變為0, 然後接著往回跑index = n - 2時的used = 0, 然後就又會進入(index = n - 1) 的子問題
// , 因為else 中的for迴圈會把 i++;
// 然後index = n - 2執行完就會進index = n - 3, 一路做完

int main() {
    int n;
    scanf("%d", &n);
    int arr[11], used[11] = { 0 }, tmp[11] = { 0 };
    for (int i = 0; i < n; i++) scanf("%d", &(arr[i]));

    bbsort(arr, n);

    f(0, arr, used, tmp, n);


    return 0;
}
