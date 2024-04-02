#include <bits/stdc++.h>
using namespace std;

int main() {
    int n;
    cin >> n;
    vector<vector<int>> G(n, vector<int>(n));
    int x = 0, y = 0;
    for(int i = 0; i < n; ++i)
        for(int j = 0; j < n; ++j) {
            cin >> G[i][j];
            if(!G[i][j]) {
                x = i;
                y = j;
            }
        }
    auto getPath = [](int x1, int y1, int x2, int y2) -> string {
        string ret = "";
        for(int i = x1; i < x2; ++i)
            ret += 'R';
        for(int i = y1; i < y2; ++i)
            ret += 'D';
        for(int i = x2; i < x1; ++i)
            ret += 'L';
        for(int i = y2; i < y1; ++i)
            ret += 'U';
        return ret;
    };
    auto moveLeft = [](int r, int c, int x1, int y1) -> string {
        string ret = "";
        if(x1 < r && c == y1) {
            ret += 'R';
            ++y1;
        }
        if(y1 > c) {
            for(int i = x1; i < r + 1; ++i)
                ret += 'D';
            x1 = r + 1;
            for(int i = c - 1; i < y1; ++i)
                ret += 'L';
            y1 = c - 1;
        } else {
            for(int i = x1; i < r; ++i)
                ret += 'D';
            x1 = r;
            for(int i = y1; i < c - 1; ++i)
                ret += 'R';
            y1 = c - 1;
        }
        for(int i = r; i < x1; ++i)
            ret += 'U';
        ret += 'R';
        return ret;
    };
    auto moveRight = [](int r, int c, int x1, int y1) -> string {
        string ret = "";
        if(x1 > r && y1 <= c) {
            for(int i = y1; i < c + 1; ++i)
                ret += 'R';
            y1 = c + 1;
            for(int i = x1; i < r; ++i)
                ret += 'D';
            x1 = r;
        } else {
            if(y1 < c) {
                for(int i = x1; i < r + 1; ++i)
                    ret += 'D';
                x1 = r + 1;
                for(int i = y1; i < c + 1; ++i)
                    ret += 'R';
                y1 = c + 1;
            } else {
                for(int i = x1; i < r; ++i)
                    ret += 'D';
                x1 = r;
                for(int i = c + 1; i < y1; ++i)
                    ret += 'L';
                y1 = c + 1;
            }
        }
        for(int i = r; i < x1; ++i)
            ret += 'U';
        ret += 'L';
        return ret;
    };
    auto moveUp = [](int r, int c, int x1, int y1) -> string {
        string ret = "";
        if(x1 < r) {
            for(int i = x1; i < r - 1; ++i)
                ret += 'D';
            x1 = r - 1;
            for(int i = y1; i < c; ++i)
                ret += 'R';
            for(int i = c; i < y1; ++i)
                ret += 'L';
            y1 = c;
        } else {
            if(y1 > c) {
                for(int i = r - 1; i < x1; ++i)
                    ret += 'U';
                x1 = r - 1;
                for(int i = c; i < y1; ++i)
                    ret += 'L';
                y1 = c;
            } else {
                for(int i = y1; i < c + 1; ++i)
                    ret += 'R';
                y1 = c + 1;
                for(int i = r - 1; i < x1; ++i)
                    ret += 'U';
                x1 = r - 1;
                for(int i = c; i < y1; ++i)
                    ret += 'L';
                y1 = c;
            }
        }
        ret += 'D';
        return ret;
    };
    auto moveDown = [](int r, int c, int x1, int y1) -> string {
        string ret = "";
        if(x1 > r) {
            for(int i = r + 1; i < x1; ++i)
                ret += 'U';
            x1 = r + 1;
            for(int i = y1; i < c; ++i)
                ret += 'R';
            for(int i = c; i < y1; ++i)
                ret += 'L';
            y1 = c;
        } else {
            if(y1 < c) {
                for(int i = x1; i < r + 1; ++i)
                    ret += 'D';
                x1 = r + 1;
                for(int i = y1; i < c; ++i)
                    ret += 'R';
                y1 = c;
            } else {
                for(int i = y1; i < c + 1; ++i)
                    ret += 'R';
                y1 = c + 1;
                for(int i = x1; i < r + 1; ++i)
                    ret += 'D';
                x1 = r + 1;
                for(int i = c; i < y1; ++i)
                    ret += 'L';
                y1 = c;
            }
        }
        ret += 'U';
        return ret;
    };

    return 0;
}