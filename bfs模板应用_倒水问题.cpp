/*
三个水桶等分8升水：有三个容积分别是3升、5升、8升的水桶，其中容积为8升的水桶中装满了水，
3升和5升的桶都是空的。三个水桶都没有体积刻度，现在要将大水桶中的8升水等分为两份，
每份都是4升水，条件是只能利用其余两个空桶，请问有多少种方法？并输出所有方法
*/

#include <iostream>
#include <vector>
#include <map>
#include <deque>
#include <algorithm>
#include <Windows.h>

using namespace std;

//水桶的数量
const int BUCKET_COUNT = 3;

//水桶的容量
int capacity[BUCKET_COUNT] = { 3, 5, 8 };

//动作(驱动状态转移)
struct Action
{
	int from;
	int to;

	//构造函数
	Action()
	{
		from = -1;
		to = -1;
	}

	Action(int from, int to)
	{
		this->from = from;
		this->to = to;
	}
};

//状态(搜索节点)
struct State
{
	int buckets[BUCKET_COUNT]; //水桶当前的水量n
	int lastState; //上一个状态的索引

	//构造函数
	State(int buckets[])
	{
		for (int i = 0; i < BUCKET_COUNT; ++i)
		{
			this->buckets[i] = buckets[i];
		}
		lastState = -1;
	}

	//重载==运算符
	bool operator==(const State &s)
	{
		//TODO:判断两个状态是否相等
		for (int i = 0; i < BUCKET_COUNT; ++i)
		{
			if (buckets[i] != s.buckets[i])
			{
				return false;
			}
		}
		return true;
	}
};

//保存所有动作
vector<Action> actions;

//扩展状态
deque<State> q;

//保存搜索过的路径
vector<State> visitedStates;

//判断当前状态是否合法
bool IsStateLegal(const State &state)
{
	for (int i = 0; i < BUCKET_COUNT; ++i)
	{
		if (state.buckets[i] < 0 || state.buckets[i] > capacity[i])
		{
			return false;
		}
	}
}

//判断状态是否被访问过
bool HasVisited(const State &state)
{
	return (find(visitedStates.begin(), visitedStates.end(), state) != visitedStates.end());
}

//判断是否是目标状态
bool IsTargetState(const State &state)
{
	return (state.buckets[0] == 0 && state.buckets[1] == 4 && state.buckets[2] == 4);
}

//判断是否能应用状态(用来剪枝，有时条件会与IsTargetState重合)
bool CanApplyAction(const State &state, const Action &action)
{
	if (action.from == action.to)
	{
		return false;
	}

	int capFrom = capacity[action.from];
	int capTo = capacity[action.to];
	int waterFrom = state.buckets[action.from];
	int waterTo = state.buckets[action.to];
	int spaceFrom = capFrom - waterFrom;
	int spaceTo = capTo - waterTo;

	if (waterFrom == 0)
	{
		return false;
	}
	if (spaceTo == 0)
	{
		return false;
	}

	return true;
}

//对状态应用动作
void ApplyAction(State &state, const Action &action)
{
	int capFrom = capacity[action.from];
	int capTo = capacity[action.to];
	int waterFrom = state.buckets[action.from];
	int waterTo = state.buckets[action.to];
	int spaceFrom = capFrom - waterFrom;
	int spaceTo = capTo - waterTo;

	if (spaceTo >= waterFrom)
	{
		state.buckets[action.from] = 0;
		state.buckets[action.to] += waterFrom;
	}
	else
	{
		state.buckets[action.from] -= spaceTo;
		state.buckets[action.to] = capTo;
	}
}

void bfs(State init)
{
	//将初始状态入队
	q.push_back(init);
	while (!q.empty())
	{
		//取队首元素为当前处理的状态
		State current = q.front();
		q.pop_front();

		//状态已被访问过
		if (HasVisited(current))
		{
			continue;
		}

		//标记当前状态为已访问
		visitedStates.push_back(current);

		//达到目标状态
		if (IsTargetState(current))
		{
			//TODO:输出最短路径
			//此时visitedStates的最后一个元素为目标状态，通过State::lastState可回溯之前的状态，直到lastState的值为-1
			State s = visitedStates[visitedStates.size() - 1];
			while (s.lastState != -1)
			{
				cout << "(" << s.buckets[0] << " " << s.buckets[1] << " " << s.buckets[2] << ")" << " ";
				s = visitedStates[s.lastState];
			}
			cout << "(" << s.buckets[0] << " " << s.buckets[1] << " " << s.buckets[2] << ")" << " " << endl;
			return;
		}

		for (int i = 0; i < actions.size(); ++i)
		{
			if (CanApplyAction(current, actions[i]))
			{
				//将新状态入队
				State next = current;
				ApplyAction(next, actions[i]);
				if (IsStateLegal(next))
				{
					next.lastState = visitedStates.size() - 1;
					q.push_back(next);
				}
			}
		}
	} 
}

int main()
{
	//初始化动作
	for (int i = 0; i < BUCKET_COUNT; ++i)
	{
		for (int j = 0; j < BUCKET_COUNT; ++j)
		{
			if (i != j)
			{
				actions.push_back(Action(i, j));
			}
		}
	}

	//从初始状态开始搜索
	int a[] = { 0, 0, 8 };
	bfs(State(a));

	system("pause");
	return 0;
}