#include <bits/stdc++.h>
using namespace std;

struct Point {
    int x, y;
    Point(){};
    Point(int _x, int _y) : x(_x), y(_y){};

    Point getLeft() const { return Point(x, y - 1); }
    Point getRight() const { return Point(x, y + 1); }
    Point getUp() const { return Point(x - 1, y); }
    Point getDown() const { return Point(x + 1, y); }
};
ostream &operator<<(ostream &os, const Point &dir) {
    os << dir.x << ' ' << dir.y;
    return os;
}

const int dx[] = {1, 0, -1, 0};
const int dy[] = {0, 1, 0, -1};
const string dir[] = {"D", "R", "U", "L"};

inline bool isValid(int x, int y, int n) {
    return x >= 0 && x < n && y >= 0 && y < n;
}

inline int heuristic(const Point &a, const Point &b) {
    return abs(a.x - b.x) + abs(a.y - b.y); // マンハッタン距離
}

class Node {
  public:
    Point p;
    int g, f; // g: スタートからのコスト, f: g + h (ヒューリスティック関数)
    string path; // ここに移動の方向を保存
    Node(){};
    Node(Point _p, int _g, int _f, string _path)
        : p(_p), g(_g), f(_f), path(_path) {}
};
bool operator>(const Node &lhs, const Node &rhs) { return lhs.f > rhs.f; }
bool operator>(const shared_ptr<Node> &lhs, const shared_ptr<Node> &rhs) {
    return *(lhs) > *(rhs);
}

string AStar(const vector<vector<int>> &grid, const Point &start,
             const Point &end) {
    int n = grid.size();
    vector<vector<bool>> visited(n, vector<bool>(n, false));
    priority_queue<shared_ptr<Node>, vector<shared_ptr<Node>>,
                   greater<shared_ptr<Node>>>
        pq;
    pq.push(make_shared<Node>(Node{start, 0, heuristic(start, end), ""}));

    while(!pq.empty()) {
        auto current = pq.top();
        pq.pop();
        Point p = current->p;
        if(p.x == end.x && p.y == end.y)
            return current->path;

        if(visited[p.x][p.y])
            continue;

        visited[p.x][p.y] = true;

        for(int i = 0; i < 4; ++i) {
            int nx = p.x + dx[i];
            int ny = p.y + dy[i];

            if(!isValid(nx, ny, n) || visited[nx][ny] || grid[nx][ny])
                continue;

            auto newPath = current->path + dir[i];
            Node nxt = Node({nx, ny}, current->g + 1,
                            current->g + 1 + heuristic({nx, ny}, end), newPath);
            auto newNode = make_shared<Node>(nxt);
            pq.push(newNode);
        }
    }
    return "nan"; // パスが見つからなかった場合
}

int main() {
    int n;
    cin >> n;
    vector<vector<int>> G(n, vector<int>(n));
    vector<vector<int>> decided(n, vector<int>(n));
    vector<Point> pos(n * n);
    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < n; ++j) {
            cin >> G[i][j];
            pos[G[i][j]] = Point(i, j);
        }
    }
    auto swapZero = [&](char c, Point &zero) -> void {
        if(c == 'L') {
            swap(pos[0], pos[G[zero.x][zero.y - 1]]);
            swap(G[pos[0].x][pos[0].y], G[zero.x][zero.y + 1]);
        }
        if(c == 'R') {
            swap(pos[0], pos[G[zero.x][zero.y + 1]]);
            swap(G[pos[0].x][pos[0].y], G[zero.x][zero.y - 1]);
        }
        if(c == 'U') {
            swap(pos[0], pos[G[zero.x - 1][zero.y]]);
            swap(G[pos[0].x][pos[0].y], G[zero.x + 1][zero.y]);
        }
        if(c == 'D') {
            swap(pos[0], pos[G[zero.x + 1][zero.y]]);
            swap(G[pos[0].x][pos[0].y], G[zero.x - 1][zero.y]);
        }
    };
    auto moveLeft = [&](Point &t, Point &zero) -> string {
        decided[t.x][t.y] = 1;
        string ret = AStar(decided, zero, t.getLeft());
        decided[t.x][t.y] = 0;
        assert(ret != "nan");
        ret += 'R';
        for(char i : ret)
            swapZero(i, zero);
        return ret;
    };
    auto moveRight = [&](Point &t, Point &zero) -> string {
        decided[t.x][t.y] = 1;
        string ret = AStar(decided, zero, t.getRight());
        decided[t.x][t.y] = 0;
        assert(ret != "nan");
        ret += 'L';
        for(char i : ret)
            swapZero(i, zero);
        return ret;
    };
    auto moveUp = [&](Point &t, Point &zero) -> string {
        decided[t.x][t.y] = 1;
        string ret = AStar(decided, zero, t.getUp());
        decided[t.x][t.y] = 0;
        assert(ret != "nan");
        ret += 'D';
        for(char i : ret)
            swapZero(i, zero);
        return ret;
    };
    auto moveDown = [&](Point &t, Point &zero) -> string {
        decided[t.x][t.y] = 1;
        string ret = AStar(decided, zero, t.getDown());
        decided[t.x][t.y] = 0;
        assert(ret != "nan");
        ret += 'U';
        for(char i : ret)
            swapZero(i, zero);
        return ret;
    };
    auto move = [&](char c, Point &t, Point &zero) -> string {
        if(c == 'L') {
            return moveLeft(t, zero);
        } else if(c == 'R') {
            return moveRight(t, zero);
        } else if(c == 'U') {
            return moveUp(t, zero);
        } else {
            return moveDown(t, zero);
        }
    };
    string ans = "";
    for(int i = 0; i < n - 2; ++i) {
        string path;
        if(!decided[i][i]) {
            path = AStar(decided, pos[i * n + i + 1], Point(i, i));
            for(char k : path) {
                ans += move(k, pos[i * n + i + 1], pos[0]);
            }
            decided[i][i] = 1;
        }
        int j = i + 1, k = i + 1;
        while(j < n - 2 && decided[i][j])
            ++j;
        while(k < n - 2 && decided[k][i])
            ++k;
        while(j < n - 2 || k < n - 2) {
            vector<pair<int, int>> action;
            int right = j;
            for(int l = i; l < min(k + 1, n - 2); ++l) {
                if(right < n - 2) {
                    if((l == 0 || decided[l - 1][right]) &&
                       !decided[l][right]) {
                        action.emplace_back(l, right);
                    }
                }
                right = max(right - 2, i);

                while(right > i && !decided[l + 1][right - 1])
                    --right;
            }
            vector<int> cmp(action.size());
            for(int l = 0; l < action.size(); ++l) {
                cmp[l] = AStar(decided,
                               pos[action[l].first * n + action[l].second + 1],
                               Point(action[l].first, action[l].second))
                             .size();
            }
            int idx =
                distance(cmp.begin(), min_element(cmp.begin(), cmp.end()));
            int x = action[idx].first, y = action[idx].second;
            path = AStar(decided, pos[x * n + y + 1], Point(x, y));
            for(char c : path) {
                ans += move(c, pos[x * n + y + 1], pos[0]);
            }
            decided[x][y] = 1;
            if(x == i)
                ++j;
            if(y == i)
                ++k;
        }

        if(G[i][n - 2] == i * n + n - 1 && G[i][n - 1] == i * n + n) {
            decided[i][n - 2] = 1;
            decided[i][n - 1] = 1;
        } else {
            path = AStar(decided, pos[i * n + n], Point(i, n - 2));
            for(char c : path) {
                ans += move(c, pos[i * n + n], pos[0]);
            }
            decided[i][n - 2] = 1;
            if(G[i][n - 1] == 0 && G[i + 1][n - 1] == i * n + n - 1) {
                string magic = "LDDRUULDRULDRDLURULD";
                for(char c : magic) {
                    ans += c;
                    swapZero(c, pos[0]);
                }
                decided[i][n - 2] = 1;
                decided[i][n - 1] = 1;
            } else if(G[i + 1][n - 2] == 0 && G[i][n - 1] == i * n + n - 1) {
                string magic = "RULDDRUULDRULDRDLURULD";
                for(char c : magic) {
                    ans += c;
                    swapZero(c, pos[0]);
                }
                decided[i][n - 2] = 1;
                decided[i][n - 1] = 1;
            } else {
                path = AStar(decided, pos[i * n + n - 1], Point(i + 1, n - 2));
                for(char c : path) {
                    ans += move(c, pos[i * n + n - 1], pos[0]);
                }
                decided[i + 1][n - 2] = 1;
                decided[i][n - 2] = 0;

                path = AStar(decided, pos[i * n + n], Point(i, n - 1));
                for(char c : path) {
                    ans += move(c, pos[i * n + n], pos[0]);
                }
                decided[i][n - 1] = 1;
                decided[i + 1][n - 2] = 0;
                ans += 'D';
                swapZero('D', pos[0]);
                decided[i][n - 2] = 1;
            }
        }
        if(G[n - 2][i] == n * (n - 2) + i + 1 &&
           G[n - 1][i] == n * (n - 1) + i + 1) {
            decided[n - 2][i] = 1;
            decided[n - 1][i] = 1;
        } else {
            path = AStar(decided, pos[n * (n - 1) + i + 1], Point(n - 2, i));
            for(char c : path) {
                ans += move(c, pos[n * (n - 1) + i + 1], pos[0]);
            }
            decided[n - 2][i] = 1;
            if(G[n - 1][i] == 0 && G[n - 1][i + 1] == n * (n - 2) + i + 1) {
                string magic = "URRDLLURDLURDRULDLUR";
                for(char c : magic) {
                    ans += c;
                    swapZero(c, pos[0]);
                }
                decided[n - 2][i] = 1;
                decided[n - 1][i] = 1;
            } else if(G[n - 1][i] == n * (n - 2) + i + 1 &&
                      G[n - 2][i + 1] == 0) {
                string magic = "DLURRDLLURDLURDRULDLUR";
                for(char c : magic) {
                    ans += c;
                    swapZero(c, pos[0]);
                }
                decided[n - 2][i] = 1;
                decided[n - 1][i] = 1;
            } else {
                path = AStar(decided, pos[n * (n - 2) + i + 1],
                             Point(n - 2, i + 1));
                for(char c : path) {
                    ans += move(c, pos[n * (n - 2) + i + 1], pos[0]);
                }
                decided[n - 2][i + 1] = 1;
                decided[n - 2][i] = 0;

                path =
                    AStar(decided, pos[n * (n - 1) + i + 1], Point(n - 1, i));

                for(char c : path) {
                    ans += move(c, pos[n * (n - 1) + i + 1], pos[0]);
                }
                decided[n - 1][i] = 1;
                decided[n - 2][i + 1] = 0;
                ans += 'R';
                swapZero('R', pos[0]);
                decided[n - 2][i] = 1;
            }
        }
    }

    if(G[n - 2][n - 2] == 0) {
        if(G[n - 1][n - 1] == n * (n - 2) + n - 1) {
            ans += "RDLURD";
            swapZero('R', pos[0]);
            swapZero('D', pos[0]);
            swapZero('L', pos[0]);
            swapZero('U', pos[0]);
            swapZero('R', pos[0]);
            swapZero('D', pos[0]);
        } else if(G[n - 1][n - 2] == n * (n - 1) + n - 1) {
            ans += "RD";
            swapZero('R', pos[0]);
            swapZero('D', pos[0]);
        } else {
            ans += "DR";
            swapZero('D', pos[0]);
            swapZero('R', pos[0]);
        }
    }

    string pri = "";
    for(char i : ans) {
        if(!pri.empty() && i == 'L' && pri.back() == 'R') {
            pri.pop_back();
        } else if(!pri.empty() && i == 'R' && pri.back() == 'L') {
            pri.pop_back();
        } else if(!pri.empty() && i == 'U' && pri.back() == 'D') {
            pri.pop_back();
        } else if(!pri.empty() && i == 'D' && pri.back() == 'U') {
            pri.pop_back();
        } else {
            pri.push_back(i);
        }
    }
    cout << pri;
    return 0;
}