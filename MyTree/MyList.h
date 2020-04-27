#pragma once
#include <memory>
#include <ostream>


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
	public:
		void SwapNodes(MyListNode<T> *a, MyListNode<T> *b)noexcept
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
		MyList(const T val) { head = std::make_shared<MyListNode<T>>(); head->value = val; end = head.get(); len = 1; }
		MyList(const MyList<T>& other) { len = other.len; end = other.end; head = other.head; }
		//MyList(const MyList<T> other) { len = other.len; end = other.end; head = other.head; }

		size_t size()const noexcept { return len; }

		bool empty()const noexcept { return len == 0; }

		void push_back(const T val)noexcept
		{
			if (len == 0) { *this = MyList<T>(val); }
			else { end->nextnode = std::make_shared<MyListNode<T>>(); end->nextnode->value = val; end->nextnode->prevnode = end; end = end->nextnode.get(); len++; }
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

		void SortMinMax()noexcept
		{
			if (head == nullptr) return;
			for (size_t i = 0; i < len; i++)
			{
				MyListNode<T> *mink = nullptr, *maxk = nullptr;
				MyListNode<T> *curnode = GetNode(i);
				MyListNode<T> *lastnode = GetNodeR(i);
				MyListNode<T> *curnode2 = curnode;
				T min = curnode2->value, max = lastnode->value;
				bool minchanged = false, maxchanged = false;
				for (size_t k = i; k < len - i; k++) {
					T kval = curnode2->value;
					if (min > kval) { min = kval; mink = curnode2; minchanged = true; }
					if (max < kval) { max = kval; maxk = curnode2; maxchanged = true; }
					curnode2 = curnode2->nextnode.get();
				}
				if (minchanged) SwapNodes(mink,curnode);
				if (maxchanged) SwapNodes(maxk, lastnode);
				/*if (minchanged)//это свап чисто значений из ноудов, а не целиком ноудов
				{
					T tmp = mink->value;
					mink->value = curnode->value;
					curnode->value = tmp;
				}
				if (maxchanged)
				{
					T tmp = maxk->value;
					maxk->value = lastnode->value;
					lastnode->value = tmp;
				}*/
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
	};