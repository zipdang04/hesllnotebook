#include "../../miscellanous/template.hpp"

namespace Persistent{
template<class T> struct Node {
	Node *l, *r; T val;
	Node(): Node(T()) {}
	Node(T val): Node(val, nullptr, nullptr) {}
	Node(T val, Node* l, Node* r): l(l), r(r), val(val) {}
};
struct Range{
	inline static int N; int l, r, mid;
	inline static void init(int N) {Range::N = N;}
	Range(): Range(N) {} Range(int n): Range(1, n){}
	Range(int l, int r): l(l), r(r) {mid = (l+r)/2;}
	inline bool isSingle() {return l == r;}
	inline bool contains(int x) {return l<=x&&x<=r;}
	inline bool isInside(int L, int R) {return L<=l&&r<=R;}
	inline bool noRelate(int L, int R) {return r<L || R<l;}
	inline Range L() {return {l, mid};}
	inline Range R() {return {mid+1, r};}
};
template<class T> struct SegTree {
	static const T DEF_VAL;
	inline static T process(T a, T b);
	vector<Node<T>*> trees;
	SegTree() {}
	template<class T2>
	Node<T>* _buildTree(T2 *arr, Range rng) {
		if (rng.isSingle())
			return new Node<T>(arr[rng.l]);
		Node<T>* l = _buildTree(arr, rng.L());
		Node<T>* r = _buildTree(arr, rng.R());
		return new Node<T>(process(l -> val, r -> val), l, r);
	}
	Node<T>* _newRoot(Node<T>* root) {
		return new Node<T>(*root);
	}
	Node<T>* _update(Range rng, Node<T>* node, int pos, T val) {
		if (rng.isSingle())
			return new Node<T>(val);
		Node<T> *l = node -> l, *r = node -> r;
		if (rng.L().contains(pos))
			l = _update(rng.L(), l, pos, val);
		else
			r = _update(rng.R(), r, pos, val);
		return new Node<T>(process(l -> val, r -> val), l, r);
	}
	T _get(Range rng, Node<T>* node, int L, int R) {
		if (rng.noRelate(L, R)) return DEF_VAL;
		if (rng.isInside(L, R)) return node -> val;
		return process(
			_get(rng.L(), node -> l, L, R),
			_get(rng.R(), node -> r, L, R)
		);
	}

	template<class T2>void buildTree(T2* arr) {
		trees = {_buildTree(arr, Range())};
	}
	void addTree(int from) {
		assert(from < (int)trees.size());
		trees.push_back(_newRoot(trees[from]));
	}
	void update(int from, int pos, T val) {
		assert(from < (int)trees.size());
		trees[from] = _update(Range(), trees[from], pos, val);
	}
	T get(int from, int L, int R) {
		assert(from < (int)trees.size());
		return _get(Range(), trees[from], L, R);
	}
};
}
// remember to declare:
// template<> const T Persistent::SegTree<T>::DEF_VAL
// template<> T Persistent::SegTree<T>::process(T a, T b)
