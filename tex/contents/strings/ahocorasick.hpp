#include "../contest/template.hpp"
namespace Aho {
	const int SZ = 26;
	char C(char c) {return c - 'a';}
	struct Node {
		Node *par=nullptr,*nxt[SZ] = {},*pi[SZ] = {};
		int depth=0;
		Node(){} Node(Node*par):par(par){depth=par->depth+1;};
	};
	struct Corasick {
		Node *root;
		Corasick() {root = new Node;}
		void add(const string s) {
			Node *node = root;
			for (char c: s) { c = C(c);
				if (node->nxt[c] == nullptr)
					node -> nxt[c] = new Node(node);
				node = node -> nxt[c];
			}
		}
	};
}
