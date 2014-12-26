#pragma once

#include "header.h"

#ifndef INT_MAX
#define INT_MAX 2147483647
#endif

#ifndef INT_MIN
#define INT_MIN (-INT_MAX - 1)
#endif

template<class KeyType, class ValueType>
class Treap {
public:
	struct Node
	{
		KeyType key;
		ValueType val;
		uint32_t wt, size, s;
		Node *c[2];
		
		Node(KeyType _key, ValueType _val, Node *C) {
			key = _key;
			val = _val;
			wt = rand();
			size = s = 1;
			c[0] = c[1] = C;
		}
		void rz() {
			size = c[0]->size + s + c[1]->size;
		}
	} *root, *Null;
	
	void rot(Node *&t, bool d) {
		Node *p = t->c[d];
		t->c[d] = p->c[!d];
		p->c[!d] = t;
		t->rz();
		p->rz();
		t = p;
	}
	
	Node * _find(Node *t, KeyType x) {
		if (t == Null) return Null;
		if (t->key == x) return t;
		if (x < t->key) return _find(t->c[0], x);
		else return _find(t->c[1], x);
	}
	
	bool _in(Node *t, KeyType x) {
		if (t == Null) return false;
		if (t -> key == x) return true;
		if (x < t->key) return _in(t->c[0], x);
		else return _in(t->c[1], x);
	}
	
	void _clear(Node *t) {
		if(t != Null){
			_clear(t->c[0]);
			_clear(t->c[1]);
			delete t;
		}
	}
	
	Node *_insert(Node *&t, KeyType x, ValueType y) {
		if (t == Null) {
			t = new Node(x, y, Null);
			return t;
		}
		if (t->key == x) {
			t->s++;
			t->size++;
			return t;
		}
		Node *ret = _insert(t->c[x > t->key], x, y);
		if (t->c[x > t->key]->wt < t->wt) rot(t, x > t->key);
		else t->rz();
		return ret;
	}
	
	void _del(Node *&t, KeyType x) {
		if(t->key == x){
			if(t->s > 1) {
				t->s--;
				t->size--;
				return;
			}
			bool d = t->c[0]->wt > t->c[1]->wt;
			if(t->c[d] == Null) {
				delete t;
				t = Null;
				return;
			}
			rot(t,d);
			_del(t->c[!d], x);
		}
		else {
			if (t == NULL){
				puts("here");
			}
			_del(t->c[x > t->key], x);
		}
		t->rz();
	}
	
	Node *_kth(Node *t, uint32_t x) {
		uint32_t r = t->c[0]->size;
		if(x <= r)return _kth(t->c[0],x);
		else if(x > r + t->s) return _kth(t->c[1],x-r-t->s);
		return t;
	}
	uint32_t _rank(Node *t, KeyType x) {
		uint32_t r=t->c[0]->size;
		if(x<t->key)return _rank(t->c[0],x);
		else if(x>t->key)return _rank(t->c[1],x)+r+t->s;
		return r;
	}
	Node *_prev(Node *t, KeyType x) {
		if(t == Null)return INT_MIN;
		if(x <= t->key)return _prev(t->c[0], x);
		return max(t->key, _prev(t->c[1], x));
	}
	Node *_next(Node *t, KeyType x) {
		if(t == Null)return INT_MAX;
		if(x >= t->key)return _next(t->c[1], x);
		return min(t->key,_next(t->c[0], x));
	}
	
	Treap() {
		srand((unsigned int) (time(NULL))); //optional
		
		Null=new Node(KeyType(), ValueType(), 0);
		Null->size=Null->s=0;Null->wt=INT_MAX;
		Null->c[0]=Null->c[1]=Null;root=Null;
	}
	
	//right value expression
	ValueType &operator[] (const KeyType &key) {
		Node *get = _find(root, key);
		if (get != Null) return get->val;
		else return _insert(root, key, ValueType())->val;
	}
	
	//left value expression
	ValueType operator[] (const KeyType &key) const {
		Node *get = _find(root, key);
		if (get != Null) return get->val;
		else return Null;
	}
	
	bool in(KeyType x)	{return _in(root, x);}
	uint32_t size()		{return root->size;}
	void clear()		{_clear(root);root=Null;}
	void insert(KeyType x, ValueType y)	{_insert(root, x, y);}
	void del(KeyType x)	{_del(root, x);}
	KeyType kth(uint32_t x) 	{return _kth(root, x)->key;}
	uint32_t rank(KeyType x)	{return _rank(root, x);}
	KeyType prev(KeyType x)		{return _prev(root, x)->val;}
	KeyType next(KeyType x)		{return _next(root, x)->val;}
};

