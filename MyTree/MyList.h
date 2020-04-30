#pragma once
#include <memory>
#include <ostream>
#include <string>


template<typename T>
class MyList;

template<typename T>
class MyListNode
{
	friend class MyList<T>;
private:
	T value;
	std::shared_ptr<MyListNode<T>> nextnode = nullptr;
	MyListNode<T> *prevnode = nullptr;
public:
	MyListNode() = default;
	MyListNode(T val,  MyListNode<T> *prev = nullptr) { value = val; prevnode = prev; }
	MyListNode<T>* GetNextNode()const { return nextnode.get(); }
	T GetValue()const { return value; }
};


template<typename T>
class MyList
{
private:
	std::shared_ptr<MyListNode<T>> head = nullptr;
	MyListNode<T>* end = nullptr;
	size_t len = 0;

	void SwapNodes(const MyListNode<T> *a, const MyListNode<T> *b)noexcept
	{
		if (a == b || a == nullptr || b == nullptr) return;
		std::shared_ptr<MyListNode<T>> atmp(nullptr); std::shared_ptr<MyListNode<T>> btmp(nullptr);
		MyListNode<T>* preva = a->prevnode; auto nexta = a->nextnode;
		MyListNode<T>* prevb = b->prevnode; auto nextb = b->nextnode;
		if (preva != nullptr) atmp = preva->nextnode; else atmp = head;
		if (prevb != nullptr) btmp = prevb->nextnode; else btmp = head;

		if (nexta.get() == btmp.get())
		{
			if (preva != nullptr) preva->nextnode = btmp;
			if (nexta != nullptr) nexta->prevnode = preva;
			if (prevb != nullptr) prevb->nextnode = nextb;
			if (nextb != nullptr) nextb->prevnode = atmp.get();
			atmp->prevnode = btmp.get();
			btmp->nextnode = atmp;
		}
		else if (nextb.get() == atmp.get())
		{
			if (preva != nullptr) preva->nextnode = nexta;
			if (nexta != nullptr) nexta->prevnode = btmp.get();
			if (prevb != nullptr) prevb->nextnode = atmp;
			if (nextb != nullptr) nextb->prevnode = prevb;
			atmp->nextnode = btmp;
			btmp->prevnode = atmp.get();
		}
		else
		{
			if (preva != nullptr) preva->nextnode = btmp;
			if (nexta != nullptr) nexta->prevnode = btmp.get();
			if (prevb != nullptr) prevb->nextnode = atmp;
			if (nextb != nullptr) nextb->prevnode = atmp.get();
			atmp->prevnode = prevb;
			atmp->nextnode = nextb;
			btmp->prevnode = preva;
			btmp->nextnode = nexta;
		}


		if (atmp.get() == head.get()) head = btmp;
		else if (btmp.get() == head.get()) head = atmp;
		if (atmp.get() == end) end = btmp.get();
		else if (btmp.get() == end) end = atmp.get();
	}

public:
	MyList() = default;
	MyList(const T val) { head = std::make_shared<MyListNode<T>>(val); }
	MyList(const MyList<T> &other) { len = other.len; end = other.end; head = other.head; }
	//MyList(const MyList<T> other) { len = other.len; end = other.end; head = other.head; }

	size_t size()const noexcept { return len; }

	bool empty()const noexcept { return len == 0; }

	void push_back(const T val)noexcept
	{
		if (len == 0) { *this = MyList<T>(val); len++; end = head.get(); }
		else { end->nextnode = std::make_shared<MyListNode<T>>(val,end); end = end->nextnode.get(); len++; }
	}

	T front()const
	{
		if (head == nullptr) throw std::exception("can't pop");
		return head->value;
	}

	void pop_front()noexcept
	{
		if (head == nullptr) return;
		head = head->nextnode;// shared_ptr сам очистит за собой
		len--;
	}

	T& operator[](const size_t n)const { if (n >= len) throw std::exception("wrong index"); return GetNode(n)->value; }


	void MinMaxSort()
	{
		if (head == nullptr) return;
		size_t ourlen = len / 2;
		MyListNode<T> *firstnode = head.get(), *lastnode = end, *curnode = nullptr, *minnode = nullptr, *maxnode = nullptr, *lastnode2 = nullptr;
		bool minchanged = false, maxchanged = false;
		for (size_t i = 0; i < ourlen; i++)
		{
			for (size_t i = 0; i < len; i++) std::cout << operator[](i) << " ";
			std::cout << "\n";

			minchanged = maxchanged = false;
			minnode = curnode = firstnode;
			maxnode = lastnode;
			do
			{
				if (curnode->value < minnode->value)
				{
					minchanged = true;
					minnode = curnode;
				}
				if (curnode->value > maxnode->value)
				{
					maxchanged = true;
					maxnode = curnode;
				}
				curnode = curnode->nextnode.get();
			} while (curnode != lastnode->nextnode.get());


			if (minchanged)
			{
				SwapNodes(minnode, firstnode);
				if (minnode == lastnode) lastnode = firstnode;
				firstnode = minnode;
			}
			if (maxchanged)
			{
				SwapNodes(maxnode, lastnode);
				lastnode = maxnode;
			}
			firstnode = firstnode->nextnode.get();
			lastnode = lastnode->prevnode;
		}
	}

	void BubbleSort()noexcept
	{
		MyListNode<T>* curnode = head.get();
		while (curnode != nullptr)
		{
			MyListNode<T>* curnode2 = curnode->nextnode.get();
			while (curnode2 != nullptr)
			{
				if (curnode->value > curnode2->value)
				{
					//свап только значений
					//T tmp = curnode->value;
					//curnode->value = curnode2->value;
					//curnode2->value = tmp;
					//свап ноудов
					SwapNodes(curnode, curnode2);
					auto tmp = curnode; curnode = curnode2; curnode2 = tmp;//обратно свапаю локальные указатели, так как ноуды поменялись местами, а мне надо идти по порядку
				}
				curnode2 = curnode2->nextnode.get();
			}
			curnode = curnode->nextnode.get();
		}
	}

	void RecursiveSort()//съедает стэк и может вызвать stackoverflow
	{
		MyListNode<T>* curnode = head.get();
		MyListNode<T>* prevnode = nullptr;
		while (curnode->nextnode != nullptr)
		{
			prevnode = curnode;
			curnode = curnode->nextnode.get();
			if (prevnode->value > curnode->value) 
			{
				T tmp = prevnode->value;
				prevnode->value = curnode->value;
				curnode->value = tmp;
				RecursiveSort();
			}
		}
	}


	MyListNode<T>* GetHead()const noexcept { return head.get(); }

	MyListNode<T>* GetNode(const size_t n)const noexcept
	{
		auto curnode = head;
		if (curnode == nullptr) return nullptr;
		for (size_t i = 0; i < n; i++)//буду проходить на 1 меньше указанного индекса
		{
			curnode = curnode->nextnode;
			if (curnode == nullptr) return nullptr;
		}
		return curnode.get();
	}

	MyListNode<T>* GetNodeR(const size_t n)const noexcept
	{
		auto curnode = end;
		if (curnode == nullptr) return nullptr;
		for (size_t i = 0; i < n; i++)//буду проходить на 1 меньше указанного индекса
		{
			curnode = curnode->prevnode;
			if (curnode == nullptr) return nullptr;
		}
		return curnode;
	}

	std::string ToString(const char &symbol)const noexcept
	{
		std::string str = "";
		if (head == nullptr) return str;
		MyListNode<T> *curnode = head.get();
		str += std::to_string(curnode->value);
		curnode = curnode->nextnode.get();
		while (curnode != nullptr)
		{
			str += symbol + std::to_string(curnode->value);
			curnode = curnode->nextnode.get();
		}
		return str;
	}
};