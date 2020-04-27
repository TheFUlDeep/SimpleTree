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
	MyTreeNode<T> *parent;
	T key;

public:
	MyTreeNode(const T, MyTreeNode<T>* = nullptr, const std::shared_ptr<MyTreeNode<T>>& = nullptr, const std::shared_ptr<MyTreeNode<T>>& = nullptr);
	T& GetKey();
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
	void PrettyPrintRecursive(const size_t, MyList<MyTreeNode<T>*> = MyList<MyTreeNode<T>*>(), size_t linenumber = 1, size_t = 0)const;

public:
	MyTree();
	~MyTree();
	//MyTree& operator=(MyTree&&) = default;

	MyTree<T>& operator=(const MyTree<T>&);

	void Add(T);
	//void InsertToLeft(const MyTreeNode<T>*);
	//void InsertToRight(const MyTreeNode<T>*);

	MyTreeNode<T>* Find(const T)const;

	void RemoveNode(const MyTreeNode<T>*);

	MyTreeNode<T>* FindMin()const;
	MyTreeNode<T>* FindMax()const;

	MyTreeNode<T>* GetParentNode(const MyTreeNode<T>*)const;
	MyTreeNode<T>* GetLeftNode(const MyTreeNode<T>*)const;
	MyTreeNode<T>* GetRightNode(const MyTreeNode<T>*)const;

	std::string GetAllNodes()const;


	bool IsEmpty()const;
	size_t GetNodesCount()const;
	size_t GetHeight()const;


	void PrettyPrint()const;
};