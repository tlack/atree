#include <algorithm>
#include <iostream>
#include <map>
#include <vector>

using namespace std;
using TreeIndex = int;

template<typename X> vector<X> emit(const vector<X> x, string s) {
	cout << s << ": "; for(auto xx:x) { cout << xx << " "; } cout << "\n"; return x;
}
template<typename X> X emit(const X x, string s) { cout << s << ": " << x << "\n"; return x; }
template<typename X> vector<X> emit(const vector<X> x) { for(auto xx:x) { cout << xx << " "; } cout << "\n"; return x; }
template<typename X> X emit(const X x) { cout << x << "\n"; return x; }

template<typename X>
vector<X> except(const vector<X> x, const vector<X> y) {
	vector<X> r; r.reserve(x.size());
	auto begy=begin(y), endy=end(y);
	for(auto xx:x) if(find(begy,endy,xx)==endy) r.push_back(xx); // XXX use copy here
	return r;
}
template<typename X>
vector<X> except(const vector<X> x, const X y) {
	vector<X> r; r.reserve(x.size());
	for(auto xx:x) if(xx!=y) r.push_back(xx);
	return r;
}

template <typename X>
// index X with y, and then X again with the result, until it returns the same thing twice
// returns all steps
// i.e., [ x[y], x[x[y]], x[x[x[y]]], ...
vector<X> exhaust(const vector<X> x, X y) {
	X i=y,last;
	vector<X> r;
	r.reserve(x.size());
	while (1) {
		last=i;
		i=emit(x[i]);
		if(i==last) return r;
		else r.push_back(i);
	};
	return r;
}

vector<int> til(size_t n) { // non-stupid iota
	vector<int> r; r.reserve(n);
	for(int i=0; i<n; i++) r[i]=n;
	return r;
}

template<typename X>
class Tree { 
	public:
	vector<X> x;
	vector<TreeIndex> p;
	vector<X> operator[](vector<TreeIndex> path) {
		vector<X> r; r.reserve(path.size());
		for(auto p:path) r.push_back(x[p]);
		return r;
	}
	TreeIndex adopt(const TreeIndex parent, const TreeIndex child) {
		p[child]=parent; return child;
	}
	TreeIndex insert(const TreeIndex parent, const X item) {
		x.push_back(item); p.push_back(parent); return x.size()-1;
	}
	TreeIndex parent(const TreeIndex child) {
		return p[child];
	}
	vector<TreeIndex> path(const TreeIndex child) {
		return exhaust(p, child);
	}
	vector<TreeIndex> leaves() {
		return except(til(size()), p);
	}
	size_t size() {
		return p.size();
	}
};
template<typename X>
ostream& operator<<(ostream &os, const Tree<X> T) {
	TreeIndex n=0; int depth=0;
	for(auto xx:T.x) { 
		for(int i=0;i<depth;i++) os<<" ";
		os<<n<<". "<<xx<<"\n";
		n++;
	}
	return os;
}

void test_iter() {
	vector<int> a={1, 2, 3, 4, 5};
	vector<int> b={2,5,30};
	emit(except(a,a[0]));
	emit(except(a,b));
}

int main(void) {
	emit("tree");
	Tree<string> t;
	int r=emit(t.insert(0,"root"));
	int d=emit(t.insert(r,"dogs"));
	int c=emit(t.insert(r,"cats"));
	int a=emit(t.insert(r,"snakes"));
	int da=emit(t.insert(d,"min pin"));
	int db=emit(t.insert(d,"schnauzer"));
	int dba=emit(t.insert(db,"giant schnauzer"));
	int aa=emit(t.insert(a,"anaconda"));
	emit(t, "tree");
	emit(t.leaves(), "leaves");
	emit(t.path(5), "path(5)");
	emit(t[t.path(5)], "t[path(5)]");
	auto x=exhaust(t.p, 6);
	emit(x, "exhaust");
	test_iter();
	emit("fin");
}


