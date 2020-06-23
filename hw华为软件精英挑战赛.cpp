#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <time.h>
#include <string>
#include <unordered_map>
#include <stack>
#include <iostream>
#include <algorithm>
#include <vector>

#define Test

using namespace std;


class Cycle{
public:
	unordered_map<int, vector<int>> Graph;
	unordered_map<int, unordered_map<int, vector<int>>> my_cycle2;
	unordered_map<int, int> temp;
	unordered_map<int, int> temp_k;
	unordered_map<int, int> temp_i;
	unordered_map<int, int> dfn;
	unordered_map<int, int> low;
	unordered_map<int, vector<int>> belong;
	unordered_map<int, bool> instack;
	unordered_map<int, bool> visited;
	vector<vector<int>> result3;
	vector<vector<int>> result4;
	vector<vector<int>> result5;
	vector<vector<int>> result6;
	vector<vector<int>> result7;
	stack<int> s;
	int count_a;
	int count_b;
	int index;
	int total;
	int look;
	
	void load_data(string &test_data)
	{
#ifdef Test
		auto t1 = clock();
#endif
		FILE * fp = NULL;
		fp = fopen(test_data.c_str(), "r");
		int u, v, c;
		int count = 0;//交易次数
		while (fscanf(fp, "%d,%d,%d", &u, &v, &c) != EOF){
			Graph[u].push_back(v);
			++count;// It can be deleted.
		}
		/*
		unordered_map<int, vector<int>>::iterator it = Graph.begin();
		while (it != Graph.end()) {
			
			cout << it->first << ":";

			for (size_t i = 0; i < it->second.size(); i++)
			{
				cout << it->second[i] << "; ";
			}
			cout << endl;
			it++;
		}
		for (vector<int>::iterator it = Graph[30000].begin(); it <Graph[30000].end(); it++){
			cout << *it << " ";
		}
		sort(edge[30000].begin(), edge[30000].end());*/

#ifdef Test
		cout << count << endl;  // it can be deleted.
		cout << "load_data:" << clock() - t1 << "ms" << endl;
#endif
	}


	void tarjan1()
	{
		int key = temp[index];
		count_a++;
		dfn[key] = count_a;
		low[key] = count_a;
		s.push(key);
		instack[key] = true;
		visited[key] = true;
		if (Graph.count(key) != 0)
		{
			for (int i = 0; i < Graph[key].size();i++)
			{
				int v = Graph[key][i];
				if ((visited.count(v) == 0) || (!visited[v]))
				{
					temp_k[index] = 1;
					temp_i[index] = i;
					index++;
					temp[index] = v;
					temp_k[index] = 0;
					return;
				}
				else if (instack[v]) low[key] = min(low[key], dfn[v]);
			}
		}
		if (dfn[key] == low[key])
		{
			count_b++;
			int node;
			do{
				node = s.top();
				s.pop();
				instack[node] = false;
				belong[count_b].push_back(node);


			} while (node != key);
		}
		index--;
	}

	void tarjan2()
	{
		int key = temp[index];
		low[key] = min(low[key], low[Graph[key][temp_i[index]]]);
		if (Graph.count(key) != 0)
		{
			for (int i = temp_i[index]+1; i < Graph[key].size(); i++)
			{
				int v = Graph[key][i];
				if ((visited.count(v) == 0) || (!visited[v]))
				{
					temp_k[index] = 1;
					temp_i[index] = i;
					index++;
					temp[index] = v;
					temp_k[index] = 0;
					return;
				}
				else if (instack[v]) low[key] = min(low[key], dfn[v]);
			}
		}
		if (dfn[key] == low[key])
		{
			count_b++;
			int node;
			do{
				node = s.top();
				s.pop();
				instack[node] = false;
				belong[count_b].push_back(node);


			} while (node != key);
		}
		index--;
		
	}

	void tarjan(int key)
	{
		index++;
		temp[index] = key;
		temp_k[index] = 0;
		temp_i[index] = 0;
		while (index > 0)
		{

			if (temp_k[index] == 0){
				tarjan1();
			}
			else{
				tarjan2();

			}
		}
	}

	void find()
	{
		tmp_cycle();
#ifdef Test
		double time = 0.0;
		clock_t start, end;
		start = clock(); //强连通找环时间：
#endif
		unordered_map<int, vector<int>>::iterator it = Graph.begin();
		while (it != Graph.end())
		{
			int key = it->first;
			if ((visited.count(key) != 0) && visited[key]){it++; continue; } //it's has a notes.
			
			tarjan(key);
			it++;
		}
#ifdef Test
		end = clock();

		printf("the time of finding SCG is %lfms\n", end - time);


		auto t2 = clock();
#endif

		for (auto rt = belong.begin(); rt != belong.end();rt++)
		{
			int key = rt->first;
			if (belong[key].size() < 3) { continue; }
			sort(belong[key].begin(), belong[key].end());

			//cout << "SCG" << endl;
			//for (int i = 0; i < belong[key].size(); i++)
			//{
			//	cout << belong[key][i] << ',';
			//}
			//cout << endl;   // 查看通路

			for (size_t i = 0; i < belong[key].size(); i++)
			{
				find_cycle(belong[key][i]);
				Graph.erase(belong[key][i]);
			}
		}
#ifdef Test
		cout << "find cycle:" << clock() - t2 << "ms" << endl;
#endif
	}
	
	void tmp_cycle()
	{
		for (auto it = Graph.begin(); it != Graph.end(); it++)
		{
			int left = it->first;

			if (Graph.count(left) != 0)
			{
				for (auto middle : Graph[left])
				{
					if (Graph.count(middle) != 0)
					{
						for (auto right : Graph[middle])
						{
							if (right != left) my_cycle2[left][right].push_back(middle);
						}
					}

				}
			}
		}
	}

	void find_cycle(int v)
	{
		look = v;
		vector<int> path;
		path.push_back(v);
		stack<int> stack1;
		stack<vector<int>> stack2;
		stack1.push(v);
		stack2.push(path);
		while (!stack1.empty())
		{
			v = stack1.top(); stack1.pop();
			path = stack2.top(); stack2.pop();
			if (path.size()>7)
				continue;
			if (path.size() == 6)
			{
				int a = path[0];
				int b = path[5];
				
				if (Graph.count(b) != 0)
				{
					vector<int> temp1;
					for (int i = 0; i < path.size(); i++)
					{
						temp1.push_back(path[i]);
					}
					bool tag = 0;
					for (auto isin : Graph[b])
					{
						if (isin == a){
							tag = 1;
							break;
						}
					}
					if (tag)result6.push_back(temp1);
				}
				if (my_cycle2.count(b) != 0)
				{
					if (my_cycle2[b].count(a))
					{
						vector<int> temp2;
						for (int i = 0; i < path.size(); i++)
						{
							temp2.push_back(path[i]);
						}
						for (auto val : my_cycle2[b][a])
						{
							bool tag = 0;
							for (int i = 1; i < temp2.size(); i++){
								if (temp2[i] == val)tag = 1;
							}
							if (val < look || tag){ continue; }
							temp2.push_back(val);
							result7.push_back(temp2);
							temp2.pop_back();
						}
					}
				}
				continue;
			}
			if (Graph.count(v) == 0)
				continue;
			for (auto value : Graph[v])
			{
				vector<int> temp3;
				for (int i = 0; i < path.size(); i++)
				{
					temp3.push_back(path[i]);
				}
				if (value < look)continue;
				else if (value>look){
					bool flag = 0;
					for (int i = 1; i < path.size(); i++){
						if (path[i] == value){
							flag = 1; break;
						}
					}if (!flag)
					{
						stack1.push(value);
						temp3.push_back(value);
						stack2.push(temp3);
					}					
				}
				else{
					if (temp3.size() == 3)result3.push_back(temp3);
					if (temp3.size() == 4)result4.push_back(temp3);
					if (temp3.size() == 5)result5.push_back(temp3);
				}
			}
		}
	}
	
	void sort_data()
	{
		total = result3.size() + result4.size() + result5.size() + result6.size() + result7.size();
#ifdef Test
		cout << total << endl;
#endif
		sort(result3.begin(), result3.end());
		sort(result4.begin(), result4.end());
		sort(result5.begin(), result5.end());
		sort(result6.begin(), result6.end());
		sort(result7.begin(), result7.end());
		
	}

	void save_data(string & result_copy)
	{
#ifdef Test
		auto t3 = clock();
#endif
		FILE *fp = fopen(result_copy.c_str(), "wb");
		char buf[1024];
		int index = sprintf(buf, "%d\n", total);
		buf[index] = '\0';
		fputs(buf, fp);

		for (auto ans : result3)
		{
			index = 0;
			for (int i = 0; i < 2; i++)
			{
				index += sprintf(buf + index, "%d,", ans[i]);
			}
			index += sprintf(buf + index, "%d\n", ans[2]);

			fputs(buf, fp);
		}
		for (auto ans : result4)
		{
			index = 0;
			for (int i = 0; i < 3; i++)
			{
				index += sprintf(buf + index, "%d,", ans[i]);
			}
			index += sprintf(buf + index, "%d\n", ans[3]);

			fputs(buf, fp);
		}
		for (auto ans : result5)
		{
			index = 0;
			for (int i = 0; i < 4; i++)
			{
				index += sprintf(buf + index, "%d,", ans[i]);
			}
			index += sprintf(buf + index, "%d\n", ans[4]);

			fputs(buf, fp);
		}for (auto ans : result6)
		{
			index = 0;
			for (int i = 0; i < 5; i++)
			{
				index += sprintf(buf + index, "%d,", ans[i]);
			}
			index += sprintf(buf + index, "%d\n", ans[5]);

			fputs(buf, fp);
		}for (auto ans : result7)
		{
			index = 0;
			for (int i = 0; i < 6; i++)
			{
				index += sprintf(buf + index, "%d,", ans[i]);
			}
			index += sprintf(buf + index, "%d\n", ans[6]);

			fputs(buf, fp);
		}
		fclose(fp);

#ifdef Test
		cout << "save_data:" << clock() - t3 << "ms" << endl;
#endif
	}
};



int main()
{
	string test_data = "/data/test_data.txt";
	string result_copy = "/projects/student/result.txt";
#ifdef Test
	double time = 0.0;
	clock_t start;
	start = clock();
	test_data = "C:\\Users\\Yanhui\\Desktop\\HWcode2020-TestData\\Huawei\\3738\\test_data.txt";  //  1,54,3738,38252,58284,77409,1004812
	//string test_data = "F:\\python_exe\\DeepLearning\\little_test.txt";
	result_copy = "C:\\Users\\Yanhui\\Desktop\\HWcode2020-TestData\\Huawei\\3738\\result_copy.txt";
#endif

	Cycle cycle;
	cycle.load_data(test_data);
	cycle.find();
	cycle.sort_data();
	cycle.save_data(result_copy);
	
#ifdef Test

	printf("the time is %lfms",clock()-time);

#endif
	
	printf("\n");
	system("pause");
	return 0;
}
