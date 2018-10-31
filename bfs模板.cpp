#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
#include <Windows.h>

using namespace std;

//动作(驱动状态转移)
struct Action
{
	

	//构造函数
	
};

//状态(搜索节点)
struct State
{
	
	int lastState; //上一个状态的索引

	//构造函数
	

	//重载==运算符
	bool operator==(const State &s)
	{
		//TODO:判断两个状态是否相等
		
	}
};

//保存所有动作
vector<Action> actions;

//扩展状态
deque<State> q;

//保存搜索过的路径
vector<State> visitedStates;

//标记状态为已访问
void BookState(const State &state)
{
	visitedStates.push_back(state);
}

//判断当前状态是否合法
bool IsStateLegal(const State &state)
{
	
}

//判断状态是否被访问过
bool HasVisited(const State &state)
{
	return (find(visitedStates.begin(), visitedStates.end(), state) != visitedStates.end());
}

//判断是否是目标状态
bool IsTargetState(const State &state)
{
	
}

//判断是否能应用状态(用来剪枝，有时条件会与IsTargetState重合)
bool CanApplyAction(const State &state, const Action &action)
{
	
}

//对状态应用动作
void ApplyAction(State &state, const Action &action)
{
	
}

//广度优先搜索
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
		BookState(current);

		//达到目标状态
		if (IsTargetState(current))
		{
			//TODO:输出最短路径
			//此时visitedStates的最后一个元素为目标状态，通过State::lastState可回溯之前的状态，直到lastState的值为-1
			
			return;
		}

		for (int i = 0; i < actions.size(); ++i)
		{
			//如果可以应用状态
			if (CanApplyAction(current, actions[i]))
			{
				//计算新状态
				State next = current;
				ApplyAction(next, actions[i]);
				//如果新状态合法，就将新状态入队
				if (IsStateLegal(next))
				{
					//记录上一个状态
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
	

	//从初始状态开始搜索
	

	system("pause");
	return 0;
}