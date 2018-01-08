#pragma once

#include <iomanip>
#ifndef Router_H
#define Router_H
using namespace std;
const int maxWeight = INT_MAX;
const int DeafultVertices = 30;			//代表最大顶点数
template<class T>
struct Edge
{
	int dest;			//边的另一个顶点位置
	int cost;				//边上的权值
	Edge<T> * link;	//下一条边链指针
	Edge() {}
	Edge(int num, int weight) : dest(num), cost(weight), link(NULL) {}
	bool operator != (Edge<T>& R)const
	{
		return (dest != R.dest) ? true : false;
	}
};
template<class T>
struct Vertex
{
	T data;
	Edge<T> *adj;			//边链表的头指针
	Vertex() :adj(NULL) { }
};


template<class T>
class Router // public Graph<T>
{
public:
	Router(int sz = DeafultVertices);
	~Router();
	T getValue(int i)					//取位置为i的顶点中的值
	{
		return (i >= 0 && i< numVertices) ? NodeTable[i].data : 0;
	}
	int getWeight(int v1, int v2);			//返回边(v1,v2)的权值
	int NumberOfVertices() { return numVertices; }		//返回当前顶点数
	int NunberOfEdges() { return numEdges; }			//返回当前边数
	bool insertVertex(const T vertex);			//插入一个顶点vertex	
	bool removeVertex(T v);//删去顶点v和所有与v相关的边
	bool insertEdge(int v1, int v2, int cost);//插入边(v1,v2),权为cost
	bool removeEdge(T v1, T v2);//删去边(v1,v2)
	void ShortestPath(int v, int *dist, int *path);//找下一跳
	int getVertexPos(const T vertex)
	{
		for (int i = 0; i< numVertices; i++)
			if (NodeTable[i].data == vertex)
				return i;
		return -1;
	}
private:
	int maxVertices;
	int numEdges;
	int numVertices;
	Vertex<T> * NodeTable;
};
template<class T>
Router<T>::Router(int sz /* = DeafultVertices */)
{
	maxVertices = sz;
	numVertices = 0;
	numEdges = 0;
	NodeTable = NULL;

	try
	{
		NodeTable = new Vertex<T>[maxVertices];
	}
	catch (bad_alloc & memExp)
	{
		cerr << memExp.what() << endl;
	}

	for (int i = 0; i<maxVertices; i++)
		NodeTable[i].adj = NULL;
}
template<class T>
Router<T>::~Router()
{
	for (int i = 0; i<maxVertices; i++)
	{
		Edge<T> * p = NodeTable[i].adj;
		while (p != NULL)
		{
			NodeTable[i].adj = p->link;
			delete p;
			p = NodeTable[i].adj;
		}
	}
	delete[] NodeTable;
}
template<class T>
int Router<T>::getWeight(int v1, int v2)
{
	if (v1 >= 0 && v1 < numVertices && v2 >= 0 && v2 < numVertices && v1 != v2)
	{
		Edge<T> *p = NodeTable[v1].adj;
		while (p != NULL && p->dest != v2)
		{
			p = p->link;
		}
		if (p != NULL)
			return p->cost;
		else
			return maxWeight;
	}
	else
		if (v1 == v2)
			return 0;
		else return -1;
}
template<class T>
bool Router<T>::insertVertex(const T vertex)
{
	if (getVertexPos(vertex) >= 0) return false;
	if (numVertices == maxVertices) return false;
	NodeTable[numVertices].data = vertex;
	numVertices++;
	return true;
}
template<class T>
bool Router<T>::removeVertex(T x)
{
	int v = getVertexPos(x);
	if (numVertices == 1 || v<0 || v >= numVertices)return false;
	Edge<T> *p, *s, *t;
	int k;
	while (NodeTable[v].adj != NULL)
	{
		p = NodeTable[v].adj;
		k = p->dest;
		s = NodeTable[k].adj;
		t = NULL;
		while (s != NULL && s->dest != v)
		{
			t = s;
			s = s->link;
		}
		if (s != NULL)
		{
			if (t == NULL) NodeTable[k].adj = s->link;
			else t->link = s->link;
			delete s;
		}
		NodeTable[v].adj = p->link;
		delete p;
		numEdges--;
	}
	numVertices--;
	NodeTable[v].data = NodeTable[numVertices].data;
	p = NodeTable[v].adj = NodeTable[numVertices].adj;
	while (p != NULL)
	{
		s = NodeTable[p->dest].adj;
		while (s != NULL)
			if (s->dest == numVertices) {
				s->dest = v;
				break;
			}
			else s = s->link;
			p = p->link;
	}
	return true;
}
template<class T>
bool Router<T>::insertEdge(int v1, int v2, int cost)
{
	if (v1 >= 0 && v1< numVertices && v2 >= 0 && v2<numVertices && v1 != v2)
	{
		Edge<T> *q, *p = NodeTable[v1].adj;
		while (p != NULL && p->dest != v2)
		{
			p = p->link;
		}
		if (p != NULL)
		{
			p->cost = cost;
			return true;
		}
		p = new Edge<T>;
		q = new Edge<T>;
		p->dest = v2;
		p->cost = cost;
		p->link = NodeTable[v1].adj;
		NodeTable[v1].adj = p;
		q->dest = v1;
		q->cost = cost;
		q->link = NodeTable[v2].adj;
		NodeTable[v2].adj = q;
		numEdges++;
		return true;
	}
	return false;
}
template<class T>
bool Router<T>::removeEdge(T x1, T x2)
{
	int v1 = getVertexPos(x1);
	int v2 = getVertexPos(x2);
	if (v1 >= 0 && v1 < numVertices && v2 >= 0 && v2 < numVertices && v1 != v2)
	{
		Edge<T> *p = NodeTable[v1].adj, *q = NULL, *s = p;
		while (p != NULL && p->dest != v2)
		{
			q = p;
			p = p->link;
		}
		if (p != NULL)
		{
			if (p == s)
				NodeTable[v1].adj = p->link;
			else
			{
				q->link = p->link;
			}
			delete p;
		}
		else
		{
			return false;
		}
		p = NodeTable[v2].adj;
		q = NULL;
		s = p;
		while (p->dest != v1)
		{
			q = p;
			p = p->link;
		}
		if (p == s)
			NodeTable[v2].adj = p->link;
		else
		{
			q->link = p->link;
		}
		delete p;
		return true;
	}
	return false;
}
template<class T>
void Router<T>::ShortestPath(int v, int *dist, int *path)
{
	int n = numVertices;
	bool *S = new bool[n];          //最短路径定点集
	int w, min;
	for (int i = 0; i<n; i++)
	{
		dist[i] = getWeight(v, i);   //数组初始化
		S[i] = false;
		if (i != v && dist[i] < maxWeight)
			path[i] = i;
		else path[i] = -1;
	}
	S[v] = true; dist[v] = 0;           //顶点v加入顶点集合 
	for (int i = 0; i<n - 1; i++)
	{
		min = maxWeight; int u = v;      //选不在S中具有最短路径的顶点u（选出最短的那个 
		for (int j = 0; j<n; j++)
			if (S[j] == false && dist[j] < min)
			{
				u = j;
				min = dist[j];
			}
		S[u] = true;               //将顶点u加入集合S
		for (int k = 0; k<n; k++)          //修改 
		{
			w = getWeight(u, k);
			if (S[k] == false && w < maxWeight && dist[u] + w<dist[k])//顶点k未加入S，且绕过u可以缩短路径 
			{
				dist[k] = dist[u] + w;
				path[k] = path[u];             //修改到k的最短路径 
			}
		}
	}
}
#endif