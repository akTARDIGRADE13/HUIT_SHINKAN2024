#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace std;
using namespace __gnu_pbds;

unsigned long long rng() {
    static unsigned long long rx = 123456789, ry = 362436069, rz = 521288629,
                              rw = 88675123;
    unsigned long long rt = (rx ^ (rx << 11));
    rx = ry;
    ry = rz;
    rz = rw;
    return (rw = (rw ^ (rw >> 19)) ^ (rt ^ (rt >> 8)));
}

long long randint(long long l, long long r) {
    assert(l < r);
    return l + rng() % (r - l);
}

double rnd() { return rng() * 5.42101086242752217004e-20; }

int dx[4] = {1, 0, -1, 0}, dy[4] = {0, 1, 0, -1};

unsigned long long zob[10000];

int n;

const int BAN_COUNT_KEISU = 0;

class Node {
  private:
    int correct;
    int under_limit;
    int left_limit;
    int width;

    inline int calcManhattanDist(int r, int c) const {
        int num = this->board[r * n + c] - 1;
        if(num == -1)
            return 0;
        return (abs(num / n - r) + abs(num % n - c));
    }

    inline int calcZobIdx(int r, int c) const {
        return this->board[r * n + c] * n * n + r * n + c;
    }

  public:
    int zr;
    int zc;
    int score = 0;
    int turn = 0;
    int ban_count = 0;
    unsigned long long hash;
    std::shared_ptr<Node> parent = nullptr;
    int current_act = -1;
    vector<int> board;

    Node() {}
    Node(int ul, int ll, int wi, const vector<int> &G)
        : under_limit(ul), left_limit(ll), width(wi), board(G) {
        this->turn = 0;
        this->score = 0;
        this->correct = 0;
        this->hash = 0;
        for(int i = 0; i < n; ++i)
            for(int j = 0; j < n; ++j) {
                int d = calcManhattanDist(i, j);
                if(d == 0 && this->board[i * n + j])
                    ++correct;
                this->score += d;
                this->hash ^= zob[calcZobIdx(i, j)];
                if(!this->board[i * n + j]) {
                    this->zr = i;
                    this->zc = j;
                }
            }
    }

    // ゲームの終了判定
    bool isDone() const { return this->correct == n * n - 1; }

    // 現在の状況でプレイヤーが可能な行動を全て取得する
    vector<int> legalActions() const {
        vector<int> ret;
        ret.reserve(4);
        for(int i = 0; i < 4; ++i) {
            if(this->current_act >= 0 && abs(this->current_act - i) == 2)
                continue;
            if(zr + dx[i] < under_limit || zc + dy[i] < left_limit ||
               zr + dx[i] >= under_limit + width ||
               zc + dy[i] >= left_limit + width)
                continue;
            ret.push_back(i);
        }
        return ret;
    }

    unsigned long long getHashIfAction(int act) {
        unsigned long long ret = this->hash;
        ret ^= zob[calcZobIdx(zr, zc)];
        ret ^= zob[calcZobIdx(zr + dx[act], zc + dy[act])];
        swap(this->board[zr * n + zc],
             this->board[(zr + dx[act]) * n + zc + dy[act]]);
        ret ^= zob[calcZobIdx(zr, zc)];
        ret ^= zob[calcZobIdx(zr + dx[act], zc + dy[act])];
        swap(this->board[zr * n + zc],
             this->board[(zr + dx[act]) * n + zc + dy[act]]);
        return ret;
    }

    void doAction(int act) {
        this->current_act = act;

        this->hash ^= zob[calcZobIdx(zr, zc)];
        this->hash ^= zob[calcZobIdx(zr + dx[act], zc + dy[act])];
        this->score -= calcManhattanDist(zr + dx[act], zc + dy[act]);
        if(calcManhattanDist(zr + dx[act], zc + dy[act]) == 0)
            --this->correct;

        swap(this->board[zr * n + zc],
             this->board[(zr + dx[act]) * n + zc + dy[act]]);

        this->hash ^= zob[calcZobIdx(zr, zc)];
        this->hash ^= zob[calcZobIdx(zr + dx[act], zc + dy[act])];
        this->score += calcManhattanDist(zr, zc);
        if(calcManhattanDist(zr, zc) == 0) {
            ++this->correct;
        }

        zr += dx[act];
        zc += dy[act];
        ++this->turn;
    }
};
bool operator>(const Node &state_1, const Node &state_2) {
    return state_1.score - state_1.ban_count * BAN_COUNT_KEISU >
           state_2.score - state_2.ban_count * BAN_COUNT_KEISU;
}
bool operator<(const Node &state_1, const Node &state_2) {
    return state_1.score - state_1.ban_count * BAN_COUNT_KEISU <
           state_2.score - state_2.ban_count * BAN_COUNT_KEISU;
}

bool operator>(const std::shared_ptr<Node> &node_ptr1,
               const std::shared_ptr<Node> &node_ptr2) {
    return (*node_ptr1) > (*node_ptr2);
}
bool operator<(const std::shared_ptr<Node> &node_ptr1,
               const std::shared_ptr<Node> &node_ptr2) {
    return (*node_ptr1) < (*node_ptr2);
}

Node beamSearchActionSimpleWithTurn(const Node &state, const int beam_width,
                                    int turn) {
    // Nodeを管理するpq
    std::priority_queue<std::shared_ptr<Node>,
                        std::vector<std::shared_ptr<Node>>,
                        std::greater<std::shared_ptr<Node>>>
        now_beam;

    // 最良の状態
    Node best_state = state;

    // 初期状態を追加
    now_beam.emplace(std::make_shared<Node>(state));

    // 重複を管理するセット
    cc_hash_table<unsigned long long, monostate> check;

    for(int t = 0; t < turn; t++) {
        // 次のターンのpq
        std::priority_queue<std::shared_ptr<Node>,
                            std::vector<std::shared_ptr<Node>>,
                            std::greater<std::shared_ptr<Node>>>
            next_beam;

        for(int i = 0; i < beam_width; i++) {
            if(now_beam.empty())
                break;

            Node now_state = *now_beam.top();
            now_beam.pop();

            if(check.find(now_state.hash) != check.end())
                continue;
            check[now_state.hash];

            // 合法手を取得
            auto legal_actions = now_state.legalActions();

            for(const auto &action : legal_actions) {
                Node next_state = now_state;
                next_state.parent = std::make_shared<Node>(now_state);
                unsigned long long next_hash =
                    next_state.getHashIfAction(action);
                if(check.find(next_hash) != check.end())
                    continue;
                next_state.doAction(action);
                next_beam.emplace(std::make_shared<Node>(next_state));

                if(best_state > next_state) {
                    best_state = next_state;
                }
            }
        }

        // 状態の更新
        now_beam = next_beam;

        if(best_state.isDone() || now_beam.empty()) {
            break;
        }
    }

    return best_state;
}

class State {
  private:
    int zr;
    int zc;

  public:
    int score = 0;
    int turn = 0;
    int ban_count = 0;
    string order = "";
    std::shared_ptr<State> parent = nullptr;
    unsigned long long hash;
    vector<int> board;
    State(){};
    State(const vector<int> &G) {
        this->board = G;
        for(int i = 0; i < n * n; ++i) {
            if(!G[i]) {
                zr = i / n;
                zc = i % n;
            }
        }
        Node start(0, 0, n, G);
        score = start.score;
        hash = start.hash;
        turn = start.turn;
        ban_count = start.ban_count;
    };

    void doAction(int start_x, int start_y, int width) {
        order = "";

        int cnt = 0;
        while(zr < start_x) {
            swap(board[zr * n + zc], board[(zr + 1) * n + zc]);
            ++zr;
            ++cnt;
            order += 'D';
        }
        while(zr >= start_x + width) {
            swap(board[zr * n + zc], board[(zr - 1) * n + zc]);
            --zr;
            ++cnt;
            order += 'U';
        }
        while(zc < start_y) {
            swap(board[zr * n + zc], board[zr * n + zc + 1]);
            ++zc;
            ++cnt;
            order += 'R';
        }
        while(zc >= start_y + width) {
            swap(board[zr * n + zc], board[zr * n + zc - 1]);
            --zc;
            ++cnt;
            order += 'L';
        }

        Node start(start_x, start_y, width, board);
        assert(zr == start.zr && zc == start.zc);
        Node ans = beamSearchActionSimpleWithTurn(start, 100, 100);

        zr = ans.zr;
        zc = ans.zc;
        board = ans.board;
        score = ans.score;
        hash = ans.hash;
        turn = ans.turn;
        ban_count = ans.ban_count;

        string DRUL = "DRUL";
        string plu = "";
        while(ans.parent) {
            plu += DRUL[ans.current_act];
            ans = *ans.parent;
        }
        reverse(order.begin(), order.end());
        order = plu + order;
    }

    bool isDone() const { return score == 0; }
};

bool operator>(const State &state_1, const State &state_2) {
    if(state_1.score*5 + state_1.turn - state_1.ban_count * BAN_COUNT_KEISU !=
       state_2.score*5 + state_2.turn - state_2.ban_count * BAN_COUNT_KEISU) {
        return state_1.score*5 + state_1.turn -
                   state_1.ban_count * BAN_COUNT_KEISU >
               state_2.score*5 + state_2.turn -
                   state_2.ban_count * BAN_COUNT_KEISU;
    }
    return state_1.turn > state_2.turn;
}
bool operator<(const State &state_1, const State &state_2) {
    if(state_1.score*5 + state_1.turn - state_1.ban_count * BAN_COUNT_KEISU !=
       state_2.score*5 + state_2.turn) {
        return state_1.score*5 + state_1.turn -
                   state_1.ban_count * BAN_COUNT_KEISU <
               state_2.score*5 + state_2.turn -
                   state_2.ban_count * BAN_COUNT_KEISU;
    }
    return state_1.turn < state_2.turn;
}

bool operator>(const std::shared_ptr<State> &node_ptr1,
               const std::shared_ptr<State> &node_ptr2) {
    return (*node_ptr1) > (*node_ptr2);
}
bool operator<(const std::shared_ptr<State> &node_ptr1,
               const std::shared_ptr<State> &node_ptr2) {
    return (*node_ptr1) < (*node_ptr2);
}

State beamSearchActionSimple(const State &state, const int beam_width,
                             const int width) {
    // Nodeを管理するpq
    vector<std::priority_queue<std::shared_ptr<State>,
                               std::vector<std::shared_ptr<State>>,
                               std::greater<std::shared_ptr<State>>>>
        now_beams(9);

    // 最良の状態
    State best_state = state;

    // 初期状態を追加
    now_beams[0].emplace(std::make_shared<State>(state));

    // 重複を管理するセット
    cc_hash_table<unsigned long long, monostate> check;

    for(int t = 0;; t++) {
        // 次のターンのpq
        vector<std::priority_queue<std::shared_ptr<State>,
                                   std::vector<std::shared_ptr<State>>,
                                   std::greater<std::shared_ptr<State>>>>
            next_beams(9);
        int idx = 0;
        for(auto &now_beam : now_beams) {
            if(now_beam.empty()) {
                ++idx;
                continue;
            }
            for(int i = 0; i < beam_width; i++) {
                if(now_beam.empty())
                    break;
                State now_state = *now_beam.top();
                now_beam.pop();

                if(check.find(now_state.hash) != check.end())
                    continue;
                check[now_state.hash];
                for(int j = 0; j < n; j += 3) {
                    for(int k = 0; k < n; k += 3) {
                        if(idx == j / 3 * 3 + k / 3){
                            if(k >= n-width){
                                break;
                            }else{
                                continue;
                            }
                        }
                        State next_state = now_state;
                        next_state.parent = std::make_shared<State>(now_state);
                        next_state.doAction(min(j, n - width),
                                            min(k, n - width), width);
                        if(check.find(next_state.hash) != check.end()) {
                            if(k >= n - width) {
                                break;
                            } else {
                                continue;
                            }
                        }
                        next_beams[j / 3 * 3 + k / 3].emplace(
                            std::make_shared<State>(next_state));
                        if(best_state > next_state) {
                            best_state = next_state;
                        }
                        if(k >= n - width) {
                            break;
                        } else {
                            continue;
                        }
                    }
                    if(j >= n - width) {
                        break;
                    } else {
                        continue;
                    }
                }
            }
            ++idx;
        }

        // 状態の更新
        now_beams = next_beams;

        if(best_state.isDone() || t == 100) {
            break;
        }
    }

    return best_state;
}

int main() {

    // zob の初期化
    for(int i = 0; i < 10000; ++i)
        zob[i] = rng();

    // 入力の受け取り
    cin >> n;
    int start_x, start_y;
    vector<int> G(n * n);
    for(int i = 0; i < n * n; ++i) {
        cin >> G[i];
        if(!G[i]) {
            start_x = i / n;
            start_y = i % n;
        }
    }

    int width = 7;

    string order = "";

    State start(G);

    State ans = beamSearchActionSimple(start, 5, width);
    cerr << ans.ban_count << endl;
    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < n; ++j) {
            cerr << ans.board[i * n + j] << ' ';
        }
        cerr << endl;
    }

    while(ans.parent) {
        order += ans.order;
        ans = *ans.parent;
    }
    reverse(order.begin(), order.end());

    cout << order;

    return 0;
}