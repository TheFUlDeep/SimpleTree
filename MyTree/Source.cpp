#include <iostream>
#include "MyTree.cpp"
#include <list>

using namespace std;

void SortByTree() 
{
	MyTree<int> tree;
	std::list<int> list;
	for (size_t i = 0; i < 100; i++) tree.Add(rand());
	auto a = tree.ToString(' ');
	cout << a << endl;
}

void SortByMinMax()
{
	MyList<int> list;
	for (size_t i = 0; i < 1000; i++) list.push_back(rand() % 1001);
	list.BubbleSort();
	//list.ToString(' ');
	//cout << list.ToString(' ') << "\n\n";
}

int main()
{
	setlocale(LC_ALL, "ru");


	MyTree<int> a;
	cout << "Пустое ли дерево: " << a.IsEmpty() << endl;
	cout << "Количество ноудов: " << a.GetNodesCount() << endl;
	cout << "Высота: " << a.GetHeight() << endl;
	cout << "Cписок всех ключей: " << a.ToString(' ') << endl;
	for (size_t i = 0; i < 15; i++) { int k = rand() %100; a.Add(k); cout << "Вставил " << k << endl; }
	//a.Add(1);a.Add(-1);a.Add(11); a.Add(7); a.Add(5); a.Add(4); a.Add(6); a.Add(9); a.Add(10); a.Add(16); a.Add(13); a.Add(8); a.Add(12); a.Add(14); a.Add(18); a.Add(17); a.Add(19);

	//a.Add(0);
	//add 1); add 2); add 3); add 4); add 5);
	//a.Add(-1); a.Add(-2); a.Add(-3); a.Add(-4); a.Add(-5);

	cout << endl;
	cout << "Пустое ли дерево: " << a.IsEmpty() << endl;
	cout << "Количество ноудов: " << a.GetNodesCount() << endl;
	cout << "Высота: " << a.GetHeight() << endl;

	//cout << "Ноуд с ключом 10: " << a.Find(10) << endl;	
	cout << "Минимальный ключ: " << a.FindMin(a.GetHead())->GetKey() << endl;
	cout << "Макимальный ключ: " << a.FindMax(a.GetHead())->GetKey() << endl;

	cout << "Cписок всех ключей: " << a.ToString(' ') << endl;

	a.PrettyPrint();

	cout << "Удаляю звено с ключом 16" << endl;
	a.RemoveNode(a.Find(16));
	cout << "Количество ноудов: " << a.GetNodesCount() << endl;
	cout << "Высота: " << a.GetHeight() << endl;
	cout << "Cписок всех ключей: " << a.ToString(' ') << endl;
	a.PrettyPrint();

	cout << "\n\n\nПолучение разных звеньев разными способами\n";
	auto b = a.GetHead()->GetLeftNode();
	auto c = a.GetLeftNode(a.GetHead());
	auto d = a.GetHead()->GetRightNode();
	auto f = a.GetRightNode(a.GetHead());
	auto g = a.GetHead()->GetParentNode();
	auto h = a.GetParentNode(a.GetHead());
	cout << b << " " << c << "\t" << d << " " << f << "\t" << g << " " << h << endl;
	


	MyList<int> list;
	//for (size_t i = 0; i < 10; i++) list.push_back(rand() % 1001);
	list.push_back(2); list.push_back(5); list.push_back(2); list.push_back(3); list.push_back(2); list.push_back(1); list.push_back(0); list.push_back(-1);
	list.MinMaxSort();
	cout << list.ToString(' ') << endl;

	SortByTree();
	SortByMinMax();
	SortByMinMax();
	SortByTree();
	SortByTree();


	system("pause");
	return 0;
}