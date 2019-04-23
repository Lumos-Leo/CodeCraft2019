#include<vector>
using namespace std;

class planing
{
public:
	planing(int car_id);
	~planing();

	int _car_id;
	vector<int >_road_vec;
	int _start_time;
	int	_car_plan_time = 0;
	int _road_len =0;
private:

};

planing::planing(int car_id)
{
	_car_id = car_id;
	_car_plan_time = 0;
}

planing::~planing()
{
}