#include <iostream>
#include "MyTree.cpp"

using namespace std;

int main()
{
	setlocale(LC_ALL, "ru");


	MyTree<int> a;
	cout << "������ �� ������: " << a.IsEmpty() << endl;
	cout << "���������� ������: " << a.GetNodesCount() << endl;

	for (size_t i = 0; i < 15; i++) { int k = rand() %100; a.Add(k); cout << "������� " << k << endl; }
	//a.Add(1);a.Add(-1);a.Add(10); a.Add(7); a.Add(5); a.Add(4); a.Add(6); a.Add(8); a.Add(9); a.Add(11); a.Add(12);
	//a.Add(1); a.Add(2); a.Add(3);a.Add(4); a.Add(5);


	cout << endl;
	cout << "������ �� ������: " << a.IsEmpty() << endl;
	cout << "���������� ������: " << a.GetNodesCount() << endl;
	cout << "������: " << a.GetHeight() << endl;

	cout << "���� � ������ 10: " << a.Find(10) << endl;	
	cout << "����������� ����: " << a.FindMin()->GetKey() << endl;
	cout << "����������� ����: " << a.FindMax()->GetKey() << endl;

	cout << "C����� ���� ������: " << a.GetAllNodes() << endl;

	//cout << "���������� ��������� � ������ ������ " << a.NodeLineToList(1).size() << endl;
	a.PrettyPrint();

	//auto ptr = make_shared<int>(10);
	//cout << &*ptr << endl;

	system("pause");
	return 0;
}