#include <bits/stdc++.h>
using namespace std;

template <typename T> struct Segtree {
    Segtree(){};
    explicit Segtree(int _n, std::function<T(T, T)> _fx, T _ex)
        : fx(_fx), ex(_ex) {
        n = 1;
        while(n < _n)
            n *= 2;
        val.assign(n * 2, ex);
    }

    void set_vec(std::vector<T> a) {
        for(int i = 0; i < (int)a.size(); ++i)
            val[i + n] = a[i];
        for(int i = n - 1; i > 0; --i)
            val[i] = fx(val[i << 1], val[(i << 1) | 1]);
    }

    void set(int i, T x) {
        assert(i >= 0 && i < n);
        i += n;
        val[i] = x;
        i >>= 1;
        while(i) {
            val[i] = fx(val[i << 1], val[(i << 1) | 1]);
            i >>= 1;
        }
    }

    T get(int i) const {
        assert(i >= 0 && i < n);
        i += n;
        return val[i];
    }

    T prod(int l, int r) const {
        assert(l >= 0 && r <= n && l <= r);
        T vl = ex, vr = ex;
        l += n;
        r += n;
        while(l < r) {
            if(l & 1)
                vl = fx(vl, val[l++]);
            if(r & 1)
                vr = fx(val[--r], vr);
            l >>= 1;
            r >>= 1;
        }
        return fx(vl, vr);
    }

    T all_prod() const { return this->prod(0, n); }

  private:
    int n;
    std::vector<T> val;
    std::function<T(T, T)> fx;
    const T ex;
};

template <class Key, class T> struct HashTable {
  public:
    explicit HashTable(uint32_t n) {
        if(n % 2 == 0) {
            ++n;
        }
        n_ = n;
        valid_.resize(n_, false);
        data_.resize(n_);
    }

    // 戻り値
    // - 存在するならtrue、存在しないならfalse
    // - index
    pair<bool, int> get_index(Key key) const {
        Key i = key % n_;
        while(valid_[i]) {
            if(data_[i].first == key) {
                return {true, i};
            }
            if(++i == n_) {
                i = 0;
            }
        }
        return {false, i};
    }

    // 指定したindexにkeyとvalueを格納する
    void set(int i, Key key, T value) {
        valid_[i] = true;
        data_[i] = {key, value};
    }

    // 指定したindexのvalueを返す
    T get(int i) const {
        assert(valid_[i]);
        return data_[i].second;
    }

    void clear() { fill(valid_.begin(), valid_.end(), false); }

  private:
    uint32_t n_;
    vector<bool> valid_;
    vector<pair<Key, T>> data_;
};

struct Action {
    int act;

    Action(int _act) : act(_act) {}

    char decode() const { return "DRUL"[act]; }

    bool operator==(const Action &other) const { return act == other.act; }
};

struct Evaluator {
    int cost;

    Evaluator(int _cost) : cost(_cost) {}

    int evaluate() const { return cost; }
};

struct Candidate {
    Action action;
    Evaluator evaluator;
    unsigned long long hash;
    int parent;

    Candidate(Action action, Evaluator evaluator, unsigned long long hash,
              int parent)
        : action(action), evaluator(evaluator), hash(hash), parent(parent) {}
};

class Selector {

    int beam_width;
    vector<Candidate> candidates;
    HashTable<unsigned long long, int> hash_to_index;
    bool full;
    vector<pair<int, int>> costs;
    vector<Candidate> finished_candidates;
    Segtree<pair<int, int>> st;

  public:
    explicit Selector(const int _beam_width, const int _capacity)
        : hash_to_index(_capacity), beam_width(_beam_width) {
        candidates.reserve(beam_width);
        full = false;
        costs.assign(beam_width, {0, 0});
        for(int i = 0; i < beam_width; ++i) {
            costs[i].second = i;
        }
        auto fx = [](pair<int, int> a, pair<int, int> b) {
            if(a.first >= b.first) {
                return a;
            } else {
                return b;
            }
        };
        Segtree<pair<int, int>> st(beam_width, fx,
                                   {numeric_limits<int>::min(), -1});
    }

    void push(const Candidate &candidate, bool finished) {
        if(finished) {
            finished_candidates.emplace_back(candidate);
            return;
        }
        int cost = candidate.evaluator.evaluate();
        if(full && cost >= st.all_prod().first) {
            // 保持しているどの候補よりもコストが小さくないとき
            return;
        }
        auto [valid, i] = hash_to_index.get_index(candidate.hash);

        if(valid) {
            int j = hash_to_index.get(i);
            if(candidate.hash == candidates[j].hash) {
                // ハッシュ値が等しいものが存在しているとき
                if(full) {
                    // segment treeが構築されている場合
                    if(cost < st.get(j).first) {
                        candidates[j] = candidate;
                        st.set(j, {cost, j});
                    }
                } else {
                    // segment treeが構築されていない場合
                    if(cost < costs[j].first) {
                        candidates[j] = candidate;
                        costs[j].first = cost;
                    }
                }
                return;
            }
        }
        if(full) {
            // segment treeが構築されている場合
            int j = st.all_prod().second;
            hash_to_index.set(i, candidate.hash, j);
            candidates[j] = candidate;
            st.set(j, {cost, j});
        } else {
            // segment treeが構築されていない場合
            int j = candidates.size();
            hash_to_index.set(i, candidate.hash, j);
            candidates.emplace_back(candidate);
            costs[j].first = cost;

            if(candidates.size() == beam_width) {
                // 保持している候補がビーム幅分になったときにsegment
                // treeを構築する
                full = true;
                st.set_vec(costs);
            }
        }
    }

    // 選んだ候補を返す
    const vector<Candidate> &select() const { return candidates; }

    // 実行可能解が見つかったか
    bool have_finished() const { return !finished_candidates.empty(); }

    // 実行可能解に到達するCandidateを返す
    vector<Candidate> get_finished_candidates() const {
        return finished_candidates;
    }

    // 最もよいCandidateを返す
    Candidate calculate_best_candidate() const {
        if(full) {
            int best = 0;
            for(int i = 0; i < beam_width; ++i) {
                if(st.get(i).first < st.get(best).first) {
                    best = i;
                }
            }
            return candidates[best];
        } else {
            int best = 0;
            for(int i = 0; i < candidates.size(); ++i) {
                if(costs[i].first < costs[best].first) {
                    best = i;
                }
            }
            return candidates[best];
        }
    }

    void clear() {
        candidates.clear();
        hash_to_index.clear();
        full = false;
    }
};

struct Input {
    int n;
    vector<int> G;

    void input() {
        cin >> n;
        G.assign(n * n, 0);
        for(int i = 0; i < n * n; ++i)
            cin >> G[i];
    }
};

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

class State {
  public:
    explicit State(const Input &input) : n(input.n), board(input.G) {
        board = input.G;
        this->score = 0;
        for(int i = 0; i < n; ++i) {
            for(int j = 0; j < n; ++j) {
                if(!board[i * n + j]) {
                    zr = i;
                    zc = j;
                }
            }
        }
    }

    // EvaluatorとHashの初期値を返す
    pair<Evaluator, unsigned long long> make_initial_node() {
        int cost = 0;
        unsigned long long hash = 0;
        for(int i = 0; i < n; ++i)
            for(int j = 0; j < n; ++j) {
                cost += calcManhattanDist(i, j, board[i * n + j]);
                if(board[i * n + j])
                    hash ^= zob[calcZobIdx(i, j, board[i * n + j])];
            }
        return {Evaluator(cost), hash};
    }

    void expand(const Evaluator &evaluator, unsigned long long hash, int parent,
                Selector &selector) {
        auto push_candidate = [&](int dir) {
            Action new_action(dir);

            int new_cost =
                evaluator.cost -
                calcManhattanDist(zr + dx[dir], zc + dy[dir],
                                  board[(zr + dx[dir]) * n + zc + dy[dir]]) +
                calcManhattanDist(zr, zc,
                                  board[(zr + dx[dir]) * n + zc + dy[dir]]);

            unsigned long long new_hash =
                hash ^
                zob[calcZobIdx(zr + dx[dir], zc + dy[dir],
                               board[(zr + dx[dir]) * n + zc + dy[dir]])] ^
                zob[calcZobIdx(zr, zc,
                               board[(zr + dx[dir]) * n + zc + dy[dir]])];

            bool finished = (new_cost == 0);

            Evaluator new_evaluator(new_cost);

            selector.push(
                Candidate(new_action, new_evaluator, new_hash, parent),
                finished);
        };

        for(int act : legalActions()) {
            push_candidate(act);
        }
    }

    // actionを実行して次の状態に遷移する
    void move_forward(Action action) {
        swap(board[zr * n + zc],
             board[(zr + dx[action.act]) * n + zc + dy[action.act]]);
        zr += dx[action.act];
        zc += dy[action.act];
        pre.push_back(action.act);
    }

    // actionを実行する前の状態に遷移する
    // 今の状態は、親からactionを実行して遷移した状態である
    void move_backward(Action action) {
        pre.pop_back();
        zr -= dx[action.act];
        zc -= dy[action.act];
        swap(board[zr * n + zc],
             board[(zr + dx[action.act]) * n + zc + dy[action.act]]);
    }

  private:
    const int n;
    int zr;
    int zc;
    int score = 0;
    vector<int> pre = {-1};
    vector<int> board;

    inline int calcManhattanDist(int r, int c, int num) const {
        return (abs(num / n - r) + abs(num % n - c));
    }

    inline bool calcCorrect(int r, int c, int num) const {
        return (abs(num / n - r) + abs(num % n - c) == 0);
    }

    inline int calcZobIdx(int r, int c, int num) const {
        return num * n * n + r * n + c;
    }

    vector<int> legalActions() {
        vector<int> ret;
        ret.reserve(4);
        for(int i = 0; i < 4; ++i) {
            if(pre.back() >= 0 && abs(pre.back() - i) == 2)
                continue;
            if(zr + dx[i] < 0 || zc + dy[i] < 0 ||
               max(zr + dx[i], zc + dy[i]) >= n)
                continue;
            ret.push_back(i);
        }
        return ret;
    }
};

class Tree {
  public:
    explicit Tree(const State &_state, const int beam_width,
                  const int tour_capacity)
        : state(_state) {
        curr_tour.reserve(tour_capacity);
        next_tour.reserve(tour_capacity);
        leaves.reserve(beam_width);
        buckets.assign(beam_width, {});
    }

    // 状態を更新しながら深さ優先探索を行い、次のノードの候補を全てselectorに追加する
    void dfs(Selector &selector) {
        if(curr_tour.empty()) {
            // 最初のターン
            auto [evaluator, hash] = state.make_initial_node();
            state.expand(evaluator, hash, 0, selector);
            return;
        }

        for(auto [leaf_index, action] : curr_tour) {
            if(leaf_index >= 0) {
                // 葉
                state.move_forward(action);
                auto &[evaluator, hash] = leaves[leaf_index];
                state.expand(evaluator, hash, leaf_index, selector);
                state.move_backward(action);
            } else if(leaf_index == -1) {
                // 前進辺
                state.move_forward(action);
            } else {
                // 後退辺
                state.move_backward(action);
            }
        }
    }

    // 木を更新する
    void update(const vector<Candidate> &candidates) {
        leaves.clear();

        if(curr_tour.empty()) {
            // 最初のターン
            for(const Candidate &candidate : candidates) {
                curr_tour.push_back({(int)leaves.size(), candidate.action});
                leaves.push_back({candidate.evaluator, candidate.hash});
            }
            return;
        }

        for(const Candidate &candidate : candidates) {
            buckets[candidate.parent].push_back(
                {candidate.action, candidate.evaluator, candidate.hash});
        }

        auto it = curr_tour.begin();

        // 一本道を反復しないようにする
        while(it->first == -1 && it->second == curr_tour.back().second) {
            Action action = (it++)->second;
            state.move_forward(action);
            direct_road.push_back(action);
            curr_tour.pop_back();
        }

        // 葉の追加や不要な辺の削除をする
        while(it != curr_tour.end()) {
            auto [leaf_index, action] = *(it++);
            if(leaf_index >= 0) {
                // 葉
                if(buckets[leaf_index].empty()) {
                    continue;
                }
                next_tour.push_back({-1, action});
                for(auto [new_action, evaluator, hash] : buckets[leaf_index]) {
                    int new_leaf_index = leaves.size();
                    next_tour.push_back({new_leaf_index, new_action});
                    leaves.push_back({evaluator, hash});
                }
                buckets[leaf_index].clear();
                next_tour.push_back({-2, action});
            } else if(leaf_index == -1) {
                // 前進辺
                next_tour.push_back({-1, action});
            } else {
                // 後退辺
                auto [old_leaf_index, old_action] = next_tour.back();
                if(old_leaf_index == -1) {
                    next_tour.pop_back();
                } else {
                    next_tour.push_back({-2, action});
                }
            }
        }
        swap(curr_tour, next_tour);
        next_tour.clear();
    }

    // 根からのパスを取得する
    vector<Action> calculate_path(int parent, int turn) const {
        vector<Action> ret = direct_road;
        ret.reserve(turn);
                    cerr << "Yes" << endl;
        for(auto [leaf_index, action] : curr_tour) {
            cerr << leaf_index << ' ' << action.act << endl;
            if(leaf_index >= 0) {
                if(leaf_index == parent) {
                    ret.push_back(action);
                    cerr << "Yes" << endl;
                    return ret;
                }
            } else if(leaf_index == -1) {
                ret.push_back(action);
            } else {
                ret.pop_back();
            }
        }
    }

  private:
    State state;
    vector<pair<int, Action>> curr_tour;
    vector<pair<int, Action>> next_tour;
    vector<pair<Evaluator, unsigned long long>> leaves;
    vector<vector<tuple<Action, Evaluator, unsigned long long>>> buckets;
    vector<Action> direct_road;
};

vector<Action> beam_search(const int beam_width, const int tour_capacity,
                           const int hash_capacity, const int max_turn,
                           const State &state) {
    Tree tree(state, beam_width, tour_capacity);

    // 新しいノード候補の集合
    Selector selector(beam_width, hash_capacity);

    for(int turn = 0; turn < max_turn; ++turn) {
        // Euler Tourでselectorに候補を追加する
        tree.dfs(selector);

        if(selector.have_finished()) {
            // ターン数最小化型の問題で実行可能解が見つかったとき
            Candidate candidate = selector.get_finished_candidates()[0];
            vector<Action> ret =
                tree.calculate_path(candidate.parent, turn + 1);
            ret.push_back(candidate.action);
            return ret;
        }

        assert(!selector.select().empty());

        if(turn == max_turn - 1) {
            // ターン数固定型の問題で全ターンが終了したとき
            Candidate best_candidate = selector.calculate_best_candidate();
            cerr << "Yes" << endl;
            vector<Action> ret =
                tree.calculate_path(best_candidate.parent, turn + 1);
            cerr << "Yes" << endl;
            ret.push_back(best_candidate.action);
            cerr << "Yes" << endl;
            return ret;
        }

        // 木を更新する
        tree.update(selector.select());

        selector.clear();
    }
}

constexpr int MAX_TURN = 1;
constexpr int BEAM_WIDTH = 1000;
constexpr int TOUR_CAPACITY = 15 * BEAM_WIDTH;
constexpr int HASH_MAP_CAPACITY = 5000000;

struct Solver {
    const Input input;
    vector<Action> output;

    Solver(const Input &input) : input(input) {}

    void solve() {
        State state(input);
        output = beam_search(BEAM_WIDTH, TOUR_CAPACITY, HASH_MAP_CAPACITY,
                             MAX_TURN, state);
    }

    void print() const {
        for(Action action : output) {
            char x = action.decode();
            cout << x;
        }
    }
};

int main() {
    // zob の初期化
    for(int i = 0; i < 10000; ++i)
        zob[i] = rng();

    ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    Input input;
    input.input();

    Solver solver(input);
    solver.solve();
    solver.print();

    return 0;
}