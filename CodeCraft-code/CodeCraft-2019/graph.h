#include<vector>
#include <algorithm>
#include<stack>
using namespace std;
#define MAX_INT 6553555

class Graph
{
public:
	Graph(vector<cross> cross_vector,vector<road> road_vector);
	~Graph();
	void generate_map_graph(vector<int> graph_index);
	void reset_map();
	string Dijkstra(int v, int dest);

	
	vector<cross> _cross_vector;
	vector<road> _road_vector;
	int _cross_num;
	vector<vector<int>> graph_array;//邻接矩阵
	vector<vector<int>> original_graph_array;
private:

};

Graph::Graph(vector<cross> cross_vector, vector<road> road_vector)
{
	_cross_vector = cross_vector;
	_road_vector = road_vector;
	_cross_num = cross_vector.size();
}

Graph::~Graph()
{
}

void Graph::reset_map()
{
	graph_array.assign(original_graph_array.begin(),original_graph_array.end());
}

void Graph::generate_map_graph(vector<int >graph_index)
{
	
	int road_index,cross_index_1, cross_index_2,distance;
	int road_vector_size = _road_vector.size();
	int cross_vector_size = _cross_vector.size();
	int duplex_flag;
	graph_array.resize(cross_vector_size, vector<int>(cross_vector_size));
	for (cross_index_1 = 0; cross_index_1 < cross_vector_size; cross_index_1++)
	{
		for (cross_index_2 = 0; cross_index_2 < cross_vector_size; cross_index_2++)
		{
			if (cross_index_1 == cross_index_2)
				graph_array[cross_index_1][cross_index_2] = 0;
			else
				graph_array[cross_index_1][cross_index_2] = MAX_INT;
		}
	}

	

	for (road_index = 0; road_index < road_vector_size; road_index++)
	{
		for (int find_index = 0; find_index < graph_index.size(); find_index++)// find cross index in graph
		{
			if (_road_vector[road_index].get_road_from() == graph_index[find_index])
				cross_index_1 = find_index;
			if (_road_vector[road_index].get_road_to() == graph_index[find_index])
				{cross_index_2 = find_index;}
		}
		duplex_flag = _road_vector[road_index].get_duplexflag();
		distance = _road_vector[road_index].get_road_length();
		
		graph_array[cross_index_1][cross_index_2] = distance;
		if (duplex_flag)
		{
			graph_array[cross_index_2][cross_index_1] = distance;
		}
	}
	original_graph_array.assign(graph_array.begin(),graph_array.end());
	
	/*
	
	for (int i = 0; i < cross_vector_size; i++)
	{
		for (int j = 0; j < cross_vector_size; j++)
			cout << graph_array[i][j]<<" ";
		cout << endl;
	}
	*/
	// cout << "generate map finished!" << endl;
}
string Graph::Dijkstra(int v, int dest)
{
	vector<int> dist;
	vector<string> path;
	vector<int> s;
	vector<bool>Final;
	dist.resize(_cross_num);
	path.resize(_cross_num);
	Final.resize(_cross_num);

	for (int i = 0; i < _cross_num; i++)
	{
		Final[i] = false;
		dist[i] = graph_array[v][i];
		if (dist[i] != MAX_INT)
			path[i] = to_string(v) + " " + to_string(i);
		else
			path[i] = " ";
	}
	s.push_back(v); // 初始化s
	Final[v] = true;
	int num = 1;
	while (num < _cross_num)
	{
		// 在dist中查找最小值元素
		int k = 0, min = MAX_INT;
		for (int i = 0; i <_cross_num; i++)
		{
			if (i == v)continue;
			if (!Final[i] && dist[i] < min)
			{
				k = i;
				min = dist[i];
			}
		}
		s.push_back(k);// 将新生成的结点加入集合s
		num++;
		Final[k] = true;
		// 修改dist和path数组
		for (int i = 0; i < _cross_num; i++)
		{
			if (!Final[i] && dist[i] > dist[k] + graph_array[k][i])
			{
				dist[i] = dist[k] + graph_array[k][i];
				path[i] = path[k] + " " + to_string(i);
			}
		}
	}
	return  path[dest];
}
