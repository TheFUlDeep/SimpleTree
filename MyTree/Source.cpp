#include <iostream>
#include "MyTree.cpp"

using namespace std;

int main()
{
	setlocale(LC_ALL, "ru");


	MyTree<int> a;
	cout << "Пустое ли дерево: " << a.IsEmpty() << endl;
	cout << "Количество ноудов: " << a.GetNodesCount() << endl;

	for (size_t i = 0; i < 15; i++) { int k = rand() %100; a.Add(k); cout << "Вставил " << k << endl; }
	//a.Add(1);a.Add(-1);a.Add(10); a.Add(7); a.Add(5); a.Add(4); a.Add(6); a.Add(8); a.Add(9); a.Add(11); a.Add(12);
	//a.Add(1); a.Add(2); a.Add(3);a.Add(4); a.Add(5);


	cout << endl;
	cout << "Пустое ли дерево: " << a.IsEmpty() << endl;
	cout << "Количество ноудов: " << a.GetNodesCount() << endl;
	cout << "Высота: " << a.GetHeight() << endl;

	cout << "Ноуд с ключом 10: " << a.Find(10) << endl;	
	cout << "Минимальный ключ: " << a.FindMin()->GetKey() << endl;
	cout << "Макимальный ключ: " << a.FindMax()->GetKey() << endl;

	cout << "Cписок всех ключей: " << a.GetAllNodes() << endl;

	//cout << "Количество элементов в первой строке " << a.NodeLineToList(1).size() << endl;
	a.PrettyPrint();

	//auto ptr = make_shared<int>(10);
	//cout << &*ptr << endl;

	system("pause");
	return 0;
}