#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include<fstream>
using namespace std;
#include "Router.h"
const int MaxVertices = 100;

template<class T>
void Print(Router<T> &G, int v)
{
	int n = G.NumberOfVertices();
	int  *path = new int[n];
	int  *dist = new int[n];
	cout << "顶点个数为:" << G.NumberOfVertices() << endl;
	cout << "边的条数为:" << G.NunberOfEdges() << endl;
	cout << "结点集合为:";
	for (int i = 0; i< G.NumberOfVertices(); i++)
		cout << " R" << G.getValue(i);
	cout << endl;
	G.ShortestPath(v, dist, path);
	for (int i = 0; i < G.NumberOfVertices(); i++)
	{
		if (i != v)
			cout << " 目的路由器 ： R" << G.getValue(i) << "      下一跳 ：R" << path[i] << endl;
	}
}
template<class T>
void Remove(Router<T> &G)
{
	cout << "  删除顶点-1，删除边-2   :";
	int i;
	cin >> i;
	if (i == 1) {
		cout << endl << "输入删除的顶点（即路由器序号）：";
		int v;
		cin >> v;
		if (G.removeVertex(G.getVertexPos(v)))
			return;
	}
	else {
		cout << endl << "输入删除的边（即哪两个路由器序号，用空格分开）：";
		int v1, v2;
		cin >> v1 >> v2;
		if (G.removeEdge(G.getVertexPos(v1), G.getVertexPos(v2)))
			return;
	}

}


int main() {
	Router<int> g1;
	ifstream fin1("D:\\vertex.txt");
	int v;
	while (!fin1.eof()) {
		fin1 >> v;
		g1.insertVertex(v);
	}
	ifstream fin2("D:\\edge.txt");
	int v1, v2, e;
	while (!fin2.eof())
	{
		fin2 >> v1;
		fin2 >> v2;
		fin2 >> e;
		g1.insertEdge(v1, v2, e);
	}
	fin2.close();
	fin1.close();
	int  vv;
	cout << "请输入路由器" << endl;
	cin >> vv;
	int pos = g1.getVertexPos(vv);
	Print(g1, pos);
	Remove(g1);
	Print(g1, pos);
	return 0;
}


