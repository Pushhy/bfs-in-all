#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
#include <Windows.h>

using namespace std;

//迷宫(0代表空地，1代表墙壁，左上角为起点，右上角为终点)
int row = 5; //迷宫总行数
int column = 5; //迷宫总列数
int m[5][5] =
{
	0, 1, 0, 1, 0,
	0, 1, 0, 0, 0,
	0, 1, 0, 1, 0,
	0, 0, 0, 1, 0,
	0, 0, 0, 1, 0
};

//动作(驱动状态转移)
struct Action
{
	int rOff, cOff; //行和列的偏移量

	//构造函数
	Action(int rOff, int cOff)
	{
		this->rOff = rOff;
		this->cOff = cOff;
	}
};

//状态(搜索节点)
struct State
{
	int r, c; //当前位置
	int lastState; //上一个状态的索引
	
	//构造函数
	State(int r, int c)
	{
		this->r = r;
		this->c = c;
		lastState = -1; //关键
	}

	//重载==运算符(prepare for find())
	bool operator==(const State &s)
	{
		return r == s.r && c == s.c;
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
	return (state.r >= 0 && state.r < row && state.c >= 0 && state.c < column && m[state.r][state.c] == 0);
}

//判断状态是否被访问过
bool HasVisited(const State &state)
{
	return (find(visitedStates.begin(), visitedStates.end(), state) != visitedStates.end());
}

//判断是否是目标状态
bool IsTargetState(const State &state)
{
	return (state.r == row - 1 && state.c == column - 1);
}

//判断是否能应用状态(用来剪枝，有时条件会与IsTargetState重合)
bool CanApplyAction(const State &state, const Action &action)
{
	//条件与IsStateLegal重合
	return true;
}

//对状态应用动作
void ApplyAction(State &state, const Action &action)
{
	state.r += action.rOff;
	state.c += action.cOff;
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
			State s = visitedStates[visitedStates.size() - 1];
			while (s.lastState != -1)
			{
				cout << "(" << s.r << ", " << s.c << ") ";
				s = visitedStates[s.lastState];
			}
			cout << "(" << s.r << ", " << s.c << ") " << endl;
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
	actions.push_back(Action(0, 1)); //向右
	actions.push_back(Action(0, -1)); //向左
	actions.push_back(Action(1, 0)); //向下
	actions.push_back(Action(-1, 0)); //向上

	//从初始状态开始搜索
	bfs(State(0, 0));

	system("pause");
	return 0;
}