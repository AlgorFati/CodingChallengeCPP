#include <iostream>
#include <queue>
#include <set>
#include <unordered_map>
#include <stack>

using namespace std;

const int HumanNum = 100;
const int CannibalNum = 100;
const int BoatNum = 10;

class State
{
public:
	// 왼쪽 섬 데이터
	int human;
	int cannibal;
	bool boat;
	int level;

	State(int human, int cannibal, bool boat, int level)
		: human(human), cannibal(cannibal), boat(boat), level(level)
	{}

	State otherSideState() const
	{
		return State(HumanNum - human, CannibalNum - cannibal, !boat, level);
	}

	int key() const
	{
		return boat * 1000 * 1000 + cannibal * 1000 + human;
	}

	void print() const
	{
		cout << "human : " << human << " cannibal : " << cannibal << " boat : " << boat << " level : " << level << endl;
	}
};

queue<State> que;
set<int> visited;
unordered_map<int, State> backtrack;

void cross(const State& state)
{
	int unary = state.boat ? -1 : 1;

	for (int h = 0; h <= BoatNum; h++)
	{
		for (int c = 0; c <= BoatNum; c++)
		{
			if (h == 0 && c == 0)
				continue;

			if (h + c > BoatNum)
				continue;

			int moveHuman = h * unary;
			int moveCannibal = c * unary;

			State nextState(state.human + moveHuman, state.cannibal + moveCannibal, !state.boat, state.level + 1);

			// 경계 값 예외처리
			if (nextState.human < 0 || nextState.cannibal < 0 ||
				nextState.human > HumanNum || nextState.cannibal > CannibalNum)
			{
				continue;
			}

			// 방문 예외처리
			if (visited.find(nextState.key()) != visited.end())
			{
				continue;
			}

			// 배 위에서의 예외처리
			if (c > h && h > 0)
			{
				continue;
			}

			que.push(nextState);
			visited.insert(nextState.key());

			if (backtrack.find(nextState.key()) == backtrack.end())
			{
				backtrack.insert(make_pair(nextState.key(), state));
			}
		}
	}
}


bool checkScenarioBySide(const State& state)
{
	if (state.cannibal > state.human && state.human > 0)
	{
		return false;
	}
	return true;
}

bool checkScenario(const State& state)
{
	return checkScenarioBySide(state) && checkScenarioBySide(state.otherSideState());
}

void printBacktrack(const State& finish)
{
	stack<State> stack;

	State start = finish.otherSideState();
	State state = finish;
	stack.push(finish);

	while (state.key() != start.key())
	{
		auto it = backtrack.find(state.key());

		if (it != backtrack.end())
		{
			state = it->second;
			stack.push(state);
		}
		else
		{
			cout << "Error " << endl;
		}
	}


	while (!stack.empty())
	{
		State state = stack.top();
		State other = state.otherSideState();

		cout << "---------------------------" << endl;
		state.print();
		other.print();
		cout << "---------------------------" << endl;

		stack.pop();
	}
}

int main()
{

	State start(HumanNum, CannibalNum, true, 0);
	State finish = start.otherSideState();
	bool found = false;
	que.push(start);

	while (!que.empty())
	{
		State state = que.front();
		que.pop();

		if (!checkScenario(state))
		{
			continue;
		}

		//state.print();

		if (state.key() == finish.key())
		{
			finish.level = state.level;
			found = true;
			break;
		}

		cross(state);
	}

	if (found)
	{
		cout << finish.level << endl;


		printBacktrack(finish);
	}
	else
	{
		cout << -1;
	}

	return 0;
}