#include "MyTree.h"

template<typename T>
MyTreeNode<T>::MyTreeNode(const T key, MyTreeNode<T> *prnt, const std::shared_ptr<MyTreeNode<T>> &lft, const std::shared_ptr<MyTreeNode<T>> &rght)
{
	parent = prnt; left = lft; right = rght; this->key = key;
}

template<typename T>
MyTreeNode<T>::~MyTreeNode() = default;


template<typename T>
T& MyTreeNode<T>::GetKey() { return key; }


template<typename T>
MyTree<T>::MyTree() = default;


template<typename T>
MyTree<T>::~MyTree() = default;


template<typename T>
MyTree<T>& MyTree<T>::operator=(const MyTree<T> &other) noexcept { nodesCount = other->nodesCount; head = other.head; }//TODO возможно shared_ptr сам нормально скопируется. Надо проверить


template<typename T>
bool MyTree<T>::IsEmpty()const noexcept { return nodesCount == 0; }


template<typename T>
size_t MyTree<T>::GetNodesCount()const { return nodesCount; }


template<typename T>
void MyTree<T>::Add(T k)noexcept
{
	if (head == nullptr) head = std::make_shared<MyTreeNode<T>>(k);
	else
	{
		MyTreeNode<T> *curnode = head.get();
		MyTreeNode<T> *prevnode = nullptr;
		while (curnode != nullptr)
		{
			prevnode = curnode;
			if (k == curnode->key) return;//если такой ключ уже есть, то пропускается
			else if (k < curnode->key) curnode = curnode->left.get();
			else curnode = curnode->right.get();
		}

		if (k < prevnode->key) prevnode->left = std::make_shared<MyTreeNode<T>>(k,prevnode);
		else prevnode->right = std::make_shared<MyTreeNode<T>>(k, prevnode);
	}
	nodesCount++;
}


template<typename T>
MyTreeNode<T>* MyTree<T>::RecursiveFind(const T k, MyTreeNode<T>* node)const
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
MyTreeNode<T>* MyTree<T>::Find(const T k)const noexcept { return RecursiveFind(k,head.get()); }


template<typename T>
size_t MyTree<T>::RecursiveGetHeight(const MyTreeNode<T>* node)const
{
	if (node == nullptr) return 0;
	return 1 + std::max(RecursiveGetHeight(node->left.get()), RecursiveGetHeight(node->right.get()));
}


template<typename T>
size_t MyTree<T>::GetHeight()const noexcept{ return RecursiveGetHeight(head.get()); }

template<typename T>
MyTreeNode<T>* MyTree<T>::GetHead() const noexcept { return head.get(); }


template<typename T>
void MyTree<T>::RemoveNode(const MyTreeNode<T>* node)noexcept
{
	if (node == nullptr) return;

	MyTreeNode<T> *nodeleft = node->left.get();
	MyTreeNode<T> *noderight = node->right.get();

	if (node->parent != nullptr)//если есть роидетль (если это не голова)
	{
		MyTreeNode<T> *parentnode = node->parent;

		bool parentleft = parentnode->left.get() == node; //определяю, этот ноуд слева от родителя или справа

		if (nodeleft == nullptr && noderight == nullptr) //если нет листьев
		{
			if (parentleft) parentnode->left = nullptr;
			else parentnode->right = nullptr;
		}
		else if (nodeleft != nullptr && noderight == nullptr)//если есть только левый ноуд
		{
			if (parentleft) parentnode->left = node->left;
			else parentnode->right = node->left;
		}
		else if (nodeleft == nullptr && noderight != nullptr)//если есть только правый ноуд
		{
			if (parentleft) parentnode->left = node->right;
			else parentnode->right = node->right;
		}
		else//если есть оба ноуда 
		{
			//вместо удаленного ноуда вставляю левый. Но можно было правый. 

			auto rightnode = node->right;

			if (parentleft) parentnode->left = node->left;
			else parentnode->right = node->left;

			MyTreeNode<T> *curnode = parentnode;
			while (curnode->right != nullptr) curnode = curnode->right.get();
			curnode->right = rightnode;
		}
	}
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
			MyTreeNode<T> *curnode = head.get();
			while (curnode->right != nullptr) curnode = curnode->right.get();
			curnode->right = rightnode;
		}
	}
	nodesCount--;
}


template<typename T>
MyTreeNode<T>* MyTree<T>::FindMin()const noexcept
{
	if (head == nullptr) return nullptr;
	MyTreeNode<T> *curnode = head.get();
	while (curnode->left != nullptr) curnode = curnode->left.get();
	return curnode;
}


template<typename T>
MyTreeNode<T>* MyTree<T>::FindMax()const noexcept
{
	if (head == nullptr) return nullptr;
	MyTreeNode<T> *curnode = head.get();
	while (curnode->right != nullptr) curnode = curnode->right.get();
	return curnode;
}

template<typename T>
MyTreeNode<T>* MyTree<T>::GetParentNode(const MyTreeNode<T>* node) const noexcept
{
	if (node == nullptr) return nullptr;
	return node->parent;
}

template<typename T>
MyTreeNode<T>* MyTree<T>::GetLeftNode(const MyTreeNode<T>* node) const noexcept
{
	if (node == nullptr) return nullptr;
	return node->left.get();
}

template<typename T>
MyTreeNode<T>* MyTree<T>::GetRightNode(const MyTreeNode<T>* node) const noexcept
{
	if (node == nullptr) return nullptr;
	return node->right.get();
}


template<typename T>
MyList<T> MyTree<T>::AllKeysToListRerusive(const MyTreeNode<T>* node, MyList<T>* lst)const
{
	if (lst == nullptr) lst = &MyList<T>();
	if (node == nullptr) return *lst;
	lst->push_back(node->key);
	AllKeysToListRerusive(node->right.get(),lst);
	AllKeysToListRerusive(node->left.get(),lst);
	return *lst;
}


template<typename T>
std::string MyTree<T>::GetAllNodes()const noexcept
{
	std::string str = "";
	MyList<T> lst = AllKeysToListRerusive(head.get());
	lst.BubbleSort();
	while (!lst.empty())
	{
		str += (" " + std::to_string(lst.front()));
		lst.pop_front();
	}
	return str;
}


template<typename T>
void MyTree<T>::PrettyPrintRecursive(const size_t h, MyList<MyTreeNode<T>*> prevline, size_t linenumber, size_t maxlen)const
{

	if (head == nullptr) return;
	if (prevline.empty()) prevline.push_back(head.get());
	MyList<MyTreeNode<T>*> curlist;

	bool needDeeper = false;

	//первая позиция, сдвиг, количество символов
	//size_t symbolscount = pow(2,linenumber-1);

	//это надо считать только 1 раз
	if (maxlen == 0)
	{
		size_t maxlenmax = std::to_string(FindMax()->key).length();
		size_t maxlenmin = std::to_string(FindMin()->key).length();
		maxlenmax > maxlenmin ? maxlen = maxlenmax : maxlen = maxlenmin;
	}

	//этот стринг тоже надо бы только 1 раз считать, но я не хочу добавлять еще один аргумент
	std::string spacestr = "";
	for (size_t i = 0; i < maxlen; i++) spacestr += " ";

	size_t firstpos = (size_t)pow(2, h - linenumber);
	size_t space = firstpos * 2;


	for (size_t i = 1; i < firstpos; i++) std::cout << spacestr;//начальный сдвиг
	while (!prevline.empty())
	{
		auto val = prevline.front();
		if (val == nullptr) { prevline.pop_front(); for (size_t i = 1; i < space + 1; i++) std::cout << spacestr; curlist.push_back(nullptr); curlist.push_back(nullptr); continue; }
		std::cout << val->key;
		size_t vallen = std::to_string(val->key).length();
		if (vallen < maxlen) for (size_t i = 0; i < maxlen - vallen; i++) std::cout << " ";
		for (size_t i = 1; i < space; i++) std::cout << spacestr;
		if (val->left != nullptr || val->right != nullptr) needDeeper = true;
		curlist.push_back(val->left.get());
		curlist.push_back(val->right.get());
		prevline.pop_front();
	}
	for (size_t i = 0; i < firstpos; i++) std::cout << "\n";

	if (needDeeper) PrettyPrintRecursive(h,curlist,linenumber+1);
}


template<typename T>
void MyTree<T>::PrettyPrint() const noexcept
{
	PrettyPrintRecursive(GetHeight());
}