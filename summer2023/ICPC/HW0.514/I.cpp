#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
using namespace std;

vector<int> score1;
vector<int> score2;

int main() {
    cin.tie(0);
    ios_base::sync_with_stdio(false);

    int n, x, ans2;
    cin >> n >> x;
    ans2 = 0;
    score1.reserve(n);
    score2.reserve(n);
    for (int i = 0, in; i < n; i++) cin >> in, score1.push_back(in);
    for (int i = 0, in; i < n; i++) cin >> in, score2.push_back(in);
    sort(score1.begin(), score1.begin()+n);
    sort(score2.begin(), score2.begin()+n);
    while (!score1.empty()) {
        if (score1.back() >= x) {
            score1.pop_back();
            score2.erase(score2.begin());
            ans2++;
        }
        else if (score2.back() >= x) {
            score2.pop_back();
            score1.erase(score1.begin());
            ans2++;
        }
        else
            break;
    }

    int idx, d;
    while (!score1.empty()) {
        if (score1.back() + score2.back() < x)
            break;
        if (score1.back() > score2.back()) {
            d = x - score1.back();
            idx = lower_bound(score2.begin(), score2.end(), d) - score2.begin();
            score1.pop_back();
            score2.erase(score2.begin() + idx);
        }
        else {  // score2.back() >= score1.back()
            d = x - score2.back();
            idx = lower_bound(score1.begin(), score1.end(), d) - score1.begin();
            score2.pop_back();
            score1.erase(score1.begin() + idx);
        }
        ans2++;
    }

    cout << "1 " << ans2 << '\n';

    return 0;
}