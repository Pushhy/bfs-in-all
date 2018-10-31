/*
数字华容道：M*N的棋盘上有M*N-1个数，分别为1到M*N-1，还有一个空位，
数字可以借助空位移动，直到所有数字按照从左到右、从上到下的顺序依次排列
		本程序示例(3*3)：
		2 6 8    1 2 3
		1 4 0 -->4 5 6 (0表示空位)
		7 5 3    7 8 0
*/

#include <iostream>
#include <vector>
#include <deque>
#include <map>
#include <iomanip>
#include <algorithm>
#include <Windows.h>

using namespace std;

//M*N puzzle
const int M = 3;
const int N = 3;

enum ActionTag
{
	UP,
	DOWN,
	LEFT,
	RIGHT
};

//动作(驱动状态转移)
struct Action
{
	ActionTag tag; //将空格向四个方向移动

	//构造函数
	Action(ActionTag tag)
	{
		this->tag = tag;
	}
};

//状态(搜索节点)
struct State
{
	int a[M][N];
	int rSpace, cSpace; //空格的位置
	int lastState; //上一个状态的索引

	//构造函数
	State(int a[][N])
	{
		for (int i = 0; i < M; ++i)
		{
			for (int j = 0; j < N; ++j)
			{
				if (a[i][j] == 0)
				{
					rSpace = i;
					cSpace = j;
				}
				this->a[i][j] = a[i][j];
			}
		}
		lastState = -1;
	}

	//重载==运算符
	bool operator==(const State &s)
	{
		//TODO:判断两个状态是否相等
		for (int i = 0; i < M; ++i)
		{
			for (int j = 0; j < N; ++j)
			{
				if (a[i][j] != s.a[i][j])
				{
					return false;
				}
			}
		}
		return true;
	}

	//哈希函数
	long long hash() const
	{
		long long res = 0;
		for (int i = 0; i < M; ++i)
		{
			for (int j = 0; j < N; ++j)
			{
				res = res * 10 + a[i][j];
			}
		}
		return res;
	}

	void print()
	{
		for (int i = 0; i < M; ++i)
		{
			for (int j = 0; j < N; ++j)
			{
				cout << a[i][j] << " ";
			}
			cout << endl;
		}
		cout << endl;
	}
};

//保存所有动作
vector<Action> actions;

//扩展状态
deque<State> q;

//保存搜索过的路径
vector<State> visitedStates;

//判断重复
map<long long, int> m;

//判断当前状态是否合法
bool IsStateLegal(const State &state)
{
	return true;
}

//判断状态是否被访问过
bool HasVisited(const State &state)
{
	return m.count(state.hash()) == 1;
}

//标记状态为已访问
void BookState(const State &state)
{
	visitedStates.push_back(state);
	m[state.hash()] = 1;
}

//判断是否是目标状态
bool IsTargetState(const State &state)
{
	return state.hash() == 123456780; //最终状态的哈希值
}

//判断是否能应用状态(用来剪枝，有时条件会与IsTargetState重合)
bool CanApplyAction(const State &state, const Action &action)
{
	switch (action.tag)
	{
	case UP:
		return state.rSpace > 0;
		break;
	case DOWN:
		return state.rSpace < M - 1;
		break;
	case LEFT:
		return state.cSpace > 0;
		break;
	case RIGHT:
		return state.cSpace < N - 1;
		break;
	}
}

//对状态应用动作
void ApplyAction(State &state, const Action &action)
{
	int rSpace = state.rSpace, cSpace = state.cSpace;
	switch (action.tag)
	{
	case UP:
		swap(state.a[rSpace][cSpace], state.a[rSpace - 1][cSpace]);
		state.rSpace--;
		break;
	case DOWN:
		swap(state.a[rSpace][cSpace], state.a[rSpace + 1][cSpace]);
		state.rSpace++;
		break;
	case LEFT:
		swap(state.a[rSpace][cSpace], state.a[rSpace][cSpace - 1]);
		state.cSpace--;
		break;
	case RIGHT:
		swap(state.a[rSpace][cSpace], state.a[rSpace][cSpace + 1]);
		state.cSpace++;
		break;
	}
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
				s.print();
				s = visitedStates[s.lastState];
			}
			s.print();
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
	actions.push_back(Action(UP));
	actions.push_back(Action(DOWN));
	actions.push_back(Action(LEFT));
	actions.push_back(Action(RIGHT));

	//从初始状态开始搜索
	int a[M][N] = 
	{
		8, 2, 0,
		6, 5, 4,
		1, 7, 3
	};
	bfs(State(a));

	system("pause");
	return 0;
}