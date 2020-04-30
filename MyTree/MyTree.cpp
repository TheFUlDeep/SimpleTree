﻿#include "MyTree.h"

template<typename T>
MyTreeNode<T>::MyTreeNode(const T key, MyTreeNode<T> *prnt, const std::shared_ptr<MyTreeNode<T>> &lft, const std::shared_ptr<MyTreeNode<T>> &rght, const std::shared_ptr<MyTreeNode<T>> &cntrl)
{
	parent = prnt; left = lft; right = rght; this->key = key; central = cntrl;
}

template<typename T>
MyTreeNode<T>::~MyTreeNode() = default;


template<typename T>
T& MyTreeNode<T>::GetKey() { return key; }

template<typename T>
MyTreeNode<T>* MyTreeNode<T>::GetParentNode() const noexcept { return parent; }

template<typename T>
MyTreeNode<T>* MyTreeNode<T>::GetLeftNode() const noexcept { return left.get(); }

template<typename T>
MyTreeNode<T>* MyTreeNode<T>::GetRightNode() const noexcept { return right.get(); }

template<typename T>
MyTreeNode<T>* MyTreeNode<T>::GetCentralNode() const noexcept { return central.get(); }


template<typename T>
MyTree<T>::MyTree() = default;


template<typename T>
MyTree<T>::~MyTree() = default;


template<typename T>
MyTree<T>& MyTree<T>::operator=(const MyTree<T> &other) noexcept { nodesCount = other->nodesCount; head = other.head; }//TODO возможно shared_ptr сам нормально скопируется. Надо проверить


template<typename T>
bool MyTree<T>::IsEmpty()const noexcept { return nodesCount == 0; }


template<typename T>
size_t MyTree<T>::GetNodesCount()const noexcept { return nodesCount; }


template<typename T>
void MyTree<T>::Add(const T &k)noexcept
{
	if (head == nullptr) head = std::make_shared<MyTreeNode<T>>(k);
	else
	{
		MyTreeNode<T> *curnode = head.get();
		MyTreeNode<T> *prevnode = nullptr;
		while (curnode != nullptr)
		{
			prevnode = curnode;
			if (k == curnode->key) curnode = curnode->central.get();
			else if (k < curnode->key) curnode = curnode->left.get();
			else curnode = curnode->right.get();
		}
		auto newnode = std::make_shared<MyTreeNode<T>>(k, prevnode);
		if (k < prevnode->key) prevnode->left = newnode;
		else if (k > prevnode->key) prevnode->right = newnode;
		else prevnode->central = newnode;
		//PrettyPrint();
		if (newnode->parent != nullptr) Balance(newnode->parent->parent);
		//PrettyPrint();
	}
	nodesCount++;
}


template<typename T>
MyTreeNode<T>* MyTree<T>::RecursiveFind(const T &k, MyTreeNode<T>* node)const
{
	if (node == nullptr) return nullptr;
	if (node->key == k) return node;
	else
	{
		if (k < node->key && node->left != nullptr) return RecursiveFind(k, node->left.get());
		else if (k > node->key && node->right != nullptr) return RecursiveFind(k, node->right.get());
		else return nullptr;
	}
}


template<typename T>
MyTreeNode<T>* MyTree<T>::Find(const T &k)const noexcept { return RecursiveFind(k,head.get()); }


template<typename T>
size_t MyTree<T>::RecursiveGetHeight(const MyTreeNode<T>* node, const bool &ignorcentral)const noexcept
{
	if (node == nullptr) return 0;
	if (ignorcentral) return 1 + std::max(RecursiveGetHeight(node->left.get(), ignorcentral), RecursiveGetHeight(node->right.get(), ignorcentral));
	else return 1 + std::max(RecursiveGetHeight(node->left.get(), ignorcentral), std::max(RecursiveGetHeight(node->right.get()), RecursiveGetHeight(node->central.get(), ignorcentral)));
}


template<typename T>
size_t MyTree<T>::GetHeight(const bool &ignorcentral)const noexcept{ return RecursiveGetHeight(head.get(), ignorcentral); }

template<typename T>
MyTreeNode<T>* MyTree<T>::GetHead() const noexcept { return head.get(); }


template<typename T>
void MyTree<T>::RemoveNode(MyTreeNode<T>* node)noexcept
{
	if (node == nullptr) return;

	MyTreeNode<T> *nodeleft = node->left.get(), *noderight = node->right.get(), *nodecentral = node->central.get();
	int pos = IsNodeLeft(node);
	auto tmpnode = GetNodeSharedPtr(node);//сохраняю во временную переменную, чтобы ноуд не умер раньше положенного

	if (node->central.get() != nullptr)//если есть центральный наследник
	{
		node->central->parent = node->parent;
		if (pos == MyTree::nodepos::central) node->parent->central = node->central;
		else if (pos == MyTree::nodepos::left) node->parent->left = node->central;
		else if (pos == MyTree::nodepos::right) node->parent->right = node->central;
		else head = node->central;

		if (node->left.get() != nullptr) node->left->parent = node->central.get();
		if (node->right.get() != nullptr) node->right->parent = node->central.get();

		node->central->left = node->left;
		node->central->right = node->right;
	}
	//дальше подрузамевается, что центрального наследника нет
	else if (node->parent != nullptr)//если есть роидетль (если это не голова)
	{
		MyTreeNode<T> *parentnode = node->parent;

		bool parentleft = parentnode->left.get() == node; //определяю, этот ноуд слева от родителя или справа

		if (nodeleft == nullptr && noderight == nullptr) //если нет листьев
		{
			if (pos == MyTree::nodepos::central) parentnode->central = nullptr;
			else if (pos == MyTree::nodepos::left) parentnode->left = nullptr;
			else if (pos == MyTree::nodepos::right) parentnode->right = nullptr;
		}
		else if (nodeleft != nullptr && noderight == nullptr)//если есть только левый ноуд
		{
			if (pos == MyTree::nodepos::central) { parentnode->central = node->left; parentnode->central->parent = parentnode; }
			else if (pos == MyTree::nodepos::left) { parentnode->left = node->left; parentnode->left->parent = parentnode; }
			else if (pos == MyTree::nodepos::right) { parentnode->right = node->left; parentnode->right->parent = parentnode; }
		}
		else if (nodeleft == nullptr && noderight != nullptr)//если есть только правый ноуд
		{
			if (pos == MyTree::nodepos::central) { parentnode->central = node->right; parentnode->central->parent = parentnode; }
			else if (pos == MyTree::nodepos::left) { parentnode->left = node->right; parentnode->left->parent = parentnode; }
			else if (pos == MyTree::nodepos::right) { parentnode->right = node->right; parentnode->right->parent = parentnode; }
		}
		else//если есть оба ноуда 
		{
			//вместо удаленного ноуда вставляю левый. Но можно было правый. 

			auto rightnode = node->right;

			if (pos == MyTree::nodepos::central) { parentnode->central = node->left; parentnode->central->parent = parentnode; }
			else if (pos == MyTree::nodepos::left) { parentnode->left = node->left; parentnode->left->parent = parentnode; }
			else if (pos == MyTree::nodepos::right) { parentnode->right = node->left; parentnode->right->parent = parentnode; }

			MyTreeNode<T> *curnode = parentnode;
			while (curnode->right != nullptr) curnode = curnode->right.get();
			curnode->right = rightnode;
			rightnode->parent = curnode;
		}
	}
	//если нет роидетля (если это голова)
	else
	{
		if (nodeleft == nullptr && noderight == nullptr) head = nullptr; //если нет листьев
		else if (nodeleft != nullptr && noderight == nullptr)head = head->left; //если есть только левый ноуд
		else if (nodeleft == nullptr && noderight != nullptr) head = head->right;//если есть только правый ноуд
		else//если есть оба ноуда 
		{
			//вместо удаленного ноуда вставляю левый. Но можно было правый. 
			auto rightnode = node->right;
			head = head->left;
			head->parent = nullptr;
			MyTreeNode<T> *curnode = head.get();
			while (curnode->right != nullptr) curnode = curnode->right.get();
			curnode->right = rightnode;
			rightnode->parent = curnode;
		}
	}
	Balance(head.get());
	nodesCount--;
}


template<typename T>
MyTreeNode<T>* MyTree<T>::FindMin(MyTreeNode<T>* node)const noexcept
{
	if (node == nullptr) return nullptr;
	MyTreeNode<T> *curnode = node;
	while (curnode->left != nullptr) curnode = curnode->left.get();
	return curnode;
}


template<typename T>
MyTreeNode<T>* MyTree<T>::FindMax(MyTreeNode<T>* node)const noexcept
{
	if (node == nullptr) return nullptr;
	MyTreeNode<T> *curnode = node;
	while (curnode->right != nullptr) curnode = curnode->right.get();
	return curnode;
}


template<typename T>
MyTreeNode<T>* MyTree<T>::GetRightNode(const MyTreeNode<T>* node) const noexcept 
{
	if (node == nullptr) return nullptr;
	return node->right.get();
}

template<typename T>
MyTreeNode<T>* MyTree<T>::GetLeftNode(const MyTreeNode<T>* node) const noexcept
{
	if (node == nullptr) return nullptr;
	return node->left.get();
}

template<typename T>
MyTreeNode<T>* MyTree<T>::GetParentNode(const MyTreeNode<T>* node) const noexcept
{
	if (node == nullptr) return nullptr;
	return node->parent;
}

template<typename T>
MyTreeNode<T>* MyTree<T>::GetCentralNode(const MyTreeNode<T>* node) const noexcept
{
	if (node == nullptr) return nullptr;
	return node->central.get();
}

template<typename T>
MyList<T> MyTree<T>::AllKeysToListRecusiveSorted(const MyTreeNode<T>* node, MyList<T>* lst)const
{
	if (lst == nullptr) lst = &MyList<T>();
	if (node == nullptr) return *lst;
	AllKeysToListRerusiveSorted(node->left.get(), lst);
	lst->push_back(node->key);
	AllKeysToListRerusiveSorted(node->central.get(), lst);
	AllKeysToListRerusiveSorted(node->right.get(), lst);
	return *lst;
}

template<typename T>
MyList<T> MyTree<T>::ToList()const noexcept { return AllKeysToListRerusiveSorted(head.get()); }

template<typename T>
std::string MyTree<T>::AllKeysToStringRecusiveSorted(const char &symbol, const MyTreeNode<T>* node, std::string* lst)const
{
	if (lst == nullptr) lst = &std::string("");
	if (node == nullptr) return *lst;
	AllKeysToStringRecusiveSorted(symbol, node->left.get(), lst);
	if (lst->length() > 0) lst->operator+=(symbol);
	lst->operator+=(std::to_string(node->key));
	AllKeysToStringRecusiveSorted(symbol, node->central.get(), lst);
	AllKeysToStringRecusiveSorted(symbol, node->right.get(), lst);
	return *lst;
}


template<typename T>
std::string MyTree<T>::ToString(const char &symbol)const noexcept { return AllKeysToStringRecusiveSorted(symbol, head.get()); }


template<typename T>
void MyTree<T>::PrettyPrintRecursive(const size_t *h, const size_t *maxlen, const char* spacestr, MyList<MyTreeNode<T>*> prevline, size_t linenumber)const
{

	if (head == nullptr) return;
	if (prevline.empty()) prevline.push_back(head.get());
	MyList<MyTreeNode<T>*> curlist;

	bool needDeeper = false;

	//первая позиция, сдвиг, количество символов
	//size_t symbolscount = pow(2,linenumber-1);

	size_t firstpos = (size_t)pow(2, *h - linenumber);
	size_t space = firstpos * 2;


	for (size_t i = 1; i < firstpos; i++) std::cout << spacestr;//начальный сдвиг
	while (!prevline.empty())
	{
		auto val = prevline.front();
		if (val == nullptr) { prevline.pop_front(); for (size_t i = 1; i < space + 1; i++) std::cout << spacestr; curlist.push_back(nullptr); curlist.push_back(nullptr); continue; }
		std::cout << val->key;
		size_t vallen = std::to_string(val->key).length();
		if (vallen < *maxlen) for (size_t i = 0; i < *maxlen - vallen; i++) std::cout << " ";
		for (size_t i = 1; i < space; i++) std::cout << spacestr;
		if (val->left != nullptr || val->right != nullptr) needDeeper = true;
		curlist.push_back(val->left.get());
		curlist.push_back(val->right.get());
		prevline.pop_front();
	}
	for (size_t i = 0; i < firstpos; i++) std::cout << "\n";

	if (needDeeper) PrettyPrintRecursive(h, maxlen,spacestr,curlist,linenumber+1);
}


template<typename T>
void MyTree<T>::PrettyPrint() const noexcept
{
	std::cout << "\n";
	size_t maxlen;
	size_t maxlenmax = std::to_string(FindMax(head.get())->key).length();
	size_t maxlenmin = std::to_string(FindMin(head.get())->key).length();
	maxlenmax > maxlenmin ? maxlen = maxlenmax : maxlen = maxlenmin;

	std::string spacestr = "";
	for (size_t i = 0; i < maxlen; i++) spacestr += " ";

	size_t h = GetHeight(true);
	PrettyPrintRecursive(&h, &maxlen, spacestr.c_str());
}




template<typename T>
int MyTree<T>::IsNodeLeft(MyTreeNode<T>* node)
{
	if (node == nullptr || node->parent == nullptr) return MyTree::nodepos::nil;
	else if (node->parent->left.get() == node) return MyTree::nodepos::left;
	else if (node->parent->central.get() == node) return MyTree::nodepos::central;
	else return MyTree::nodepos::right;
}

template<typename T>
std::shared_ptr<MyTreeNode<T>>& MyTree<T>::GetNodeSharedPtr(MyTreeNode<T>* node)
{
	std::shared_ptr<MyTreeNode<T>> ptr(nullptr);
	if (node == nullptr) return ptr;
	int isleft = IsNodeLeft(node);
	if (isleft == MyTree::nodepos::left) return node->parent->left;
	else if (isleft == MyTree::nodepos::right) return node->parent->right;
	else if (isleft == MyTree::nodepos::central) return node->parent->central;
	else return head;
}

template<typename T>
void MyTree<T>::SwapNodes(MyTreeNode<T>* a, MyTreeNode<T>* b)
{
	if (a == nullptr || b == nullptr || a == b) return;

	auto nodea = GetNodeSharedPtr(a);
	auto nodeb = GetNodeSharedPtr(b);

	auto parenta = GetNodeSharedPtr(a->parent);
	auto lefta = a->left;
	auto righta = a->right;
	auto centrala = a->central;

	auto parentb = GetNodeSharedPtr(b->parent);
	auto leftb = b->left;
	auto rightb = b->right;
	auto centralb = b->central;

	auto islefta = IsNodeLeft(a);
	auto isleftb = IsNodeLeft(b);

	nodea->parent = parentb;
	nodea->left = leftb;
	nodea->right = rightb;
	nodea->central = centralb;
	if (parentb.get() != nullptr)
	{
		if (isleftb == MyTree::nodepos::left) parentb->left = nodea;
		else if (isleftb == MyTree::nodepos::right) parentb->right = nodea;
		else if (isleftb == MyTree::nodepos::central) parentb->central = nodea;
		else head = nodea;
	}
	if (leftb != nullptr) leftb->parent = nodea.get();
	if (rightb != nullptr) rightb->parent = nodea.get();
	if (centralb != nullptr) centralb->parent = nodea.get();


	nodeb->parent = parenta.get();
	nodeb->left = lefta;
	nodeb->right = righta;
	nodeb->central = centrala;
	if (parenta.get() != nullptr)
	{
		if (islefta == MyTree::nodepos::left) parenta->left = nodeb;
		else if (islefta == MyTree::nodepos::right) parenta->right = nodeb;
		else if (islefta == MyTree::nodepos::central) parenta->central = nodeb;
		else head = nodeb;
	}
	if (lefta != nullptr) lefta->parent = nodeb.get();
	if (righta != nullptr) righta->parent = nodeb.get();
	if (centrala != nullptr) centrala->parent = nodeb.get();
}

template<typename T>
void MyTree<T>::Balance(MyTreeNode<T>* node)
{
	if (node == nullptr) return;
	MyTreeNode<T>* nextnodeleft = node->left.get(), *nextnoderight = node->right.get();
	if (node->left != nullptr && node->right == nullptr)
	{
		if (node->left->left != nullptr)
		{
			node->left->right = GetNodeSharedPtr(node);
			node->left->parent = node->parent;

			int isleft = IsNodeLeft(node);
			if (isleft == MyTree::nodepos::left) node->parent->left = node->left;
			else if (isleft == MyTree::nodepos::right) node->parent->right = node->left;
			else head = node->left;

			node->parent = node->left.get();

			node->left = nullptr;

			nextnodeleft = node->parent->left.get();
			nextnoderight = node;
		}
		else if (node->left->right != nullptr)
		{
			node->left->right->right = GetNodeSharedPtr(node);
			node->left->right->parent = node->parent;

			int isleft = IsNodeLeft(node);
			if (isleft == MyTree::nodepos::right) node->parent->right = node->left->right;
			else if (isleft == MyTree::nodepos::left) node->parent->left = node->left->right;
			else head = node->left->right;

			node->parent = node->left->right.get();

			node->left->right->left = node->left;
			node->left->parent = node->left->right.get();

			node->left->right = nullptr;

			node->left = nullptr;

			nextnoderight = node;
			nextnodeleft = node->parent->left.get();
		}
	}
	else if (node->right != nullptr && node->left == nullptr)
	{
		if (node->right->left != nullptr)
		{
			node->right->left->left = GetNodeSharedPtr(node);
			node->right->left->parent = node->parent;

			int isleft = IsNodeLeft(node);
			if (isleft == MyTree::nodepos::left) node->parent->left = node->right->left;
			else if (isleft == MyTree::nodepos::right) node->parent->right = node->right->left;
			else head = node->right->left;

			node->parent = node->right->left.get();

			node->right->left->right = node->right;
			node->right->parent = node->right->left.get();

			node->right->left = nullptr;

			node->right = nullptr;

			nextnodeleft = node;
			nextnoderight = node->parent->right.get();
		}
		else if (node->right->right != nullptr)
		{
			node->right->left = GetNodeSharedPtr(node);
			node->right->parent = node->parent;

			int isleft = IsNodeLeft(node);
			if (isleft == MyTree::nodepos::left) node->parent->left = node->right;
			else if (isleft == MyTree::nodepos::right) node->parent->right = node->right;
			else head = node->right;

			node->parent = node->right.get();

			node->right = nullptr;

			nextnodeleft = node;
			nextnoderight = node->parent->right.get();
		}
	}
	else if (node->left == nullptr && node->parent != nullptr && node->parent->right != nullptr)
	{
		if (node->parent->right->left != nullptr)
		{
			if (node->parent->right->left->left != nullptr)
			{

			}
			else if (node->parent->right->left->right != nullptr)
			{

			}
		}
		else if (node->parent->right->right != nullptr)
		{
			if (node->parent->right->right->left != nullptr)
			{

			}
			else if (node->parent->right->right->right != nullptr)
			{

			}
		}
	}
	else if (node->right == nullptr && node->parent != nullptr && node->parent->left != nullptr)
	{
	if (node->parent->left->left != nullptr)
	{
		if (node->parent->left->left->left != nullptr)
		{

		}
		else if (node->parent->left->left->right != nullptr)
		{

		}
	}
	else if (node->parent->left->right != nullptr)
	{
		if (node->parent->left->right->left != nullptr)
		{

		}
		else if (node->parent->left->right->right != nullptr)
		{

		}
	}
	}
	Balance(nextnodeleft);
	Balance(nextnoderight);
}