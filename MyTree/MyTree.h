#pragma once
#include <memory>
#include <string>
#include <algorithm>
#include "MyList.h"


template<typename T>
class MyTree;

template<typename T>
class MyTreeNode
{
	friend class MyTree<T>;

private:
	std::shared_ptr<MyTreeNode<T>> left;
	std::shared_ptr<MyTreeNode<T>> right;
	MyTreeNode<T> *parent;//обычный указатель для того, чтобы 2 shared_ptr'a не ссылались друга на друга
	T key;

public:
	MyTreeNode(const T, MyTreeNode<T>* = nullptr, const std::shared_ptr<MyTreeNode<T>>& = nullptr, const std::shared_ptr<MyTreeNode<T>>& = nullptr);
	~MyTreeNode();
	T& GetKey();

	MyTreeNode<T>* GetParentNode()const noexcept;
	MyTreeNode<T>* GetLeftNode()const noexcept;
	MyTreeNode<T>* GetRightNode()const noexcept;
};



template<typename T>
class MyTree
{
private:
	std::shared_ptr<MyTreeNode<T>> head = nullptr;
	size_t nodesCount = 0;
	
	//поиск по ключу, но только ниже указателя, который передается вторым аргументом
	MyTreeNode<T>* RecursiveFind(const T, MyTreeNode<T>* = nullptr)const;
	size_t RecursiveGetHeight(const MyTreeNode<T>* = nullptr)const;
	//std::shared_ptr<T*> AllNodesToArray();
	MyList<T> AllKeysToListRerusive(const MyTreeNode<T>* = nullptr, MyList<T>* = nullptr)const;
	void PrettyPrintRecursive(const size_t*, const size_t*, const char*, MyList<MyTreeNode<T>*> = MyList<MyTreeNode<T>*>(), size_t linenumber = 1)const;

public:
	MyTree();
	~MyTree();
	//MyTree& operator=(MyTree&&) = default;

	MyTree<T>& operator=(const MyTree<T>&)noexcept;

	void Add(T)noexcept;
	//void InsertToLeft(const MyTreeNode<T>*);
	//void InsertToRight(const MyTreeNode<T>*);

	MyTreeNode<T>* Find(const T)const noexcept;

	void RemoveNode(const MyTreeNode<T>*) noexcept;

	MyTreeNode<T>* FindMin(MyTreeNode<T>*)const noexcept;//аргумент не константный патамушта
	MyTreeNode<T>* FindMax(MyTreeNode<T>*)const noexcept;//аргумент не константный патамушта

	std::string GetAllNodes()const noexcept;

	MyTreeNode<T>* GetParentNode(const MyTreeNode<T>*)const noexcept;
	MyTreeNode<T>* GetLeftNode(const MyTreeNode<T>*)const noexcept;
	MyTreeNode<T>* GetRightNode(const MyTreeNode<T>*)const noexcept;

	bool IsEmpty()const noexcept;
	size_t GetNodesCount()const;
	size_t GetHeight()const noexcept;
	MyTreeNode<T>* GetHead()const noexcept;


	void PrettyPrint()const noexcept;
};