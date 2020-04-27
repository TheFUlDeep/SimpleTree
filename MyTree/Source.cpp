#include <iostream>
#include "MyTree.cpp"

using namespace std;

int main()
{
	setlocale(LC_ALL, "ru");


	MyTree<int> a;
	cout << "Пустое ли дерево: " << a.IsEmpty() << endl;
	cout << "Количество ноудов: " << a.GetNodesCount() << endl;
	cout << "Высота: " << a.GetHeight() << endl;
	//for (size_t i = 0; i < 15; i++) { int k = rand() %100; a.Add(k); cout << "Вставил " << k << endl; }
	a.Add(1);a.Add(-1);a.Add(11); a.Add(7); a.Add(5); a.Add(4); a.Add(6); a.Add(9); a.Add(10); a.Add(16); a.Add(13); a.Add(8); a.Add(12); a.Add(14); a.Add(18); a.Add(17); a.Add(19);

	a.Add(1); a.Add(1); a.Add(1); a.Add(1); a.Add(1);//ничего не произойдет


	cout << endl;
	cout << "Пустое ли дерево: " << a.IsEmpty() << endl;
	cout << "Количество ноудов: " << a.GetNodesCount() << endl;
	cout << "Высота: " << a.GetHeight() << endl;

	//cout << "Ноуд с ключом 10: " << a.Find(10) << endl;	
	cout << "Минимальный ключ: " << a.FindMin()->GetKey() << endl;
	cout << "Макимальный ключ: " << a.FindMax()->GetKey() << endl;

	cout << "Cписок всех ключей: " << a.GetAllNodes() << endl;

	a.PrettyPrint();

	cout << "Удаляю звено с ключом 16" << endl;
	a.RemoveNode(a.Find(16));
	cout << "Количество ноудов: " << a.GetNodesCount() << endl;
	cout << "Высота: " << a.GetHeight() << endl;
	cout << "Cписок всех ключей: " << a.GetAllNodes() << endl;
	a.PrettyPrint();


	system("pause");
	return 0;
}