#include "bits/stdc++.h"
#include <ext/pb_ds/assoc_container.hpp>
         
using namespace std;
using namespace __gnu_pbds;
 
/* typedef tree<int,null_type,less<int>,rb_tree_tag,
tree_order_statistics_node_update> indexed_set; */
         
#define el "\n"
#define faster ios_base::sync_with_stdio(0); cin.tie(0);
#define sq(x) (x)*(x)
#define pb push_back
#define mp make_pair
#define F first
#define S second
#define sz(x) (int)(x).size()
         
typedef long long ll;
typedef long double ld;
typedef pair<int, int> pii;
typedef vector<int> vi;
typedef vector<pii> vpii;
typedef vector<vi> vvi;
typedef pair<ll,ll> pll;
typedef vector<ll> vll;
typedef vector<vll> vvll;
typedef vector<pll> vpll;
         
#define all(x) (x).begin(), (x).end()

#define rep(i, a, b) for (int i = (a); i < (b); ++i)
#define per(i, a, b) for (int i = (b-1); i >= (a); --i)
#define REP(i, a, b) for (int i = (a); i <= (b); ++i)
#define trav(a, x) for (auto &a : x)
         
struct chash { // large odd number for C
      const uint64_t C = ll(4e18 * acos(0)) | 71;
      ll operator()(ll x) const { return __builtin_bswap64(x * C); }
};
 
template<typename T> using fast_set = __gnu_pbds::gp_hash_table<T, null_type, chash>;
template<typename T, typename H> using fast_set_h = __gnu_pbds::gp_hash_table<T, null_type, H>;
template<typename T, typename U> using fast_map = __gnu_pbds::gp_hash_table<T, U, chash>;
 
void __print(int x) {cerr << x;}
void __print(long x) {cerr << x;}
void __print(long long x) {cerr << x;}
void __print(unsigned x) {cerr << x;}
void __print(unsigned long x) {cerr << x;}
void __print(unsigned long long x) {cerr << x;}
void __print(float x) {cerr << x;}
void __print(double x) {cerr << x;}
void __print(long double x) {cerr << x;}
void __print(char x) {cerr << '\'' << x << '\'';}
void __print(const char *x) {cerr << '\"' << x << '\"';}
void __print(const string &x) {cerr << '\"' << x << '\"';}
void __print(bool x) {cerr << (x ? "true" : "false");}

template<typename T, typename V>
void __print(const pair<T, V> &x);
template<typename T>
void __print(const T &x) {int f = 0; cerr << '{'; for (auto &i: x) cerr << (f++ ? ", " : ""), __print(i); cerr << "}";}
template<typename T, typename V>
void __print(const pair<T, V> &x) {cerr << '{'; __print(x.first); cerr << ", "; __print(x.second); cerr << '}';}
void _print() {cerr << "]\n";}
template <typename T, typename... V>
void _print(T t, V... v) {cerr << "\e[92m"; __print(t); if (sizeof...(v)) cerr << ", "; _print(v...);}

#ifdef DEBUG
#define dbg(x...) cerr << "\e[92m"<<__func__<<":"<<__LINE__<<" [" << #x << "] = ["; _print(x); cerr << "\e[39m" << endl;
#else
#define dbg(x...)
#endif


const ll mod = 1e9+7; 

const ll C = 3;

// OBS! har tagit Mod structen från kactl

struct Mod {
    ll x;
    Mod(ll xx) : x(xx) {}
    Mod(): x(0) {}
    Mod operator+(Mod b) { return Mod((x + b.x) % mod); }
    Mod operator-(Mod b) { return Mod((x - b.x + mod) % mod); }
    Mod operator*(Mod b) { return Mod((x * b.x) % mod); }
    bool operator==(Mod b) const { return (x == b.x);}
    Mod operator^(ll e) {
        if (!e) return Mod(1);
        Mod r = *this ^ (e / 2); r = r * r;
        return e&1 ? *this * r : r;
    }
};

void solve() {
    string s, decr;
    cin >> s >> decr;
    ll n = sz(s), m = sz(decr);
    const ll inf = n + 1;
    if (m > n) {
        cout << 0;
        return;
    }
    vector<Mod> pw(300000);
    pw[0] = 1;  
    rep(i, 1, 300000) {
        pw[i] = pw[i-1] * C;
    }

    vll pos(26, inf);
    vll nxt(m, inf);
    per(i, 0, m) {
        if (pos[decr[i] - 'a'] != inf) {
            nxt[i] = pos[decr[i] - 'a'] - i;
        }
        pos[decr[i] - 'a'] = i;
    }
    Mod ph(nxt[0]);
    rep(i, 1, m) {
        ph = ph * C + nxt[i];
    }
    pos = vll(26, inf);
    nxt.assign(n, inf);
    vll prev(n, inf);
    per(i, 0, n) {
        if (pos[s[i] - 'a'] != inf) {
            nxt[i] = pos[s[i] - 'a'];
            prev[nxt[i]] = i;
            nxt[i] -= i;
        }
        pos[s[i] - 'a'] = i;
    }

    if (nxt[0] > m-1) nxt[0] = inf;
    Mod w_hash(nxt[0]);
    rep(i, 1, m) {
        if (nxt[i] + i > m-1) nxt[i] = inf;
        w_hash = w_hash * C + nxt[i];
    }
    ll ind = m - 1;
    vll ans;
    while(ind < n) {
        if (w_hash == ph) ans.pb(ind - m + 1);
        // increment i and update w_hash normally
        ind++;
        if (ind > n-1) break;
        w_hash = w_hash - (pw[m-1] * nxt[ind-m]);
        nxt[ind] = inf;
        w_hash = w_hash * C + nxt[ind];

        // update special if needed (make some previous inf, into (not inf) );
        if (ind - m + 1 <= prev[ind] && prev[ind] < ind) {
            ll potens = m - (prev[ind] - (ind - m));
            w_hash = w_hash - (pw[potens] * nxt[prev[ind]]);
            nxt[prev[ind]] = ind - prev[ind];   
            w_hash = w_hash + (pw[potens] * nxt[prev[ind]]);
        }
    }

    if (sz(ans) == 1) {
        string output = "";
        output = s.substr(ans.back(), m);
        cout << output;
    }
    else cout << sz(ans);
        
}

int main() {
        faster
        int test = 1; // cin >> test;
    REP(tc, 1, test) {
        solve();
    }
}
