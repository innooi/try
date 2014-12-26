//#include "treap.h"

template<class Tp> Treap <Tp>::Treap() {
	srand(12123); //optional
	
	Null=new Node(0, 0);
	Null->size=Null->s=0;Null->key=INT_MAX;
	Null->c[0]=Null->c[1]=Null;root=Null;
}

template<class Tp> void Treap <Tp>::rot(Node *&t, bool d) {
	Node *p=t->c[d];t->c[d]=p->c[!d];
	p->c[!d]=t;t->rz();p->rz();t=p;
}

template<class Tp> bool Treap <Tp>::_search(Node *t, Tp x) {
	if(t==Null) return false;
	if(t->val==x) return true;
	if(x<t->val) return _search(t->c[0],x);
	else return _search(t->c[1],x);
}

template<class Tp> void Treap <Tp>::_clear(Node *t) {
	if(t!=Null){_clear(t->c[0]);_clear(t->c[1]);delete t;}
}

template<class Tp> void Treap <Tp>::_insert(Node *&t, Tp x) {
	if(t==Null){t=new Node(x,Null);return;}
	if(t->val==x){t->s++;t->size++;return;}
	_insert(t->c[x>t->val],x);
	if(t->c[x>t->val]->key<t->key)
	rot(t,x>t->val);else t->rz();
}

template<class Tp> void Treap <Tp>::_del(Node *&t, Tp x) {
	if(t->val==x){
		if(t->s>1){t->s--;t->size--;return;}
		bool d=t->c[0]->key>t->c[1]->key;
		if(t->c[d]==Null){delete t;t=Null;return;}	
		rot(t,d);_del(t->c[!d],x);
	}else _del(t->c[x>t->val],x);t->rz();
}

template<class Tp> Tp Treap <Tp>::_kth(Node *t, uint32_t x) {
	uint32_t r=t->c[0]->size;
	if(x<=r)return _kth(t->c[0],x);
	else if(x>r+t->s) return _kth(t->c[1],x-r-t->s);
	return t->val;
}

template<class Tp> uint32_t Treap <Tp>::_rank(Node *t, Tp x) {
	uint32_t r=t->c[0]->size;
	if(x<t->val)return _rank(t->c[0],x);
	else if(x>t->val)return _rank(t->c[1],x)+r+t->s;
	return r;
}

template<class Tp> Tp Treap <Tp>::_prev(Node *t, Tp x) {
	if(t==Null)return INT_MIN;
	if(x<=t->val)return _prev(t->c[0],x);
	return max(t->val,_prev(t->c[1],x));
}

template<class Tp> Tp Treap <Tp>::_next(Node *t, Tp x) {
	if(t==Null)return INT_MAX;
	if(x>=t->val)return _next(t->c[1],x);
	return min(t->val,_next(t->c[0],x));
}