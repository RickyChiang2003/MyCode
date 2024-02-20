void process(int n, int p, int sum, int arr[], int total[]) {
    if (p == n) {
        total[sum]++;
    }
    else {
        process(n, p + 1, sum + arr[p], arr, total);
        process(n, p + 1, sum, arr, total);
    }
}